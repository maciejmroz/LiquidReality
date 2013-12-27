/*****************************************************************
 LiquidReality base mesh node plugins
 (c) 2002 Maciej Mróz
******************************************************************/

#include "StdAfx.h"
#include "LRPBaseMeshNodes.h"
#include "zlib.h"

using namespace LR;

//(pos,n,tex_tg,tex) - 44 bytes
static D3DVERTEXELEMENT9 sPerPixelTexVF[]=
{
	{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
	{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},
	{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	{0,32,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TANGENT,0},
	D3DDECL_END()
};

#define VFORMAT_SIZE 44

LRPSphereMesh::LRPSphereMesh()
{
	m_pRadiusEnv=NPluginDllGlobals::plg_pEngine->createEnvelope();
	m_dwFlipped=0;
	m_bStatic=FALSE;
	m_dwPrimitiveCount=0;
	m_dwVertexCount=0;
	m_pVertexDecl=NULL;
	m_bCreated=FALSE;
	m_bInCreate=FALSE;
}

LRPSphereMesh::~LRPSphereMesh()
{
	NPluginDllGlobals::plg_pEngine->destroyEnvelope(m_pRadiusEnv);
	clear();
	m_pRadiusEnv=NULL;
}

const char* LRPSphereMesh::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPSphereMesh::getObjectClass()
{
	return "Sphere";
}

void LRPSphereMesh::registerProperties()
{
	REGISTER_INTEGER_PROPERTY("UTesselation",&m_fTesselationU,20);
	REGISTER_INTEGER_PROPERTY("VTesselation",&m_fTesselationV,20);
	REGISTER_ENUM_PROPERTY("Flipped",&m_dwFlipped,0);
	ADD_ENUM_CHOICE("False");
	ADD_ENUM_CHOICE("True");
	REGISTER_ENVELOPE_PROPERTY("Radius",m_pRadiusEnv,1.0f);
}

HRESULT LRPSphereMesh::getNewInstance(BaseObject **ppOut)
{
	LRPSphereMesh	*pTmpMesh=new LRPSphereMesh();
	pTmpMesh->registerProperties();
	pTmpMesh->initPropertyMap();
	*ppOut=pTmpMesh;
	return S_OK;
}

void LRPSphereMesh::clear()
{
	MeshNodeImpl::clear();
	if(m_pRadiusEnv)
	{
		m_pRadiusEnv->clear();
	}
}

void LRPSphereMesh::initialize(AutoPtr<fs> &pfs)
{
	if(m_bCreated)
	{ 
		return;
	}
	if(m_bInCreate)
	{
		return;
	}
	m_bInCreate=TRUE;
	MeshNodeImpl::initialize(pfs);
	m_dwVertexCount=(DWORD)((m_fTesselationU+1)*(m_fTesselationV+1));
	m_dwPrimitiveCount=(DWORD)(m_fTesselationU*m_fTesselationV*2);
	long lNumKeys=m_pRadiusEnv->getNumKeys();
	if(lNumKeys<=1)
	{
		m_bStatic=TRUE;
	}
	else
	{
		m_bStatic=FALSE;
	}
	m_pVertexDecl=&sPerPixelTexVF[0];
	VBCreateStruct vbCreateStruct;
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	vbCreateStruct.bUsagePoints=FALSE;
	vbCreateStruct.dwSize=m_dwVertexCount;
	vbCreateStruct.dwStreamCount=1;
	//flags
	vbCreateStruct.streams[0].dwFlags=0;
	if(m_bRenderable)
	{
		vbCreateStruct.streams[0].dwFlags|=SF_RENDERABLE;
	}
	if(!m_bStatic)
	{
		vbCreateStruct.streams[0].dwFlags|=SF_DYNAMIC;
	}
	//stream mode
	if(m_bStatic)
	{
		vbCreateStruct.streams[0].eMode=SM_STATIC;
	}
	else
	{
		vbCreateStruct.streams[0].eMode=SM_DYNAMIC;
	}
	memcpy(&(vbCreateStruct.streams[0].ExternalDecl[0]),m_pVertexDecl,5*sizeof(D3DVERTEXELEMENT9));
	memcpy(&(vbCreateStruct.streams[0].InternalDecl[0]),m_pVertexDecl,5*sizeof(D3DVERTEXELEMENT9));
	m_pRenderer->createVertexBuffer(&vbCreateStruct,m_pVB);
	IBCreateStruct ibCreateStruct;
	ibCreateStruct.bUsagePoints=FALSE;
	ibCreateStruct.dwFlags=IBF_RENDERABLE;
	ibCreateStruct.Format=D3DFMT_INDEX16;
	ibCreateStruct.dwSize=3*m_dwPrimitiveCount;
	m_pRenderer->createIndexBuffer(&ibCreateStruct,m_pIB);

	//TODO: what if throws
	update(0.0f);
	m_bInCreate=false;
	m_bCreated=true;
}

void LRPSphereMesh::updateToMemPtr(void *VertexDataPtr,float fTime)
{
	//variables used by object generator
	int i=0,j=0;
	float s=0.0f,t=0.0f,r=0.0f;
	float cos_s=0.0f,cos_t=0.0f;
	float sin_s=0.0f,sin_t=0.0f;
	float *pTmpVBPtr=NULL;
	r=m_pRadiusEnv->evalEnvelope(fTime);
	m_fLastRadius=r;
	if(m_dwFlipped)
	{
		for(i=0;i<(int)(m_fTesselationU+1);i++)
		{
			for(j=0;j<(int)(m_fTesselationV+1);j++)
			{
				s=1.0f/m_fTesselationU*i;
				t=1.0f/m_fTesselationV*j;
				cos_s=FastCos(s*2.0f*PI);
				cos_t=FastCos((t-0.5f)*PI);
				sin_s=FastSin(s*2.0f*PI);
				sin_t=FastSin((t-0.5f)*PI);
				register float x=r*cos_t*cos_s;
				register float y=r*cos_t*sin_s;
				register float z=r*sin_t;
				//vertex pos
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j))]=x;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+1)]=y;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+2)]=z;
				//vertex normal
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+3)]=-x/r;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+4)]=-y/r;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+5)]=-z/r;
				//base texture coordinates
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+6)]=s;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+7)]=t;
				//tangent (dt partial derivatives, normalized)
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+8)]=-cos_s*sin_t;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+9)]=-sin_s*sin_t;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+10)]=cos_t;
			}
		}
	}
	else
	{
		for(i=0;i<(int)(m_fTesselationU+1);i++)
		{
			for(j=0;j<(int)(m_fTesselationV+1);j++)
			{
				s=1.0f/m_fTesselationU*i;
				t=1.0f/m_fTesselationV*j;
				cos_s=FastCos(s*2.0f*PI);
				cos_t=FastCos((t-0.5f)*PI);
				sin_s=FastSin(s*2.0f*PI);
				sin_t=FastSin((t-0.5f)*PI);
				register float x=r*cos_t*cos_s;
				register float y=r*cos_t*sin_s;
				register float z=r*sin_t;
				//vertex pos
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j))]=x;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+1)]=y;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+2)]=z;
				//vertex normal
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+3)]=x/r;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+4)]=y/r;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+5)]=z/r;
				//base texture coordinates
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+6)]=s;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+7)]=t;
				//tangent (dt partial derivatives, normalized)
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+8)]=-cos_s*sin_t;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+9)]=-sin_s*sin_t;
				((float*)VertexDataPtr)[(DWORD)(11*(i*(m_fTesselationV+1)+j)+10)]=cos_t;
			}
		}
	}
}

void LRPSphereMesh::update(float fTime)
{
	static float m_fLastUpdate=0.0f;
	if(m_bCreated)
	{
		if(fTime==m_fLastUpdate)
		{
			return;
		}
		float r=0.0f;
		r=m_pRadiusEnv->evalEnvelope(fTime);
		if(r==m_fLastRadius)
		{
			return;
		}
	}
	else
	{
		void *IndexDataPtr=NULL;
		//index buffer is always static for this mesh class
		IndexDataPtr=(void*)new USHORT[m_dwPrimitiveCount*3];
		DWORD dwI=0,dwJ=0;
		if(m_dwFlipped)
		{
			for(dwI=0;dwI<(DWORD)m_fTesselationU;dwI++)
			{
				for(dwJ=0;dwJ<(DWORD)m_fTesselationV;dwJ++)
				{
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ))]=(USHORT)((m_fTesselationV+1)*(dwI+1)+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+1)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+2)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ+1);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+3)]=(USHORT)((m_fTesselationV+1)*(dwI+1)+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+4)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ+1);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+5)]=(USHORT)((m_fTesselationV+1)*(dwI+1)+(dwJ+1));
				}
			}
		}
		else
		{
			for(dwI=0;dwI<m_fTesselationU;dwI++)
			{
				for(dwJ=0;dwJ<m_fTesselationV;dwJ++)
				{
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ))]=(USHORT)((m_fTesselationV+1)*(dwI+1)+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+1)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ+1);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+2)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+3)]=(USHORT)((m_fTesselationV+1)*(dwI+1)+dwJ);
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+4)]=(USHORT)((m_fTesselationV+1)*(dwI+1)+(dwJ+1));
					((USHORT*)IndexDataPtr)[(DWORD)(6*(m_fTesselationV*dwI+dwJ)+5)]=(USHORT)((m_fTesselationV+1)*dwI+dwJ+1);
				}
			}
		}
		try
		{
			m_pIB->loadIB(IndexDataPtr,3*m_dwPrimitiveCount,D3DFMT_INDEX16);
		}
		catch(Exception&)
		{
			delete [] IndexDataPtr;
			throw;
		}
		delete [] IndexDataPtr;
	}
	//use different API to access static vs dynamic vertex buffer
	if(m_bStatic && m_bCreated)
	{
		return;
	}
	if(m_bStatic)
	{
		AutoPtr<VertexStream> pStream;
		m_pVB->getVertexStream(0,pStream);
		void *VertexDataPtr=NULL;
		VertexDataPtr=new BYTE[m_dwVertexCount*VFORMAT_SIZE];
		try
		{
			updateToMemPtr(VertexDataPtr,0.0f);
			pStream->loadStream(VertexDataPtr,m_dwVertexCount,m_pVertexDecl);
			delete [] VertexDataPtr;
			VertexDataPtr=NULL;
		}
		catch(Exception&)
		{
			delete [] VertexDataPtr;
			VertexDataPtr=NULL;
			throw;
		}
	}
	else
	{
		AutoPtr<VertexStream> pStream;
		m_pVB->getVertexStream(0,pStream);
		void *VertexDataPtr=NULL;
		pStream->BaseWriteLock(&VertexDataPtr,m_dwVertexCount,VFORMAT_SIZE);
		updateToMemPtr(VertexDataPtr,fTime);
		pStream->unlock();
	}
}

PRIMITIVE_TYPE LRPSphereMesh::getPrimitiveType()
{
	return PT_INDEXED_TRIANGLES;
}

void LRPSphereMesh::getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount)
{
	*bStaticPrimitiveCount=TRUE;
	*pdwCount=m_dwPrimitiveCount;
}

const D3DVERTEXELEMENT9* LRPSphereMesh::getVertexDecl()
{
	return m_pVertexDecl;
}

void LRPSphereMesh::getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount)
{
	*bStaticVertexCount=TRUE;
	*pdwCount=m_dwVertexCount;
}

void LRPSphereMesh::getBoundingBox(float fTime,BoundingBox *pBBox)
{
	float fRadius=1.0f;
	fRadius=m_pRadiusEnv->evalEnvelope(fTime);
	pBBox->fMaxX=fRadius;
	pBBox->fMinX=fRadius;
	pBBox->fMaxY=fRadius;
	pBBox->fMinY=fRadius;
	pBBox->fMaxZ=fRadius;
	pBBox->fMinZ=fRadius;
}

DWORD LRPSphereMesh::getTexCoordCount()
{
	//switch(m_dwVertexFormat)
	//{
	//	case 0:
	//		return 0;
	//		break;
	//	case 1:
	//		return 1;
	//		break;
	//	case 2:
	//		return 0;
	//		break;
	//	case 3:
			return 1;
	//		break;
	//	case 4:
	//		return 2;
	//		break;
	//	default:
	//		return 0;
	//}
}

/*
 *	FileMesh implementation
 */

LRPFileMesh::LRPFileMesh()
{
	m_bHasTangent=FALSE;
	m_dwTexcoordCount=0;
	m_bCreated=FALSE;
	m_bInCreate=FALSE;
	ZeroMemory(&m_BBox,sizeof(BoundingBox));
}

LRPFileMesh::~LRPFileMesh()
{
	DEBUG_STRING("LRPFileMesh - entering destructor\n");
	DEBUG_STRING("LRPFileMesh - leaving destructor\n");
}

const char* LRPFileMesh::getObjectSuperClass()
{
	return "Standard";
}

const char* LRPFileMesh::getObjectClass()
{
	return "FileMesh";
}

void LRPFileMesh::registerProperties()
{
	MeshNodeImpl::registerProperties();
	REGISTER_FILENAME_PROPERTY("GeometryFile",&m_lstrFLMFile,"FLM files|*.flm||");
}

HRESULT LRPFileMesh::getNewInstance(BaseObject **ppOut)
{
	LRPFileMesh	*pTmpMesh=new LRPFileMesh();
	pTmpMesh->registerProperties();
	pTmpMesh->initPropertyMap();
	*ppOut=pTmpMesh;
	return S_OK;
}

void LRPFileMesh::clear()
{
	MeshNodeImpl::clear();
}

void LRPFileMesh::initialize(AutoPtr<fs> &pfs)
{
	if(m_bCreated)
	{ 
		return;
	}
	if(m_bInCreate)
	{
		return;
	}
	m_bInCreate=TRUE;
	MeshNodeImpl::initialize(pfs);
	//we need to read file header to get following:
	//vertex count and format
	//primitive count and (possibly) index format
	//maximum therotical FLM1 header size is 48+17*MAX_VERTEX_ELEMENTS*8=??? bytes
	//in practice it can be much less as current code does not support
	//multi-stream files
	BYTE FLMHeader[192];
	//first read just 48+MAX_VERTEX_ELEMENTS*8=192 (this will be enough for 99% cases)
	//try
	//{
		pfs->readFileHeader(m_lstrFLMFile.getPtr(),192,FLMHeader);
	//}
	//catch(IOException&)
	//{
	//	MessageBox(NULL,"Failed to read FLM header!!!","FileMesh error!",MB_TASKMODAL);
	//	throw;
	//}
	//check primitive type
	if(*((DWORD*)&FLMHeader[4])!=0)
	{	//only triangles for now
		//MessageBox(NULL,"For now only triangles are supported!!!",
		//	"FileMesh error!",MB_TASKMODAL);
		throw InvalidOperationExceptionImpl("FLM:For now only triangles are supported!!!");
	}
	//verify that stream count is 1
	if(*((DWORD*)&FLMHeader[164])!=1)
	{
		throw FileFormatExceptionImpl("FLM:For now only one stream is supported!!!");
	}
	//verify that index format is 16-bit
	if(*((DWORD*)&FLMHeader[152])!=0)
	{
		throw FileFormatExceptionImpl("FLM:For now only 16-bit indices are supported!!!");
	}
	memcpy(m_VertexDecl,(void*)&FLMHeader[8],144);
	VBCreateStruct vbCreateStruct;
	const D3DCAPS9 *pCaps=m_pRenderer->getHardwareCaps();
	vbCreateStruct.bUsagePoints=FALSE;
	vbCreateStruct.dwSize=*((DWORD*)&FLMHeader[160]);
	vbCreateStruct.dwStreamCount=1;
	//flags
	vbCreateStruct.streams[0].dwFlags=0;
	if(m_bRenderable)
	{
		vbCreateStruct.streams[0].dwFlags|=SF_RENDERABLE;
	}
	vbCreateStruct.streams[0].eMode=SM_STATIC;
	memcpy(&(vbCreateStruct.streams[0].ExternalDecl[0]),
		m_VertexDecl,MAX_STREAM_DECL_COMPONENTS*sizeof(D3DVERTEXELEMENT9));
	memcpy(&(vbCreateStruct.streams[0].InternalDecl[0]),
		m_VertexDecl,MAX_STREAM_DECL_COMPONENTS*sizeof(D3DVERTEXELEMENT9));
	m_pRenderer->createVertexBuffer(&vbCreateStruct,m_pVB);
	IBCreateStruct ibCreateStruct;
	ibCreateStruct.bUsagePoints=FALSE;
	ibCreateStruct.dwFlags=IBF_RENDERABLE;
	ibCreateStruct.Format=D3DFMT_INDEX16;
	ibCreateStruct.dwSize=*((DWORD*)&FLMHeader[156]);
	m_pRenderer->createIndexBuffer(&ibCreateStruct,m_pIB);
	m_dwPrimitiveCount=*((DWORD*)&FLMHeader[156])/3;
	m_dwVertexCount=*((DWORD*)&FLMHeader[160]);
	//bounding box, tangent vector presence cache, texcoord count
	//(vertex decl crack)
	D3DVERTEXELEMENT9 *pElem=&m_VertexDecl[0];
	m_bHasTangent=FALSE;
	m_dwTexcoordCount=0;
	ZeroMemory(&m_BBox,sizeof(BoundingBox));
	while(pElem->Stream!=0xff)
	{
		if(pElem->Usage==D3DDECLUSAGE_TEXCOORD)
		{
			if((pElem->UsageIndex==0) &&
				((pElem->Type==D3DDECLTYPE_FLOAT3)
				|| (pElem->Type==D3DDECLTYPE_DEC3N)))
			{
				m_bHasTangent=TRUE;
			}
			else
			{
				m_dwTexcoordCount++;
			}
		}
		pElem++;
	}
	memcpy(&m_BBox,&FLMHeader[168],6*sizeof(float));	

	//ok, we need to read real vertex data
	//for now assume 156 byte offset from the beginning
	ULONG uVFileHandle=0;
	unsigned char *pFileBuf=NULL;
	//try
	//{
		pfs->openFileAsPtr(m_lstrFLMFile.getPtr(),uVFileHandle,&pFileBuf);
	//}
	//catch(IOException &e)
	//{
		//MessageBox(NULL,e.getDescription(),"Exception caught during Mesh::create()",MB_TASKMODAL);
		//return E_FAIL;
	//}
	DWORD dwIBSize=3*m_dwPrimitiveCount*sizeof(USHORT);
	if(*((DWORD*)&pFileBuf[192])==0)
	{
		//+12 bytes from header end
		try
		{
			m_pIB->loadIB(&pFileBuf[204],3*m_dwPrimitiveCount,D3DFMT_INDEX16);
		}
		catch(Exception&)
		{
			pfs->closeFile(uVFileHandle);
			throw;
		}
	}
	else
	{
		BYTE *pIBDecompressBuf=new BYTE[*((DWORD*)&pFileBuf[200])];
		ULONG uTmp=*((DWORD*)&pFileBuf[200]);
		if(Z_OK!=uncompress(pIBDecompressBuf,&uTmp,
			&pFileBuf[204],*((DWORD*)&pFileBuf[196])))
		{
			pfs->closeFile(uVFileHandle);
			delete [] pIBDecompressBuf;
			throw InvalidOperationExceptionImpl("FLM:Failed to uncompress index buffer");
		}
		try
		{
			m_pIB->loadIB(pIBDecompressBuf,3*m_dwPrimitiveCount,D3DFMT_INDEX16);
		}
		catch(Exception&)
		{
			pfs->closeFile(uVFileHandle);
			delete [] pIBDecompressBuf;
			throw;
		}
		dwIBSize=*((DWORD*)&pFileBuf[196]);
		delete [] pIBDecompressBuf;
	}
	//vertex data
	BYTE *pVBDecompressBuf=NULL;
	if(*((DWORD*)&pFileBuf[192+12+dwIBSize])!=0)
	{
		pVBDecompressBuf=new BYTE[*((DWORD*)&pFileBuf[192+20+dwIBSize])];
		//decompress vertex data
		ULONG uTmp=*((DWORD*)&pFileBuf[192+20+dwIBSize]);
		if(Z_OK!=uncompress(pVBDecompressBuf,&uTmp,
			&pFileBuf[192+24+dwIBSize],*((DWORD*)&pFileBuf[192+16+dwIBSize])))
		{
			pfs->closeFile(uVFileHandle);
			delete [] pVBDecompressBuf;
			throw InvalidOperationExceptionImpl("FLM:Failed to uncompress vertex buffer");
		}
	}
	AutoPtr<VertexStream> pStream;
	try
	{
		m_pVB->getVertexStream(0,pStream);
	}
	catch(Exception &)
	{
		pfs->closeFile(uVFileHandle);
		delete [] pVBDecompressBuf;
		throw;
	}
	if(*((DWORD*)&pFileBuf[192+12+dwIBSize])==0)
	{
		try
		{
			pStream->loadStream(&pFileBuf[216+dwIBSize],
				m_dwVertexCount,&m_VertexDecl[0]);
		}
		catch(Exception&)
		{
			pfs->closeFile(uVFileHandle);
			delete [] pVBDecompressBuf;
			throw;
		}
	}
	else
	{
		try
		{
			pStream->loadStream(pVBDecompressBuf,
				m_dwVertexCount,&m_VertexDecl[0]);
		}
		catch(Exception &)
		{
			pfs->closeFile(uVFileHandle);
			delete [] pVBDecompressBuf;
			throw;
		}
	}
	pfs->closeFile(uVFileHandle);
	delete [] pVBDecompressBuf;
	m_bCreated=TRUE;
	m_bInCreate=FALSE;
}

void LRPFileMesh::update(float fTime)
{
}

PRIMITIVE_TYPE LRPFileMesh::getPrimitiveType()
{
	return PT_INDEXED_TRIANGLES;
}

void LRPFileMesh::getPrimitiveCount(bool *bStaticPrimitiveCount,DWORD *pdwCount)
{
	*bStaticPrimitiveCount=true;
	*pdwCount=m_dwPrimitiveCount;
}

const D3DVERTEXELEMENT9* LRPFileMesh::getVertexDecl()
{
	return &m_VertexDecl[0];
}

void LRPFileMesh::getVertexCount(bool *bStaticVertexCount,DWORD *pdwCount)
{
	*bStaticVertexCount=true;
	*pdwCount=m_dwVertexCount;
}

void LRPFileMesh::getBoundingBox(float fTime,BoundingBox *pBBox)
{
	memcpy(pBBox,&m_BBox,sizeof(BoundingBox));
}

DWORD LRPFileMesh::getTexCoordCount()
{
	return m_dwTexcoordCount;
}
