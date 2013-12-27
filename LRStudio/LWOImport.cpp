/*
 *	Implementation od LWO2->LiquidReality importer
 *  (c) 2002 Maciej Mróz
 */

#include "stdafx.h"
#include <vector>
#include <hash_map>
#include <set>
#include "zlib.h"
#include <assert.h>

#pragma unmanaged

struct LW_Point
{
	float x;
	float y;
	float z;
};

struct LW_UV
{
	float u;
	float v;
};

typedef char LW_String[256];
typedef unsigned char LW_ID4[4];	//reverse byte order

struct LW_Layer
{
	USHORT usNumber;
	USHORT usFlags;
	LW_Point pivot;
	LW_String szName;
	USHORT usParent;		//verify - does 0xffff==no parent??
};

struct LW_Triangle
{
	UINT v0;
	UINT v1;
	UINT v2;
	//from PTAG:
	//SMGP
	USHORT usSmoothingGroup;
	//SURF
	USHORT usSurface;
};

struct LW_VMAPEntry1D
{
	UINT	uiVertexIndex;
	float	fVal0;
};

struct LW_VMAPEntry2D
{
	UINT	uiVertexIndex;
	float	fVal0;
	float	fVal1;
};

struct LW_VMAPEntry3D
{
	UINT	uiVertexIndex;
	float	fVal0;
	float	fVal1;
	float	fVal2;
};

struct LW_VertexMap
{
	LW_ID4		type;
	USHORT		usDimension;
	LW_String	szName;
	UINT		uiEntryCount;
	union
	{
		LW_VMAPEntry1D	*pVMAP1D;
		LW_VMAPEntry2D	*pVMAP2D;
		LW_VMAPEntry3D	*pVMAP3D;
	};
};

struct LW_VMADEntry1D
{
	UINT	uiVertexIndex;
	UINT	uiTriangleIndex;
	float	fVal0;
};

struct LW_VMADEntry2D
{
	UINT	uiVertexIndex;
	UINT	uiTriangleIndex;
	float	fVal0;
	float	fVal1;
};

struct LW_VMADEntry3D
{
	UINT	uiVertexIndex;
	UINT	uiTriangleIndex;
	float	fVal0;
	float	fVal1;
	float	fVal2;
};

struct LW_DiscontinuousVertexMap
{
	LW_ID4		type;
	USHORT		usDimension;
	LW_String	szName;
	UINT		uiEntryCount;
	union
	{
		LW_VMADEntry1D	*pVMAD1D;
		LW_VMADEntry2D	*pVMAD2D;
		LW_VMADEntry3D	*pVMAD3D;
	};
};

//envelope type
#define LWO_ENVELOPE_TYPE_POSITION_X			1
#define LWO_ENVELOPE_TYPE_POSITION_Y			2
#define LWO_ENVELOPE_TYPE_POSITION_Z			3
#define LWO_ENVELOPE_TYPE_ROTATION_HEADING		4
#define LWO_ENVELOPE_TYPE_ROTATION_PITCH		5
#define LWO_ENVELOPE_TYPE_ROTATION_BANK			6
#define LWO_ENVELOPE_TYPE_SCALE_X				7
#define LWO_ENVELOPE_TYPE_SCALE_Y				8
#define LWO_ENVELOPE_TYPE_SCALE_Z				9
#define LWO_ENVELOPE_TYPE_COLOR_R				10
#define LWO_ENVELOPE_TYPE_COLOR_G				11
#define LWO_ENVELOPE_TYPE_COLOR_B				12
#define LWO_ENVELOPE_TYPE_FALLOFF_X				13
#define LWO_ENVELOPE_TYPE_FALLOFF_Y				14
#define LWO_ENVELOPE_TYPE_FALLOFF_Z				15

//envelope pre/post behavior
#define LWO_ENVELOPE_BEH_RESET				0
#define LWO_ENVELOPE_BEH_CONSTANT			1
#define LWO_ENVELOPE_BEH_REPEAT				2
#define LWO_ENVELOPE_BEH_OSCILLATE			3
#define LWO_ENVELOPE_BEH_OFFSET_REPEAT		4
#define LWO_ENVELOPE_BEH_LINEAR				5

//include info from both KEY and SPAN chunks!!!
struct LW_EnvelopeKey
{
	float fTime;
	float fValue;
	LW_ID4	SpanType;
	float	fSpanParam0;
	float	fSpanParam1;
	float	fSpanParam2;
	float	fSpanParam3;
};

#define MAX_ENVELOPE_COUNT	256

struct LW_Envelope
{
	UINT			uiIndex;
	//TYPE
	BYTE			UserFormat;
	BYTE			Type;
	//PRE
	USHORT			usPreBehavior;
	//POST
	USHORT			usPostBehavior;
	//keys
	int				iKeyCount;
	int				iKeyTblSize;
	LW_EnvelopeKey	*pKeys;
};

#define MAX_CLIP_COUNT		256

struct LW_Clip
{
	LW_ID4			ClipType;		//only STIL is supported
	LW_String		szFileName;
	UINT			uiClipIndex;
};

#define MAX_SURFACE_BLOK_COUNT 256

struct LW_TMap
{
	//CNTR
	BOOL		bCenterPresent;
	LW_Point	Center;
	UINT		uiCenterEnvIndex;
	//SIZE
	BOOL		bSizePresent;
	LW_Point	Size;
	UINT		uiSizeEnvIndex;
	//ROTA
	BOOL		bRotationPresent;
	LW_Point	Rotation;
	UINT		uiRotationEnvIndex;
	//OREF
	LW_String	szReferenceObject;
	//FALL
	BOOL		bFalloffPresent;
	USHORT		usFalloffType;
	LW_Point	Falloff;
	UINT		uiFalloffEnvIndex;
	//CSYS
	BOOL		bCoordinateSystemPresent;
	USHORT		usCoordinateSystem;
};

struct LW_SurfBlockHeader
{
	LW_ID4		Type;		//IMAP/PROC/GRAD/SHDR
	LW_String	szOrdinal;
	//CHAN
	LW_ID4		TextureChannel;
	USHORT		usEnable;
	//OPAC
	USHORT		usOpacityType;
	float		fOpacity;
	UINT		uiOpacityEnvIndex;
	//NEGA
	USHORT		usNegative;
	//AXIS
	USHORT		usDisplacementAxis;
};

//projection modes
#define TMAP_PROJECTION_MODE_PLANAR				0
#define TMAP_PROJECTION_MODE_CYLINDRICAL		1
#define TMAP_PROJECTION_MODE_SPHERICAL			2
#define TMAP_PROJECTION_MODE_CUBIC				3
#define TMAP_PROJECTION_MODE_FRONT_PROJECTION	4
#define TMAP_PROJECTION_MODE_UV					5

//texture axes
#define TMAP_AXIS_X								0
#define TMAP_AXIS_Y								1
#define TMAP_AXIS_Z								2

//texture wrapping
#define TMAP_WRAP_RESET							0
#define TMAP_WRAP_REPEAT						1
#define TMAP_WRAP_MIRRORED_REPEAT				2
#define TMAP_WRAP_EDGE							3

struct LW_SurfaceBlock
{
	LW_SurfBlockHeader BlockHeader;
	//TMAP subchunk
	LW_TMap	TMAP;
	//PROJ
	USHORT	usProjectionMode;
	//AXIS
	USHORT	usTextureAxis;
	//IMAG
	UINT	uiImageMapIndex;
	//WRAP
	USHORT	usImageWrapOptionsW;
	USHORT	usImageWrapOptionsH;
	//WRPW
	float	fWrapW;
	UINT	uiWrapWEnvIndex;
	//WRPH
	float	fWrapH;
	UINT	uiWrapHEnvIndex;
	//VMAP
	LW_String	szVMapName;
	//AAST
	USHORT	usAntialiasingStrengthOptions;
	float	fAntialiasingStrength;
	//PIXB
	USHORT	usPixelBlendingFlags;
	//STCK
	USHORT	usStickyProjection;
	float	fStickyProjectionTime;
	//TAMP
	float	fTextureAmplitude;
	UINT	uiTextureAmplitudeEnvIndex;
};

#define MAX_SURFACE_COUNT 256

struct LW_Surface
{
	LW_String	szName;
	LW_String	szSource;
	//attributes:
	//COLR
	float		fColorR;
	float		fColorG;
	float		fColorB;
	UINT		uiColorEnvIndex;
	//DIFF
	float		fDiffuse;
	UINT		uiDiffuseEnvIndex;
	//LUMI
	float		fLuminosity;
	UINT		uiLuminosityEnvIndex;
	//SPEC
	float		fSpecular;
	UINT		uiSpecularEnvIndex;
	//REFL
	float		fReflection;
	UINT		uiReflectionEnvIndex;
	//TRAN
	float		fTransparency;
	UINT		uiTransparencyEnvIndex;
	//TRNL
	float		fTranslucency;
	UINT		uiTranslucencyEnvIndex;
	//GLOS
	float		fGlossiness;
	UINT		uiGlossinessEnvIndex;
	//SHRP
	float		fDiffuseSharpness;
	UINT		uiDiffuseSharpnessEnvIndex;
	//BUMP
	float		fBumpIntensity;
	UINT		uiBumpIntensityEnvIndex;
	//SIDE
	USHORT		usPolySideness;
	//SMAN
	float		fMaxSmoothingAngle;
	//RFOP
	USHORT		usReflectionOptions;
	//RIMG
	UINT		uiReflectionMapImageIndex;
	//RSAN
	float		fReflectionMapSeamAngle;
	UINT		uiReflectionMapSeamAngleEnvIndex;
	//RBLR
	float		fReflectionBlurring;
	UINT		uiReflectionBlurringEnvIndex;
	//RIND
	float		fRefractiveIndex;
	UINT		uiRefractiveIndexEnvIndex;
	//TROP
	USHORT		usTransparencyOptions;
	//TIMG
	UINT		uiRefractionMapImageIndex;
	//TBLR
	float		fRefractionBlurring;
	UINT		uiRefractionBlurringEnvIndex;
	//CLRH
	float		fColorHighlights;
	UINT		uiColorHighlightsEnvIndex;
	//CLRF
	float		fColorFilter;
	UINT		uiColorFilterEnvIndex;
	//ADTR
	float		fAdditiveTransparency;
	UINT		uiAdditiveTransparencyEnvIndex;
	//GLOW
	BOOL		bGlow;
	USHORT		usGlowType;
	float		fGlowIntensity;
	UINT		uiGlowIntensityEnvIndex;
	float		fGlowSize;
	UINT		uiGlowSizeEnvIndex;
	//LINE
	BOOL		bRenderOutlines;
	USHORT		usRenderOutlinesFlags;
	BOOL		bRenderOutlinesSize;		//size is optional
	float		fRenderOutlinesSize;
	UINT		uiRenderOutlinesSizeEnvIndex;
	BOOL		bRenderOutlinesColor;		//color is optional
	float		fRenderOutlinesColorR;
	float		fRenderOutlinesColorG;
	float		fRenderOutlinesColorB;
	UINT		uiRenderOutlinesColorEnvIndex;
	//ALPH
	USHORT		usAlphaMode;
	float		fAlphaValue;
	//VCOL
	float		fVertexColorMapIntensity;
	UINT		uiVertexColorMapIntensityEnvIndex;
	LW_ID4		szVMapType;
	LW_String	szVMapName;

	//texture/shader layer blocks
	UINT			uiSurfaceBlockCount;
	LW_SurfaceBlock SurfaceBlocks[MAX_SURFACE_BLOK_COUNT];
};

#define MAX_TAG_COUNT 256
#define MAX_VMAP_COUNT 256
#define MAX_VMAD_COUNT 256

struct LW_Object
{
	UINT						uiPointCount;		//importer will never output more than 65536 vertices
									//in single .flm (to meet limits of Geforce class hw) but lwo itself
									//can contain more vertices - it is enough that single surface
									//doesn't exceed 65536 vertices. VMAD chunk, surface
									//smoothing angle settings, fixing npatch seams etc may require
									//vertex doubling so this limit may actually be lower on specific objects.
									//Surface may (in future) exceed those limits _only_ for reference
									//high-poly model used for normal and displacement map generation
	LW_Point					*pPNTS;
	UINT						uiTagCount;
	LW_String					Tags[MAX_TAG_COUNT];
	UINT						uiTriCount;			//triangle count limits are same as vertex count limits
	LW_Triangle					*pTriangles;
	UINT						uiVMapCount;
	LW_VertexMap				VMaps[MAX_VMAP_COUNT];
	UINT						uiVMadCount;
	LW_DiscontinuousVertexMap	VMads[MAX_VMAD_COUNT];
	UINT						uiClipCount;
	LW_Clip						Clips[MAX_CLIP_COUNT];
	UINT						uiEnvelopeCount;
	LW_Envelope					Envelopes[MAX_ENVELOPE_COUNT];
	UINT						uiSurfaceCount;
	LW_Surface					Surfaces[MAX_SURFACE_COUNT];
};

//some helper functions for reversing byte order
//all of them return TRUE on success

//generic functions for reading 2 and 4 byte values
BOOL read_2b_val(FILE *f,void *pOut)
{
	if((!f) || (!pOut))
	{
		return FALSE;
	}
	int nRead=fread(pOut,1,2,f);
	if(nRead!=2)
	{
		return FALSE;
	}
	unsigned char ucTmp=0;
	ucTmp=((unsigned char*)pOut)[0];
	((unsigned char*)pOut)[0]=((unsigned char*)pOut)[1];
	((unsigned char*)pOut)[1]=ucTmp;
	return TRUE;
}

BOOL read_2b_val_block(FILE *f,int iCount,void *pOut) {
	if((!f) || (!pOut))
	{
		return FALSE;
	}
	int nRead=fread(pOut,1,2*iCount,f);
	if(nRead!=(2*iCount))
	{
		return FALSE;
	}
	int iTmp=0;
	for(iTmp=0;iTmp<iCount;iTmp++)
	{
		unsigned char *pucTmp=&(((unsigned char*)pOut)[2*iTmp]);
		unsigned char ucTmp=0;
		ucTmp=pucTmp[0];
		pucTmp[0]=pucTmp[1];
		pucTmp[1]=ucTmp;
	}
	return TRUE;
}

BOOL read_4b_val(FILE *f,void *pOut)
{
	if((!f) || (!pOut))
	{
		return FALSE;
	}
	int nRead=fread(pOut,1,4,f);
	if(nRead!=4)
	{
		return FALSE;
	}
	unsigned char ucTmp=0;
	ucTmp=((unsigned char*)pOut)[0];
	((unsigned char*)pOut)[0]=((unsigned char*)pOut)[3];
	((unsigned char*)pOut)[3]=ucTmp;
	ucTmp=((unsigned char*)pOut)[1];
	((unsigned char*)pOut)[1]=((unsigned char*)pOut)[2];
	((unsigned char*)pOut)[2]=ucTmp;
	return TRUE;
}

BOOL read_4b_val_block(FILE *f,int iCount,void *pOut)
{
	if((!f) || (!pOut))
	{
		return FALSE;
	}
	int nRead=fread(pOut,1,4*iCount,f);
	if(nRead!=(4*iCount))
	{
		return FALSE;
	}
	int iTmp=0;
	for(iTmp=0;iTmp<iCount;iTmp++)
	{
		unsigned char *pucTmp=&(((unsigned char*)pOut)[4*iTmp]);
		unsigned char ucTmp=0;
		ucTmp=pucTmp[0];
		pucTmp[0]=pucTmp[3];
		pucTmp[3]=ucTmp;
		ucTmp=pucTmp[1];
		pucTmp[1]=pucTmp[2];
		pucTmp[2]=ucTmp;
	}
	return TRUE;
}

//more error proof functions providing type safety
//signed short
BOOL readI2(FILE *f,SHORT *psOut)
{
	return read_2b_val(f,(void*)psOut);
}

BOOL readI2Block(FILE *f,int iCount,SHORT *psOut)
{
	return read_2b_val_block(f,iCount,(void*)psOut);
}

//unsigned short
BOOL readU2(FILE *f,USHORT *pusOut)
{
	return read_2b_val(f,(void*)pusOut);
}

BOOL readU2Block(FILE *f,int iCount,USHORT *pusOut)
{
	return read_2b_val_block(f,iCount,(void*)pusOut);
}

//signed int
BOOL readI4(FILE *f,int *piOut)
{
	return read_4b_val(f,(void*)piOut);
}

BOOL readI4Block(FILE *f,int iCount,int *piOut)
{
	return read_4b_val_block(f,iCount,(void*)piOut);
}

//unsigned int
BOOL readU4(FILE *f,unsigned int *puiOut)
{
	return read_4b_val(f,(void*)puiOut);
}

BOOL readU4Block(FILE *f,int iCount,unsigned int *puiOut)
{
	return read_4b_val_block(f,iCount,(void*)puiOut);
}

//float
BOOL readF4(FILE *f,float *pfOut)
{
	return read_4b_val(f,(void*)pfOut);
}

BOOL readF4Block(FILE *f,int iCount,float *pfOut)
{
	return read_4b_val_block(f,iCount,(void*)pfOut);
}

//ID4
BOOL readID4(FILE *f,LW_ID4 szID)
{
	return read_4b_val(f,(void*)szID);
}

//helper ID4 compare
BOOL isChunkOfType(LW_ID4 szID, int ID)
{
	if(*((int*)szID)==ID)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//helper string reader
BOOL readLWString(FILE *f,LW_String &lw_string,UINT &nRead)
{
	nRead=0;
	unsigned char cTmp=0;
	ZeroMemory(&lw_string,sizeof(LW_String));
	do
	{
		if(fread(&cTmp,1,1,f)!=1)
		{
			return FALSE;
		}
		if(nRead==sizeof(LW_String))
		{
			MessageBox(NULL,L"Name too long!!! Use shorter names\n"
				L"or contact your favorite programmer :)",L"LWO2 import error!",MB_TASKMODAL);
			nRead++;
			return FALSE;
		}
		lw_string[nRead++]=cTmp;		
	} while(cTmp!=0);
	if(nRead%2)
	{	//odd size
		fseek(f,1,SEEK_CUR);
		nRead++;
	}
	return TRUE;
}

//read 'LAYR' chunk (starting from length)
BOOL readLAYR(FILE *f,LW_Layer &layer,UINT &nRead)
{
	UINT uiLAYRLength=0;
	nRead=0;
	if(!readU4(f,&uiLAYRLength))
	{
		return FALSE;
	}
	nRead+=4;
	if(!readU2(f,&layer.usNumber))
	{
		return FALSE;
	}
	nRead+=2;
	if(!readU2(f,&layer.usFlags))
	{
		return FALSE;
	}
	nRead+=2;
	if(!readF4Block(f,3,(float*)&(layer.pivot)))
	{
		return FALSE;
	}
	nRead+=12;
	UINT uiNameLength=0;
	if(!readLWString(f,layer.szName,uiNameLength))
	{
		nRead+=uiNameLength;
		return FALSE;
	}
	nRead+=uiNameLength;
	if((nRead-4)<uiLAYRLength)
	{	//there is optional 'parent'
		if(!readU2(f,&layer.usParent))
		{
			return FALSE;
		}
		nRead+=2;
	}
	if(nRead%2)
	{	//pad to even <--is it really needed here?
		fseek(f,1,SEEK_CUR);
		nRead++;
	}
	return TRUE;
}

//read 'PNTS' chunk (starting from length)
//uiOffset is accumulated size of all previous PNTS
//chunks - needed to correctly offset vertex indices in POLS
//chunk (and other chunks that use vertex indices - like VMAP or VMAD),
//it does matter only when multiple layers are present
BOOL readPNTS(FILE *f,LW_Object &lwo,LW_Layer &layer,UINT &uiOffset,UINT &nRead)
{
	UINT uiPNTSLength=0;
	nRead=0;
	if(!readU4(f,&uiPNTSLength))
	{
		return FALSE;
	}
	nRead+=4;
	uiOffset=lwo.uiPointCount;
	if(lwo.pPNTS)
	{
		if(lwo.uiPointCount==0)
		{
			MessageBox(NULL,L"Something went absolutely wrong.\nContact your favorite programmer!!!",
				L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		LW_Point *pTmpPNTS=NULL;
		pTmpPNTS=new LW_Point[lwo.uiPointCount+uiPNTSLength/12];
#ifdef _DEBUG
		{
			wchar_t wszMsg[256];
			wsprintf(wszMsg,L"Allocated %d bytes of memory for LW_Point structures\n",
				(lwo.uiPointCount+uiPNTSLength/12)*sizeof(LW_Point));
			OutputDebugString(wszMsg);
		}
#endif
		memcpy(pTmpPNTS,lwo.pPNTS,lwo.uiPointCount*sizeof(LW_Point));
		//read new data
		if(!readF4Block(f,uiPNTSLength/4,&((float*)pTmpPNTS)[lwo.uiPointCount*3]))
		{
			delete [] pTmpPNTS;
			pTmpPNTS=NULL;
			return FALSE;
		}
		nRead+=uiPNTSLength;
		//offset by pivot
		UINT uiTmp=0;
		for(uiTmp=0;uiTmp<uiPNTSLength/12;uiTmp++)
		{
			pTmpPNTS[lwo.uiPointCount+uiTmp].x-=layer.pivot.x;
			pTmpPNTS[lwo.uiPointCount+uiTmp].y-=layer.pivot.y;
			pTmpPNTS[lwo.uiPointCount+uiTmp].z-=layer.pivot.z;
		}
		delete [] lwo.pPNTS;
		lwo.pPNTS=pTmpPNTS;
		lwo.uiPointCount+=uiPNTSLength/12;
		return TRUE;
	}
	else
	{
		if(lwo.uiPointCount!=0)
		{
			MessageBox(NULL,L"Something went absolutely wrong.\nContact your favorite programmer!!!",
				L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		LW_Point *pTmpPNTS=NULL;
		pTmpPNTS=new LW_Point[uiPNTSLength/12];
#ifdef _DEBUG
		{
			wchar_t wszMsg[256];
			wsprintf(wszMsg,L"Allocated (2) %d bytes of memory for LW_Point structures\n",
				(uiPNTSLength/12)*sizeof(LW_Point));
			OutputDebugString(wszMsg);
		}
#endif
		//read new data
		if(!readF4Block(f,uiPNTSLength/4,&((float*)pTmpPNTS)[0]))
		{
			delete [] pTmpPNTS;
			pTmpPNTS=NULL;
			return FALSE;
		}
		nRead+=uiPNTSLength;
		//offset by pivot
		UINT uiTmp=0;
		for(uiTmp=0;uiTmp<uiPNTSLength/12;uiTmp++)
		{
			pTmpPNTS[uiTmp].x+=layer.pivot.x;
			pTmpPNTS[uiTmp].y+=layer.pivot.y;
			pTmpPNTS[uiTmp].z+=layer.pivot.z;
		}
		lwo.pPNTS=pTmpPNTS;
		lwo.uiPointCount=uiPNTSLength/12;
		return TRUE;
	}
}

BOOL readVX(FILE *f,UINT &uiIndex,UINT &nRead)
{
	UINT uiTmp=0;
	nRead=0;
	if(fread(&uiTmp,1,2,f)!=2)
	{
		return FALSE;
	}
	nRead+=2;
	if(*((BYTE*)&uiTmp)==0xff)
	{	//long form
		if(fread(&(((BYTE*)&uiTmp)[2]),1,2,f)!=2)
		{
			return FALSE;
		}
		nRead+=2;
		BYTE *pbTmp=(BYTE*)&uiTmp;
		pbTmp[0]=pbTmp[3];
		pbTmp[3]=0;
		BYTE bTmp=pbTmp[2];
		pbTmp[2]=pbTmp[1];
		pbTmp[1]=bTmp;
		uiIndex=uiTmp;
		return TRUE;
	}
	else
	{							//short form
		BYTE *pbTmp=(BYTE*)&uiTmp;
		BYTE bTmp=pbTmp[0];
		pbTmp[0]=pbTmp[1];
		pbTmp[1]=bTmp;
		pbTmp[2]=0;
		pbTmp[3]=0;
		uiIndex=uiTmp;
		return TRUE;
	}
}

//read 'POLS' chunk (starting from length)
BOOL readPOLS(FILE *f,LW_Object &lwo,UINT uiVertexOffset,
			  UINT &uiTriangleOffset,UINT &nRead)
{
	UINT uiPOLSLength=0;
	uiTriangleOffset=lwo.uiTriCount;
	nRead=0;
	if(!readU4(f,&uiPOLSLength))
	{
		return FALSE;
	}
	nRead+=4;
	LW_ID4 szType;
	if(!readID4(f,szType))
	{
		return FALSE;
	}
	nRead+=4;
	UINT			uiTriCount=0;
	UINT			uiTriBufSize=0;
	LW_Triangle		*pTris=NULL;
	switch(*((int*)szType))
	{
		case 'FACE':	//read polygon data
			{
				do
				{
					USHORT usNumVerts=0;
					if(!readU2(f,&usNumVerts))
					{
						if(pTris)
						{
							delete [] pTris;
						}
						return FALSE;
					}
					nRead+=2;
					usNumVerts&=0x3ff;			//TODO:o co tu chodzilo ?????
					if(usNumVerts!=3)
					{
						MessageBox(NULL,L"Polygon different than triangle was "
							L"found!\nPlease Triple first (and remove 1/2 point"
							L" polygons). Import failed!",
							L"LWO2 import error!!!",MB_TASKMODAL);
						if(pTris)
						{
							delete [] pTris;
						}
						return FALSE;
					}
					//read and add polygon
					if(uiTriCount==uiTriBufSize)
					{
						if(uiTriCount==0)
						{
							uiTriBufSize=1000;
							pTris=new LW_Triangle[uiTriBufSize];
#ifdef _DEBUG
							{
								wchar_t wszMsg[256];
								wsprintf(wszMsg,L"Allocated (3) %d bytes of "
									L"memory for LW_Triangle structures\n",
									(uiTriBufSize)*sizeof(LW_Triangle));
								OutputDebugString(wszMsg);
							}
#endif
						}
						else
						{
							if(!pTris)
							{
								MessageBox(NULL,L"Something is wrong.\nContact "
									L"your favorite programmer.",
									L"LWO2 import error!!!",MB_TASKMODAL);
							}
							LW_Triangle *pTmpTris=NULL;
							pTmpTris=new LW_Triangle[2*uiTriBufSize];
#ifdef _DEBUG
							{
								wchar_t wszMsg[256];
								wsprintf(wszMsg,L"Allocated (4) %d bytes of "
									L"memory for LW_Triangle structures\n",
									(2*uiTriBufSize)*sizeof(LW_Triangle));
								OutputDebugString(wszMsg);
							}
#endif
							ZeroMemory(pTmpTris,2*uiTriBufSize*sizeof(LW_Triangle));
							memcpy(pTmpTris,pTris,uiTriBufSize*sizeof(LW_Triangle));
							delete [] pTris;
							pTris=pTmpTris;
							uiTriBufSize*=2;
						}
					}
					int i=0;
					for(i=0;i<3;i++)
					{
						UINT uiVXSize=0;
						UINT uiVX=0;
						if(!readVX(f,uiVX,uiVXSize))
						{
							return FALSE;
						}
						nRead+=uiVXSize;
						(&(pTris[uiTriCount].v0))[i]=uiVX+uiVertexOffset;
					}
					pTris[uiTriCount].usSmoothingGroup=0;
					pTris[uiTriCount].usSurface=0;
					uiTriCount++;
				} while((nRead-4)<uiPOLSLength);
				//concatenate to (possibly) existing triangle buffer
				if(lwo.pTriangles)
				{
					if(lwo.uiTriCount==0)
					{
						MessageBox(NULL,L"Something is wrong.\nContact your "
							L"favorite programmer.",
							L"LWO2 import error!!!",MB_TASKMODAL);
						if(pTris)
						{
							delete [] pTris;
						}
						return FALSE;
					}
					LW_Triangle *pTmpTriangles=NULL;
					pTmpTriangles=new LW_Triangle[lwo.uiTriCount+uiTriCount];
#ifdef _DEBUG
					{
						wchar_t wszMsg[256];
						wsprintf(wszMsg,L"Allocated (5) %d bytes of memory for "
							L"LW_Triangle structures\n",
							(lwo.uiTriCount+uiTriCount)*sizeof(LW_Triangle));
						OutputDebugString(wszMsg);
					}
#endif
					memcpy(pTmpTriangles,lwo.pTriangles,lwo.uiTriCount*sizeof(LW_Triangle));
					memcpy(&pTmpTriangles[lwo.uiTriCount],pTris,uiTriCount*sizeof(LW_Triangle));
					delete [] lwo.pTriangles;
					delete [] pTris;
					lwo.pTriangles=pTmpTriangles;
					lwo.uiTriCount+=uiTriCount;
				}
				else
				{
					lwo.pTriangles=pTris;
					lwo.uiTriCount=uiTriCount;
				}
			}
			break;
		case 'CURV':
			MessageBox(NULL,L"Splines are not supported!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'PTCH':
			MessageBox(NULL,L"Subdivision patches are not supported!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'MBAL':
			MessageBox(NULL,L"Metaballs are not supported!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'BONE':
			MessageBox(NULL,L"Bones are not supported (YET!!!)!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		default:
			MessageBox(NULL,L"Unknown POLS subtype!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
	}
	return TRUE;
}

//read 'VMAP' chunk (starting from length)
BOOL readVMAP(FILE *f,LW_Object &lwo,UINT uiVertexOffset,UINT &nRead)
{
	UINT uiVMAPLength=0;
	nRead=0;
	if(lwo.uiVMapCount==MAX_VMAP_COUNT)
	{
		MessageBox(NULL,L"Too many VMAP-s in lwo file\n!!!",
			L"LWO2 import error!!",MB_TASKMODAL);
		return FALSE;
	}
	if(!readU4(f,&uiVMAPLength))
	{
		return FALSE;
	}
	nRead+=4;
	LW_ID4 szType;
	if(!readID4(f,szType))
	{
		return FALSE;
	}
	nRead+=4;
	USHORT usVMAPDimension=0;
	switch(*((int*)szType))
	{
		case 'PICK':
			//skip the rest
			fseek(f,uiVMAPLength-4,SEEK_CUR);
			nRead+=uiVMAPLength-4;
			return TRUE;
//			MessageBox(NULL,"PICK VMAP subchunk unsupported, TODO: skip","LWO2 import error!!",MB_TASKMODAL);
//			return FALSE;
			break;
		case 'WGHT':	//weightmap, verify that dimension is 1
			if(!readU2(f,&usVMAPDimension))
			{
				return FALSE;
			}
			nRead+=2;
			if(usVMAPDimension!=1)
			{
				MessageBox(NULL,L"Weight maps must have dimension of 1!!!",
					L"LWO2 import error!!",MB_TASKMODAL);
			}
			break;
		case 'MNVW':
			MessageBox(NULL,L"Subpatch weight maps are not supported!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'RGB ':
		case 'RGBA':
			MessageBox(NULL,L"Color maps are not supported!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'TXUV':	//uv map, verify that dimension is 2
			if(!readU2(f,&usVMAPDimension))
			{
				return FALSE;
			}
			nRead+=2;
			if(usVMAPDimension!=2)
			{
				MessageBox(NULL,L"UV maps must have dimension of 2!!!",
					L"LWO2 import error!!",MB_TASKMODAL);
			}
			break;
		case 'MORF':
			MessageBox(NULL,L"Vertex displacement delta maps are not "
				L"supported (YET!)",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'SPOT':
			MessageBox(NULL,L"Alternative vertex position maps are not "
				L"supported (YET!)",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		default:
			MessageBox(NULL,L"Unknown VMAP type, TODO:skip",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
	}
	//if nothing went wrong, read VMAP name
	UINT uiNameLen=0;
	if(!readLWString(f,lwo.VMaps[lwo.uiVMapCount].szName,uiNameLen))
	{
		return FALSE;
	}
	nRead+=uiNameLen;
	lwo.VMaps[lwo.uiVMapCount].usDimension=usVMAPDimension;
	memcpy(&lwo.VMaps[lwo.uiVMapCount].type,szType,sizeof(LW_ID4));
	UINT uiVMAPDataPos=0;
	UINT uiMaxEntries=0;
	switch(usVMAPDimension)
	{
		case 1:
			//first estimate maximum possible number of VMAP entries to speed
			//things up a little and avoid memory reallocation
			uiMaxEntries=(uiVMAPLength-uiNameLen-6)/(sizeof(float)+2);
			lwo.VMaps[lwo.uiVMapCount].uiEntryCount=0;
			if(lwo.VMaps[lwo.uiVMapCount].pVMAP1D)
			{
#ifdef _DEBUG
				OutputDebugString(L"LWO2 IMPORT (WARNING):Possibly "
					L"uninitialized/uncleaned LW_Object data!!!\n"
					L"Trying to free memory ...\n");
#endif
				delete [] lwo.VMaps[lwo.uiVMapCount].pVMAP1D;
			}
			lwo.VMaps[lwo.uiVMapCount].pVMAP1D=new LW_VMAPEntry1D[uiMaxEntries];	
#ifdef _DEBUG
			{
				wchar_t wszMsg[256];
				wsprintf(wszMsg,L"Allocated (6) %d bytes of memory for "
					L"LW_VMAPEntry1D structures\n",
					(uiMaxEntries)*sizeof(LW_VMAPEntry1D));
				OutputDebugString(wszMsg);
			}
#endif
			while((nRead-4)<uiVMAPLength)
			{
				if(lwo.VMaps[lwo.uiVMapCount].uiEntryCount==uiMaxEntries)
				{
					MessageBox(NULL,L"Unknown error reading LWO file.\n"
						L"Contact your favorite programmer.",L"LWO2 import error!!",MB_TASKMODAL);
					return FALSE;
				}
				//read single vertex data
				UINT uiVXSize=0;
				if(!readVX(
					f,
					lwo.VMaps[lwo.uiVMapCount].pVMAP1D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].uiVertexIndex,
					uiVXSize))
				{
					return FALSE;
				}
				nRead+=uiVXSize;
				lwo.VMaps[lwo.uiVMapCount].
					pVMAP1D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].uiVertexIndex+=uiVertexOffset;
				if(!readF4(
					f,
					&(lwo.VMaps[lwo.uiVMapCount].pVMAP1D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].fVal0)))
				{
					return FALSE;
				}
				nRead+=4;	//sizeof(float)
				lwo.VMaps[lwo.uiVMapCount].uiEntryCount++;
			}
			break;
		case 2:
			//first estimate maximum possible number of VMAP entries to speed
			//things up a little and avoid memory reallocation
			uiMaxEntries=(uiVMAPLength-uiNameLen-6)/(sizeof(float)*2+2);
			lwo.VMaps[lwo.uiVMapCount].uiEntryCount=0;
			if(lwo.VMaps[lwo.uiVMapCount].pVMAP2D)
			{
#ifdef _DEBUG
				OutputDebugString(L"LWO2 IMPORT (WARNING):"
					L"Possibly uninitialized/uncleaned LW_Object data!!!\nTrying to free memory ...\n");
#endif
				delete [] lwo.VMaps[lwo.uiVMapCount].pVMAP2D;
			}
			lwo.VMaps[lwo.uiVMapCount].pVMAP2D=new LW_VMAPEntry2D[uiMaxEntries];	
#ifdef _DEBUG
			{
				wchar_t wszMsg[256];
				wsprintf(wszMsg,L"Allocated (7) %d bytes of memory for "
					L"LW_VMAPEntry2D structures\n",
					(uiMaxEntries)*sizeof(LW_VMAPEntry2D));
				OutputDebugString(wszMsg);
			}
#endif
			while((nRead-4)<uiVMAPLength)
			{
				if(lwo.VMaps[lwo.uiVMapCount].uiEntryCount==uiMaxEntries)
				{
					MessageBox(NULL,L"Unknown error reading LWO file.\n"
						L"Contact your favorite programmer.",
						L"LWO2 import error!!",MB_TASKMODAL);
					return FALSE;
				}
				//read single vertex data
				UINT uiVXSize=0;
				if(!readVX(
					f,
					lwo.VMaps[lwo.uiVMapCount].pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].uiVertexIndex,
					uiVXSize))
				{
					return FALSE;
				}
				lwo.VMaps[lwo.uiVMapCount].
					pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].uiVertexIndex+=uiVertexOffset;
				nRead+=uiVXSize;
				if(!readF4(
					f,
					&(lwo.VMaps[lwo.uiVMapCount].pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].fVal0)))
				{
					return FALSE;
				}
				nRead+=4;	//sizeof(float)
				if(!readF4(
					f,
					&(lwo.VMaps[lwo.uiVMapCount].pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].fVal1)))
				{
					return FALSE;
				}
				lwo.VMaps[lwo.uiVMapCount].pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].fVal1=
					1.0f-lwo.VMaps[lwo.uiVMapCount].pVMAP2D[lwo.VMaps[lwo.uiVMapCount].uiEntryCount].fVal1;
				nRead+=4;	//sizeof(float)
				lwo.VMaps[lwo.uiVMapCount].uiEntryCount++;
			}
			break;
		default:
			MessageBox(NULL,L"Invalid (unsupported) VMAP dimension!!!",L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
	}
	lwo.uiVMapCount++;
	return TRUE;
}

//read 'VMAD' chunk (starting from length)
BOOL readVMAD(FILE *f,LW_Object &lwo,UINT uiVertexOffset,UINT uiTriangleOffset,UINT &nRead)
{
	UINT uiVMADLength=0;
	nRead=0;
	if(lwo.uiVMadCount==MAX_VMAD_COUNT)
	{
		MessageBox(NULL,L"Too many VMAD-s in lwo file\n!!!",L"LWO2 import error!!",MB_TASKMODAL);
		return FALSE;
	}
	if(!readU4(f,&uiVMADLength))
	{
		return FALSE;
	}
	nRead+=4;
	LW_ID4 szType;
	if(!readID4(f,szType))
	{
		return FALSE;
	}
	nRead+=4;
	USHORT usVMADDimension=0;
	switch(*((int*)szType))
	{
		case 'PICK':
			fseek(f,uiVMADLength-4,SEEK_CUR);
			nRead+=uiVMADLength-4;
			return TRUE;
//			MessageBox(NULL,"PICK VMAD subchunk unsupported, TODO: skip","LWO2 import error!!",MB_TASKMODAL);
//			return FALSE;
			break;
		case 'WGHT':	//weightmap, verify that dimension is 1
			if(!readU2(f,&usVMADDimension))
			{
				return FALSE;
			}
			nRead+=2;
			if(usVMADDimension!=1)
			{
				MessageBox(NULL,L"Weight maps must have dimension of 1!!!",
					L"LWO2 import error!!",MB_TASKMODAL);
			}
			break;
		case 'MNVW':
			MessageBox(NULL,L"Subpatch weight maps are not supported!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'RGB ':
		case 'RGBA':
			MessageBox(NULL,L"Color maps are not supported!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'TXUV':	//uv map, verify that dimension is 2
			if(!readU2(f,&usVMADDimension))
			{
				return FALSE;
			}
			nRead+=2;
			if(usVMADDimension!=2)
			{
				MessageBox(NULL,L"UV maps must have dimension of 2!!!",
					L"LWO2 import error!!",MB_TASKMODAL);
			}
			break;
		case 'MORF':
			MessageBox(NULL,L"Vertex displacement delta maps are not "
				L"supported (YET!)",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		case 'SPOT':
			MessageBox(NULL,L"Alternative vertex position maps are not "
				L"supported (YET!)",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
		default:
			MessageBox(NULL,L"Unknown VMAD type, TODO:skip",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
	}
	//if nothing went wrong, read VMAD name
	UINT uiNameLen=0;
	if(!readLWString(f,lwo.VMads[lwo.uiVMadCount].szName,uiNameLen))
	{
		return FALSE;
	}
	nRead+=uiNameLen;
	lwo.VMads[lwo.uiVMadCount].usDimension=usVMADDimension;
	memcpy(&lwo.VMads[lwo.uiVMadCount].type,szType,sizeof(LW_ID4));
	UINT uiVMADDataPos=0;
	UINT uiMaxEntries=0;
	switch(usVMADDimension)
	{
		case 1:
			//first estimate maximum possible number of VMAD entries to speed
			//things up a little and avoid memory reallocation
			uiMaxEntries=(uiVMADLength-uiNameLen-6)/(sizeof(float)+4);
			lwo.VMads[lwo.uiVMadCount].uiEntryCount=0;
			if(lwo.VMads[lwo.uiVMadCount].pVMAD1D)
			{
#ifdef _DEBUG
				OutputDebugString(L"LWO2 IMPORT (WARNING):"
					L"Possibly uninitialized/uncleaned LW_Object data!!!\nTrying to free memory ...\n");
#endif
				delete [] lwo.VMads[lwo.uiVMadCount].pVMAD1D;
			}
			lwo.VMads[lwo.uiVMadCount].pVMAD1D=new LW_VMADEntry1D[uiMaxEntries];	
#ifdef _DEBUG
			{
				wchar_t wszMsg[256];
				wsprintf(wszMsg,L"Allocated (8) %d bytes of memory for "
					L"LW_VMADEntry1D structures\n",
					(uiMaxEntries)*sizeof(LW_VMADEntry1D));
				OutputDebugString(wszMsg);
			}
#endif
			while((nRead-4)<uiVMADLength)
			{
				if(lwo.VMads[lwo.uiVMadCount].uiEntryCount==uiMaxEntries)
				{
					MessageBox(NULL,L"Unknown error reading LWO file.\n"
						L"Contact your favorite programmer.",L"LWO2 import error!!",MB_TASKMODAL);
					return FALSE;
				}
				//read single vertex data
				UINT uiVXSize=0;
				if(!readVX(
					f,
					lwo.VMads[lwo.uiVMadCount].pVMAD1D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiVertexIndex,
					uiVXSize))
				{
					return FALSE;
				}
				lwo.VMads[lwo.uiVMadCount].
					pVMAD1D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiVertexIndex+=uiVertexOffset;
				nRead+=uiVXSize;
				if(!readVX(
					f,
					lwo.VMads[lwo.uiVMadCount].pVMAD1D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiTriangleIndex,
					uiVXSize))
				{
					return FALSE;
				}
				lwo.VMads[lwo.uiVMadCount].
					pVMAD1D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiTriangleIndex+=uiTriangleOffset;
				nRead+=uiVXSize;
				if(!readF4(
					f,
					&(lwo.VMads[lwo.uiVMadCount].pVMAD1D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].fVal0)))
				{
					return FALSE;
				}
				nRead+=4;	//sizeof(float)
				lwo.VMads[lwo.uiVMadCount].uiEntryCount++;
			}
			break;
		case 2:
			//first estimate maximum possible number of VMAD entries to speed
			//things up a little and avoid memory reallocation
			uiMaxEntries=(uiVMADLength-uiNameLen-6)/(sizeof(float)*2+4);
			lwo.VMads[lwo.uiVMadCount].uiEntryCount=0;
			if(lwo.VMads[lwo.uiVMadCount].pVMAD2D)
			{
#ifdef _DEBUG
				OutputDebugString(L"LWO2 IMPORT (WARNING):"
					L"Possibly uninitialized/uncleaned LW_Object data!!!\nTrying to free memory ...\n");
#endif
				delete [] lwo.VMads[lwo.uiVMadCount].pVMAD1D;
			}
			lwo.VMads[lwo.uiVMadCount].pVMAD2D=new LW_VMADEntry2D[uiMaxEntries];	
#ifdef _DEBUG
			{
				wchar_t wszMsg[256];
				wsprintf(wszMsg,L"Allocated (9) %d bytes of memory for "
					L"LW_VMADEntry2D structures\n",
					(uiMaxEntries)*sizeof(LW_VMADEntry2D));
				OutputDebugString(wszMsg);
			}
#endif
			while((nRead-4)<uiVMADLength)
			{
				if(lwo.VMads[lwo.uiVMadCount].uiEntryCount==uiMaxEntries)
				{
					MessageBox(NULL,L"Unknown error reading LWO file.\n"
						L"Contact your favorite programmer.",
						L"LWO2 import error!!",MB_TASKMODAL);
					return FALSE;
				}
				//read single vertex data
				UINT uiVXSize=0;
				if(!readVX(
					f,
					lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiVertexIndex,
					uiVXSize))
				{
					return FALSE;
				}
				lwo.VMads[lwo.uiVMadCount].
					pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiVertexIndex+=uiVertexOffset;
				nRead+=uiVXSize;
				if(!readVX(
					f,
					lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiTriangleIndex,
					uiVXSize))
				{
					return FALSE;
				}
				lwo.VMads[lwo.uiVMadCount].
					pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].uiTriangleIndex+=uiTriangleOffset;
				nRead+=uiVXSize;
				if(!readF4(
					f,
					&(lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].fVal0)))
				{
					return FALSE;
				}
				nRead+=4;	//sizeof(float)
				if(!readF4(
					f,
					&(lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].fVal1)))
				{
					return FALSE;
				}
				lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].fVal1=
					1.0f-lwo.VMads[lwo.uiVMadCount].pVMAD2D[lwo.VMads[lwo.uiVMadCount].uiEntryCount].fVal1;
				nRead+=4;	//sizeof(float)
				lwo.VMads[lwo.uiVMadCount].uiEntryCount++;
			}
			break;
		default:
			MessageBox(NULL,L"Invalid (unsupported) VMAD dimension!!!",
				L"LWO2 import error!!",MB_TASKMODAL);
			return FALSE;
			break;
	}
	lwo.uiVMadCount++;
	return TRUE;
}

//read 'TAGS' chunk
BOOL readTAGS(FILE *f,LW_Object &lwo,UINT &nRead)
{
	UINT uiTAGSLength=0;
	nRead=0;
	if(!readU4(f,&uiTAGSLength))
	{
		return FALSE;
	}
	nRead+=4;
	if(lwo.uiTagCount!=0)
	{
		MessageBox(NULL,
			L"More than one TAGS chunk encountered!!!.\nThings are likely to "
			L"break down!!!",
			L"LWO import warning",MB_TASKMODAL);
	}
	do
	{
		if(lwo.uiTagCount==MAX_TAG_COUNT)
		{
			return FALSE;
		}
		UINT uiStrLen=0;
		if(!readLWString(f,lwo.Tags[lwo.uiTagCount],uiStrLen))
		{
			return FALSE;
		}
		nRead+=uiStrLen;
		lwo.uiTagCount++;
	} while ((nRead-4)<uiTAGSLength);
	return TRUE;
}

//read 'PTAG' chunk
BOOL readPTAG(FILE *f,LW_Object &lwo,UINT uiTriangleOffset,UINT &nRead)
{
	UINT uiPTAGLength=0;
	nRead=0;
	if(!readU4(f,&uiPTAGLength))
	{
		return FALSE;
	}
	nRead+=4;
	LW_ID4 szID;
	if(!readID4(f,szID))
	{
		return FALSE;
	}
	nRead+=4;
	switch((*((int*)szID)))
	{
		case 'SURF':
			do
			{
				//read poly index
				UINT uiTriangle=0;
				UINT uiVXSize=0;
				if(!readVX(f,uiTriangle,uiVXSize))
				{
					return FALSE;
				}
				nRead+=uiVXSize;
				uiTriangle+=uiTriangleOffset;
				//read surface index
				USHORT usSurfIndex=0;
				if(!readU2(f,&usSurfIndex))
				{
					return FALSE;
				}
				nRead+=2;
				lwo.pTriangles[uiTriangle].usSurface=usSurfIndex;
			} while((nRead-4)<uiPTAGLength);
			break;
		default:
			//skip the rest
			fseek(f,uiPTAGLength-4,SEEK_CUR);
			nRead+=uiPTAGLength-4;
			break;
	}
	return TRUE;
}

//read 'ENVL' chunk
BOOL readENVL(FILE *f,LW_Object &lwo,UINT &nRead)
{
	UINT uiENVLLength=0;
	nRead=0;
	if(lwo.uiEnvelopeCount==MAX_ENVELOPE_COUNT)
	{
		return FALSE;
	}
	if(!readU4(f,&uiENVLLength))
	{
		return FALSE;
	}
	nRead+=4;
	//read VX index
	UINT uiENVIndex=0;
	UINT uiVXSize=0;
	if(!readVX(f,lwo.Envelopes[lwo.uiEnvelopeCount].uiIndex,uiVXSize))
	{
		return FALSE;
	}
	nRead+=uiVXSize;
	lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount=0;
	lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize=0;
	do {
		//read ENVL subchunk
		LW_ID4 szSubChunkID;
		USHORT usSubChunkLength=0;
		if(!readID4(f,szSubChunkID))
		{
			return FALSE;
		}
		nRead+=4;
		if(!readU2(f,&usSubChunkLength))
		{
			return FALSE;
		}
		nRead+=2;
		switch(*((int*)szSubChunkID))
		{
			case 'TYPE':
				//just skip
				fseek(f,2,SEEK_CUR);
				nRead+=2;
				break;
			case 'PRE ':
				if(!readU2(f,&lwo.Envelopes[lwo.uiEnvelopeCount].usPreBehavior))
				{
					return FALSE;
				}
				nRead+=2;
				break;
			case 'POST':
				if(!readU2(f,&lwo.Envelopes[lwo.uiEnvelopeCount].usPostBehavior))
				{
					return FALSE;
				}
				nRead+=2;
				break;
			case 'KEY ':
				if(lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount==lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize)
				{
					//add and init new key
					if(lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount==0) {
						lwo.Envelopes[lwo.uiEnvelopeCount].pKeys=new LW_EnvelopeKey[20];
#ifdef _DEBUG
						{
							wchar_t wszMsg[256];
							wsprintf(wszMsg,L"Allocated (10) %d bytes of memory "
								L"for LW_EnvelopeKey structures\n",
								20*sizeof(LW_EnvelopeKey));
							OutputDebugString(wszMsg);
						}
#endif
						ZeroMemory(lwo.Envelopes[lwo.uiEnvelopeCount].pKeys,20*sizeof(LW_EnvelopeKey));
						lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize=20;
						lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount++;
					}
					else
					{
						LW_EnvelopeKey	*pTempKeys=NULL;
						pTempKeys=new LW_EnvelopeKey[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize*2];
#ifdef _DEBUG
						{
							wchar_t wszMsg[256];
							wsprintf(wszMsg,L"Allocated (11) %d bytes of memory "
								L"for LW_EnvelopeKey structures\n",
								lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize*2*sizeof(LW_EnvelopeKey));
							OutputDebugString(wszMsg);
						}
#endif
						ZeroMemory(lwo.Envelopes[lwo.uiEnvelopeCount].pKeys,
							lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize*2*sizeof(LW_EnvelopeKey));
						memcpy(pTempKeys,lwo.Envelopes[lwo.uiEnvelopeCount].pKeys,
							lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize*sizeof(LW_EnvelopeKey));
						lwo.Envelopes[lwo.uiEnvelopeCount].iKeyTblSize*=2;
						lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount++;
						delete [] lwo.Envelopes[lwo.uiEnvelopeCount].pKeys;
						lwo.Envelopes[lwo.uiEnvelopeCount].pKeys=pTempKeys;
					}
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam0=0.0f;
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam1=0.0f;
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam2=0.0f;
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam3=0.0f;
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fTime=0.0f;
					lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fValue=0.0f;
				}
				//read key
				if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
					pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fTime)))
				{
					return FALSE;
				}
				nRead+=4;
				if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
					pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fValue)))
				{
					return FALSE;
				}
				nRead+=4;
				break;
			case 'SPAN':
				{
					int iTmp=0;
					if(!readID4(f,lwo.Envelopes[lwo.uiEnvelopeCount].
						pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].SpanType))
					{
						return FALSE;
					}
					nRead+=4;
					for(iTmp=0;iTmp<(usSubChunkLength/4-1);iTmp++)
					{
						switch(iTmp)
						{
							case 0:
								if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
									pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam0)))
								{
									return FALSE;
								}
								nRead+=4;
								break;
							case 1:
								if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
									pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam1)))
								{
									return FALSE;
								}
								nRead+=4;
								break;
							case 2:
								if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
									pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam2)))
								{
									return FALSE;
								}
								nRead+=4;
								break;
							case 3:
								if(!readF4(f,&(lwo.Envelopes[lwo.uiEnvelopeCount].
									pKeys[lwo.Envelopes[lwo.uiEnvelopeCount].iKeyCount-1].fSpanParam3)))
								{
									return FALSE;
								}
								nRead+=4;
								break;
							default:
								MessageBox(NULL,L"Too big SPAN envelope subchunk!!!",
									L"LWO import error!!!",MB_TASKMODAL);
								return FALSE;
								break;
						}
					}
				}
				break;
			case 'CHAN':
			case 'NAME':
				//just skip
				fseek(f,usSubChunkLength,SEEK_CUR);
				nRead+=usSubChunkLength;
				break;
			default:
				MessageBox(NULL,L"Unknown envelope subchunk!!!!",
					L"LWO import error!!!",
					MB_TASKMODAL);
				return FALSE;
				break;
		}
	} while((nRead-4)<uiENVLLength);
	lwo.uiEnvelopeCount++;
	return TRUE;
}

//read 'CLIP' chunk
BOOL readCLIP(FILE *f,LW_Object &lwo,UINT &nRead)
{
	UINT uiCLIPLength=0;
	nRead=0;
	if(lwo.uiClipCount==MAX_CLIP_COUNT)
	{
		return FALSE;
	}
	if(!readU4(f,&uiCLIPLength))
	{
		return FALSE;
	}
	nRead+=4;
	//read U4 index
	UINT uiCLIPIndex=0;
	if(!readU4(f,&lwo.Clips[lwo.uiClipCount].uiClipIndex))
	{
		return FALSE;
	}
	nRead+=4;
	BOOL bSTILFound=FALSE;
	while((nRead-4)<uiCLIPLength)
	{
		//read subchunk
		LW_ID4	szSubChunkID;
		USHORT	usSubChunkLength=0;
		if(!readID4(f,szSubChunkID))
		{
			return FALSE;
		}
		nRead+=4;
		if(!readU2(f,&usSubChunkLength))
		{
			return FALSE;
		}
		nRead+=2;
		UINT uiTmpStringSize=0;		//not used but we need to pass something to readLWString()
		switch(*((int*)szSubChunkID))
		{
			case 'STIL':
				bSTILFound=TRUE;
				if(!readLWString(f,lwo.Clips[lwo.uiClipCount].szFileName,uiTmpStringSize))
				{
					return FALSE;
				}
				break;
			default:
				//just skip it
				fseek(f,usSubChunkLength,SEEK_CUR);
				break;
		}
		nRead+=usSubChunkLength;
	}
	if(!bSTILFound)
	{
		return FALSE;
	}
	lwo.uiClipCount++;
	return TRUE;
}

//read 'BLOK' surface subchunk
BOOL readBLOK(FILE *f,LW_SurfaceBlock &block,USHORT usBlockSize)
{
	USHORT usBlockBytesRead=0;
	if(!readID4(f,block.BlockHeader.Type))
	{
		return FALSE;
	}
	usBlockBytesRead+=4;
	if(*((int*)block.BlockHeader.Type)!='IMAP')
	{
		//skip it, for now we only care about IMAP blocks
		fseek(f,usBlockSize-4,SEEK_CUR);
		return TRUE;
	}
	else {
		//first read common header data
		USHORT usBlockHeaderSize=0;
		if(!readU2(f,&usBlockHeaderSize))
		{
			return FALSE;
		}
		usBlockBytesRead+=2;
		USHORT usBlockHeaderBytesRead=0;
		UINT uiTmpSize=0;
		if(!readLWString(f,block.BlockHeader.szOrdinal,uiTmpSize))
		{
			return FALSE;
		}
		usBlockHeaderBytesRead+=uiTmpSize;
		LW_ID4	szTmpID;
		while(usBlockHeaderBytesRead<usBlockHeaderSize)
		{
			if(!readID4(f,szTmpID))
			{
				return FALSE;
			}
			usBlockHeaderBytesRead+=4;
			USHORT usSubChunkLength=0;
			if(!readU2(f,&usSubChunkLength))
			{
				return FALSE;
			}
			usBlockHeaderBytesRead+=2;
			UINT	uiVXSize=0;
			switch(*((int*)szTmpID))
			{
				case 'CHAN':
					if(!readID4(f,block.BlockHeader.TextureChannel))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=4;
					break;
				case 'ENAB':
					if(!readU2(f,&block.BlockHeader.usEnable))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=2;
					break;
				case 'OPAC':
					if(!readU2(f,&block.BlockHeader.usOpacityType))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=2;
					if(!readF4(f,&block.BlockHeader.fOpacity))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=4;
					if(!readVX(f,block.BlockHeader.uiOpacityEnvIndex,uiVXSize))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=uiVXSize;
					break;
				case 'NEGA':
					if(!readU2(f,&block.BlockHeader.usNegative))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=2;
					break;
				case 'AXIS':
					if(!readU2(f,&block.BlockHeader.usDisplacementAxis))
					{
						return FALSE;
					}
					usBlockHeaderBytesRead+=2;
					break;
				default:
					MessageBox(NULL,L"Unknown BLOK header subchunk!!!!",
						L"LWO2 import error!!!",MB_TASKMODAL);
					return FALSE;
					break;
			}
		}
		usBlockBytesRead+=usBlockHeaderSize;
		//now read other stuff like TMAP
		while(usBlockBytesRead<usBlockSize)
		{
			if(!readID4(f,szTmpID))
			{
				return FALSE;
			}
			usBlockBytesRead+=4;
			USHORT usSubChunkLength=0;
			if(!readU2(f,&usSubChunkLength))
			{
				return FALSE;
			}
			usBlockBytesRead+=2;
			switch(*((int*)szTmpID))
			{
				case 'TMAP':				//TMAP subchunk
					{

						USHORT usTMAPBytesRead=0;
						while(usTMAPBytesRead<usSubChunkLength)
						{
							LW_ID4 szSubChunkID;
							if(!readID4(f,szSubChunkID))
							{
								return FALSE;
							}
							usTMAPBytesRead+=4;
							USHORT usTMAPSubChunkLength=0;
							if(!readU2(f,&usTMAPSubChunkLength))
							{
								return FALSE;
							}
							usTMAPBytesRead+=2;
							switch(*((int*)szSubChunkID))
							{
								case 'CNTR':
									{
										USHORT usTMAPBytesReadStart=usTMAPBytesRead;
										block.TMAP.bCenterPresent=TRUE;
										if(!readF4(f,&block.TMAP.Center.x))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Center.y))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Center.z))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readVX(f,block.TMAP.uiCenterEnvIndex,uiTmpSize))
										{
											return FALSE;
										}
										usTMAPBytesRead+=uiTmpSize;
										if(usTMAPSubChunkLength>(usTMAPBytesRead-usTMAPBytesReadStart))
										{
											fseek(f,usTMAPSubChunkLength-(usTMAPBytesRead-usTMAPBytesReadStart),
												SEEK_CUR);
											usTMAPBytesRead+=usTMAPSubChunkLength-
												(usTMAPBytesRead-usTMAPBytesReadStart);
										}
									}
									break;
								case 'SIZE':
									{
										USHORT usTMAPBytesReadStart=usTMAPBytesRead;
										block.TMAP.bSizePresent=TRUE;
										if(!readF4(f,&block.TMAP.Size.x))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Size.y))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Size.z))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readVX(f,block.TMAP.uiSizeEnvIndex,uiTmpSize))
										{
											return FALSE;
										}
										usTMAPBytesRead+=uiTmpSize;
										if(usTMAPSubChunkLength>(usTMAPBytesRead-usTMAPBytesReadStart))
										{
											fseek(f,usTMAPSubChunkLength-(usTMAPBytesRead-usTMAPBytesReadStart),SEEK_CUR);
											usTMAPBytesRead+=usTMAPSubChunkLength-
												(usTMAPBytesRead-usTMAPBytesReadStart);
										}
										usTMAPBytesRead+=uiTmpSize;
									}
									break;
								case 'ROTA':
									{
										USHORT usTMAPBytesReadStart=usTMAPBytesRead;
										block.TMAP.bRotationPresent=TRUE;
										if(!readF4(f,&block.TMAP.Rotation.x))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Rotation.y))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readF4(f,&block.TMAP.Rotation.z))
										{
											return FALSE;
										}
										usTMAPBytesRead+=4;
										if(!readVX(f,block.TMAP.uiRotationEnvIndex,uiTmpSize))
										{
											return FALSE;
										}
										usTMAPBytesRead+=uiTmpSize;
										usTMAPBytesRead+=uiTmpSize;
										if(usTMAPSubChunkLength>(usTMAPBytesRead-usTMAPBytesReadStart))
										{
											fseek(f,usTMAPSubChunkLength-(usTMAPBytesRead-usTMAPBytesReadStart),SEEK_CUR);
											usTMAPBytesRead+=usTMAPSubChunkLength-
												(usTMAPBytesRead-usTMAPBytesReadStart);
										}
									}
									break;
								case 'OREF':
									if(!readLWString(f,block.TMAP.szReferenceObject,uiTmpSize))
									{
										return FALSE;
									}
									usTMAPBytesRead+=uiTmpSize;
									break;
								case 'FALL':
									block.TMAP.bFalloffPresent=TRUE;
									if(!readU2(f,&block.TMAP.usFalloffType))
									{
										return FALSE;
									}
									usTMAPBytesRead+=2;
									if(!readF4(f,&block.TMAP.Falloff.x))
									{
										return FALSE;
									}
									usTMAPBytesRead+=4;
									if(!readF4(f,&block.TMAP.Falloff.y))
									{
										return FALSE;
									}
									usTMAPBytesRead+=4;
									if(!readF4(f,&block.TMAP.Falloff.z))
									{
										return FALSE;
									}
									usTMAPBytesRead+=4;
									if(!readVX(f,block.TMAP.uiFalloffEnvIndex,uiTmpSize))
									{
										return FALSE;
									}
									usTMAPBytesRead+=uiTmpSize;
									break;
								case 'CSYS':
									block.TMAP.bCoordinateSystemPresent=TRUE;
									if(!readU2(f,&block.TMAP.usCoordinateSystem))
									{
										return FALSE;
									}
									usTMAPBytesRead+=2;
									break;
								default:
									//unknown subchunk - skip it
									fseek(f,usTMAPSubChunkLength,SEEK_CUR);
									usTMAPBytesRead+=usTMAPSubChunkLength;
									break;
							}
						}
					}
					break;
				case 'PROJ':
					if(!readU2(f,&block.usProjectionMode))
					{
						return FALSE;
					}
					break;
				case 'AXIS':
					if(!readU2(f,&block.usTextureAxis))
					{
						return FALSE;
					}
					break;
				case 'IMAG':
					if(!readVX(f,block.uiImageMapIndex,uiTmpSize))
					{
						return FALSE;
					}
					break;
				case 'WRAP':
					if(!readU2(f,&block.usImageWrapOptionsW))
					{
						return FALSE;
					}
					if(!readU2(f,&block.usImageWrapOptionsH))
					{
						return FALSE;
					}
					break;
				case 'WRPW':
					if(!readF4(f,&block.fWrapW))
					{
						return FALSE;
					}
					if(!readVX(f,block.uiWrapWEnvIndex,uiTmpSize))
					{
						return FALSE;
					}
					break;
				case 'WRPH':
					if(!readF4(f,&block.fWrapH))
					{
						return FALSE;
					}
					if(!readVX(f,block.uiWrapHEnvIndex,uiTmpSize))
					{
						return FALSE;
					}
					break;
				case 'VMAP':
					if(!readLWString(f,block.szVMapName,uiTmpSize))
					{
						return FALSE;
					}
					break;
				case 'AAST':
					if(!readU2(f,&block.usAntialiasingStrengthOptions))
					{
						return FALSE;
					}
					if(!readF4(f,&block.fAntialiasingStrength))
					{
						return FALSE;
					}
					break;
				case 'PIXB':
					if(!readU2(f,&block.usPixelBlendingFlags))
					{
						return FALSE;
					}
					break;
				case 'STCK':
					if(!readU2(f,&block.usStickyProjection))
					{
						return FALSE;
					}
					if(!readF4(f,&block.fStickyProjectionTime))
					{
						return FALSE;
					}
					break;
				case 'TAMP':
					if(!readF4(f,&block.fTextureAmplitude))
					{
						return FALSE;
					}
					if(!readVX(f,block.uiTextureAmplitudeEnvIndex,uiTmpSize))
					{
						return FALSE;
					}
					break;
				default:
					//unknown chunk, skip it
					fseek(f,usSubChunkLength,SEEK_CUR);
					break;

			}
			usBlockBytesRead+=usSubChunkLength;
		}
	}
	return TRUE;
}

//read 'SURF' chunk
BOOL readSURF(FILE *f,LW_Object &lwo,UINT &nRead)
{
	UINT uiSURFLength=0;
	nRead=0;
	if(lwo.uiSurfaceCount==MAX_SURFACE_COUNT)
	{
		return FALSE;
	}
	if(!readU4(f,&uiSURFLength))
	{
		return FALSE;
	}
	nRead+=4;
	UINT uiTmpLength=0;
	//name and source
	if(!readLWString(f,lwo.Surfaces[lwo.uiSurfaceCount].szName,uiTmpLength))
	{
		return FALSE;
	}
	nRead+=uiTmpLength;
	if(!readLWString(f,lwo.Surfaces[lwo.uiSurfaceCount].szSource,uiTmpLength))
	{
		return FALSE;
	}
	nRead+=uiTmpLength;
	//now read all subchunks
	while((nRead-4)<uiSURFLength)
	{
		LW_ID4	szSubChunkID;
		USHORT	usSubChunkLength=0;
		if(!readID4(f,szSubChunkID))
		{
			return FALSE;
		}
		nRead+=4;
		if(!readU2(f,&usSubChunkLength))
		{
			return FALSE;
		}
		nRead+=2;
		UINT uiVXSize=0;
		switch(*((int*)szSubChunkID))
		{
			case 'COLR':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fColorR))
				{
					return FALSE;
				}
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fColorG))
				{
					return FALSE;
				}
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fColorB))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiColorEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'DIFF':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fDiffuse))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiDiffuseEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'LUMI':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fLuminosity))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiLuminosityEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'SPEC':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fSpecular))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiSpecularEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'REFL':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fReflection))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiReflectionEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'TRAN':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fTransparency))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiTransparencyEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'TRNL':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fTranslucency))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiTranslucencyEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'GLOS':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fGlossiness))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiGlossinessEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'SHRP':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fDiffuseSharpness))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiDiffuseSharpnessEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'BUMP':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fBumpIntensity))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiBumpIntensityEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'SIDE':
				if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usPolySideness))
				{
					return FALSE;
				}
				break;
			case 'SMAN':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fMaxSmoothingAngle)) {
					return FALSE;
				}
				break;
			case 'RFOP':
				if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usReflectionOptions))
				{
					return FALSE;
				}
				break;
			case 'RIMG':
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiReflectionMapImageIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'RSAN':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fReflectionMapSeamAngle))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiReflectionMapSeamAngleEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'RBLR':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fReflectionBlurring))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiReflectionBlurringEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'RIND':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRefractiveIndex))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiRefractiveIndexEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'TROP':
				if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usTransparencyOptions))
				{
					return FALSE;
				}
				break;
			case 'TIMG':
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiRefractionMapImageIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'TBLR':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRefractionBlurring))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiRefractionBlurringEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'CLRH':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fColorHighlights))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiColorHighlightsEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'CLRF':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fColorFilter))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiColorFilterEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'ADTR':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fAdditiveTransparency))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiAdditiveTransparencyEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'GLOW':
				if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usGlowType))
				{
					return FALSE;
				}
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fGlowIntensity))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiGlowIntensityEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fGlowSize))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiGlowSizeEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'LINE':
				//this one is a bit more complicated because it can have multiple forms
				{
					USHORT usLineBytesRead=0;
					lwo.Surfaces[lwo.uiSurfaceCount].bRenderOutlines=TRUE;
					if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usRenderOutlinesFlags))
					{
						return FALSE;
					}
					usLineBytesRead+=2;
					if(usLineBytesRead<usSubChunkLength)
					{
						lwo.Surfaces[lwo.uiSurfaceCount].bRenderOutlinesSize=TRUE;
						if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRenderOutlinesSize))
						{
							return FALSE;
						}
						usLineBytesRead+=4;
						if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiRenderOutlinesSizeEnvIndex,uiVXSize))
						{
							return FALSE;
						}
						usLineBytesRead+=(USHORT)uiVXSize;
					}
					if(usLineBytesRead<usSubChunkLength)
					{
						lwo.Surfaces[lwo.uiSurfaceCount].bRenderOutlinesColor=TRUE;
						if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRenderOutlinesColorR))
						{
							return FALSE;
						}
						if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRenderOutlinesColorG))
						{
							return FALSE;
						}
						if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fRenderOutlinesColorB))
						{
							return FALSE;
						}
						if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiRenderOutlinesColorEnvIndex,uiVXSize))
						{
							return FALSE;
						}
					}
				}
				break;
			case 'ALPH':
				if(!readU2(f,&lwo.Surfaces[lwo.uiSurfaceCount].usAlphaMode))
				{
					return FALSE;
				}
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fAlphaValue))
				{
					return FALSE;
				}
				break;
			case 'VCOL':
				if(!readF4(f,&lwo.Surfaces[lwo.uiSurfaceCount].fVertexColorMapIntensity))
				{
					return FALSE;
				}
				if(!readVX(f,lwo.Surfaces[lwo.uiSurfaceCount].uiVertexColorMapIntensityEnvIndex,uiVXSize))
				{
					return FALSE;
				}
				if(!readID4(f,lwo.Surfaces[lwo.uiSurfaceCount].szVMapType))
				{
					return FALSE;
				}
				if(!readLWString(f,lwo.Surfaces[lwo.uiSurfaceCount].szVMapName,uiVXSize))
				{
					return FALSE;
				}
				break;
			case 'BLOK':
				{
					if(lwo.Surfaces[lwo.uiSurfaceCount].uiSurfaceBlockCount==MAX_SURFACE_BLOK_COUNT)
					{
						MessageBox(NULL,L"MAX_SURFACE_BLOK_COUNT reached!!!\n Contact your favorite programmer!!!",
							L"LWO Import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					if(!readBLOK(f,
						lwo.Surfaces[lwo.uiSurfaceCount].
						SurfaceBlocks[lwo.Surfaces[lwo.uiSurfaceCount].uiSurfaceBlockCount],
						usSubChunkLength))
					{
						return FALSE;
					}
					lwo.Surfaces[lwo.uiSurfaceCount].uiSurfaceBlockCount++;
				}
				break;
			default:
				//skip it
				break;
		}
		nRead+=usSubChunkLength;
	}
	lwo.uiSurfaceCount++;
	return TRUE;
}

//read 'LWO2' chunk (starting from length)
BOOL readLWO2(FILE *f,LW_Object &lwo,UINT uiLWO2Length)
{
	UINT nRead=0;
	LW_Layer CurrentLayer;
	UINT uiCurrentPNTSOffset=0;
	UINT uiCurrentPOLSOffset=0;
	ZeroMemory(&CurrentLayer,sizeof(LW_Layer));
	do
	{
		LW_ID4	szChunkID;
		ZeroMemory(szChunkID,sizeof(LW_ID4));
		if(!readID4(f,szChunkID))
		{
			return FALSE;
		}
		nRead+=4;
		switch(*((int*)szChunkID))
		{
			case 'LAYR':
				{
					UINT uiLayerBytesRead=0;
					if(lwo.uiPointCount)
					{
						MessageBox(NULL,L"Multiple layers are not supported",
							L"LWO2 import error",MB_TASKMODAL);
						return FALSE;
					}
					if(!readLAYR(f,CurrentLayer,uiLayerBytesRead))
					{
						nRead+=uiLayerBytesRead;
						return FALSE;
					}
					nRead+=uiLayerBytesRead;
				}
				break;
			case 'PNTS':
				{
					UINT uiPNTSBytesRead=0;
					if(!readPNTS(f,lwo,CurrentLayer,uiCurrentPNTSOffset,uiPNTSBytesRead))
					{
						nRead+=uiPNTSBytesRead;
						return FALSE;
					}
					nRead+=uiPNTSBytesRead;
				}
				break;
			case 'POLS':
				{
					UINT uiPOLSBytesRead=0;
					if(!readPOLS(f,lwo,uiCurrentPNTSOffset,uiCurrentPOLSOffset,uiPOLSBytesRead))
					{
						nRead+=uiPOLSBytesRead;
						return FALSE;
					}
					nRead+=uiPOLSBytesRead;
				}
				break;
			case 'VMAP':
				{
					UINT uiVMAPBytesRead=0;
					if(!readVMAP(f,lwo,uiCurrentPNTSOffset,uiVMAPBytesRead))
					{
						nRead+=uiVMAPBytesRead;
						return FALSE;
					}
					nRead+=uiVMAPBytesRead;
				}
				break;
			case 'TAGS':
				{
					UINT uiTAGSBytesRead=0;
					if(!readTAGS(f,lwo,uiTAGSBytesRead))
					{
						nRead+=uiTAGSBytesRead;
						return FALSE;
					}
					nRead+=uiTAGSBytesRead;
				}
				break;
			case 'PTAG':
				{
					UINT uiPTAGBytesRead=0;
					if(!readPTAG(f,lwo,uiCurrentPOLSOffset,uiPTAGBytesRead))
					{
						nRead+=uiPTAGBytesRead;
						return FALSE;
					}
					nRead+=uiPTAGBytesRead;
				}
				break;
			case 'VMAD':
				{
					UINT uiVMADBytesRead=0;
					if(!readVMAD(f,lwo,uiCurrentPNTSOffset,uiCurrentPOLSOffset,uiVMADBytesRead))
					{
						nRead+=uiVMADBytesRead;
						return FALSE;
					}
					nRead+=uiVMADBytesRead;
				}
				break;
			case 'ENVL':
				{
					UINT uiENVLBytesRead=0;
					if(!readENVL(f,lwo,uiENVLBytesRead))
					{
						nRead+=uiENVLBytesRead;
						return FALSE;
					}
					nRead+=uiENVLBytesRead;
				}
				break;
			case 'CLIP':
				{
					UINT uiCLIPBytesRead=0;
					if(!readCLIP(f,lwo,uiCLIPBytesRead))
					{
						nRead+=uiCLIPBytesRead;
						return FALSE;
					}
					nRead+=uiCLIPBytesRead;
				}
				break;
			case 'SURF':
				{
					UINT uiSURFBytesRead=0;
					if(!readSURF(f,lwo,uiSURFBytesRead))
					{
						nRead+=uiSURFBytesRead;
						return FALSE;
					}
					nRead+=uiSURFBytesRead;
				}
				break;
			default:		//skip chunk, we don't need it
				{
					UINT uiChunkLength=0;
					if(!readU4(f,&uiChunkLength))
					{
						return FALSE;
					}
					nRead+=4;
					fseek(f,uiChunkLength,SEEK_CUR);
					nRead+=uiChunkLength;
				}
				break;
		}
	} while(nRead<uiLWO2Length);
	return TRUE;
}

/********************************************************************************
 Second stage mesh processing
 Here is what real importer work is done: finding smoothing groups,
 generating normals and tangents, dividing mesh by surface,
 fixing npatch seam edges, duplicating vertices on uv seams ...
 and finally we get something that can be used to generate
 3d engine readable data
 All data structures in this part are prefixed with LWIMP_ (from LW IMPorter) 
 ********************************************************************************/

//uv set limit is set at 8 by Direct3D
//this is per-surface limit, not per-object!!!
//the limit is actually 7 because of how tangent vector is stored :(
#define MAX_UV_SETS 8

struct LWIMP_Vector
{
	float	x;
	float	y;
	float	z;
};

struct LWIMP_Texcoord
{
	float	s;
	float	t;
	float	r;
	UINT	uiUVSetID;		//UVSetID - index into UVSetDescTbl in LWIMP_Object
};

struct LWIMP_VMAPDesc
{
	UINT	uiVMAPIndex;	//index into LW_Object's VMAP tbl
	UINT	uiEntryIndex;	//index into specific VMAP entry;
};

struct LWIMP_Vertex
{
	UINT							uiPositionIndex;			//index to LW_Object.PNTS table
	LWIMP_Vector					vNormal;					//normal vector
	LWIMP_Vector					vTangent;					//tangent vector
	float							Blendweights[4];			//vertex blend weights
	DWORD							dwBlendIndices;				//four bone indices packed into DWORD
	LWIMP_Texcoord					Texcoords[MAX_UV_SETS];		//texture coordinates
	std::vector<int>				FaceList;					//list of faces using this vertex (indices)
	std::vector<LWIMP_VMAPDesc>		VMAPDescList;				//needed to identify LW_Object's VMAP in
																//final import stage
	LWIMP_Vertex& operator=(const LWIMP_Vertex &rfVertex)
	{
		uiPositionIndex=rfVertex.uiPositionIndex;
		vNormal=rfVertex.vNormal;
		vTangent=rfVertex.vTangent;
		UINT i=0;
		for(i=0;i<4;i++)
		{
			Blendweights[i]=rfVertex.Blendweights[i];
		}
		dwBlendIndices=rfVertex.dwBlendIndices;
		for(i=0;i<MAX_UV_SETS;i++)
		{
			Texcoords[i]=rfVertex.Texcoords[i];
		}
		FaceList.clear();
		for(i=0;i<rfVertex.FaceList.size();i++)
		{
			FaceList.push_back(rfVertex.FaceList[i]);
		}
		VMAPDescList.clear();
		for(i=0;i<rfVertex.VMAPDescList.size();i++)
		{
			VMAPDescList.push_back(rfVertex.VMAPDescList[i]);
		}
		return *this;
	}

	LWIMP_Vertex(const LWIMP_Vertex &rfVertex)
	{
		uiPositionIndex=rfVertex.uiPositionIndex;
		vNormal=rfVertex.vNormal;
		vTangent=rfVertex.vTangent;
		UINT i=0;
		for(i=0;i<4;i++)
		{
			Blendweights[i]=rfVertex.Blendweights[i];
		}
		dwBlendIndices=rfVertex.dwBlendIndices;
		for(i=0;i<MAX_UV_SETS;i++)
		{
			Texcoords[i]=rfVertex.Texcoords[i];
		}
		FaceList.clear();
		for(i=0;i<rfVertex.FaceList.size();i++)
		{
			FaceList.push_back(rfVertex.FaceList[i]);
		}
		VMAPDescList.clear();
		for(i=0;i<rfVertex.VMAPDescList.size();i++)
		{
			VMAPDescList.push_back(rfVertex.VMAPDescList[i]);
		}
	}

	LWIMP_Vertex()
	{
	}
};

//describe (single) VMAD for whole face
struct LWIMP_VMADDesc
{
	UINT	uiVMADIndex;
	BOOL	bOverrideVertex1;
	UINT	uiVertex1EntryIndex;
	BOOL	bOverrideVertex2;
	UINT	uiVertex2EntryIndex;
	BOOL	bOverrideVertex3;
	UINT	uiVertex3EntryIndex;
};

struct LWIMP_Face
{
	UINT							uiVertex1Index;
	UINT							uiVertex2Index;
	UINT							uiVertex3Index;			//indices into global vertex table
	UINT							uiEdge1Index;			//(1,2)
	UINT							uiEdge2Index;			//(2,3)
	UINT							uiEdge3Index;			//(3,1)
	LWIMP_Vector					vFaceNormal;
	LWIMP_Vector					vFaceTangent;
	UINT							uiSurfaceIndex;
	std::vector<LWIMP_VMADDesc>		VMADDescList;
	UINT							uiSmoothingGroup;

	LWIMP_Face& operator=(const LWIMP_Face &rfFace)
	{
		uiVertex1Index=rfFace.uiVertex1Index;
		uiVertex2Index=rfFace.uiVertex2Index;
		uiVertex3Index=rfFace.uiVertex3Index;
		uiEdge1Index=rfFace.uiEdge1Index;
		uiEdge2Index=rfFace.uiEdge2Index;
		uiEdge3Index=rfFace.uiEdge3Index;
		vFaceNormal=rfFace.vFaceNormal;
		vFaceTangent=rfFace.vFaceTangent;
		VMADDescList=rfFace.VMADDescList;
		uiSmoothingGroup=rfFace.uiSmoothingGroup;
		return *this;
	}
};

struct LWIMP_Edge
{
//	UINT	uiFace1Index;
//	BOOL	bFace2Present;			//edges may belong to only one face
//	UINT	uiFace2Index;
	std::set<UINT>		Faces;
	//start/end vertex - these actually can be extracted from LWIMP_Face structure
	//but it's useful to have them here for fast edge identification
	UINT	uiVertex1Index;
	UINT	uiVertex2Index;
	LWIMP_Edge& operator=(const LWIMP_Edge &rfEdge)
	{
		//uiFace1Index=rfEdge.uiFace1Index;
		//bFace2Present=rfEdge.bFace2Present;
		//uiFace2Index=rfEdge.uiFace2Index;
		std::set<UINT>::const_iterator iter=rfEdge.Faces.begin();
		Faces.clear();
		while(iter!=rfEdge.Faces.end())
		{
			Faces.insert(*iter);
			iter++;
		}
		uiVertex1Index=rfEdge.uiVertex1Index;
		uiVertex2Index=rfEdge.uiVertex2Index;
		return *this;
	}

	LWIMP_Edge(const LWIMP_Edge &rfEdge)
	{
		//uiFace1Index=rfEdge.uiFace1Index;
		//bFace2Present=rfEdge.bFace2Present;
		//uiFace2Index=rfEdge.uiFace2Index;
		std::set<UINT>::const_iterator iter=rfEdge.Faces.begin();
		Faces.clear();
		while(iter!=rfEdge.Faces.end())
		{
			Faces.insert(*iter);
			iter++;
		}
		uiVertex1Index=rfEdge.uiVertex1Index;
		uiVertex2Index=rfEdge.uiVertex2Index;
	}

	LWIMP_Edge()
	{
		//uiFace1Index=0xffffffff;
		//bFace2Present=FALSE;
		//uiFace2Index=0xffffffff;
		uiVertex1Index=0xffffffff;
		uiVertex2Index=0xffffffff;
	}
};

/*struct eqEdge {
	bool operator()(const LWIMP_Edge &e1,const LWIMP_Edge &e2) const {
		if(((e1.uiVertex1Index==e2.uiVertex1Index) &&
			(e1.uiVertex2Index==e2.uiVertex2Index)) ||
			((e1.uiVertex1Index==e2.uiVertex2Index) &&
			(e1.uiVertex2Index==e2.uiVertex1Index))) {
			return true;
		}
		return false;
	}
};*/

struct hashEdge
{
	static const size_t bucket_size = 4;
	static const size_t min_buckets = 8;
	size_t operator()(const LWIMP_Edge &x) const
	{
		return (x.uiVertex1Index+x.uiVertex2Index);
	}
	//peform total ordering for elements with the same hash key
	bool operator()(const LWIMP_Edge &e1,const LWIMP_Edge &e2) const
	{
/*//		if((e1.uiVertex1Index+e1.uiVertex2Index)==(e2.uiVertex1Index+e2.uiVertex2Index)) {
			if(((((UINT64)e1.uiVertex1Index)<<32)+e1.uiVertex2Index)==
				((((UINT64)e2.uiVertex1Index)<<32)+e2.uiVertex2Index)) {
				return false;
			}
			else {
				return true;
			}*/
/*		}
		else {
			return true;
		}*/
		if(((e1.uiVertex1Index==e2.uiVertex1Index) &&
			(e1.uiVertex2Index==e2.uiVertex2Index)) ||
			((e1.uiVertex1Index==e2.uiVertex2Index) &&
			(e1.uiVertex2Index==e2.uiVertex1Index)))
		{
			return false;
		}
		return true;
	}
};

//there are 3 ways to define texture coordinates:
//1. VMAP
//2. VMAD (which usually assists VMAP in defining the same coord set,
//   it can be standalone definition, though)
//3. Implicit uv mapping through predefined modes (planar, cylindrical etc)
//We need a way to map all this to common UV set ID's
struct LWIMP_UVSetDesc
{
	//0 - VMAP only
	//1 - VMAP+VMAD
	//2 - VMAD only
	//3 - implicit uv
	UINT uiMappingType;
	//validity of stuff below depends on mapping type
	UINT uiVMAPIndex;
	UINT uiVMADIndex;
	UINT uiSurfaceIndex;		//used only with IMPLICIT uv's
	UINT uiSurfaceBlockIndex;	//BLOK subchunk that stores mapping info
};

struct LWIMP_Object
{
	//geometry data
	std::vector<LWIMP_Vertex>		VertexTbl;
	std::vector<LWIMP_Face>			FaceTbl;
	std::vector<LWIMP_Edge>			EdgeTbl;
	//uv mapping info
	std::vector<LWIMP_UVSetDesc>	UVSetDescTbl;
};

UINT getSurfaceIndexFromTAGIndex(LW_Object &lwo,UINT uiTagIndex)
{
	if(uiTagIndex>lwo.uiTagCount)
	{
		MessageBox(NULL,L"Tag index out of range detected!!!",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return 0xffffffff;
	}
	UINT i=0;
	for(i=0;i<lwo.uiSurfaceCount;i++)
	{
		if(strcmp(lwo.Tags[uiTagIndex],lwo.Surfaces[i].szName)==0)
		{
			return i;
		}
	}
	MessageBox(NULL,L"Surface with given tag does not exist!!!",
		L"LWO2 import error!!!",MB_TASKMODAL);
	return 0xffffffff;
}

//build mesh structure from lwo
//fill Desc structures for VMAP's,VMAD's and UV sets
//lwo -IN
//imp_object - OUT
BOOL buildSecondStageMesh(LW_Object &lwo,LWIMP_Object &imp_object)
{
	//chect that we get passed an empty LWIMP_Object
	if(imp_object.VertexTbl.size()!=0)
	{
		return FALSE;
	}
	if(imp_object.FaceTbl.size()!=0)
	{
		return FALSE;
	}
	if(imp_object.EdgeTbl.size()!=0)
	{
		return FALSE;
	}
	if(imp_object.UVSetDescTbl.size()!=0)
	{
		return FALSE;
	}
	//copy vertices
	UINT i=0;
	for(i=0;i<lwo.uiPointCount;i++)
	{
		LWIMP_Vertex tmp_vertex;
		ZeroMemory(tmp_vertex.Blendweights,4*sizeof(float));
		tmp_vertex.dwBlendIndices=0;
		ZeroMemory(tmp_vertex.Texcoords,MAX_UV_SETS*sizeof(LWIMP_Texcoord));
		tmp_vertex.uiPositionIndex=i;
		ZeroMemory(&tmp_vertex.vNormal,3*sizeof(float));
		ZeroMemory(&tmp_vertex.vTangent,3*sizeof(float));
		imp_object.VertexTbl.push_back(tmp_vertex);
	}
	//copy faces
	for(i=0;i<lwo.uiTriCount;i++)
	{
		LWIMP_Face tmp_face;
		tmp_face.uiEdge1Index=0xffffffff;
		tmp_face.uiEdge2Index=0xffffffff;
		tmp_face.uiEdge3Index=0xffffffff;
		tmp_face.uiVertex1Index=lwo.pTriangles[i].v0;
		tmp_face.uiVertex2Index=lwo.pTriangles[i].v1;
		tmp_face.uiVertex3Index=lwo.pTriangles[i].v2;
		tmp_face.uiSurfaceIndex=
			getSurfaceIndexFromTAGIndex(lwo,lwo.pTriangles[i].usSurface);
		ZeroMemory(&tmp_face.vFaceNormal,3*sizeof(float));
		ZeroMemory(&tmp_face.vFaceTangent,3*sizeof(float));
		imp_object.FaceTbl.push_back(tmp_face);
		//extend face list for each vertex
		//ASSUMPTION: all triangles are built of 3 _different_ vertices
		imp_object.VertexTbl[tmp_face.uiVertex1Index].FaceList.push_back(i);
		imp_object.VertexTbl[tmp_face.uiVertex2Index].FaceList.push_back(i);
		imp_object.VertexTbl[tmp_face.uiVertex3Index].FaceList.push_back(i);
		//calc face normal
		float v1[3];
		v1[0]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex2Index].uiPositionIndex].x-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].x;
		v1[1]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex2Index].uiPositionIndex].y-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].y;
		v1[2]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex2Index].uiPositionIndex].z-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].z;
		float v2[3];
		v2[0]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex3Index].uiPositionIndex].x-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].x;
		v2[1]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex3Index].uiPositionIndex].y-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].y;
		v2[2]=lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex3Index].uiPositionIndex].z-
			lwo.pPNTS[imp_object.VertexTbl[tmp_face.uiVertex1Index].uiPositionIndex].z;
		imp_object.FaceTbl[i].vFaceNormal.x=v1[1]*v2[2]-v1[2]*v2[1];
		imp_object.FaceTbl[i].vFaceNormal.y=v1[2]*v2[0]-v1[0]*v2[2];
		imp_object.FaceTbl[i].vFaceNormal.z=v1[0]*v2[1]-v1[1]*v2[0];
		float fLen=(float)sqrt(imp_object.FaceTbl[i].vFaceNormal.x*imp_object.FaceTbl[i].vFaceNormal.x+
			imp_object.FaceTbl[i].vFaceNormal.y*imp_object.FaceTbl[i].vFaceNormal.y+
			imp_object.FaceTbl[i].vFaceNormal.z*imp_object.FaceTbl[i].vFaceNormal.z);
		if(fLen>0.0f)
		{
			imp_object.FaceTbl[i].vFaceNormal.x/=fLen;
			imp_object.FaceTbl[i].vFaceNormal.y/=fLen;
			imp_object.FaceTbl[i].vFaceNormal.z/=fLen;
		}
		else
		{
			imp_object.FaceTbl[i].vFaceNormal.x=0.0f;
			imp_object.FaceTbl[i].vFaceNormal.y=0.0f;
			imp_object.FaceTbl[i].vFaceNormal.z=1.0f;
		}
	}
	//loop through VMAP's/VMAD's
	for(i=0;i<lwo.uiVMapCount;i++)
	{
		UINT j=0;
		for(j=0;j<lwo.VMaps[i].uiEntryCount;j++)
		{
			LWIMP_VMAPDesc tmp_vmap_desc;
			tmp_vmap_desc.uiEntryIndex=j;
			tmp_vmap_desc.uiVMAPIndex=i;
			switch(lwo.VMaps[i].usDimension)
			{
				case 1:
					imp_object.VertexTbl[lwo.VMaps[i].pVMAP1D[j].uiVertexIndex].
						VMAPDescList.push_back(tmp_vmap_desc);
					break;
				case 2:
					imp_object.VertexTbl[lwo.VMaps[i].pVMAP2D[j].uiVertexIndex].
						VMAPDescList.push_back(tmp_vmap_desc);
					break;
				case 3:
					imp_object.VertexTbl[lwo.VMaps[i].pVMAP3D[j].uiVertexIndex].
						VMAPDescList.push_back(tmp_vmap_desc);
					break;
				default:
					MessageBox(NULL,L"Unsupported VMAP dimension!!!",L"LWO2 import error!!!",
						MB_TASKMODAL);
					return FALSE;
					break;
			}
		}
	}
	for(i=0;i<lwo.uiVMadCount;i++)
	{
		UINT j=0;
		for(j=0;j<lwo.VMads[i].uiEntryCount;j++)
		{
			LWIMP_VMADDesc tmp_vmad_desc;
			tmp_vmad_desc.bOverrideVertex1=FALSE;
			tmp_vmad_desc.bOverrideVertex2=FALSE;
			tmp_vmad_desc.bOverrideVertex3=FALSE;
			tmp_vmad_desc.uiVMADIndex=i;
			switch(lwo.VMads[i].usDimension)
			{
				case 1:
					{
						LWIMP_Face &triangle=
							imp_object.FaceTbl[lwo.VMads[i].pVMAD1D[j].uiTriangleIndex];
						//search if there is already any desc for this traingle/vmad pair 
						BOOL bFound=FALSE;
						UINT uiDescIndex=0;
						for(uiDescIndex=0;uiDescIndex<triangle.VMADDescList.size();uiDescIndex++)
						{
							if(triangle.VMADDescList[uiDescIndex].uiVMADIndex==i)
							{
								bFound=TRUE;
								break;
							}
						}
						if(!bFound)
						{
							triangle.VMADDescList.push_back(tmp_vmad_desc);
							uiDescIndex=triangle.VMADDescList.size()-1;
						}
						LWIMP_VMADDesc &vmad_desc=triangle.VMADDescList[uiDescIndex];
						if(triangle.uiVertex1Index==lwo.VMads[i].pVMAD1D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex1=TRUE;
							vmad_desc.uiVertex1EntryIndex=j;
						}
						else if(triangle.uiVertex2Index==lwo.VMads[i].pVMAD1D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex2=TRUE;
							vmad_desc.uiVertex2EntryIndex=j;
						}
						else if(triangle.uiVertex3Index==lwo.VMads[i].pVMAD1D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex3=TRUE;
							vmad_desc.uiVertex3EntryIndex=j;
						}
						else
						{
							//we didn't find it!!!
							MessageBox(NULL,L"Vertex not found in triangle during VMAD processing!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					break;
				case 2:
					{
						LWIMP_Face &triangle=
							imp_object.FaceTbl[lwo.VMads[i].pVMAD2D[j].uiTriangleIndex];
						//search if there is already any desc for this triangle/vmad pair 
						BOOL bFound=FALSE;
						UINT uiDescIndex=0;
						for(uiDescIndex=0;uiDescIndex<triangle.VMADDescList.size();uiDescIndex++)
						{
							if(triangle.VMADDescList[uiDescIndex].uiVMADIndex==i)
							{
								bFound=TRUE;
								break;
							}
						}
						if(!bFound)
						{
							triangle.VMADDescList.push_back(tmp_vmad_desc);
							uiDescIndex=triangle.VMADDescList.size()-1;
						}
						LWIMP_VMADDesc &vmad_desc=triangle.VMADDescList[uiDescIndex];
						if(triangle.uiVertex1Index==lwo.VMads[i].pVMAD2D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex1=TRUE;
							vmad_desc.uiVertex1EntryIndex=j;
						}
						else if(triangle.uiVertex2Index==lwo.VMads[i].pVMAD2D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex2=TRUE;
							vmad_desc.uiVertex2EntryIndex=j;
						}
						else if(triangle.uiVertex3Index==lwo.VMads[i].pVMAD2D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex3=TRUE;
							vmad_desc.uiVertex3EntryIndex=j;
						}
						else
						{
							//we didn't find it!!!
							MessageBox(NULL,L"Vertex not found in triangle during VMAD processing!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					break;
				case 3:
					{
						LWIMP_Face &triangle=
							imp_object.FaceTbl[lwo.VMads[i].pVMAD3D[j].uiTriangleIndex];
						//search if there is already any desc for this traingle/vmad pair 
						BOOL bFound=FALSE;
						UINT uiDescIndex=0;
						for(uiDescIndex=0;uiDescIndex<triangle.VMADDescList.size();uiDescIndex++)
						{
							if(triangle.VMADDescList[uiDescIndex].uiVMADIndex==i)
							{
								bFound=TRUE;
								break;
							}
						}
						if(!bFound)
						{
							triangle.VMADDescList.push_back(tmp_vmad_desc);
							uiDescIndex=triangle.VMADDescList.size()-1;
						}
						LWIMP_VMADDesc &vmad_desc=triangle.VMADDescList[uiDescIndex];
						if(triangle.uiVertex1Index==lwo.VMads[i].pVMAD3D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex1=TRUE;
							vmad_desc.uiVertex1EntryIndex=j;
						}
						else if(triangle.uiVertex2Index==lwo.VMads[i].pVMAD3D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex2=TRUE;
							vmad_desc.uiVertex2EntryIndex=j;
						}
						else if(triangle.uiVertex3Index==lwo.VMads[i].pVMAD3D[j].uiVertexIndex)
						{
							vmad_desc.bOverrideVertex3=TRUE;
							vmad_desc.uiVertex3EntryIndex=j;
						}
						else
						{
							//we didn't find it!!!
							MessageBox(NULL,L"Vertex not found in triangle during VMAD processing!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					break;
				default:
					MessageBox(NULL,L"Unsupported VMAD dimension!!!",L"LWO2 import error!!!",
						MB_TASKMODAL);
					return FALSE;
					break;
			}
		}
	}
	//build edge table
	stdext::hash_map<LWIMP_Edge,UINT,hashEdge>	EdgeHash;
	for(i=0;i<imp_object.FaceTbl.size();i++)
	{
		LWIMP_Edge tmp_edge;
		//(1,2) edge
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.Faces.clear();
		if(EdgeHash.find(tmp_edge)==EdgeHash.end())
		{
			tmp_edge.Faces.insert(i);
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge1Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge1Index;
		}
		else
		{
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			imp_object.FaceTbl[i].uiEdge1Index=uiEdgeIndex;
			imp_object.EdgeTbl[uiEdgeIndex].Faces.insert(i);
		}
		//(2,3) edge
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.Faces.clear();
		if(EdgeHash.find(tmp_edge)==EdgeHash.end())
		{
			tmp_edge.Faces.insert(i);
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge2Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge2Index;
		}
		else
		{
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			imp_object.FaceTbl[i].uiEdge2Index=uiEdgeIndex;
			imp_object.EdgeTbl[uiEdgeIndex].Faces.insert(i);
		}
		//(3,1) edge
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.Faces.clear();
		if(EdgeHash.find(tmp_edge)==EdgeHash.end())
		{
			tmp_edge.Faces.insert(i);
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge3Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge3Index;
		}
		else
		{
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			imp_object.FaceTbl[i].uiEdge3Index=uiEdgeIndex;
			imp_object.EdgeTbl[uiEdgeIndex].Faces.insert(i);
		}
	}
	//build UV set table
	//loop through vmaps
	for(i=0;i<lwo.uiVMapCount;i++)
	{
		if(*((int*)(lwo.VMaps[i].type))!='TXUV')
		{
			continue;
		}
		BOOL bVMAPOnly=TRUE;
		UINT j=0;
		for(j=0;j<lwo.uiVMadCount;j++)
		{
			if(*((int*)(lwo.VMads[j].type))!='TXUV')
			{
				continue;
			}
			if(strcmp(lwo.VMaps[i].szName,lwo.VMads[j].szName)==0)
			{
				bVMAPOnly=FALSE;
				break;
			}
		}
		LWIMP_UVSetDesc		uv_set_desc;
		ZeroMemory(&uv_set_desc,sizeof(LWIMP_UVSetDesc));
		if(bVMAPOnly)
		{
			uv_set_desc.uiMappingType=0;
			uv_set_desc.uiVMAPIndex=i;
		}
		else
		{
			uv_set_desc.uiMappingType=1;
			uv_set_desc.uiVMAPIndex=i;
			uv_set_desc.uiVMADIndex=j;
		}
		imp_object.UVSetDescTbl.push_back(uv_set_desc);
	}
	//loop through vmads (look for "orphans")
	for(i=0;i<lwo.uiVMadCount;i++)
	{
		if(*((int*)(lwo.VMads[i].type))!='TXUV')
		{
			continue;
		}
		BOOL bVMAPFound=FALSE;
		UINT j=0;
		for(j=0;i<lwo.uiVMapCount;j++)
		{
			if(*((int*)(lwo.VMaps[j].type))!='TXUV')
			{
				continue;
			}
			if(strcmp(lwo.VMads[i].szName,lwo.VMaps[j].szName)==0)
			{
				bVMAPFound=TRUE;
				break;
			}
		}
		if(!bVMAPFound)
		{
			LWIMP_UVSetDesc		uv_set_desc;
			ZeroMemory(&uv_set_desc,sizeof(LWIMP_UVSetDesc));
			uv_set_desc.uiMappingType=2;
			uv_set_desc.uiVMADIndex=i;
			imp_object.UVSetDescTbl.push_back(uv_set_desc);
			MessageBox(NULL,L"Orphan VMAD found!!! Troubles in VMAD splitter may appear!!!",
				L"LWO2 import warning!!",MB_TASKMODAL);
		}
	}
	//loop through all surfaces for implicit uv mapping
	for(i=0;i<lwo.uiSurfaceCount;i++)
	{
		UINT j=0;
		for(j=0;j<lwo.Surfaces[i].uiSurfaceBlockCount;j++)
		{
			if(*((int*)(lwo.Surfaces[i].SurfaceBlocks[j].BlockHeader.Type))=='IMAP')
			{
				if(lwo.Surfaces[i].SurfaceBlocks[j].usProjectionMode!=TMAP_PROJECTION_MODE_UV)
				{
					if(lwo.Surfaces[i].SurfaceBlocks[j].usProjectionMode!=TMAP_PROJECTION_MODE_PLANAR)
					{
						MessageBox(NULL,L"For non-uv mapping only planar mapping is supported!!!",
							L"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					LWIMP_UVSetDesc		uv_set_desc;
					ZeroMemory(&uv_set_desc,sizeof(LWIMP_UVSetDesc));
					uv_set_desc.uiMappingType=3;
					uv_set_desc.uiSurfaceIndex=i;
					uv_set_desc.uiSurfaceBlockIndex=j;
					imp_object.UVSetDescTbl.push_back(uv_set_desc);
				}
			}
		}
	}
	return TRUE;
}

void getVertexUV(UINT uiFaceIndex,UINT uiVertexIndex,UINT uiUVSetIndex,
				 LWIMP_Object &imp_object,LW_Object &lwo,float *u,float *v)
{
	//first check mapping mode
	if(imp_object.UVSetDescTbl[uiUVSetIndex].uiMappingType==3) {
		//generate planar uv
		float x=lwo.pPNTS[imp_object.VertexTbl[uiVertexIndex].uiPositionIndex].x;
		float y=lwo.pPNTS[imp_object.VertexTbl[uiVertexIndex].uiPositionIndex].y;
		float z=lwo.pPNTS[imp_object.VertexTbl[uiVertexIndex].uiPositionIndex].z;
		if(lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.bCenterPresent) {
			x-=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Center.x;
			y-=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Center.y;
			z-=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Center.z;
		}
		float tx_size_x=1.0f;
		float tx_size_y=1.0f;
		float tx_size_z=1.0f;
		if(lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.bSizePresent)
		{
			tx_size_x=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
				SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Size.x;
			tx_size_y=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
				SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Size.y;
			tx_size_z=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
				SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].TMAP.Size.z;
		}
		USHORT usAxis=lwo.Surfaces[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceIndex].
			SurfaceBlocks[imp_object.UVSetDescTbl[uiUVSetIndex].uiSurfaceBlockIndex].usTextureAxis;
		if(usAxis==TMAP_AXIS_X)
		{
			*u=z/tx_size_z+0.5f;
			*v=-y/tx_size_y+0.5f;
		}
		else if(usAxis==TMAP_AXIS_Y)
		{
			*u=x/tx_size_x+0.5f;
			*v=-z/tx_size_z+0.5f;
		}
		else
		{
			*u=x/tx_size_x+0.5f;
			*v=-y/tx_size_y+0.5f;
		}
	}
	else
	{
		//get uv from vmap/vmad
		*u=0.0f;
		*v=0.0f;
		//get coords from vmap
		if((imp_object.UVSetDescTbl[uiUVSetIndex].uiMappingType==0) ||
			(imp_object.UVSetDescTbl[uiUVSetIndex].uiMappingType==1))
		{
			if(imp_object.VertexTbl[uiVertexIndex].VMAPDescList.size()==0)
			{
				//TODO: issue warning
				return;
			}
			UINT i=0;
			for(i=0;i<imp_object.VertexTbl[uiVertexIndex].VMAPDescList.size();i++)
			{
				if(imp_object.UVSetDescTbl[uiUVSetIndex].uiVMAPIndex==
					imp_object.VertexTbl[uiVertexIndex].VMAPDescList[i].uiVMAPIndex)
				{
					break;
				}
			}
			if(i==imp_object.VertexTbl[uiVertexIndex].VMAPDescList.size())
			{
				//TODO: issue warning
				return;
			}
			UINT uiVMAPEntryIndex=imp_object.VertexTbl[uiVertexIndex].VMAPDescList[i].uiEntryIndex;
			*u=lwo.VMaps[imp_object.UVSetDescTbl[uiUVSetIndex].uiVMAPIndex].
				pVMAP2D[uiVMAPEntryIndex].fVal0;
			*v=lwo.VMaps[imp_object.UVSetDescTbl[uiUVSetIndex].uiVMAPIndex].
				pVMAP2D[uiVMAPEntryIndex].fVal1;
		}
		//get from vmad (if in use)
		if((imp_object.UVSetDescTbl[uiUVSetIndex].uiMappingType==1) ||
			(imp_object.UVSetDescTbl[uiUVSetIndex].uiMappingType==2))
		{
			if(imp_object.FaceTbl[uiFaceIndex].VMADDescList.size()==0)
			{
				//TODO: issue warning
				return;
			}
			UINT i=0;
			for(i=0;i<imp_object.FaceTbl[uiFaceIndex].VMADDescList.size();i++)
			{
				if(imp_object.UVSetDescTbl[uiUVSetIndex].uiVMADIndex==
					imp_object.FaceTbl[uiFaceIndex].VMADDescList[i].uiVMADIndex)
				{
					break;
				}
			}
			if(i==imp_object.FaceTbl[uiFaceIndex].VMADDescList.size())
			{
				//it is possible that vmad entry is not found
				//it is perfectly legal in lwo file!
				return;
			}
			LWIMP_VMADDesc	&rfVMADDesc=imp_object.FaceTbl[uiFaceIndex].VMADDescList[i];
			LWIMP_Face		&rfFace=imp_object.FaceTbl[uiFaceIndex];
			if(rfFace.uiVertex1Index==uiVertexIndex)
			{
				if(rfVMADDesc.bOverrideVertex1)
				{
					*u=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex1EntryIndex].fVal0;
					*v=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex1EntryIndex].fVal1;
				}
				return;
			}
			if(rfFace.uiVertex2Index==uiVertexIndex)
			{
				if(rfVMADDesc.bOverrideVertex2)
				{
					*u=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex2EntryIndex].fVal0;
					*v=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex2EntryIndex].fVal1;
				}
				return;
			}
			if(rfFace.uiVertex3Index==uiVertexIndex)
			{
				if(rfVMADDesc.bOverrideVertex3)
				{
					*u=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex3EntryIndex].fVal0;
					*v=lwo.VMads[rfVMADDesc.uiVMADIndex].pVMAD2D[rfVMADDesc.uiVertex3EntryIndex].fVal1;
				}
				return;
			}
		}
	}
}

//calculate face tangents with respect to color texture mapping (if possible)
//or first available mapping when no color texture is present
BOOL calcFaceTangents(LWIMP_Object &imp_object,LW_Object &lwo)
{
	UINT			uiStartFaceIndex=0;
	std::set<int>	ProcessedSurfaces;
	while(uiStartFaceIndex<imp_object.FaceTbl.size())
	{
		if(ProcessedSurfaces.find(imp_object.FaceTbl[uiStartFaceIndex].uiSurfaceIndex)==
			ProcessedSurfaces.end())
		{
			UINT uiSurfaceIndex=imp_object.FaceTbl[uiStartFaceIndex].uiSurfaceIndex;
			ProcessedSurfaces.insert(uiSurfaceIndex);
			UINT uiRefUVSetID=0;
			BOOL bUVSetFound=FALSE;
			//find reference UV set for this surface
			UINT j=0;
			for(j=0;j<lwo.Surfaces[uiSurfaceIndex].uiSurfaceBlockCount;j++) {
				if((*((int*)(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].BlockHeader.Type))
					=='IMAP') &&
					(*((int*)(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].BlockHeader.TextureChannel))
					=='COLR'))
				{
					if(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].usProjectionMode==TMAP_PROJECTION_MODE_UV)
					{
						UINT k=0;
						for(k=0;k<imp_object.UVSetDescTbl.size();k++)
						{
							if((imp_object.UVSetDescTbl[k].uiMappingType==0) ||
								(imp_object.UVSetDescTbl[k].uiMappingType==1))
							{	//"pure VMAD" not supported
								if(strcmp(lwo.VMaps[imp_object.UVSetDescTbl[k].uiVMAPIndex].szName,
									lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].szVMapName)==0)
								{
									bUVSetFound=TRUE;
									uiRefUVSetID=k;
									break;
								}
							}
						}
					}
					else
					{
						UINT k=0;
						for(k=0;k<imp_object.UVSetDescTbl.size();k++)
						{
							if(imp_object.UVSetDescTbl[k].uiMappingType==3)
							{
								if((imp_object.UVSetDescTbl[k].uiSurfaceIndex==uiSurfaceIndex) &&
									(imp_object.UVSetDescTbl[k].uiSurfaceBlockIndex==j))
								{
									bUVSetFound=TRUE;
									uiRefUVSetID=k;
									break;
								}
							}
						}
					}
				}
				if(bUVSetFound)
				{
					break;
				}
			}
			if(!bUVSetFound)
			{	//no mapping on color texture, we need to use another one (any IMAP)
				for(j=0;j<lwo.Surfaces[uiSurfaceIndex].uiSurfaceBlockCount;j++)
				{
					if((*((int*)(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].BlockHeader.Type))
						=='IMAP'))
					{
						if(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].usProjectionMode==TMAP_PROJECTION_MODE_UV)
						{
							UINT k=0;
							for(k=0;k<imp_object.UVSetDescTbl.size();k++)
							{
								if((imp_object.UVSetDescTbl[k].uiMappingType==0) ||
									(imp_object.UVSetDescTbl[k].uiMappingType==1))
								{	//"pure VMAD" not supported
									if(strcmp(lwo.VMaps[imp_object.UVSetDescTbl[k].uiVMAPIndex].szName,
										lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[j].szVMapName)==0)
									{
										bUVSetFound=TRUE;
										uiRefUVSetID=k;
										break;
									}
								}
							}
						}
						else
						{
							UINT k=0;
							for(k=0;k<imp_object.UVSetDescTbl.size();k++)
							{
								if(imp_object.UVSetDescTbl[k].uiMappingType==3)
								{
									if((imp_object.UVSetDescTbl[k].uiSurfaceIndex==uiSurfaceIndex) &&
										(imp_object.UVSetDescTbl[k].uiSurfaceBlockIndex==j))
									{
										bUVSetFound=TRUE;
										uiRefUVSetID=k;
										break;
									}
								}
							}
						}
					}
					if(bUVSetFound)
					{
						break;
					}
				}
			}
			if(!bUVSetFound)
			{
				//just place any vector perpendicular to normal everywhere
				for(j=uiStartFaceIndex;j<imp_object.FaceTbl.size();j++)
				{
					if(imp_object.FaceTbl[j].uiSurfaceIndex!=uiSurfaceIndex)
					{
						continue;
					}
					//actually any vector is about the same good ...
					float BaseTangent[3]={0.0f,1.0f,0.0f};
					float BaseBinormal[3]={0.0f,1.0f,0.0f};
					float *v1=NULL;
					float *v2=NULL;
					//B=NxT
					v1=&(imp_object.FaceTbl[j].vFaceNormal.x);
					v2=&BaseTangent[0];
					BaseBinormal[0]=v1[1]*v2[2]-v1[2]*v2[1];
					BaseBinormal[1]=v1[2]*v2[0]-v1[0]*v2[2];
					BaseBinormal[2]=v1[0]*v2[1]-v1[1]*v2[0];
					float fLen=(float)sqrt(BaseBinormal[0]*BaseBinormal[0]+BaseBinormal[1]*BaseBinormal[1]+
						BaseBinormal[2]*BaseBinormal[2]);
					if(fLen>0.0f)
					{
						BaseBinormal[0]/=fLen;
						BaseBinormal[1]/=fLen;
						BaseBinormal[2]/=fLen;
					}
					//T=BxN
					v1=&BaseBinormal[0];
					v2=&(imp_object.FaceTbl[j].vFaceNormal.x);
					BaseTangent[0]=v1[1]*v2[2]-v1[2]*v2[1];
					BaseTangent[1]=v1[2]*v2[0]-v1[0]*v2[2];
					BaseTangent[2]=v1[0]*v2[1]-v1[1]*v2[0];
					fLen=(float)sqrt(BaseTangent[0]*BaseTangent[0]+BaseTangent[1]*BaseTangent[1]+
						BaseTangent[2]*BaseTangent[2]);
					if(fLen>0.0f)
					{
						BaseTangent[0]/=fLen;
						BaseTangent[1]/=fLen;
						BaseTangent[2]/=fLen;
					}
					imp_object.FaceTbl[j].vFaceTangent.x=BaseTangent[0];
					imp_object.FaceTbl[j].vFaceTangent.y=BaseTangent[1];
					imp_object.FaceTbl[j].vFaceTangent.z=BaseTangent[2];
				}
			}
			else
			{
				//calculate tangents based on found uv set
				for(j=uiStartFaceIndex;j<imp_object.FaceTbl.size();j++)
				{
					if(imp_object.FaceTbl[j].uiSurfaceIndex!=uiSurfaceIndex)
					{
						continue;
					}
					float Vertex1UV[2]={0.0f,0.0f};
					float Vertex2UV[2]={0.0f,0.0f};
					float Vertex3UV[2]={0.0f,0.0f};
					float Vertex1Pos[3]={0.0f,0.0f,0.0f};
					float Vertex2Pos[3]={0.0f,0.0f,0.0f};
					float Vertex3Pos[3]={0.0f,0.0f,0.0f};
					Vertex1Pos[0]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex1Index].uiPositionIndex].x;
					Vertex1Pos[1]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex1Index].uiPositionIndex].y;
					Vertex1Pos[2]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex1Index].uiPositionIndex].z;
					Vertex2Pos[0]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex2Index].uiPositionIndex].x;
					Vertex2Pos[1]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex2Index].uiPositionIndex].y;
					Vertex2Pos[2]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex2Index].uiPositionIndex].z;
					Vertex3Pos[0]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex3Index].uiPositionIndex].x;
					Vertex3Pos[1]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex3Index].uiPositionIndex].y;
					Vertex3Pos[2]=lwo.pPNTS[imp_object.VertexTbl[imp_object.FaceTbl[j].uiVertex3Index].uiPositionIndex].z;
					getVertexUV(j,imp_object.FaceTbl[j].uiVertex1Index,uiRefUVSetID,imp_object,
						lwo,&Vertex1UV[0],&Vertex1UV[1]);
					getVertexUV(j,imp_object.FaceTbl[j].uiVertex2Index,uiRefUVSetID,imp_object,
						lwo,&Vertex2UV[0],&Vertex2UV[1]);
					getVertexUV(j,imp_object.FaceTbl[j].uiVertex3Index,uiRefUVSetID,imp_object,
						lwo,&Vertex3UV[0],&Vertex3UV[1]);
					float Tangent[3]={0.0f,0.0f,0.0f};
					if(Vertex1UV[0]!=Vertex2UV[0])
					{
						Tangent[0]+=(Vertex1Pos[0]-Vertex2Pos[0])/(Vertex1UV[0]-Vertex2UV[0]);
						Tangent[1]+=(Vertex1Pos[1]-Vertex2Pos[2])/(Vertex1UV[0]-Vertex2UV[0]);
						Tangent[2]+=(Vertex1Pos[2]-Vertex2Pos[2])/(Vertex1UV[0]-Vertex2UV[0]);
					}
					if(Vertex2UV[0]!=Vertex3UV[0])
					{
						Tangent[0]+=(Vertex2Pos[0]-Vertex3Pos[0])/(Vertex2UV[0]-Vertex3UV[0]);
						Tangent[1]+=(Vertex2Pos[1]-Vertex3Pos[2])/(Vertex2UV[0]-Vertex3UV[0]);
						Tangent[2]+=(Vertex2Pos[2]-Vertex3Pos[2])/(Vertex2UV[0]-Vertex3UV[0]);
					}
					if(Vertex3UV[0]!=Vertex1UV[0])
					{
						Tangent[0]+=(Vertex3Pos[0]-Vertex1Pos[0])/(Vertex3UV[0]-Vertex1UV[0]);
						Tangent[1]+=(Vertex3Pos[1]-Vertex1Pos[2])/(Vertex3UV[0]-Vertex1UV[0]);
						Tangent[2]+=(Vertex3Pos[2]-Vertex1Pos[2])/(Vertex3UV[0]-Vertex1UV[0]);
					}
					float fLen=(float)sqrt(Tangent[0]*Tangent[0]+Tangent[1]*Tangent[1]+
						Tangent[2]*Tangent[2]);
					if(fLen>0.0f)
					{
						Tangent[0]/=fLen;
						Tangent[1]/=fLen;
						Tangent[2]/=fLen;
					}
					//now make it absolutely perpendicular to normal
					float BaseBinormal[3]={0.0f,1.0f,0.0f};
					float *v1=NULL;
					float *v2=NULL;
					//B=NxT
					v1=&(imp_object.FaceTbl[j].vFaceNormal.x);
					v2=&Tangent[0];
					BaseBinormal[0]=v1[1]*v2[2]-v1[2]*v2[1];
					BaseBinormal[1]=v1[2]*v2[0]-v1[0]*v2[2];
					BaseBinormal[2]=v1[0]*v2[1]-v1[1]*v2[0];
					fLen=(float)sqrt(BaseBinormal[0]*BaseBinormal[0]+BaseBinormal[1]*BaseBinormal[1]+
						BaseBinormal[2]*BaseBinormal[2]);
					if(fLen>0.0f)
					{
						BaseBinormal[0]/=fLen;
						BaseBinormal[1]/=fLen;
						BaseBinormal[2]/=fLen;
					}
					//T=BxN
					v1=&BaseBinormal[0];
					v2=&(imp_object.FaceTbl[j].vFaceNormal.x);
					Tangent[0]=v1[1]*v2[2]-v1[2]*v2[1];
					Tangent[1]=v1[2]*v2[0]-v1[0]*v2[2];
					Tangent[2]=v1[0]*v2[1]-v1[1]*v2[0];
					fLen=(float)sqrt(Tangent[0]*Tangent[0]+Tangent[1]*Tangent[1]+
						Tangent[2]*Tangent[2]);
					if(fLen>0.0f)
					{
						Tangent[0]/=fLen;
						Tangent[1]/=fLen;
						Tangent[2]/=fLen;
					}
					imp_object.FaceTbl[j].vFaceTangent.x=Tangent[0];
					imp_object.FaceTbl[j].vFaceTangent.y=Tangent[1];
					imp_object.FaceTbl[j].vFaceTangent.z=Tangent[2];
				}
			}
		}
		else
		{
			uiStartFaceIndex++;
		}
	}
	return TRUE;
}

//identify smoothing groups, perform "split by smoothing group" operation
//then average and renormalize normals (and possibly tangents) where
//applicable
BOOL processSmoothingGroups(LWIMP_Object &imp_object,LW_Object &lwo,BOOL bFixNPatchSeams)
{
	UINT i;
	//0 is reserved value - means "not assigned"
	for(i=0;i<imp_object.FaceTbl.size();i++)
	{
		imp_object.FaceTbl[i].uiSmoothingGroup=0;
	}
	UINT uiCurrentSGID=1;
	UINT uiCurrentFace=0;
#ifdef _DEBUG
	DWORD dwFacesAssigned=0;
	DWORD dwNumFaces=imp_object.FaceTbl.size();
#endif
	while(uiCurrentFace<imp_object.FaceTbl.size())
	{
		if(imp_object.FaceTbl[uiCurrentFace].uiSmoothingGroup==0)
		{
			std::vector<UINT>	FaceStack;
			FaceStack.push_back(uiCurrentFace);
			imp_object.FaceTbl[uiCurrentFace].uiSmoothingGroup=uiCurrentSGID;
#ifdef _DEBUG
			dwFacesAssigned++;
#endif
			do
			{
				UINT uiFaceIndex=FaceStack.back();
				FaceStack.pop_back();
				//determine minimum value of dot product between face normals that allows smoothing
				//- cosine of max smoothing angle (face normals are unit length already)
				float fCriticalDP3Val=
					(float)cos(lwo.Surfaces[imp_object.FaceTbl[uiFaceIndex].uiSurfaceIndex].fMaxSmoothingAngle);
				//edge 1
				std::set<UINT>::iterator iter=
					imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].Faces.begin();
				while(iter!=imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].Faces.end())
				{
					if((*iter)!=uiFaceIndex)
					{
						UINT uiNeighborFace=*iter;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0)
						{
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val)
							{
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
#ifdef _DEBUG
								dwFacesAssigned++;
#endif
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
					iter++;
				}
				//edge 2
				iter=imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].Faces.begin();
				while(iter!=imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].Faces.end())
				{
					if((*iter)!=uiFaceIndex)
					{
						UINT uiNeighborFace=*iter;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0)
						{
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val)
							{
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
#ifdef _DEBUG
								dwFacesAssigned++;
#endif
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
					iter++;
				}
				//edge 3
				iter=imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].Faces.begin();
				while(iter!=imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].Faces.end())
				{
					if((*iter)!=uiFaceIndex)
					{
						UINT uiNeighborFace=*iter;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0)
						{
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val)
							{
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
#ifdef _DEBUG
								dwFacesAssigned++;
#endif
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
					iter++;
				}
/*				if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].uiFace1Index==uiFaceIndex) {
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].bFace2Present) {
						UINT uiNeighborFace=
							imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].uiFace2Index;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val) {
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
				}
				else {
					if(!imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].bFace2Present) {
						MessageBox(NULL,"Corrupted edge found (border edge - no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].uiFace2Index!=uiFaceIndex) {
						MessageBox(NULL,"Corrupted edge found (no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					UINT uiNeighborFace=
						imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge1Index].uiFace1Index;
					if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
						LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
						LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
						float fNormalDot=rfNormal1.x*rfNormal2.x+
							rfNormal1.y*rfNormal2.y+
							rfNormal1.z*rfNormal2.z;
						if(fNormalDot>=fCriticalDP3Val) {
							imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
							FaceStack.push_back(uiNeighborFace);
						}
					}
				}*/
				//edge 2
/*				if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].uiFace1Index==uiFaceIndex) {
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].bFace2Present) {
						UINT uiNeighborFace=
							imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].uiFace2Index;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val) {
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
				}
				else {
					if(!imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].bFace2Present) {
						MessageBox(NULL,"Corrupted edge found (border edge - no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].uiFace2Index!=uiFaceIndex) {
						MessageBox(NULL,"Corrupted edge found (no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					UINT uiNeighborFace=
						imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge2Index].uiFace1Index;
					if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
						LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
						LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
						float fNormalDot=rfNormal1.x*rfNormal2.x+
							rfNormal1.y*rfNormal2.y+
							rfNormal1.z*rfNormal2.z;
						if(fNormalDot>=fCriticalDP3Val) {
							imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
							FaceStack.push_back(uiNeighborFace);
						}
					}
				}*/
				//edge 3
/*				if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].uiFace1Index==uiFaceIndex) {
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].bFace2Present) {
						UINT uiNeighborFace=
							imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].uiFace2Index;
						if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
							LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
							LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
							float fNormalDot=rfNormal1.x*rfNormal2.x+
								rfNormal1.y*rfNormal2.y+
								rfNormal1.z*rfNormal2.z;
							if(fNormalDot>=fCriticalDP3Val) {
								imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
								FaceStack.push_back(uiNeighborFace);
							}
						}
					}
				}
				else {
					if(!imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].bFace2Present) {
						MessageBox(NULL,"Corrupted edge found (border edge - no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					if(imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].uiFace2Index!=uiFaceIndex) {
						MessageBox(NULL,"Corrupted edge found (no face index match)!!!",
							"LWO2 import error!!!",MB_TASKMODAL);
						return FALSE;
					}
					UINT uiNeighborFace=
						imp_object.EdgeTbl[imp_object.FaceTbl[uiFaceIndex].uiEdge3Index].uiFace1Index;
					if(imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup==0) {
						LWIMP_Vector &rfNormal1=imp_object.FaceTbl[uiFaceIndex].vFaceNormal;
						LWIMP_Vector &rfNormal2=imp_object.FaceTbl[uiNeighborFace].vFaceNormal;
						float fNormalDot=rfNormal1.x*rfNormal2.x+
							rfNormal1.y*rfNormal2.y+
							rfNormal1.z*rfNormal2.z;
						if(fNormalDot>=fCriticalDP3Val) {
							imp_object.FaceTbl[uiNeighborFace].uiSmoothingGroup=uiCurrentSGID;
							FaceStack.push_back(uiNeighborFace);
						}
					}
				}*/
			} while(FaceStack.size()>0);
			uiCurrentSGID++;
		}
		else
		{
			uiCurrentFace++;
		}
	}
	UINT uiTmpVertexCount=imp_object.VertexTbl.size();
	for(i=0;i<uiTmpVertexCount;i++)
	{
		LWIMP_Vertex *pVertex=&imp_object.VertexTbl[i];
		UINT j=0;
		BOOL bUniform=FALSE;
		do
		{
			bUniform=TRUE;
			UINT uiRefSmoothingGroup=0;
			UINT uiTmp=pVertex->FaceList.size();
			for(j=0;j<pVertex->FaceList.size();j++)
			{
				if(j==0) {
					uiRefSmoothingGroup=imp_object.FaceTbl[pVertex->FaceList[j]].uiSmoothingGroup;
				}
				else
				{
					if(uiRefSmoothingGroup!=
						imp_object.FaceTbl[pVertex->FaceList[j]].uiSmoothingGroup)
					{
						bUniform=FALSE;
						break;
					}
				}
			}
			if(!bUniform)
			{
				uiRefSmoothingGroup=imp_object.FaceTbl[pVertex->FaceList[0]].uiSmoothingGroup;
				LWIMP_Vertex tmp_vertex=*pVertex;
				//all faces using RefSmoothingGroup are going to use tmp_vertex
				//rest is left using old vertex
				std::vector<int>::iterator iter=tmp_vertex.FaceList.end();
				do
				{
					iter-=1;
					if(iter>=tmp_vertex.FaceList.begin())
					{
						if(imp_object.FaceTbl[*iter].uiSmoothingGroup!=uiRefSmoothingGroup)
						{
							tmp_vertex.FaceList.erase(iter);
							iter=tmp_vertex.FaceList.end();
						}
					}
				} while(iter>tmp_vertex.FaceList.begin());
				UINT uiNewVertexIndex=imp_object.VertexTbl.size();
				imp_object.VertexTbl.push_back(tmp_vertex);
				pVertex=&imp_object.VertexTbl[i];
				for(j=0;j<pVertex->FaceList.size();j++)
				{
					LWIMP_Face &rfFace=imp_object.FaceTbl[pVertex->FaceList[j]];
					if(rfFace.uiSmoothingGroup==uiRefSmoothingGroup)
					{
						if(rfFace.uiVertex1Index==i)
						{
							rfFace.uiVertex1Index=uiNewVertexIndex;
						}
						if(rfFace.uiVertex2Index==i)
						{
							rfFace.uiVertex2Index=uiNewVertexIndex;
						}
						if(rfFace.uiVertex3Index==i)
						{
							rfFace.uiVertex3Index=uiNewVertexIndex;
						}
					}
				}
				iter=pVertex->FaceList.end();
				do
				{
					iter-=1;
					if(iter>=pVertex->FaceList.begin())
					{
						if(imp_object.FaceTbl[*iter].uiSmoothingGroup==uiRefSmoothingGroup)
						{
							pVertex->FaceList.erase(iter);
							iter=pVertex->FaceList.end();
						}
					}
				} while(iter>pVertex->FaceList.begin());
			}
		} while(!bUniform);
	}
	imp_object.EdgeTbl.clear();
	//build edge table
/*	std::hash_map<LWIMP_Edge,UINT,hashEdge,eqEdge>	EdgeHash;
	for(i=0;i<imp_object.FaceTbl.size();i++) {
		LWIMP_Edge tmp_edge;
		//(1,2) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge1Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge1Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge1Index=uiEdgeIndex;
		}
		//(2,3) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge2Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge2Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge2Index=uiEdgeIndex;
		}
		//(3,1) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge3Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge3Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge3Index=uiEdgeIndex;
		}
	}*/
	//average normals/tangents
	for(i=0;i<imp_object.VertexTbl.size();i++)
	{
		UINT j=0;
		float AccumNormal[3]={0.0f,0.0f,0.0f};
		float AccumTangent[3]={0.0f,0.0f,0.0f};
		for(j=0;j<imp_object.VertexTbl[i].FaceList.size();j++)
		{
			AccumNormal[0]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceNormal.x;
			AccumNormal[1]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceNormal.y;
			AccumNormal[2]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceNormal.z;
			AccumTangent[0]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceTangent.x;
			AccumTangent[1]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceTangent.y;
			AccumTangent[2]+=imp_object.FaceTbl[imp_object.VertexTbl[i].FaceList[j]].vFaceTangent.z;
		}
		float fLen=(float)sqrt(AccumNormal[0]*AccumNormal[0]+AccumNormal[1]*AccumNormal[1]+
			AccumNormal[2]*AccumNormal[2]);
		if(fLen>0.0f)
		{
			AccumNormal[0]/=fLen;
			AccumNormal[1]/=fLen;
			AccumNormal[2]/=fLen;
		}
		fLen=(float)sqrt(AccumTangent[0]*AccumTangent[0]+AccumTangent[1]*AccumTangent[1]+
			AccumTangent[2]*AccumTangent[2]);
		if(fLen>0.0f)
		{
			AccumTangent[0]/=fLen;
			AccumTangent[1]/=fLen;
			AccumTangent[2]/=fLen;
		}
		imp_object.VertexTbl[i].vNormal.x=AccumNormal[0];
		imp_object.VertexTbl[i].vNormal.y=AccumNormal[1];
		imp_object.VertexTbl[i].vNormal.z=AccumNormal[2];
		imp_object.VertexTbl[i].vTangent.x=AccumTangent[0];
		imp_object.VertexTbl[i].vTangent.y=AccumTangent[1];
		imp_object.VertexTbl[i].vTangent.z=AccumTangent[2];
	}
	return TRUE;
}

//"split by surface" - after that two surfaces don't
//share _any_ vertices
BOOL splitBySurface(LWIMP_Object &imp_object)
{
	UINT i=0;
	UINT uiTmpVertexCount=imp_object.VertexTbl.size();
	for(i=0;i<uiTmpVertexCount;i++)
	{
		LWIMP_Vertex *pVertex=&imp_object.VertexTbl[i];
		UINT j=0;
		BOOL bUniform=FALSE;
		do
		{
			bUniform=TRUE;
			UINT uiRefSurface=0;
			UINT uiTmp=pVertex->FaceList.size();
			for(j=0;j<pVertex->FaceList.size();j++)
			{
				if(j==0)
				{
					uiRefSurface=imp_object.FaceTbl[pVertex->FaceList[j]].uiSurfaceIndex;
				}
				else
				{
					if(uiRefSurface!=
						imp_object.FaceTbl[pVertex->FaceList[j]].uiSurfaceIndex)
					{
						bUniform=FALSE;
						break;
					}
				}
			}
			if(!bUniform)
			{
				uiRefSurface=imp_object.FaceTbl[pVertex->FaceList[0]].uiSurfaceIndex;
				LWIMP_Vertex tmp_vertex=*pVertex;
				//all faces using RefSurface are going to use tmp_vertex
				//rest is left using old vertex
				std::vector<int>::iterator iter=tmp_vertex.FaceList.end();
				do
				{
					iter-=1;
					if(iter>=tmp_vertex.FaceList.begin())
					{
						if(imp_object.FaceTbl[*iter].uiSurfaceIndex!=uiRefSurface)
						{
							tmp_vertex.FaceList.erase(iter);
							iter=tmp_vertex.FaceList.end();
						}
					}
				} while(iter>tmp_vertex.FaceList.begin());
				UINT uiNewVertexIndex=imp_object.VertexTbl.size();
				imp_object.VertexTbl.push_back(tmp_vertex);
				pVertex=&imp_object.VertexTbl[i];
				for(j=0;j<pVertex->FaceList.size();j++)
				{
					LWIMP_Face &rfFace=imp_object.FaceTbl[pVertex->FaceList[j]];
					if(rfFace.uiSurfaceIndex==uiRefSurface)
					{
						if(rfFace.uiVertex1Index==i)
						{
							rfFace.uiVertex1Index=uiNewVertexIndex;
						}
						if(rfFace.uiVertex2Index==i)
						{
							rfFace.uiVertex2Index=uiNewVertexIndex;
						}
						if(rfFace.uiVertex3Index==i)
						{
							rfFace.uiVertex3Index=uiNewVertexIndex;
						}
					}
				}
				iter=pVertex->FaceList.end();
				do
				{
					iter-=1;
					if(iter>=pVertex->FaceList.begin())
					{
						if(imp_object.FaceTbl[*iter].uiSurfaceIndex==uiRefSurface)
						{
							pVertex->FaceList.erase(iter);
							iter=pVertex->FaceList.end();
						}
					}
				} while(iter>pVertex->FaceList.begin());
			}
		} while(!bUniform);
	}
	imp_object.EdgeTbl.clear();
	//build edge table
/*	std::hash_map<LWIMP_Edge,UINT,hashEdge,eqEdge>	EdgeHash;
	for(i=0;i<imp_object.FaceTbl.size();i++) {
		LWIMP_Edge tmp_edge;
		//(1,2) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge1Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge1Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge1Index=uiEdgeIndex;
		}
		//(2,3) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex2Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge2Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge2Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge2Index=uiEdgeIndex;
		}
		//(3,1) edge
		ZeroMemory(&tmp_edge,sizeof(LWIMP_Edge));
		tmp_edge.uiVertex1Index=imp_object.FaceTbl[i].uiVertex3Index;
		tmp_edge.uiVertex2Index=imp_object.FaceTbl[i].uiVertex1Index;
		tmp_edge.uiFace1Index=i;
		if(EdgeHash.find(tmp_edge)==EdgeHash.end()) {
			imp_object.EdgeTbl.push_back(tmp_edge);
			imp_object.FaceTbl[i].uiEdge3Index=imp_object.EdgeTbl.size()-1;
			EdgeHash[imp_object.EdgeTbl[imp_object.EdgeTbl.size()-1]]=imp_object.FaceTbl[i].uiEdge3Index;
		}
		else {
			UINT uiEdgeIndex=EdgeHash[tmp_edge];
			if(imp_object.EdgeTbl[uiEdgeIndex].bFace2Present) {
				MessageBox(NULL,"More than two faces sharing an edge!!!",
					"LWO2 import error!!!",MB_TASKMODAL);
				return FALSE;
			}
			imp_object.EdgeTbl[uiEdgeIndex].bFace2Present=TRUE;
			imp_object.EdgeTbl[uiEdgeIndex].uiFace2Index=i;
			imp_object.FaceTbl[i].uiEdge3Index=uiEdgeIndex;
		}
	}*/
	return TRUE;
}

//algorithm used here is suboptimal (some vertices are possibly
//doubled more times than needed!!!) and may result in presence of
//_unused_ vertices. Unused vertices are ok as long as
//extractSurface eliminates them.
//Edge list is no longer valid after this step!!!
BOOL splitByVMAD(LWIMP_Object &imp_object,LW_Object &lwo)
{
	UINT i=0;
	for(i=0;i<imp_object.FaceTbl.size();i++)
	{
		BOOL bVertex1Splitted=FALSE;
		BOOL bVertex2Splitted=FALSE;
		BOOL bVertex3Splitted=FALSE;
		UINT j=0;
		for(j=0;j<imp_object.FaceTbl[i].VMADDescList.size();j++)
		{
			//first check if this VMAD is used in currently processed surface
			BOOL bUsed=FALSE;
			UINT k=0;
			for(k=0;k<lwo.Surfaces[imp_object.FaceTbl[i].uiSurfaceIndex].uiSurfaceBlockCount;k++)
			{
				if(*((int*)(lwo.Surfaces[imp_object.FaceTbl[i].uiSurfaceIndex].
					SurfaceBlocks[k].BlockHeader.Type))=='IMAP')
				{
					if(lwo.Surfaces[imp_object.FaceTbl[i].uiSurfaceIndex].
						SurfaceBlocks[k].usProjectionMode==TMAP_PROJECTION_MODE_UV)
					{
						if(strcmp(lwo.Surfaces[imp_object.FaceTbl[i].uiSurfaceIndex].
							SurfaceBlocks[k].szVMapName,
							lwo.VMads[imp_object.FaceTbl[i].VMADDescList[j].uiVMADIndex].szName)==0)
						{
							bUsed=TRUE;
						}
					}
				}
			}
			if(!bUsed)
			{
				continue;
			}
			if(imp_object.FaceTbl[i].VMADDescList[j].bOverrideVertex1)
			{
				if(!bVertex1Splitted)
				{
					LWIMP_Vertex new_vertex=imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex1Index];
					UINT uiNewVertexIndex=imp_object.VertexTbl.size();
					imp_object.VertexTbl.push_back(new_vertex);
					imp_object.FaceTbl[i].uiVertex1Index=uiNewVertexIndex;
					bVertex1Splitted=TRUE;
				}
			}
			if(imp_object.FaceTbl[i].VMADDescList[j].bOverrideVertex2)
			{
				if(!bVertex2Splitted)
				{
					LWIMP_Vertex new_vertex=imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex2Index];
					UINT uiNewVertexIndex=imp_object.VertexTbl.size();
					imp_object.VertexTbl.push_back(new_vertex);
					imp_object.FaceTbl[i].uiVertex2Index=uiNewVertexIndex;
					bVertex2Splitted=TRUE;
				}
			}
			if(imp_object.FaceTbl[i].VMADDescList[j].bOverrideVertex3)
			{
				if(!bVertex3Splitted)
				{
					LWIMP_Vertex new_vertex=imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex3Index];
					UINT uiNewVertexIndex=imp_object.VertexTbl.size();
					imp_object.VertexTbl.push_back(new_vertex);
					imp_object.FaceTbl[i].uiVertex3Index=uiNewVertexIndex;
					bVertex3Splitted=TRUE;
				}
			}
		}
	}
	return TRUE;
}

//DO NOT release pEnvelope!!!
static BOOL copyLWEnvelopeToLREnvelope(LW_Envelope &rfSrcEnvelope,LR::EnvelopePtr pEnvelope)
{
	if(!pEnvelope)
	{
		return FALSE;
	}
	pEnvelope->clear();
	int i=0;
	for(i=0;i<rfSrcEnvelope.iKeyCount;i++)
	{
		long lTmp=0;
		if(isChunkOfType(rfSrcEnvelope.pKeys[i].SpanType,'TCB '))
		{
			pEnvelope->addKey(&lTmp,(long)(rfSrcEnvelope.pKeys[i].fTime*30),rfSrcEnvelope.pKeys[i].fValue,
				LR::SHAPE_TCB,rfSrcEnvelope.pKeys[i].fSpanParam0,rfSrcEnvelope.pKeys[i].fSpanParam1,
				rfSrcEnvelope.pKeys[i].fSpanParam2);
		}
		else if(isChunkOfType(rfSrcEnvelope.pKeys[i].SpanType,'BEZI'))
		{
			pEnvelope->addKey(&lTmp,(long)(rfSrcEnvelope.pKeys[i].fTime*30),rfSrcEnvelope.pKeys[i].fValue,
				LR::SHAPE_BEZI,0.0f,0.0f,0.0f,rfSrcEnvelope.pKeys[i].fSpanParam0,rfSrcEnvelope.pKeys[i].fSpanParam1);
		}
		else if(isChunkOfType(rfSrcEnvelope.pKeys[i].SpanType,'BEZ2'))
		{
			pEnvelope->addKey(&lTmp,(long)(rfSrcEnvelope.pKeys[i].fTime*30),rfSrcEnvelope.pKeys[i].fValue,
				LR::SHAPE_BEZ2,0.0f,0.0f,0.0f,rfSrcEnvelope.pKeys[i].fSpanParam0,rfSrcEnvelope.pKeys[i].fSpanParam1,
				rfSrcEnvelope.pKeys[i].fSpanParam2,rfSrcEnvelope.pKeys[i].fSpanParam3);
		}
		else if(isChunkOfType(rfSrcEnvelope.pKeys[i].SpanType,'LINE'))
		{
			pEnvelope->addKey(&lTmp,(long)(rfSrcEnvelope.pKeys[i].fTime*30),rfSrcEnvelope.pKeys[i].fValue,
				LR::SHAPE_LINE);
		}
		else if(isChunkOfType(rfSrcEnvelope.pKeys[i].SpanType,'STEP'))
		{
			pEnvelope->addKey(&lTmp,(long)(rfSrcEnvelope.pKeys[i].fTime*30),rfSrcEnvelope.pKeys[i].fValue,
				LR::SHAPE_STEP);
		}
		else
		{
			MessageBox(NULL,L"Unknown envelope span type!!",L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
	}
	return TRUE;
}

//output path may be ralative or absolute!!!
static void convertLWPathToWin32Path(char szLWPath[256],char szWin32Path[256])
{
	std::string str_out="";
	if(strlen(szLWPath)<3)
	{
		strcpy(szWin32Path,szLWPath);
		return;
	}
	if(szLWPath[1]==':')
	{
		str_out+=szLWPath[0];
		str_out+=szLWPath[1];
		str_out+="\\";
		str_out+=&(szLWPath[2]);
	}
	else
	{
		str_out+=szLWPath;
	}
	//change '/' to '\'
	UINT i=0;
	for(i=0;i<str_out.size();i++)
	{
		if(str_out[i]=='/')
		{
			str_out[i]='\\';
		}
	}
	strcpy(szWin32Path,str_out.c_str());
}

//try to find file in the following order:
//- absolute or relative path as specified
//- subdirectories relative to object file location: ..\textures, ..\images
//- same directory as object file
//return FALSE if not found
//TODO: try by scene root? (low priority)
static BOOL findFileByLWPath(char szObjectFilePath[256],char szLWPath[256],char szReadFileName[256])
{
	std::string obj_dir=extractDirectoryNameFromFullPath(szObjectFilePath);
	char szWin32Path[256];
	convertLWPathToWin32Path(szLWPath,szWin32Path);
	if(szWin32Path[1]==':')
	{
		//try exact fit
		FILE *f=NULL;
		f=fopen(szWin32Path,"rb");
		if(f)
		{
			fclose(f);
			strcpy(szReadFileName,szWin32Path);
			return TRUE;
		}
	}
	else
	{
		//try relative path (assume that LW content dir is one dir higher)
		std::string relative_path=obj_dir;
		relative_path+="..\\";
		relative_path+=szWin32Path;
		FILE *f=NULL;
		f=fopen(relative_path.c_str(),"rb");
		if(f)
		{
			fclose(f);
			strcpy(szReadFileName,relative_path.c_str());
			return TRUE;
		}
	}
	std::string filename=extractFileNameFromFullPath(szWin32Path);
	std::string tmp_str="";
	//- /textures
	tmp_str=obj_dir;
	tmp_str+="..\\textures\\";
	tmp_str+=filename;
	FILE *f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	//- /images
	tmp_str=obj_dir;
	tmp_str+="..\\images\\";
	tmp_str+=filename;
	f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	//object directory
	tmp_str=obj_dir;
	tmp_str+=filename;
	f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	return FALSE;
}

#define COLOR_CHANNEL_INDEX			0
#define DIFFUSE_CHANNEL_INDEX		1
#define LUMINOSITY_CHANNEL_INDEX	2
#define SPECULARITY_CHANNEL_INDEX	3
#define GLOSSINESS_CHANNEL_INDEX	4
#define REFLECTIVITY_CHANNEL_INDEX	5
#define TRANSPARENCY_CHANNEL_INDEX	6
#define RIND_CHANNEL_INDEX			7
#define TRNL_CHANNEL_INDEX			8
#define BUMP_CHANNEL_INDEX			9

//build and save final .flm,.lrmh and .lrmt files
BOOL extractSurface(LWIMP_Object &imp_object,LW_Object &lwo,UINT uiSurfaceIndex,
					LWO2ImportDescStruct &import_desc,LWO2ImportResult *pResult)
{
	if(lwo.uiSurfaceCount<(uiSurfaceIndex+1))
	{
		MessageBox(NULL,L"Trying to extract non-existing surface",L"LWO2 import error",MB_TASKMODAL);
		return FALSE;
	}
	LW_Surface& rfSurface=lwo.Surfaces[uiSurfaceIndex];
#ifdef _DEBUG
	{
		//TODO
/*		wchar_t wszMsg[256];
		szMsg[0]=0;
		strcpy(szMsg,L"Extracting surface: ");
		strcat(szMsg,lwo.Surfaces[uiSurfaceIndex].szName);
		strcat(szMsg,"\n");
		OutputDebugString(szMsg);*/
	}
#endif
	//number of texture coordinates used in this surface
	UINT uiTexcoordCount=0;
	//table below contains indices into LWIMP_Object::UVSetDescTbl
	//up to 7 coordinate sets are supported (tangent vector is always put into
	//first texture coordinate), needed for UV generation
	UINT TexcoordToUVDescMap[7];
	//table below lets easy mapping of original LW channels to real vertex buffer
	//texcoords
	UINT ChannelToTexcoordMap[10];
	UINT i=0;
	for(i=0;i<10;i++)
	{
		ChannelToTexcoordMap[i]=0xffffffff;
		if(i<7)
		{
			TexcoordToUVDescMap[i]=0xffffffff;
		}
	}
	for(i=0;i<rfSurface.uiSurfaceBlockCount;i++)
	{
		UINT uiChannelIndex=0xffffffff;
		if(*((int*)(rfSurface.SurfaceBlocks[i].BlockHeader.Type))=='IMAP')
		{
			switch(*((int*)(rfSurface.SurfaceBlocks[i].BlockHeader.TextureChannel)))
			{
				case 'COLR':
					uiChannelIndex=COLOR_CHANNEL_INDEX;
					break;
				case 'DIFF':
					uiChannelIndex=DIFFUSE_CHANNEL_INDEX;
					break;
				case 'LUMI':
					uiChannelIndex=LUMINOSITY_CHANNEL_INDEX;
					break;
				case 'SPEC':
					uiChannelIndex=SPECULARITY_CHANNEL_INDEX;
					break;
				case 'GLOS':
					uiChannelIndex=GLOSSINESS_CHANNEL_INDEX;
					break;
				case 'REFL':
					uiChannelIndex=REFLECTIVITY_CHANNEL_INDEX;
					break;
				case 'TRAN':	//no support yet
					//TODO: transparency texture support
					break;
				case 'RIND':	//no support yet
					break;
				case 'TRNL':	//no support yet
					break;
				case 'BUMP':
					uiChannelIndex=BUMP_CHANNEL_INDEX;
					break;
				default:
					MessageBox(NULL,L"Unknown texture channel",L"LWO2 import warning",MB_TASKMODAL);
					break;
			}
		}
		if(uiChannelIndex!=0xffffffff)
		{
			//ok, we are going to use this texture so we need to find uvdesc for this block
			if(ChannelToTexcoordMap[uiChannelIndex]!=0xffffffff)
			{
				//multiple texture layers are not supported!!!
				MessageBox(NULL,L"Multiple texture layers are not supported!!",
					L"LWO2 import critical warning",MB_TASKMODAL);
			}
			else
			{
				UINT j=0;
				//importer actually allows importing of UV set without
				//any texture associated so every UV set (associated with given surface)
				for(j=0;j<imp_object.UVSetDescTbl.size();j++)
				{
					if((imp_object.UVSetDescTbl[j].uiSurfaceIndex!=uiSurfaceIndex) &&
						(imp_object.UVSetDescTbl[j].uiMappingType==3))
					{
						continue;		//we don't want implicit uv's from other surfaces
					}
					if((imp_object.UVSetDescTbl[j].uiMappingType==3) &&
						(imp_object.UVSetDescTbl[j].uiSurfaceIndex==uiSurfaceIndex) &&
						(imp_object.UVSetDescTbl[j].uiSurfaceBlockIndex==i))
					{	
						//implicit mapping related to this surface block
						if(uiTexcoordCount==7)
						{
							MessageBox(NULL,L"Too many texture coordinates in surface",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
						TexcoordToUVDescMap[uiTexcoordCount++]=j;
						ChannelToTexcoordMap[uiChannelIndex]=uiTexcoordCount-1;
					}
					else
					{
						if(imp_object.UVSetDescTbl[j].uiMappingType!=3)
						{
							int k=0;
							int l=0;
							//ok,we got vmap/vmad, check if this mapping is used in this surface block
							BOOL bMappingUsed=TRUE;
							if(imp_object.UVSetDescTbl[j].uiMappingType!=2)
							{		//not VMAD only
								if(strcmp(rfSurface.SurfaceBlocks[i].szVMapName,
									lwo.VMaps[imp_object.UVSetDescTbl[j].uiVMAPIndex].szName)!=0)
								{
									bMappingUsed=FALSE;
								}
							}
							if(imp_object.UVSetDescTbl[j].uiMappingType!=0)
							{		//not VMAP only
								if(strcmp(rfSurface.SurfaceBlocks[i].szVMapName,
									lwo.VMads[imp_object.UVSetDescTbl[j].uiVMADIndex].szName)!=0)
								{
									bMappingUsed=FALSE;
								}
							}
							if(bMappingUsed)
							{
								if(uiTexcoordCount==7)
								{
									MessageBox(NULL,L"Too many texture coordinates in surface",
										L"LWO2 import error",MB_TASKMODAL);
									return FALSE;
								}
								TexcoordToUVDescMap[uiTexcoordCount++]=j;
								ChannelToTexcoordMap[uiChannelIndex]=uiTexcoordCount-1;
							}
						}
					}
				}
			}
		}
	}
	//now we have to build vertex declaration
	//only ONE stream for now
	D3DVERTEXELEMENT9 MainVertexDecl[MAX_STREAM_DECL_COMPONENTS];
	for(i=0;i<MAX_STREAM_DECL_COMPONENTS;i++)
	{
		MainVertexDecl[i].Stream=0xff;
		MainVertexDecl[i].Offset=0;
		MainVertexDecl[i].Type=D3DDECLTYPE_UNUSED;
		MainVertexDecl[i].Method=0;
		MainVertexDecl[i].Usage=0;
		MainVertexDecl[i].UsageIndex=0;
	}
	//position
	MainVertexDecl[0].Stream=0;
	MainVertexDecl[0].Offset=0;
	MainVertexDecl[0].Type=D3DDECLTYPE_FLOAT3;
	MainVertexDecl[0].Method=D3DDECLMETHOD_DEFAULT;
	MainVertexDecl[0].Usage=D3DDECLUSAGE_POSITION;
	MainVertexDecl[0].UsageIndex=0;
	//normal
	MainVertexDecl[1].Stream=0;
	MainVertexDecl[1].Offset=12;
	MainVertexDecl[1].Type=D3DDECLTYPE_FLOAT3;
	MainVertexDecl[1].Method=D3DDECLMETHOD_DEFAULT;
	MainVertexDecl[1].Usage=D3DDECLUSAGE_NORMAL;
	MainVertexDecl[1].UsageIndex=0;
	UINT uiBaseTexCoordIndex=2;
	UINT uiBaseTexCoordOffset=24;
	if(import_desc.bMakeTangentSpace)
	{
		uiBaseTexCoordIndex=3;
		uiBaseTexCoordOffset=36;
		//tangent
		MainVertexDecl[2].Stream=0;
		MainVertexDecl[2].Offset=12;
		MainVertexDecl[2].Type=D3DDECLTYPE_FLOAT3;
		MainVertexDecl[2].Method=D3DDECLMETHOD_DEFAULT;
		MainVertexDecl[2].Usage=D3DDECLUSAGE_TEXCOORD;
		MainVertexDecl[2].UsageIndex=0;
#ifdef _DEBUG
		OutputDebugString(L"Putting tangent space vector into vertex declaration\n");
#endif
	}
	for(i=0;i<uiTexcoordCount;i++)
	{
		MainVertexDecl[i+uiBaseTexCoordIndex].Stream=0;
		MainVertexDecl[i+uiBaseTexCoordIndex].Offset=uiBaseTexCoordOffset+2*sizeof(float)*i;
		MainVertexDecl[i+uiBaseTexCoordIndex].Type=D3DDECLTYPE_FLOAT2;
		MainVertexDecl[i+uiBaseTexCoordIndex].Method=D3DDECLMETHOD_DEFAULT;
		MainVertexDecl[i+uiBaseTexCoordIndex].Usage=D3DDECLUSAGE_TEXCOORD;
		MainVertexDecl[i+uiBaseTexCoordIndex].UsageIndex=(uiBaseTexCoordIndex==2) ? i : (i+1);
	}
	UINT uiVertexSize=uiBaseTexCoordOffset+2*sizeof(float)*i;
	UINT uiSurfaceVertexCount=0;
	UINT uiSurfacePrimitiveCount=0;
	UINT *pVertexIndexMap=new UINT[imp_object.VertexTbl.size()];
	for(i=0;i<imp_object.VertexTbl.size();i++)
	{
		pVertexIndexMap[i]=0xffffffff;
	}
	for(i=0;i<imp_object.FaceTbl.size();i++)
	{
		if(imp_object.FaceTbl[i].uiSurfaceIndex==uiSurfaceIndex)
		{
			uiSurfacePrimitiveCount++;
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex1Index]==0xffffffff)
			{
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex1Index]=uiSurfaceVertexCount++;
			}
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex2Index]==0xffffffff)
			{
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex2Index]=uiSurfaceVertexCount++;
			}
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex3Index]==0xffffffff)
			{
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex3Index]=uiSurfaceVertexCount++;
			}
		}
	}
	if(uiSurfacePrimitiveCount>65534)
	{
		MessageBox(NULL,L"Too many triangles in surface!!!",L"LWO2 import error",MB_TASKMODAL);
		return FALSE;
	}
	if(uiSurfaceVertexCount==0)
	{
		return TRUE;
	}
	BYTE *pVertexBuffer=new BYTE[uiVertexSize*uiSurfaceVertexCount];
	BYTE *pIndexBuffer=new BYTE[sizeof(USHORT)*3*uiSurfacePrimitiveCount];
	uiSurfaceVertexCount=0;
	uiSurfacePrimitiveCount=0;
	for(i=0;i<imp_object.VertexTbl.size();i++)
	{
		pVertexIndexMap[i]=0xffffffff;
	}
	for(i=0;i<imp_object.FaceTbl.size();i++)
	{
		if(imp_object.FaceTbl[i].uiSurfaceIndex==uiSurfaceIndex)
		{
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex1Index]==0xffffffff)
			{
				//emit vertex
				void *pNewVertex=&pVertexBuffer[uiSurfaceVertexCount*uiVertexSize];
				LWIMP_Vertex &rfImpVertex=
					imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex1Index];
				((float*)pNewVertex)[0]=lwo.pPNTS[rfImpVertex.uiPositionIndex].x;
				((float*)pNewVertex)[1]=lwo.pPNTS[rfImpVertex.uiPositionIndex].y;
				((float*)pNewVertex)[2]=lwo.pPNTS[rfImpVertex.uiPositionIndex].z;
				((float*)pNewVertex)[3]=rfImpVertex.vNormal.x;
				((float*)pNewVertex)[4]=rfImpVertex.vNormal.y;
				((float*)pNewVertex)[5]=rfImpVertex.vNormal.z;
				if(import_desc.bMakeTangentSpace)
				{
					((float*)pNewVertex)[6]=rfImpVertex.vTangent.x;
					((float*)pNewVertex)[7]=rfImpVertex.vTangent.y;
					((float*)pNewVertex)[8]=rfImpVertex.vTangent.z;
				}
				UINT j=0;
				for(j=0;j<uiTexcoordCount;j++)
				{
					getVertexUV(i,imp_object.FaceTbl[i].uiVertex1Index,TexcoordToUVDescMap[j],
						imp_object,lwo,
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+2*j],
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+1+2*j]);
				}
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex1Index]=uiSurfaceVertexCount++;
			}
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex2Index]==0xffffffff)
			{
				//emit vertex
				void *pNewVertex=&pVertexBuffer[uiSurfaceVertexCount*uiVertexSize];
				LWIMP_Vertex &rfImpVertex=
					imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex2Index];
				((float*)pNewVertex)[0]=lwo.pPNTS[rfImpVertex.uiPositionIndex].x;
				((float*)pNewVertex)[1]=lwo.pPNTS[rfImpVertex.uiPositionIndex].y;
				((float*)pNewVertex)[2]=lwo.pPNTS[rfImpVertex.uiPositionIndex].z;
				((float*)pNewVertex)[3]=rfImpVertex.vNormal.x;
				((float*)pNewVertex)[4]=rfImpVertex.vNormal.y;
				((float*)pNewVertex)[5]=rfImpVertex.vNormal.z;
				if(import_desc.bMakeTangentSpace)
				{
					((float*)pNewVertex)[6]=rfImpVertex.vTangent.x;
					((float*)pNewVertex)[7]=rfImpVertex.vTangent.y;
					((float*)pNewVertex)[8]=rfImpVertex.vTangent.z;
				}
				UINT j=0;
				for(j=0;j<uiTexcoordCount;j++)
				{
					getVertexUV(i,imp_object.FaceTbl[i].uiVertex2Index,TexcoordToUVDescMap[j],
						imp_object,lwo,
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+2*j],
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+1+2*j]);
				}
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex2Index]=uiSurfaceVertexCount++;
			}
			if(pVertexIndexMap[imp_object.FaceTbl[i].uiVertex3Index]==0xffffffff)
			{
				//emit vertex
				void *pNewVertex=&pVertexBuffer[uiSurfaceVertexCount*uiVertexSize];
				LWIMP_Vertex &rfImpVertex=
					imp_object.VertexTbl[imp_object.FaceTbl[i].uiVertex3Index];
				((float*)pNewVertex)[0]=lwo.pPNTS[rfImpVertex.uiPositionIndex].x;
				((float*)pNewVertex)[1]=lwo.pPNTS[rfImpVertex.uiPositionIndex].y;
				((float*)pNewVertex)[2]=lwo.pPNTS[rfImpVertex.uiPositionIndex].z;
				((float*)pNewVertex)[3]=rfImpVertex.vNormal.x;
				((float*)pNewVertex)[4]=rfImpVertex.vNormal.y;
				((float*)pNewVertex)[5]=rfImpVertex.vNormal.z;
				if(import_desc.bMakeTangentSpace)
				{
					((float*)pNewVertex)[6]=rfImpVertex.vTangent.x;
					((float*)pNewVertex)[7]=rfImpVertex.vTangent.y;
					((float*)pNewVertex)[8]=rfImpVertex.vTangent.z;
				}
				UINT j=0;
				for(j=0;j<uiTexcoordCount;j++)
				{
					getVertexUV(i,imp_object.FaceTbl[i].uiVertex3Index,TexcoordToUVDescMap[j],
						imp_object,lwo,
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+2*j],
						&((float*)pNewVertex)[uiBaseTexCoordIndex*3+1+2*j]);
				}
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex3Index]=uiSurfaceVertexCount++;
			}
			//emit triangle
			((USHORT*)pIndexBuffer)[uiSurfacePrimitiveCount*3]=
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex1Index];
			((USHORT*)pIndexBuffer)[uiSurfacePrimitiveCount*3+1]=
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex2Index];
			((USHORT*)pIndexBuffer)[uiSurfacePrimitiveCount*3+2]=
				pVertexIndexMap[imp_object.FaceTbl[i].uiVertex3Index];
			uiSurfacePrimitiveCount++;
		}
	}
	delete [] pVertexIndexMap;
	pVertexIndexMap=NULL;
	//write .flm file
	std::string FLMName=import_desc.szOutputFLMBaseName;
	FLMName+="_";
	FLMName+=lwo.Surfaces[uiSurfaceIndex].szName;
	FLMName+=".flm";
	FILE *f_flm=NULL;
	std::string FullFileName=import_desc.szOutputRootDir;
	FullFileName+="\\";
	FullFileName+=FLMName;
	if(import_desc.bOutputGeometry)
	{
		f_flm=fopen(FullFileName.c_str(),"wb");
		if(!f_flm)
		{
			MessageBox(NULL,L"Failed to open output .flm file!!!",
				L"LWO2 import error",MB_TASKMODAL);
			delete [] pVertexBuffer;
			delete [] pIndexBuffer;
			return FALSE;
		}
		//write file
		const char szFLMVersion1ID[]="FLM1";
		fwrite(szFLMVersion1ID,1,4,f_flm);
		DWORD dwFLMPrimitiveType=0;					//currently only triangles are supported
		fwrite(&dwFLMPrimitiveType,1,4,f_flm);
		fwrite(MainVertexDecl,1,MAX_STREAM_DECL_COMPONENTS*sizeof(D3DVERTEXELEMENT9),f_flm);
		DWORD dwFLMIndexFormat=0;					//currently only USHORT (D3DFMT_INDEX16)
		fwrite(&dwFLMIndexFormat,1,4,f_flm);
		DWORD dwFLMIndexCount=uiSurfacePrimitiveCount*3;
		fwrite(&dwFLMIndexCount,1,4,f_flm);
		DWORD dwFLMVertexCount=uiSurfaceVertexCount;
		fwrite(&dwFLMVertexCount,1,4,f_flm);
		DWORD dwFLMStreamCount=1;
		fwrite(&dwFLMStreamCount,1,4,f_flm);
		//we need to calculate bbox first ...
		float min_x=0.0f;
		float max_x=0.0f;
		float min_y=0.0f;
		float max_y=0.0f;
		float min_z=0.0f;
		float max_z=0.0f;
		min_x=max_x=((float*)&pVertexBuffer[0])[0];
		min_y=max_y=((float*)&pVertexBuffer[0])[1];
		min_z=max_z=((float*)&pVertexBuffer[0])[2];
		for(i=0;i<uiSurfaceVertexCount;i++)
		{
			if(((float*)&pVertexBuffer[i*uiVertexSize])[0]<min_x)
			{
				min_x=((float*)&pVertexBuffer[i*uiVertexSize])[0];
			}
			if(((float*)&pVertexBuffer[i*uiVertexSize])[0]>max_x)
			{
				max_x=((float*)&pVertexBuffer[i*uiVertexSize])[0];
			}
			if(((float*)&pVertexBuffer[i*uiVertexSize])[1]<min_y)
			{
				min_y=((float*)&pVertexBuffer[i*uiVertexSize])[1];
			}
			if(((float*)&pVertexBuffer[i*uiVertexSize])[1]>max_y)
			{
				max_y=((float*)&pVertexBuffer[i*uiVertexSize])[1];
			}
			if(((float*)&pVertexBuffer[i*uiVertexSize])[2]<min_z)
			{
				min_z=((float*)&pVertexBuffer[i*uiVertexSize])[2];
			}
			if(((float*)&pVertexBuffer[i*uiVertexSize])[2]>max_z)
			{
				max_z=((float*)&pVertexBuffer[i*uiVertexSize])[2];
			}
		}
		fwrite(&min_x,1,4,f_flm);
		fwrite(&max_x,1,4,f_flm);
		fwrite(&min_y,1,4,f_flm);
		fwrite(&max_y,1,4,f_flm);
		fwrite(&min_z,1,4,f_flm);
		fwrite(&max_z,1,4,f_flm);
		//index data
		DWORD dwUncompressedIBSize=dwFLMIndexCount*sizeof(USHORT);
		DWORD dwCompressedIBSize=(dwFLMIndexCount*sizeof(USHORT))*11/10+12;
		unsigned char*	pTempIBCompressOutBuf=new unsigned char[dwCompressedIBSize];
		if(compress(pTempIBCompressOutBuf,&dwCompressedIBSize,pIndexBuffer,
			dwUncompressedIBSize)!=Z_OK)
		{
			MessageBox(NULL,L"Compress error",L"LWO2 import error",MB_TASKMODAL);
			delete [] pVertexBuffer;
			delete [] pIndexBuffer;
			delete [] pTempIBCompressOutBuf;
		}
		DWORD dwIndexCompression=1;			//ZLIB
		fwrite(&dwIndexCompression,1,4,f_flm);
		fwrite(&dwCompressedIBSize,1,4,f_flm);
		fwrite(&dwUncompressedIBSize,1,4,f_flm);
		fwrite(pTempIBCompressOutBuf,1,dwCompressedIBSize,f_flm);
		delete [] pTempIBCompressOutBuf;
		pTempIBCompressOutBuf=NULL;
		//vertex data
		DWORD dwUncompressedVBSize=dwFLMVertexCount*uiVertexSize;
		DWORD dwCompressedVBSize=dwFLMVertexCount*uiVertexSize*11/10+12;
		unsigned char*	pTempVBCompressOutBuf=new unsigned char[dwCompressedVBSize];
		if(compress(pTempVBCompressOutBuf,&dwCompressedVBSize,pVertexBuffer,
			dwUncompressedVBSize)!=Z_OK)
		{
			MessageBox(NULL,L"Compress error",L"LWO2 import error",MB_TASKMODAL);
			delete [] pVertexBuffer;
			delete [] pIndexBuffer;
			delete [] pTempVBCompressOutBuf;
		}
		DWORD dwVertexCompression=1;		//ZLIB
		fwrite(&dwVertexCompression,1,4,f_flm);
		fwrite(&dwCompressedVBSize,1,4,f_flm);
		fwrite(&dwUncompressedVBSize,1,4,f_flm);
		fwrite(pTempVBCompressOutBuf,1,dwCompressedVBSize,f_flm);
		delete [] pTempVBCompressOutBuf;
		pTempVBCompressOutBuf=NULL;
		fclose(f_flm);
		f_flm=NULL;
	}
	delete [] pVertexBuffer;
	delete [] pIndexBuffer;
	//output mesh file (remember that offsetting texcoords indices by one when tangent is present
	//is shader's task
	LR::AutoPtr<LR::MeshNode> pMeshNode;		//we may need to re-save it after we create material
	if(import_desc.bOutputMesh)
	{
		LR::MeshCreateStruct MeshCreateStruct;
		ZeroMemory(&MeshCreateStruct,sizeof(LR::MeshCreateStruct));
		MeshCreateStruct.bRenderable=TRUE;
		strcpy(MeshCreateStruct.szClass,"FileMesh");
		strcpy(MeshCreateStruct.szSurface,"");
		std::string MeshName=import_desc.szOutputFLMBaseName;
		MeshName+="_";
		MeshName+=lwo.Surfaces[uiSurfaceIndex].szName;
		strcpy(pResult->MeshNameTbl[pResult->uiMeshCount],MeshName.c_str());
		MeshName+=".lrmh";
		strcpy(MeshCreateStruct.szMeshFileName,MeshName.c_str());
		strcpy(MeshCreateStruct.szSuperClass,"Standard");
		try
		{
			globals.engine->createMesh(globals.filesystem,&MeshCreateStruct,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			//mesh node most probably already exists in memory cache - that means no flush occured
			//before calling import function, mesh with this name already exists AND
			//is loaded
			MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		LR::AutoPtr<LR::Property> pFLMNameProp;
		try
		{
			pMeshNode->getPropertyByName("GeometryFile",pFLMNameProp);
		}
		catch(LR::Exception&)
		{
			//TODO proper handling
			MessageBox(NULL,L"Failed to get property from mesh!!!\nCritical error!!!",
				L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		pFLMNameProp->setPropertyValue(FLMName.c_str());
		pResult->uiMeshCount++;
	}
	//output material
	if(import_desc.bOutputMaterial)
	{
		//write .lrmt file
		std::string MaterialBaseName=import_desc.szOutputFLMBaseName;
		MaterialBaseName+="_";
		MaterialBaseName+=lwo.Surfaces[uiSurfaceIndex].szName;
		LR::ShaderPtr			pTmpShader=NULL;
		LR::SurfacePtr			pSurface=NULL;
		try
		{
			globals.engine->createShader("Default","Standard",pTmpShader);
			pTmpShader->createSurface(MaterialBaseName.c_str(),pSurface);
		}
		catch(LR::Exception &e)
		{
			//material may already exist in memory cache - that means no flush occured
			//before calling import function and material with this name already exists AND
			//is loaded
			//TODO: should probably be little more forgiving here
			MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		LR::AutoPtr<LR::Material> pMaterial;
		pMaterial.lr_dyn_cast(pSurface);
		if(pMaterial.isNull())
		{
			MessageBox(NULL,L"Dynamic cast failed!!!!\nContact your favorite programmer!",
				L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
		//start with diffuse color (retrieved from COLR chunk data)
		LR::ColorPtr pDiffuseCol;
		pMaterial->getDiffuseColor(&pDiffuseCol);
		if(pDiffuseCol)
		{
			//if color or diffuse texture is present then ignore COLR chunk
			//this is to make LW importer consistent with what appears on LW renderings
			//when layer opacity for color or diffuse texture is set to 100%
			//(as tezture layers aren't supported (yet?) layer opacity is always
			//assumed to be 100%
			BOOL bScrapCOLRData=FALSE;
			for(i=0;i<lwo.Surfaces[uiSurfaceIndex].uiSurfaceBlockCount;i++)
			{
				if(isChunkOfType(lwo.Surfaces[uiSurfaceIndex].
					SurfaceBlocks[i].BlockHeader.Type,'IMAP'))
				{
					if(isChunkOfType(lwo.Surfaces[uiSurfaceIndex].
						SurfaceBlocks[i].BlockHeader.TextureChannel,'COLR') ||
						isChunkOfType(lwo.Surfaces[uiSurfaceIndex].
						SurfaceBlocks[i].BlockHeader.TextureChannel,'DIFF'))
					{
						bScrapCOLRData=TRUE;
						break;
					}
				}
			}
			if(bScrapCOLRData)
			{
				//simply assign it (1.0,1.0,1.0,1.0) and ignore possible intensity envelope
				LR::EnvelopePtr pColorChannel;
				//TODO: check HRESULT's
				pDiffuseCol->getChannel(LR::COLOR_CHANNEL_R,&pColorChannel);
				if(pColorChannel)
				{
					pColorChannel->clear();
					long lKeyIndex=0;
					pColorChannel->addKey(&lKeyIndex,0,1.0f);
				}
				pDiffuseCol->getChannel(LR::COLOR_CHANNEL_G,&pColorChannel);
				if(pColorChannel)
				{
					pColorChannel->clear();
					long lKeyIndex=0;
					pColorChannel->addKey(&lKeyIndex,0,1.0f);
				}
				pDiffuseCol->getChannel(LR::COLOR_CHANNEL_B,&pColorChannel);
				if(pColorChannel)
				{
					pColorChannel->clear();
					long lKeyIndex=0;
					pColorChannel->addKey(&lKeyIndex,0,1.0f);
				}
				pDiffuseCol->getChannel(LR::COLOR_CHANNEL_A,&pColorChannel);
				if(pColorChannel)
				{
					pColorChannel->clear();
					long lKeyIndex=0;
					pColorChannel->addKey(&lKeyIndex,0,1.0f);
				}
			}
			else
			{
				//if color is enveloped then use enveloped values
				if(lwo.Surfaces[uiSurfaceIndex].uiColorEnvIndex!=0)
				{
					//remember that color uses three envelopes and only first
					//index out of three is given
					LR::EnvelopePtr pColorChannel;
					//TODO: check HRESULT's
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_R,&pColorChannel);
					if(pColorChannel)
					{
						if(!copyLWEnvelopeToLREnvelope(
							lwo.Envelopes[lwo.Surfaces[uiSurfaceIndex].uiColorEnvIndex-1],
							pColorChannel))
						{
							MessageBox(NULL,L"Failed to extract envelope from LWO object!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_G,&pColorChannel);
					if(pColorChannel)
					{
						if(!copyLWEnvelopeToLREnvelope(
							lwo.Envelopes[lwo.Surfaces[uiSurfaceIndex].uiColorEnvIndex],
							pColorChannel))
						{
							MessageBox(NULL,L"Failed to extract envelope from LWO object!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_B,&pColorChannel);
					if(pColorChannel)
					{
						if(!copyLWEnvelopeToLREnvelope(
							lwo.Envelopes[lwo.Surfaces[uiSurfaceIndex].uiColorEnvIndex+1],
							pColorChannel))
						{
							MessageBox(NULL,L"Failed to extract envelope from LWO object!!!",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_A,&pColorChannel);
					if(pColorChannel)
					{
						pColorChannel->clear();
						long lKeyIndex=0;
						pColorChannel->addKey(&lKeyIndex,0,1.0f);
					}
				}
				else
				{		//just use values supplied in COLR
					LR::EnvelopePtr pColorChannel;
					//TODO: check HRESULT's
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_R,&pColorChannel);
					if(pColorChannel)
					{
						pColorChannel->clear();
						long lKeyIndex=0;
						pColorChannel->addKey(&lKeyIndex,0,lwo.Surfaces[uiSurfaceIndex].fColorR);
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_G,&pColorChannel);
					if(pColorChannel)
					{
						pColorChannel->clear();
						long lKeyIndex=0;
						pColorChannel->addKey(&lKeyIndex,0,lwo.Surfaces[uiSurfaceIndex].fColorG);
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_B,&pColorChannel);
					if(pColorChannel)
					{
						pColorChannel->clear();
						long lKeyIndex=0;
						pColorChannel->addKey(&lKeyIndex,0,lwo.Surfaces[uiSurfaceIndex].fColorB);
					}
					pDiffuseCol->getChannel(LR::COLOR_CHANNEL_A,&pColorChannel);
					if(pColorChannel)
					{
						pColorChannel->clear();
						long lKeyIndex=0;
						pColorChannel->addKey(&lKeyIndex,0,1.0f);
					}
				}
			}
		}
		//as for now set specular color to (1.0,1.0,1.0,1.0)
		//TODO: possibly import "Color Highlights" envelope (not of high priority)
		//generally easy to import when only one from base color/color highlights
		//is enveloped (TODO!!!), harder when bot are (quite unlikely situation,
		//but investigate how to do it properly (intersection of two envelopes)!!!);
		LR::ColorPtr pSpecularCol;
		pMaterial->getSpecularColor(&pSpecularCol);
		if(pSpecularCol)
		{
			//simply assign it (1.0,1.0,1.0,1.0) and ignore possible intensity envelope
			LR::EnvelopePtr pColorChannel;
			//TODO: check HRESULT's
			pSpecularCol->getChannel(LR::COLOR_CHANNEL_R,&pColorChannel);
			if(pColorChannel)
			{
				pColorChannel->clear();
				long lKeyIndex=0;
				pColorChannel->addKey(&lKeyIndex,0,1.0f);
			}
			pSpecularCol->getChannel(LR::COLOR_CHANNEL_G,&pColorChannel);
			if(pColorChannel)
			{
				pColorChannel->clear();
				long lKeyIndex=0;
				pColorChannel->addKey(&lKeyIndex,0,1.0f);
			}
			pSpecularCol->getChannel(LR::COLOR_CHANNEL_B,&pColorChannel);
			if(pColorChannel)
			{
				pColorChannel->clear();
				long lKeyIndex=0;
				pColorChannel->addKey(&lKeyIndex,0,1.0f);
			}
			pSpecularCol->getChannel(LR::COLOR_CHANNEL_A,&pColorChannel);
			if(pColorChannel)
			{
				pColorChannel->clear();
				long lKeyIndex=0;
				pColorChannel->addKey(&lKeyIndex,0,1.0f);
			}
		}
		//copy diffuse to ambient (LW nas no such parameter as "material ambient"
		//and simply take diffuse value, LR uses independent colors
		LR::ColorPtr pAmbientCol;
		pMaterial->getAmbientColor(&pAmbientCol);
		if(pAmbientCol)
		{
			pMaterial->getDiffuseColor(&pDiffuseCol);
			if(pDiffuseCol)
			{
				pAmbientCol->assign(pDiffuseCol);
			}
		}
		//set up transparency settings
		if((lwo.Surfaces[uiSurfaceIndex].fTransparency!=0.0f) ||
			(lwo.Surfaces[uiSurfaceIndex].uiTransparencyEnvIndex!=0))
		{
			if(lwo.Surfaces[uiSurfaceIndex].uiTransparencyEnvIndex!=0)
			{
				LR::EnvelopePtr pTransparencyEnv;
				pMaterial->getTransparency(&pTransparencyEnv);
				if(pTransparencyEnv)
				{
					if(!copyLWEnvelopeToLREnvelope(
						lwo.Envelopes[lwo.Surfaces[uiSurfaceIndex].uiTransparencyEnvIndex-1],
						pTransparencyEnv))
					{
						MessageBox(NULL,L"Failed to extract envelope from LWO object!!!",
							L"LWO2 import error",MB_TASKMODAL);
						return FALSE;
					}
					//complement all keys to get engine friendly values
					long lNumKeys=pTransparencyEnv->getNumKeys();
					long lI=0;
					for(lI=0;lI<lNumKeys;lI++)
					{
						float fTempVal=pTransparencyEnv->getKeyValueAtIndex((unsigned int)lI);
						pTransparencyEnv->setKeyValueAtIndex((unsigned int)lI,1.0f-fTempVal);
					}
				}
			}
			else
			{
				LR::EnvelopePtr pTransparencyEnv;
				pMaterial->getTransparency(&pTransparencyEnv);
				if(pTransparencyEnv)
				{
					pTransparencyEnv->clear();
					long lKeyIndex=0;
					pTransparencyEnv->addKey(&lKeyIndex,0,1.0f-lwo.Surfaces[uiSurfaceIndex].fTransparency);
				}
			}
			//set transparency mode to "Blend"
			//TODO: support for additive transparency
			pMaterial->setTransparencyMode(1);
		}
		else
		{	//no transparency at all
			//TODO: define symblic values for transparency modes
			pMaterial->setTransparencyMode(0);
		}
		//textures
		//there are 10 texture channels in LW:COLR,DIFF,LUMI,SPEC,GLOS,REFL,TRAN,RIND,TRNL,BUMP
		//not all are of our interest, also multiple texture layers are not supported
		//mapping of LW surface channels to LRMaterial entries:
		//COLR - Color
		//LUMI - Luminosity
		//DIFF - Diffuse
		//SPEC (LW's Specularity) - Gloss
		//GLOS (LW's Glossiness) - Specular exponent
		//REFL - Reflectivity
		//TRAN (LW's transparency) - no direct mapping, use color/diffuse tex alpha!
		//RIND (LW's refraction index) - currently not used
		//TRNL (LW's translucency) - currently not used
		//BUMP - Bump
		//cache to discard multiple texture layers
		BOOL bSlotUsed[10];
		for(i=0;i<10;i++)
		{
			bSlotUsed[i]=FALSE;
		}
		for(i=0;i<lwo.Surfaces[uiSurfaceIndex].uiSurfaceBlockCount;i++)
		{
			if(isChunkOfType(lwo.Surfaces[uiSurfaceIndex].
				SurfaceBlocks[i].BlockHeader.Type,'IMAP'))
			{
				UINT uiChannelIndex=0xffffffff;
				switch(*((int*)(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[i].BlockHeader.TextureChannel)))
				{
					case 'COLR':
						uiChannelIndex=COLOR_CHANNEL_INDEX;
						break;
					case 'DIFF':
						uiChannelIndex=DIFFUSE_CHANNEL_INDEX;
						break;
					case 'LUMI':
						uiChannelIndex=LUMINOSITY_CHANNEL_INDEX;
						break;
					case 'SPEC':
						uiChannelIndex=SPECULARITY_CHANNEL_INDEX;
						break;
					case 'GLOS':	//no support in default shader
						//uiChannelIndex=GLOSSINESS_CHANNEL_INDEX;
						break;
					case 'REFL':
						uiChannelIndex=REFLECTIVITY_CHANNEL_INDEX;
						break;
					case 'TRAN':	//no support yet
						//TODO: transparency texture support
						break;
					case 'RIND':	//no support yet
						break;
					case 'TRNL':	//no support yet
						break;
					case 'BUMP':	//no support in default shader
						//uiChannelIndex=BUMP_CHANNEL_INDEX;
						break;
					default:
						MessageBox(NULL,L"Unknown texture channel",
							L"LWO2 (second stage) import warning",MB_TASKMODAL);
						break;
				}
				if(uiChannelIndex!=0xffffffff)
				{
					if(bSlotUsed[uiChannelIndex])
					{
						MessageBox(NULL,L"Multiple texture layers are not supported!!",
							L"LWO2 import critical warning",MB_TASKMODAL);
					}
					else
					{
						if(lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[i].uiImageMapIndex==0)
						{
							continue;		//sometimes LWO contains BLOK chunk for channel
											//that is of type IMAP and has no associated texture
						}
						//import texture and its mapping
						bSlotUsed[uiChannelIndex]=TRUE;
						//first try to import image file to project
						//TODO: obey overwrite options
						char szWin32FileName[256];
						//TODO: match CLIP index!!!
						if(!findFileByLWPath(import_desc.szFileName,
							lwo.Clips[lwo.Surfaces[uiSurfaceIndex].SurfaceBlocks[i].uiImageMapIndex-1].szFileName,
							szWin32FileName))
						{
							char szMessage[1024];
							sprintf(szMessage,"Failed to find file (LWPath is %s)!!!",
								lwo.Clips[lwo.Surfaces[uiSurfaceIndex].
								SurfaceBlocks[i].uiImageMapIndex-1].szFileName);
							MessageBox(NULL,/*szMessage*/L"TODO: extract message",L"LWO2 import error!!!",MB_TASKMODAL);
//							MessageBox(NULL,"Failed to find file!!!","LWO2 import error!!!",MB_TASKMODAL);
							return FALSE;
						}
						//TODO: obey image overwrite options
						importFileToProject(szWin32FileName,FALSE);
						//create&save texture object based on that image
						std::string image_name=extractFileNameFromFullPath(szWin32FileName);
						char szImageName[256];
						strcpy(szImageName,image_name.c_str());
						char *pszTmp=&szImageName[0];
						while((*pszTmp!='\0') && (*pszTmp!='.'))
						{
							pszTmp++;
						}
						if(*pszTmp=='.')
						{
							*pszTmp='\0';
						}
						std::string texture_name=szImageName;
						texture_name+=".lrtx";
						LR::AutoPtr<LR::Texture> pTexture;
						try
						{
							globals.engine->createTexture(globals.filesystem,
								"Image","Standard",
								texture_name.c_str(),pTexture);
						}
						catch(LR::Exception &e)
						{
							MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
						LR::AutoPtr<LR::Property> pColorImage;
						try
						{
							pTexture->getPropertyByName("ColorImage",pColorImage);
							pColorImage->setPropertyValue(image_name.c_str());
						}
						catch(LR::Exception &e)
						{
							MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
								L"LWS import error",MB_TASKMODAL);
							return FALSE;
						}
						//attach to material
						LR::MT_TEXTURE eTextureId=LR::MTT_COLOR;
						switch(uiChannelIndex)
						{
							case COLOR_CHANNEL_INDEX:
								eTextureId=LR::MTT_COLOR;
								break;
							case DIFFUSE_CHANNEL_INDEX:
								eTextureId=LR::MTT_DIFFUSE;
								break;
							case LUMINOSITY_CHANNEL_INDEX:
								eTextureId=LR::MTT_LUMINOSITY;
								break;
							case SPECULARITY_CHANNEL_INDEX:
								eTextureId=LR::MTT_GLOSS;
								break;
							case GLOSSINESS_CHANNEL_INDEX:
								assert(false);
								//eTextureId=5;
								break;
							case REFLECTIVITY_CHANNEL_INDEX:
								eTextureId=LR::MTT_REFLECTIVITY;
								break;
							case BUMP_CHANNEL_INDEX:
								assert(false);
								//eTextureId=8;
								break;
						}
						try
						{
							globals.engine->saveTexture(globals.filesystem,pTexture);
						}
						catch(LR::Exception &e)
						{
							MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
								L"LWO2 import error",MB_TASKMODAL);
							return FALSE;
						}
						pMaterial->attachTexture(eTextureId,pTexture);
						//use channel to texcoord map, no special fx here for now
						pMaterial->setTextureCoordSetControl(eTextureId,ChannelToTexcoordMap[uiChannelIndex]);
					}
				}
			}
		}
		if(!pMeshNode.isNull())
		{
			pMeshNode->attachSurface(pSurface);
		}
		try
		{
			globals.engine->saveSurface(globals.filesystem,pSurface);
		}
		catch(LR::Exception &e)
		{
			MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
				L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
	}
	if(!pMeshNode.isNull())
	{
		//attach shader
		LR::AutoPtr<LR::Shader> pShader;
		try
		{
			globals.engine->saveMesh(globals.filesystem,pMeshNode);
		}
		catch(LR::Exception &e)
		{
			MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",L"LWO2 import error",MB_TASKMODAL);
			return FALSE;
		}
	}
	//free mesh node
	pMeshNode=NULL;			//we need this to actually flush _this_ mesh node
	return TRUE;
}

void freeLWObject(LW_Object *pObj)
{
	if(pObj)
	{
		if(pObj->pPNTS)
		{
			delete [] pObj->pPNTS;
		}
		if(pObj->pTriangles)
		{
			delete [] pObj->pTriangles;
		}
		UINT i=0;
		for(i=0;i<pObj->uiVMapCount;i++)
		{
			switch(pObj->VMaps[i].usDimension)
			{
				case 1:
					if(pObj->VMaps[i].pVMAP1D)
					{
						delete [] pObj->VMaps[i].pVMAP1D;
					}
					break;
				case 2:
					if(pObj->VMaps[i].pVMAP2D)
					{
						delete [] pObj->VMaps[i].pVMAP2D;
					}
					break;
				case 3:
					if(pObj->VMaps[i].pVMAP3D)
					{
						delete [] pObj->VMaps[i].pVMAP3D;
					}
					break;
			}
		}
		for(i=0;i<pObj->uiVMadCount;i++)
		{
			switch(pObj->VMads[i].usDimension)
			{
				case 1:
					if(pObj->VMads[i].pVMAD1D)
					{
						delete [] pObj->VMads[i].pVMAD1D;
					}
					break;
				case 2:
					if(pObj->VMads[i].pVMAD2D)
					{
						delete [] pObj->VMads[i].pVMAD2D;
					}
					break;
				case 3:
					if(pObj->VMads[i].pVMAD3D)
					{
						delete [] pObj->VMads[i].pVMAD3D;
					}
					break;
			}
		}
		for(i=0;i<pObj->uiEnvelopeCount;i++)
		{
			if(pObj->Envelopes[i].pKeys)
			{
				delete [] pObj->Envelopes[i].pKeys;
			}
		}
		delete pObj;
	}
}

BOOL importLWO2File(LWO2ImportDescStruct& rfImportDesc,LWO2ImportResult *pResult)
{
	ZeroMemory(pResult,sizeof(LWO2ImportResult));
	FILE *f=NULL;
	LW_Object	*pLwo=NULL;			//this has bo be dynamically allocated not to cause stack overflow
									//with default vc++ stack settings
	pLwo=new LW_Object();
#ifdef _DEBUG
	{
		wchar_t wszMsg[256];
		wsprintf(wszMsg,L"Allocated (12) %d bytes of memory for LW_Object structures\n",
			sizeof(LW_Object));
		OutputDebugString(wszMsg);
	}
#endif
	ZeroMemory(pLwo,sizeof(LW_Object));
	//first read LWO contents into memory without any modifications
	//(except minor ones, like flattening all layers :) )
	f=fopen(rfImportDesc.szFileName,"rb");
	if(!f)
	{
		MessageBox(NULL,L"Failed to open file!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	LW_ID4 szChunkID;
	ZeroMemory(szChunkID,sizeof(LW_ID4));
	if(!readID4(f,szChunkID))
	{
		MessageBox(NULL,L"File corrupted or not in LWO2 format!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	if(!isChunkOfType(szChunkID,'FORM'))
	{
		MessageBox(NULL,L"File not in LWO2 format!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	UINT uiFORMLength=0;
	if(!readU4(f,&uiFORMLength))
	{
		MessageBox(NULL,L"File corrupted or not in LWO2 format!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	if(!readID4(f,szChunkID))
	{
		MessageBox(NULL,L"File corrupted or not in LWO2 format!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	if(!isChunkOfType(szChunkID,'LWO2'))
	{
		MessageBox(NULL,L"File not in LWO2 format!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	if(!readLWO2(f,*pLwo,uiFORMLength-4))
	{
		MessageBox(NULL,L"Error or unsupported chunk in LWO2 file!!!",L"LWO2 import error",MB_TASKMODAL);
		fclose(f);
		return FALSE;
	}
	fclose(f);
	f=NULL;
	//build second stage mesh
	LWIMP_Object imp_object;
	if(!buildSecondStageMesh(*pLwo,imp_object))
	{
		MessageBox(NULL,L"Failed to build second stage mesh!!!",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return FALSE;

	}
	if(!calcFaceTangents(imp_object,*pLwo))
	{
		MessageBox(NULL,L"Failed to calculate tangent space",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return FALSE;
	}
	if(!processSmoothingGroups(imp_object,*pLwo,FALSE))
	{
		MessageBox(NULL,L"Failed to process smoothing groups!!!",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return FALSE;
	}
	if(!splitBySurface(imp_object))
	{
		MessageBox(NULL,L"Failed to perform split-by-surface operation!!!",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return FALSE;
	}
	if(!splitByVMAD(imp_object,*pLwo))
	{
		MessageBox(NULL,L"Failed to perform split-by-VMAD operation!!!",
			L"LWO2 import error!!!",MB_TASKMODAL);
		return FALSE;
	}
	UINT i=0;
	for(i=0;i<pLwo->uiSurfaceCount;i++)
	{
		if(!extractSurface(imp_object,*pLwo,i,rfImportDesc,pResult))
		{
			MessageBox(NULL,L"Failed to extract surface!!!",
				L"LWO2 import error!!!",MB_TASKMODAL);
		}
	}
	//free any memory used
	freeLWObject(pLwo);
	return TRUE;
}
