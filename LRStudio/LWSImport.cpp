/*
 *	Implementation od LWS->LiquidReality importer
 *  (c) 2002 Maciej Mróz
 */

#include "stdafx.h"
#include <string>
#include <set>
#include <vector>
#include <map>

#pragma unmanaged

enum LWS_TOKEN
{
	LWST_LWSC,
	LWST_FIRST_FRAME,
	LWST_LAST_FRAME,
	LWST_PREVIEW_FIRST_FRAME,
	LWST_PREVIEW_LAST_FRAME,
	LWST_PLUGIN,
	LWST_END_PLUGIN,
	LWST_LOAD_OBJECT_LAYER,
	LWST_OBJECT_MOTION,
	LWST_NUM_CHANNELS,
	LWST_CHANNEL,
	LWST_ENVELOPE,
	LWST_KEY,
	LWST_BEHAVIORS,
	LWST_AMBIENT_COLOR,
	LWST_AMBIENT_INTENSITY,
	LWST_GLOBAL_LIGHT_INTENSITY,
	LWST_ADD_LIGHT,
	LWST_LIGHT_NAME,
	LWST_LIGHT_MOTION,
	LWST_LIGHT_COLOR,
	LWST_LIGHT_INTENSITY,
	LWST_LIGHT_TYPE,
	LWST_LIGHT_RANGE,
	LWST_LIGHT_CONE_ANGLE,
	LWST_LIGHT_EDGE_ANGLE,
	LWST_ADD_CAMERA,
	LWST_CAMERA_NAME,
	LWST_CAMERA_MOTION,
	LWST_ZOOM_FACTOR,
	LWST_SUBPATCH_LEVEL,
	LWST_HCONTROLLER,
	LWST_PCONTROLLER,
	LWST_PARENT_ITEM,
	LWST_TARGET_ITEM,
	LWST_AFFECT_CAUSTICS,
	LWST_SHADOW_TYPE,
	LWST_LIGHT_FALLOFF_TYPE,
	LWST_ADD_NULL_OBJECT,
	LWST_BACKDROP_COLOR,
	LWST_FOG_TYPE,
	LWST_FOG_MIN_DISTANCE,
	LWST_FOG_MAX_DISTANCE,
	LWST_PIVOT_POSITION,
	LWST_LOCKED_CHANNELS,
	LWST_UNKNOWN		//unknown/unneeded identifiers+numbers 
};

struct LWSIMP_Token
{
	const char	*szTokenName;
	LWS_TOKEN	TokenID;
};

const LWSIMP_Token TokenTbl[]={
	{"LWSC",LWST_LWSC},
	{"FirstFrame",LWST_FIRST_FRAME},
	{"LastFrame",LWST_LAST_FRAME},
	{"PreviewFirstFrame",LWST_PREVIEW_FIRST_FRAME},
	{"PreviewLastFrame",LWST_PREVIEW_LAST_FRAME},
	{"Plugin",LWST_PLUGIN},
	{"EndPlugin",LWST_END_PLUGIN},
	{"LoadObjectLayer",LWST_LOAD_OBJECT_LAYER},
	{"ObjectMotion",LWST_OBJECT_MOTION},
	{"NumChannels",LWST_NUM_CHANNELS},
	{"Channel",LWST_CHANNEL},
	{"Envelope",LWST_ENVELOPE},
	{"Key",LWST_KEY},
	{"Behaviors",LWST_BEHAVIORS},
	{"AmbientColor",LWST_AMBIENT_COLOR},
	{"AmbientIntensity",LWST_AMBIENT_INTENSITY},
	{"GlobalLightIntensity",LWST_GLOBAL_LIGHT_INTENSITY},
	{"AddLight",LWST_ADD_LIGHT},
	{"LightName",LWST_LIGHT_NAME},
	{"LightMotion",LWST_LIGHT_MOTION},
	{"LightColor",LWST_LIGHT_COLOR},
	{"LightIntensity",LWST_LIGHT_INTENSITY},
	{"LightType",LWST_LIGHT_TYPE},
	{"LightRange",LWST_LIGHT_RANGE},
	{"LightConeAngle",LWST_LIGHT_CONE_ANGLE},
	{"LightEdgeAngle",LWST_LIGHT_EDGE_ANGLE},
	{"AddCamera",LWST_ADD_CAMERA},
	{"CameraName",LWST_CAMERA_NAME},
	{"CameraMotion",LWST_CAMERA_MOTION},
	{"ZoomFactor",LWST_ZOOM_FACTOR},
	{"SubPatchLevel",LWST_SUBPATCH_LEVEL},
	{"HController",LWST_HCONTROLLER},
	{"PController",LWST_PCONTROLLER},
	{"ParentItem",LWST_PARENT_ITEM},
	{"TargetItem",LWST_TARGET_ITEM},
	{"AffectCaustics",LWST_AFFECT_CAUSTICS},
	{"ShadowType",LWST_SHADOW_TYPE},
	{"LightFalloffType",LWST_LIGHT_FALLOFF_TYPE},
	{"AddNullObject",LWST_ADD_NULL_OBJECT},
	{"BackdropColor",LWST_BACKDROP_COLOR},
	{"FogType",LWST_FOG_TYPE},
	{"FogMinDistance",LWST_FOG_MIN_DISTANCE},
	{"FogMaxDistance",LWST_FOG_MAX_DISTANCE},
	{"PivotPosition",LWST_PIVOT_POSITION},
	{"LockedChannels",LWST_LOCKED_CHANNELS},
};

#define LWS_TOKEN_COUNT (sizeof(TokenTbl)/sizeof(LWSIMP_Token))

static bool lr_isspace(int c)
{
	switch(c)
	{
		case ' ':
		case '\t':
		case '\n':
			return true;
			break;
		default:
			return false;
	}
}

//return FALSE on EOF
static BOOL getLWSToken(FILE *f,char szTokenString[256],LWS_TOKEN &TokenID)
{
	int c=0;
	while((c=fgetc(f))!=EOF)
	{
		if((!lr_isspace(c)) && (c!='\n'))
		{
			break;
		}
	}
	if(c==EOF)
	{
		return FALSE;
	}
	UINT uiTokenPos=0;
	szTokenString[uiTokenPos++]=c;
	while((c=fgetc(f))!=EOF)
	{
		if((!lr_isspace(c)) && (c!='\n'))
		{
			szTokenString[uiTokenPos++]=c;
		}
		else
		{
			break;
		}
	}
	szTokenString[uiTokenPos++]='\0';
	UINT i=0;
	for(i=0;i<LWS_TOKEN_COUNT;i++)
	{
		if(strcmp(TokenTbl[i].szTokenName,szTokenString)==0)
		{
			TokenID=TokenTbl[i].TokenID;
			return TRUE;
		}
	}
	TokenID=LWST_UNKNOWN;
	return TRUE;
}

//return FALSE on EOF
//read everything up start of next line - then eat ending spaces
//useful for reading pathnames and object names
static BOOL getStringToEndOfLine(FILE *f,char szTokenString[256])
{
	int c=0;
	//first eat initial spaces
	while((c=fgetc(f))!=EOF)
	{
		if((!lr_isspace(c)) && (c!='\n'))
		{
			break;
		}
	}
	if(c==EOF)
	{
		return FALSE;
	}
	UINT uiTokenPos=0;
	szTokenString[uiTokenPos++]=c;
	while((c=fgetc(f))!=EOF)
	{
		if(c!='\n')
		{
			szTokenString[uiTokenPos++]=c;
		}
		else
		{
			break;
		}
	}
	szTokenString[uiTokenPos++]='\0';
	//ok we have it .. but we have to eat ending spaces
	char *pszTmp=szTokenString+strlen(szTokenString);
	while(pszTmp>szTokenString)
	{
		pszTmp--;
		if(lr_isspace(*pszTmp))
		{
			*pszTmp='\0';
		}
		else
		{
			break;
		}
	}
	if(strlen(szTokenString)==0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//output path may be ralative or absolute!!!
static void convertLWPathToWin32Path(char szLWPath[256],char szWin32Path[256]) {
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
//- subdirectories relative to scene file location: \objects, \textures, \images
//- same directory as scene file
//return FALSE if not found
static BOOL findFileByLWPath(LWSImportDescStruct& rfImportDesc,
							 char szLWPath[256],char szReadFileName[256])
{
	std::string scene_dir=extractDirectoryNameFromFullPath(rfImportDesc.szFileName);
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
		//try relative path
		std::string relative_path=scene_dir;
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
	//- /objects
	tmp_str=scene_dir;
	tmp_str+="objects\\";
	tmp_str+=filename;
	FILE *f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	//- /textures
	tmp_str=scene_dir;
	tmp_str+="textures\\";
	tmp_str+=filename;
	f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	//- /images
	tmp_str=scene_dir;
	tmp_str+="images\\";
	tmp_str+=filename;
	f=NULL;
	f=fopen(tmp_str.c_str(),"rb");
	if(f)
	{
		fclose(f);
		strcpy(szReadFileName,tmp_str.c_str());
		return TRUE;
	}
	//scene directory
	tmp_str=scene_dir;
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

static BOOL readEnvelopeFromLWS(FILE *f,LR::EnvelopePtr pEnv,BOOL bRad2Deg=FALSE)
{
	char		szTokenString[256];
	LWS_TOKEN	TokenID=LWST_UNKNOWN;
	pEnv->clear();
	//'{'
	if(fscanf(f,"%s",szTokenString)!=1)
	{
		return FALSE;
	}
	if(!getLWSToken(f,szTokenString,TokenID))
	{
		return FALSE;
	}
	if(TokenID!=LWST_ENVELOPE)
	{
		return FALSE;
	}
	int iNumKeys=0;
	if(fscanf(f,"%d",&iNumKeys)!=1)
	{
		return FALSE;
	}
	bool bDummyKey=false;
	if(iNumKeys==0)
	{
		//there's inconsistency in lws format - envelopes with zero keys
		//still have dummy key written to file ... we read it but not add
		//to envelope
		iNumKeys=1;
		bDummyKey=true;
	}
	int j=0;
	for(j=0;j<iNumKeys;j++)
	{
		if(!getLWSToken(f,szTokenString,TokenID))
		{
			return FALSE;
		}
		if(TokenID!=LWST_KEY)
		{
			return FALSE;
		}
		float fVal=0.0f;
		float fTime=0.0f;
		int iSpanType=0;
		float fP1=0.0f;
		float fP2=0.0f;
		float fP3=0.0f;
		float fP4=0.0f;
		float fP5=0.0f;
		float fP6=0.0f;
		if(fscanf(f,"%f %f %d %f %f %f %f %f %f",&fVal,&fTime,&iSpanType,
			&fP1,&fP2,&fP3,&fP4,&fP5,&fP6)!=9)
		{
			return FALSE;
		}
		if(bRad2Deg)
		{
			fVal=fVal/3.14159f*180.0f;
		}
		//add new key to envelope
		long lOutIndex=0;		//we don't really care :)
		if(!bDummyKey)
		{
			switch(iSpanType)
			{
				case 0:	//TCB
					//P4 and P5 are documented as incoming and outgoing tangents
					//we don't need them as they are calculated inside interpolation routine
					pEnv->addKey(&lOutIndex,(long)(fTime*30.0f),fVal,LR::SHAPE_TCB,fP1,fP2,fP3);
					break;
				case 1:	//Hermite
				case 2:	//1D Bezier (obsolete, same as hermite)
					//TODO: are TCB values used during iunterpolation?
					pEnv->addKey(&lOutIndex,(long)(fTime*30.0f),fVal,LR::SHAPE_HERM,fP1,fP2,fP3,fP4,fP5);
					break;
				case 3:	//Linear
					pEnv->addKey(&lOutIndex,(long)(fTime*30.0f),fVal,LR::SHAPE_LINE);
					break;
				case 4:	//Stepped
					pEnv->addKey(&lOutIndex,(long)(fTime*30.0f),fVal,LR::SHAPE_STEP);
					break;
				case 5:	//2D Bezier
					pEnv->addKey(&lOutIndex,(long)(fTime*30.0f),fVal,LR::SHAPE_BEZ2,
						0.0f,0.0f,0.0f,fP1,fP2,fP3,fP4);
						break;
				default:	//unknown/unsupported
					MessageBox(NULL,L"Unknown key span type!!!",
						L"LWS import error!",MB_TASKMODAL);
					return FALSE;
					break;
			}
		}
	}
	//Behaviors
	if(!getLWSToken(f,szTokenString,TokenID))
	{
		return FALSE;
	}
	if(TokenID!=LWST_BEHAVIORS)
	{
		return FALSE;
	}
	int iPre=0;
	int iPost=0;
	if(fscanf(f,"%d %d",&iPre,&iPost)!=2)
	{
		return FALSE;
	}
	switch(iPre)
	{
		case 0:		//reset
			pEnv->setPreBehavior(LR::BEH_RESET);
			break;
		case 1:		//constant
			pEnv->setPreBehavior(LR::BEH_CONSTANT);
			break;
		case 2:		//repeat
			pEnv->setPreBehavior(LR::BEH_REPEAT);
			break;
		case 3:		//oscillate
			pEnv->setPreBehavior(LR::BEH_OSCILLATE);
			break;
		case 4:		//offset repeat
			pEnv->setPreBehavior(LR::BEH_OFFSET);
			break;
		case 5:		//linear
			pEnv->setPreBehavior(LR::BEH_LINEAR);
			break;
	}
	switch(iPost)
	{
		case 0:		//reset
			pEnv->setPostBehavior(LR::BEH_RESET);
			break;
		case 1:		//constant
			pEnv->setPostBehavior(LR::BEH_CONSTANT);
			break;
		case 2:		//repeat
			pEnv->setPostBehavior(LR::BEH_REPEAT);
			break;
		case 3:		//oscillate
			pEnv->setPostBehavior(LR::BEH_OSCILLATE);
			break;
		case 4:		//offset repeat
			pEnv->setPostBehavior(LR::BEH_OFFSET);
			break;
		case 5:		//linear
			pEnv->setPostBehavior(LR::BEH_LINEAR);
			break;
	}
	//'}'
	if(fscanf(f,"%s",szTokenString)!=1)
	{
		return FALSE;
	}
	return TRUE;
}

//read ObjectMotion/LightMotion/CameraMotion and all following stuff
static BOOL readMotionFromLWS(FILE *f,LR::MotionPtr pMotion)
{
	char		szTokenString[256];
	LWS_TOKEN	TokenID=LWST_UNKNOWN;
	do
	{
		if(!getLWSToken(f,szTokenString,TokenID))
		{
			return FALSE;
		}
	} while((TokenID!=LWST_OBJECT_MOTION) &&
		(TokenID!=LWST_LIGHT_MOTION) &&
		(TokenID!=LWST_CAMERA_MOTION));
	pMotion->clear();
	//get number of channels
	int iNumChannels=0;
	if(!getLWSToken(f,szTokenString,TokenID))
	{
		return FALSE;
	}
	if(TokenID!=LWST_NUM_CHANNELS)
	{
		return FALSE;
	}
	if(fscanf(f,"%d",&iNumChannels)!=1)
	{
		return FALSE;
	}
	int i=0;
	for(i=0;i<iNumChannels;i++) {
		//read single channel
		if(!getLWSToken(f,szTokenString,TokenID))
		{
			return FALSE;
		}
		if(TokenID!=LWST_CHANNEL)
		{
			return FALSE;
		}
		int iChannelID=0;
		if(fscanf(f,"%d",&iChannelID)!=1)
		{
			return FALSE;
		}
		LR::EnvelopePtr pEnv;
		pMotion->getChannel(LR::MOT_CHANNEL_POSITION_X+i,&pEnv);
		if(!readEnvelopeFromLWS(f,pEnv,((i>=3) && (i<=5)) ? TRUE : FALSE))
		{
			return FALSE;
		}
	}
	return TRUE;
}

//LWS loader caches information about objects that have
//already been imported (for efficiency reasons nad to be able to
//assign correct numbers to transform nodes)
struct LWSIMP_LoadedObject
{
	std::string					ObjectName;
	DWORD						dwLoadCount;
	std::vector<std::string>	MeshNames;	//if mesh has been loaded previously
										//here we have mesh names to use with transform node
	LWSIMP_LoadedObject()
	{
		dwLoadCount=0;
	}

	LWSIMP_LoadedObject& operator=(const LWSIMP_LoadedObject &rfObject)
	{
		ObjectName=rfObject.ObjectName;
		dwLoadCount=rfObject.dwLoadCount;
		MeshNames=rfObject.MeshNames;
		return *this;
	}
};

//LW uses numbering that is not unified -
//each node type uses different numbering
enum LWSIMP_NODE_TYPE
{
	LNT_OBJECT,
	LNT_LIGHT,
	LNT_CAMERA
};

//transform node dependencies (parenting/targeting) are set
//at the end of import process (so we can turn node index->name
//used in lrsc file format)
//will probably be extended by bone specific info in future
struct LWSIMP_TransformNodeDesc
{
	std::string			NodeName;
	LWSIMP_NODE_TYPE	eParentType;
	int					iParentNodeIndex;
	LWSIMP_NODE_TYPE	eTargetType;
	int					iTargetNodeIndex;
	LWSIMP_NODE_TYPE	eNodeType;
	LWSIMP_TransformNodeDesc()
	{
		iParentNodeIndex=iTargetNodeIndex=-1;
		eNodeType=eTargetType=eParentType=LNT_OBJECT;
	}

	LWSIMP_TransformNodeDesc& operator=(const LWSIMP_TransformNodeDesc &rfDesc)
	{
		NodeName=rfDesc.NodeName;
		iParentNodeIndex=rfDesc.iParentNodeIndex;
		iTargetNodeIndex=rfDesc.iTargetNodeIndex;
		eParentType=rfDesc.eParentType;
		eTargetType=rfDesc.eTargetType;
		return *this;
	}
};

static int getNthObjectIndex(std::vector<LWSIMP_TransformNodeDesc> &LoadedTransformNodes,int iIndex)
{
	int iBase=0;
	unsigned int iCurrentIndex=0;
	BOOL bFound=FALSE;
	while(iCurrentIndex<LoadedTransformNodes.size())
	{
		if((LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_OBJECT) &&
			(iIndex==iBase))
		{
			bFound=TRUE;
			break;
		}
		if(LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_OBJECT)
		{
			iBase++;
		}
		iCurrentIndex++;
	}
	if(bFound)
	{
		return iCurrentIndex;
	}
	return -1;
}

static int getNthLightIndex(std::vector<LWSIMP_TransformNodeDesc> &LoadedTransformNodes,int iIndex)
{
	int iBase=0;
	unsigned int iCurrentIndex=0;
	BOOL bFound=FALSE;
	while(iCurrentIndex<LoadedTransformNodes.size())
	{
		if((LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_LIGHT) &&
			(iIndex==iBase))
		{
				bFound=TRUE;
				break;
			}
			if(LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_LIGHT)
			{
				iBase++;
			}
			iCurrentIndex++;
	}
	if(bFound)
	{
		return iCurrentIndex;
	}
	return -1;
}

static int getNthCameraIndex(std::vector<LWSIMP_TransformNodeDesc> &LoadedTransformNodes,int iIndex)
{
	int iBase=0;
	unsigned int iCurrentIndex=0;
	BOOL bFound=FALSE;
	while(iCurrentIndex<LoadedTransformNodes.size())
	{
		if((LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_CAMERA) &&
			(iIndex==iBase))
		{
			bFound=TRUE;
			break;
		}
		if(LoadedTransformNodes[iCurrentIndex].eNodeType==LNT_CAMERA)
		{
			iBase++;
		}
		iCurrentIndex++;
	}
	if(bFound)
	{
		return iCurrentIndex;
	}
	return -1;
}

BOOL importLWSFile(LWSImportDescStruct& rfImportDesc)
{
	FILE *fLWS=NULL;
	fLWS=fopen(rfImportDesc.szFileName,"r");
	if(!fLWS)
	{
		MessageBox(NULL,L"Failed to open LWS file",L"LWS import error!!!",MB_TASKMODAL);
	}
	char		szTokenString[256];
	LWS_TOKEN	TokenID=LWST_UNKNOWN;
	UINT		uiTmp=0;
	int			iTmp=0;
	LR::AutoPtr<LR::Scene>	pImpScene;
	BOOL		bTmp=FALSE;
	//LoadObjectLayer always loads all layers
	//this greatly differs from std LightWave behavior
	std::map<std::string,LWSIMP_LoadedObject> LoadedObjectsSet;
	//transform nodes already imported
	std::vector<LWSIMP_TransformNodeDesc> LoadedTransformNodes;
	//this variable is used to indicate that current token has not been
	//processed yet and no new token should be read
	BOOL bGetNextToken=TRUE;
	while((!bGetNextToken) || getLWSToken(fLWS,szTokenString,TokenID))
	{
		bGetNextToken=TRUE;
		switch(TokenID)
		{
			case LWST_LWSC:
				if(!getLWSToken(fLWS,szTokenString,TokenID))
				{
					MessageBox(NULL,L"Unexpected error while reading LWS file!",
						L"LWS import error",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				sscanf(szTokenString,"%d",&uiTmp);
				if(uiTmp!=3)
				{
					MessageBox(NULL,L"Only LWS version 3 is supported!",
						L"LWS import error",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				//create LiquidReality scene
				try
				{
					globals.engine->createScene(pImpScene);
				}
				catch(LR::Exception &e)
				{
					MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
						L"LWS import error",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				break;
			case LWST_PLUGIN:			//TODO: can plugins be nested?????
				bTmp=FALSE;
				while(bTmp=getLWSToken(fLWS,szTokenString,TokenID))
				{
					if(TokenID==LWST_END_PLUGIN)
					{
						break;
					}
				}
				//if we reach that point this means no EndPlugin was encountered in entire file
				MessageBox(NULL,L"Unexpected error while reading LWS file!",
					L"LWS import error",MB_TASKMODAL);
				fclose(fLWS);
				return FALSE;
				break;
			case LWST_PREVIEW_FIRST_FRAME:
				if(pImpScene.isNull())
				{
					MessageBox(NULL,L"Scene object does not exist!!!",
						L"LWS import error!!!",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				if(!getLWSToken(fLWS,szTokenString,TokenID))
				{
					MessageBox(NULL,L"Unexpected error while reading LWS file!",
						L"LWS import error",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				sscanf(szTokenString,"%d",&iTmp);
				pImpScene->setStartFrame(iTmp);
				break;
			case LWST_PREVIEW_LAST_FRAME:
				if(pImpScene.isNull())
				{
					MessageBox(NULL,L"Scene object does not exist!!!",
						L"LWS import error!!!",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				if(!getLWSToken(fLWS,szTokenString,TokenID))
				{
					MessageBox(NULL,L"Unexpected error while reading LWS file!",
						L"LWS import error",MB_TASKMODAL);
					fclose(fLWS);
					return FALSE;
				}
				sscanf(szTokenString,"%d",&iTmp);
				pImpScene->setEndFrame(iTmp);
				break;
			case LWST_AMBIENT_COLOR:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					float fR=0.0f;
					float fG=0.0f;
					float fB=0.0f;
					if(fscanf(fLWS,"%f %f %f",&fR,&fG,&fB)!=3)
					{
						MessageBox(NULL,L"Failed to read ambient color",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					LR::ColorPtr pCol;
					pImpScene->getAmbientColor(&pCol);
					LR::EnvelopePtr pEnv;
					pCol->getChannel(LR::COLOR_CHANNEL_R,&pEnv);
					pEnv->clear();
					long lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fR);
					pCol->getChannel(LR::COLOR_CHANNEL_G,&pEnv);
					pEnv->clear();
					lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fG);
					pCol->getChannel(LR::COLOR_CHANNEL_B,&pEnv);
					pEnv->clear();
					lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fB);
				}
				break;
			case LWST_AMBIENT_INTENSITY:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					float fIntensity=0.0f;
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(strcmp(szTokenString,"(envelope)")==0)
					{
						//described by envelope
						LR::EnvelopePtr pEnv;
						pImpScene->getAmbientIntensity(&pEnv);
						pEnv->clear();
						if(!readEnvelopeFromLWS(fLWS,pEnv))
						{
							MessageBox(NULL,L"Failed to read envelope!",
								L"LWS import error!",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					else
					{
						//numerical value
						if(sscanf(szTokenString,"%f",&fIntensity)!=1)
						{
							MessageBox(NULL,L"Failed to scan intensity value",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
						LR::EnvelopePtr pEnv;
						pImpScene->getAmbientIntensity(&pEnv);
						pEnv->clear();
						long lIndex=0;
						pEnv->addKey(&lIndex,0,fIntensity);
					}
				}
				break;
			case LWST_BACKDROP_COLOR:
				{
					//the only fog mode supported is backdrop fog
					//so backdrop color maps to fog color
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					float fR=0.0f;
					float fG=0.0f;
					float fB=0.0f;
					if(fscanf(fLWS,"%f %f %f",&fR,&fG,&fB)!=3)
					{
						MessageBox(NULL,L"Failed to read backdrop color",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					LR::ColorPtr pCol;
					pImpScene->getFogColor(&pCol);
					LR::EnvelopePtr pEnv;
					pCol->getChannel(LR::COLOR_CHANNEL_R,&pEnv);
					pEnv->clear();
					long lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fR);
					pCol->getChannel(LR::COLOR_CHANNEL_G,&pEnv);
					pEnv->clear();
					lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fG);
					pCol->getChannel(LR::COLOR_CHANNEL_B,&pEnv);
					pEnv->clear();
					lOutIndex=0;
					pEnv->addKey(&lOutIndex,0,fB);
				}
				break;
			case LWST_FOG_TYPE:
				{
					//if this one is nonzero then fog is enabled
					//the only fog type supported is linear
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//fog type
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					DWORD dwType=0;
					if(sscanf(szTokenString,"%d",&dwType)!=1)
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(dwType)
					{
						pImpScene->setFogEnable(TRUE);
					}
					else
					{
						pImpScene->setFogEnable(FALSE);
					}
				}
				break;
			case LWST_FOG_MIN_DISTANCE:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					float fDistance=0.0f;
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(strcmp(szTokenString,"(envelope)")==0)
					{
						//described by envelope
						LR::EnvelopePtr pEnv;
						pImpScene->getFogStart(&pEnv);
						pEnv->clear();
						if(!readEnvelopeFromLWS(fLWS,pEnv))
						{
							MessageBox(NULL,L"Failed to read envelope!",
								L"LWS import error!",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					else
					{
						//numerical value
						if(sscanf(szTokenString,"%f",&fDistance)!=1)
						{
							MessageBox(NULL,L"Failed to scan fog start value",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
						LR::EnvelopePtr pEnv;
						pImpScene->getFogStart(&pEnv);
						pEnv->clear();
						long lIndex=0;
						pEnv->addKey(&lIndex,0,fDistance);
					}
				}
				break;
			case LWST_FOG_MAX_DISTANCE:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					float fDistance=0.0f;
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(strcmp(szTokenString,"(envelope)")==0)
					{
						//described by envelope
						LR::EnvelopePtr pEnv;
						pImpScene->getFogEnd(&pEnv);
						pEnv->clear();
						if(!readEnvelopeFromLWS(fLWS,pEnv))
						{
							MessageBox(NULL,L"Failed to read envelope!",
								L"LWS import error!",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					else
					{
						//numerical value
						if(sscanf(szTokenString,"%f",&fDistance)!=1)
						{
							MessageBox(NULL,L"Failed to scan fog start value",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
						LR::EnvelopePtr pEnv;
						pImpScene->getFogEnd(&pEnv);
						pEnv->clear();
						long lIndex=0;
						pEnv->addKey(&lIndex,0,fDistance);
					}
				}
				break;
			case LWST_LOAD_OBJECT_LAYER:	//LWO importer _always_ loads _all_ layers
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//layer number
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//object name
					if(!getStringToEndOfLine(fLWS,szTokenString))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//real win32 pathname
					char szReadFileName[256];
					if(!findFileByLWPath(rfImportDesc,szTokenString,szReadFileName))
					{
						MessageBox(NULL,L"File not found error",
							L"LWS import error",MB_TASKMODAL);
						//TODO
						//MessageBox(NULL,szReadFileName,"LW-style filename",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//find mesh base name for LWO improter
					//(and transform node naming)
					std::string strBaseNameExt=extractFileNameFromFullPath(szReadFileName);
					if((!strBaseNameExt.c_str()) || (strBaseNameExt.size()==0))
					{
						MessageBox(NULL,L"Unexpected error while processing LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					std::string strBaseName="";
					int i=0;
					while(strBaseNameExt[i] && (strBaseNameExt[i]!='.'))
					{
						strBaseName+=strBaseNameExt[i++];
					}
					char szNumber[8];
					//check base name against loaded objects set
					if(LoadedObjectsSet.find(strBaseName)!=LoadedObjectsSet.end())
					{
						//we use cached import info, so we need to increase import count
						sprintf(szNumber,"%d",LoadedObjectsSet[strBaseName].dwLoadCount+1);
					}
					else
					{
						strcpy(szNumber,"1");
					}
					std::string strFLMBaseName=strBaseName;
					strBaseName+=szNumber;
					LWSIMP_TransformNodeDesc node_desc;
					node_desc.NodeName=strBaseName;
					node_desc.eNodeType=LNT_OBJECT;
					//import should probably be deferred till bone information is known
					//as it is of great importance when processing weigthmap data
					LR::AutoPtr<LR::TransformNode> pRenderableNode;
					try
					{
						globals.engine->createTransformNode("MeshList","Renderables",
							pRenderableNode);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					pRenderableNode->setObjectName(strBaseName.c_str());
					//read motion and assign it to newly created node
					LR::MotionPtr pMotion;
					pRenderableNode->getMotion(&pMotion);
					if(!readMotionFromLWS(fLWS,pMotion))
					{
						MessageBox(NULL,L"Failed to read motion from LWS",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//read all funny stuff like parent/target/bones/controllers etc
					//this actually requires running full token reading loop as there are
					//quite numerous possibilities
					DWORD dwHeadingControl=1;
					DWORD dwPitchControl=1;
					DWORD dwItemNumber=0;
					DWORD dwNodeType=0;
					float fPivotX=0.0f;
					float fPivotY=0.0f;
					float fPivotZ=0.0f;
					while(bGetNextToken && getLWSToken(fLWS,szTokenString,TokenID))
					{
						switch(TokenID)
						{
							case LWST_SUBPATCH_LEVEL:
								//issue warning and abort import
								MessageBox(NULL,L"Subpatches are not supported!!!",
									L"LWS import error!",MB_TASKMODAL);
								fclose(fLWS);
								return FALSE;
								break;
							case LWST_HCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwHeadingControl);
								break;
							case LWST_PCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwPitchControl);
								break;
							case LWST_PIVOT_POSITION:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%f",&fPivotX);
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%f",&fPivotY);
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%f",&fPivotZ);
								break;
							case LWST_LOCKED_CHANNELS:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								break;
							case LWST_PARENT_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eParentType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eParentType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eParentType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iParentNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_TARGET_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eTargetType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eTargetType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eTargetType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iTargetNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							default:
								pRenderableNode->setTargetControlFlags(dwHeadingControl ? TRUE : FALSE,
									dwPitchControl ? TRUE : FALSE);
								bGetNextToken=FALSE;
								break;
						}
					}
					//set renderable's pivot
					LR::EnvelopePtr pTmpPivotEnv;
					long lTmp=0;
					pRenderableNode->getPivotX(&pTmpPivotEnv);
					pTmpPivotEnv->clear();
					pTmpPivotEnv->addKey(&lTmp,0,fPivotX);
					pRenderableNode->getPivotY(&pTmpPivotEnv);
					pTmpPivotEnv->clear();
					pTmpPivotEnv->addKey(&lTmp,0,fPivotY);
					pRenderableNode->getPivotZ(&pTmpPivotEnv);
					pTmpPivotEnv->clear();
					pTmpPivotEnv->addKey(&lTmp,0,fPivotZ);
					if(LoadedObjectsSet.find(strFLMBaseName)==LoadedObjectsSet.end())
					{
						//perform mesh import and attach mesh nodes to renderable
						LWO2ImportDescStruct lwo_import_desc;
						ZeroMemory(&lwo_import_desc,sizeof(LWO2ImportDescStruct));
						lwo_import_desc.bDeformers=rfImportDesc.bDeformers;
						lwo_import_desc.bMakeTangentSpace=rfImportDesc.bMakeTangentSpace;
						lwo_import_desc.bNPatches=rfImportDesc.bNPatches;
						lwo_import_desc.bOutputGeometry=rfImportDesc.bOutputGeometry;
						lwo_import_desc.bOutputMaterial=rfImportDesc.bOutputMaterial;
						lwo_import_desc.bOutputMesh=rfImportDesc.bOutputMesh;
						lwo_import_desc.bOutputTextures=rfImportDesc.bOutputTextures;
						lwo_import_desc.bOverwriteGeometry=rfImportDesc.bOverwriteGeometry;
						lwo_import_desc.bOverwriteImageFiles=rfImportDesc.bOverwriteImageFiles;
						lwo_import_desc.bOverwriteMaterial=rfImportDesc.bOverwriteMaterial;
						lwo_import_desc.bOverwriteMesh=rfImportDesc.bOverwriteMesh;
						lwo_import_desc.bOverwriteTextures=rfImportDesc.bOverwriteTextures;
						lwo_import_desc.bRenderable=rfImportDesc.bRenderable;	//TODO: does it make sense
																				//in scene import desc?
						lwo_import_desc.bSpaceWarps=rfImportDesc.bSpaceWarps;
						lwo_import_desc.dwLockabilityMask=rfImportDesc.dwLockabilityMask;
						strcpy(lwo_import_desc.szFileName,szReadFileName);
						strcpy(lwo_import_desc.szOutputFLMBaseName,strFLMBaseName.c_str());
						strcpy(lwo_import_desc.szOutputRootDir,rfImportDesc.szOutputRootDir);
//						strcpy(lwo_import_desc.szShaderName,rfImportDesc.szShaderName);
						LWO2ImportResult *pImportRes=new LWO2ImportResult();	//it's quite big structure so
																				//better allocate dynamically
						if(!importLWO2File(lwo_import_desc,pImportRes))
						{
							delete pImportRes;
							pImportRes=NULL;
							fclose(fLWS);
							return FALSE;
						}
						if(pImportRes->uiMeshCount==0)
						{
							MessageBox(NULL,L"Hmmm, seems that no meshes were imported!",
								L"LWS import error!",MB_TASKMODAL);
							delete pImportRes;
							pImportRes=NULL;
							fclose(fLWS);
							return FALSE;
						}
						//attach imported mesh nodes to renderable
						for(i=0;i<(int)(pImportRes->uiMeshCount);i++)
						{
							LR::AutoPtr<LR::Property>	pMeshList;
							//get property
							try
							{
								pRenderableNode->getPropertyByName("MeshList",pMeshList);
								//attach mesh by adding element to mesh list
								DWORD dwElements=pMeshList->getNumListElements();
								pMeshList->insertListElement(dwElements);
								pMeshList->setListPropertyValue(dwElements,pImportRes->MeshNameTbl[i]);
							}
							catch(LR::Exception &e)
							{
								MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
									L"LWS import error",MB_TASKMODAL);
								delete pImportRes;
								pImportRes=NULL;
								fclose(fLWS);
								return FALSE;
							}
						}
						//ok, import operation for this mesh was successful
						LWSIMP_LoadedObject loaded_object;
						loaded_object.ObjectName=strFLMBaseName;
						loaded_object.dwLoadCount=1;
						for(i=0;i<(int)(pImportRes->uiMeshCount);i++)
						{
							loaded_object.MeshNames.push_back(pImportRes->MeshNameTbl[i]);
						}
						LoadedObjectsSet.insert(
							std::map<std::string,LWSIMP_LoadedObject>::value_type(
							strFLMBaseName,
							loaded_object));
						delete pImportRes;
						pImportRes=NULL;
					}
					else
					{
						if(LoadedObjectsSet[strFLMBaseName].MeshNames.size()==0)
						{
							MessageBox(NULL,L"Hmmm, seems that no meshes were imported and we "
								L"somehow got away with it!",
								L"LWS import error!",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
						//attach imported mesh nodes to renderable
						for(i=0;i<(int)(LoadedObjectsSet[strFLMBaseName].MeshNames.size());i++)
						{
							LR::AutoPtr<LR::Property>	pMeshList;
							//get property
							try
							{
								pRenderableNode->getPropertyByName("MeshList",pMeshList);
								//attach mesh by adding element to mesh list
								DWORD dwElements=pMeshList->getNumListElements();
								pMeshList->insertListElement(dwElements);
								pMeshList->setListPropertyValue(dwElements,
									LoadedObjectsSet[strFLMBaseName].MeshNames[i].c_str());
							}
							catch(LR::Exception &e)
							{
								MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
									L"LWS import error",MB_TASKMODAL);
								fclose(fLWS);
								return FALSE;
							}
						}
						LoadedObjectsSet[strFLMBaseName].dwLoadCount++;
					}
					//ok, import was successful, add renderable to scene
					try
					{
						pImpScene->addTransformNode(pRenderableNode);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//and add it to loaded transform nodes set
					LoadedTransformNodes.push_back(node_desc);
				}
				break;
			case LWST_ADD_NULL_OBJECT:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//object name
					if(!getStringToEndOfLine(fLWS,szTokenString))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					std::string strNodeName=szTokenString;
					LWSIMP_TransformNodeDesc node_desc;
					node_desc.NodeName=strNodeName;
					node_desc.eNodeType=LNT_OBJECT;
					LR::AutoPtr<LR::TransformNode> pNULL;
					try
					{
						globals.engine->createTransformNode("NullObject","Helpers",
							pNULL);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					pNULL->setObjectName(strNodeName.c_str());
					//read LightWave garbage preceding motion (ShowObject X X)
					int i=0;
					for(i=0;i<3;i++)
					{
						if(!getLWSToken(fLWS,szTokenString,TokenID))
						{
							MessageBox(NULL,L"Unexpected error while reading LWS file!",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					//read motion and assign it to newly created node
					LR::MotionPtr pMotion;
					pNULL->getMotion(&pMotion);
					if(!readMotionFromLWS(fLWS,pMotion))
					{
						MessageBox(NULL,L"Failed to read motion from LWS",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//read all funny stuff like parent/target/controllers etc
					//this actually requires running full token reading loop as there are
					//quite numerous possibilities
					DWORD dwHeadingControl=1;
					DWORD dwPitchControl=1;
					DWORD dwItemNumber=0;
					DWORD dwNodeType=0;
					while(bGetNextToken && getLWSToken(fLWS,szTokenString,TokenID))
					{
						switch(TokenID)
						{
							case LWST_HCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwHeadingControl);
								break;
							case LWST_PCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwPitchControl);
								break;
							case LWST_PARENT_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eParentType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eParentType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eParentType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iParentNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_TARGET_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eTargetType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eTargetType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eTargetType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iTargetNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							default:
								pNULL->setTargetControlFlags(dwHeadingControl ? TRUE : FALSE,
									dwPitchControl ? TRUE : FALSE);
								bGetNextToken=FALSE;
								break;
						}
					}
					//ok, import was successful, add node to scene
					try
					{
						pImpScene->addTransformNode(pNULL);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//and add it to loaded transform nodes set
					LoadedTransformNodes.push_back(node_desc);
				}
				break;
			case LWST_ADD_LIGHT:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//LightName
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(TokenID!=LWST_LIGHT_NAME)
					{
						MessageBox(NULL,L"LightName expected after AddLight!\n",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//name
					if(!getStringToEndOfLine(fLWS,szTokenString))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//create light
					LR::AutoPtr<LR::TransformNode> pLight;
					try
					{
						globals.engine->createTransformNode("StandardLight","Lights",pLight);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					pLight->setObjectName(szTokenString);
					LWSIMP_TransformNodeDesc node_desc;
					node_desc.eNodeType=LNT_LIGHT;
					node_desc.NodeName=szTokenString;
					//read LightWave garbage preceding motion (ShowLight X X)
					int i=0;
					for(i=0;i<3;i++)
					{
						if(!getLWSToken(fLWS,szTokenString,TokenID))
						{
							MessageBox(NULL,L"Unexpected error while reading LWS file!",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					//read motion
					LR::MotionPtr pMotion;
					pLight->getMotion(&pMotion);
					if(!readMotionFromLWS(fLWS,pMotion))
					{
						MessageBox(NULL,L"Failed to read motion from LWS",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//read all funny stuff like parent/target/controllers etc
					//this actually requires running full token reading loop as there are
					//quite numerous possibilities
					DWORD dwHeadingControl=1;
					DWORD dwPitchControl=1;
					DWORD dwItemNumber=0;
					DWORD dwNodeType=0;
					while(bGetNextToken && getLWSToken(fLWS,szTokenString,TokenID))
					{
						switch(TokenID)
						{
							case LWST_HCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwHeadingControl);
								break;
							case LWST_PCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwPitchControl);
								break;
							case LWST_PARENT_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eParentType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eParentType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eParentType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iParentNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_TARGET_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eTargetType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eTargetType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eTargetType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iTargetNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_LIGHT_INTENSITY:
								{
									float fIntensity=0.0f;
									if(!getLWSToken(fLWS,szTokenString,TokenID))
									{
										MessageBox(NULL,L"Unexpected error while reading LWS file!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									if(strcmp(szTokenString,"(envelope)")==0)
									{
										//described by envelope
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("Intensity",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: extract description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										if(!readEnvelopeFromLWS(fLWS,pEnv))
										{
											MessageBox(NULL,L"Failed to read envelope!",
												L"LWS import error!",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
									}
									else
									{
										//numerical value
										if(sscanf(szTokenString,"%f",&fIntensity)!=1)
										{
											MessageBox(NULL,L"Failed to scan intensity value",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("Intensity",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										long lIndex=0;
										pEnv->addKey(&lIndex,0,fIntensity);
									}
								}
								break;
							case LWST_LIGHT_COLOR:
								{
									float fR=0.0f;
									float fG=0.0f;
									float fB=0.0f;
									if(fscanf(fLWS,"%f %f %f",&fR,&fG,&fB)!=3)
									{
										MessageBox(NULL,L"Failed to read light color",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									LR::AutoPtr<LR::Property> pProp;
									try
									{
										pLight->getPropertyByName("DiffuseColor",pProp);
									}
									catch(LR::Exception &e)
									{
										MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									LR::ColorPtr pCol;
									pProp->getPropertyValue(&pCol);
									LR::EnvelopePtr pEnv;
									pCol->getChannel(LR::COLOR_CHANNEL_R,&pEnv);
									pEnv->clear();
									long lOutIndex=0;
									pEnv->addKey(&lOutIndex,0,fR);
									pCol->getChannel(LR::COLOR_CHANNEL_G,&pEnv);
									pEnv->clear();
									lOutIndex=0;
									pEnv->addKey(&lOutIndex,0,fG);
									pCol->getChannel(LR::COLOR_CHANNEL_B,&pEnv);
									pEnv->clear();
									lOutIndex=0;
									pEnv->addKey(&lOutIndex,0,fB);
								}
								break;
							case LWST_LIGHT_TYPE:
								{
									DWORD dwType=0;
									if(fscanf(fLWS,"%d",&dwType)!=1)
									{
										MessageBox(NULL,L"Failed to read light type!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									LR::AutoPtr<LR::Property> pProp;
									try
									{
										pLight->getPropertyByName("Type",pProp);
										pProp->setPropertyValue(dwType);
									}
									catch(LR::Exception &e)
									{
										MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
								}
								break;
							case LWST_AFFECT_CAUSTICS:		//simply ignore
							case LWST_SHADOW_TYPE:
							case LWST_LIGHT_FALLOFF_TYPE:	//assume "linear" for light falloff
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								break;
							case LWST_LIGHT_RANGE:
								{
									float fRange=0.0f;
									if(!getLWSToken(fLWS,szTokenString,TokenID))
									{
										MessageBox(NULL,L"Unexpected error while reading LWS file!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									if(strcmp(szTokenString,"(envelope)")==0)
									{
										//described by envelope
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("Range",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										if(!readEnvelopeFromLWS(fLWS,pEnv))
										{
											MessageBox(NULL,L"Failed to read envelope!",
												L"LWS import error!",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
									}
									else
									{
										//numerical value
										if(sscanf(szTokenString,"%f",&fRange)!=1)
										{
											MessageBox(NULL,L"Failed to scan range value",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("Range",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										long lIndex=0;
										pEnv->addKey(&lIndex,0,fRange);
									}
								}
								break;
							case LWST_LIGHT_CONE_ANGLE:
								{
									float fConeAngle=0.0f;
									if(!getLWSToken(fLWS,szTokenString,TokenID))
									{
										MessageBox(NULL,L"Unexpected error while reading LWS file!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									if(strcmp(szTokenString,"(envelope)")==0)
									{
										//described by envelope
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("ConeAngle",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										if(!readEnvelopeFromLWS(fLWS,pEnv))
										{
											MessageBox(NULL,L"Failed to read envelope!",
												L"LWS import error!",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
									}
									else
									{
										//numerical value
										if(sscanf(szTokenString,"%f",&fConeAngle)!=1)
										{
											MessageBox(NULL,L"Failed to scan cone angle value",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pLight->getPropertyByName("ConeAngle",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										long lIndex=0;
										pEnv->addKey(&lIndex,0,fConeAngle);
									}
								}
								break;
							case LWST_LIGHT_EDGE_ANGLE:		//TODO
								{
									float fEdgeAngle=0.0f;
									if(!getLWSToken(fLWS,szTokenString,TokenID))
									{
										MessageBox(NULL,L"Unexpected error while reading LWS file!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									if(strcmp(szTokenString,"(envelope)")==0)
									{
										//described by envelope
										LR::EnvelopePtr pEnv;
										//for now simply create temporary envelope to discard later ...
										pEnv=globals.engine->createEnvelope();
										pEnv->clear();
										if(!readEnvelopeFromLWS(fLWS,pEnv))
										{
											MessageBox(NULL,L"Failed to read envelope!",
												L"LWS import error!",MB_TASKMODAL);
											fclose(fLWS);
											globals.engine->destroyEnvelope(pEnv);
											return FALSE;
										}
										globals.engine->destroyEnvelope(pEnv);
									}
									else
									{
										//numerical value
										if(sscanf(szTokenString,"%f",&fEdgeAngle)!=1)
										{
											MessageBox(NULL,L"Failed to scan edge angle value",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
									}
								}
								break;
							default:
								pLight->setTargetControlFlags(dwHeadingControl ? TRUE : FALSE,
									dwPitchControl ? TRUE : FALSE);
								bGetNextToken=FALSE;
								break;
						}
					}
					//ok, import was successful, add light to scene
					try
					{
						pImpScene->addTransformNode(pLight);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//and add it to loaded transform nodes set
					LoadedTransformNodes.push_back(node_desc);
				}
				break;
			case LWST_ADD_CAMERA:
				{
					if(pImpScene.isNull())
					{
						MessageBox(NULL,L"Scene object does not exist!!!",
							L"LWS import error!!!",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//CameraName
					if(!getLWSToken(fLWS,szTokenString,TokenID))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					if(TokenID!=LWST_CAMERA_NAME)
					{
						MessageBox(NULL,L"LightName expected after AddLight!\n",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//name
					if(!getStringToEndOfLine(fLWS,szTokenString))
					{
						MessageBox(NULL,L"Unexpected error while reading LWS file!",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//create light
					LR::AutoPtr<LR::TransformNode> pCamera;
					try
					{
						globals.engine->createTransformNode("StandardCamera","Cameras",pCamera);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					pCamera->setObjectName(szTokenString);
					LWSIMP_TransformNodeDesc node_desc;
					node_desc.eNodeType=LNT_CAMERA;
					node_desc.NodeName=szTokenString;
					//read LightWave garbage preceding motion (ShowCamera X X)
					int i=0;
					for(i=0;i<3;i++)
					{
						if(!getLWSToken(fLWS,szTokenString,TokenID))
						{
							MessageBox(NULL,L"Unexpected error while reading LWS file!",
								L"LWS import error",MB_TASKMODAL);
							fclose(fLWS);
							return FALSE;
						}
					}
					//read motion
					LR::MotionPtr pMotion;
					pCamera->getMotion(&pMotion);
					if(!readMotionFromLWS(fLWS,pMotion))
					{
						MessageBox(NULL,L"Failed to read motion from LWS",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//read all funny stuff like parent/target/controllers etc
					//this actually requires running full token reading loop as there are
					//quite numerous possibilities
					DWORD dwHeadingControl=1;
					DWORD dwPitchControl=1;
					DWORD dwItemNumber=0;
					DWORD dwNodeType=0;
					while(bGetNextToken && getLWSToken(fLWS,szTokenString,TokenID))
					{
						switch(TokenID)
						{
							case LWST_HCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwHeadingControl);
								break;
							case LWST_PCONTROLLER:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%d",&dwPitchControl);
								break;
							case LWST_PARENT_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eParentType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eParentType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eParentType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iParentNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_TARGET_ITEM:
								if(!getLWSToken(fLWS,szTokenString,TokenID))
								{
									MessageBox(NULL,L"Unexpected error while reading LWS file!",
										L"LWS import error",MB_TASKMODAL);
									fclose(fLWS);
									return FALSE;
								}
								sscanf(szTokenString,"%8x",&dwItemNumber);
								dwNodeType=(dwItemNumber&0xfffffff)>>28;
								switch(dwNodeType)
								{
									case 0:
										node_desc.eTargetType=LNT_OBJECT;
										break;
									case 1:
										node_desc.eTargetType=LNT_LIGHT;
										break;
									case 2:
										node_desc.eTargetType=LNT_CAMERA;
										break;
									default:
										MessageBox(NULL,L"Unsupported node type used for parenting/targeting!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
										break;
								}
								dwItemNumber&=0xfffffff;
								node_desc.iTargetNodeIndex=(int)dwItemNumber;	//and lets hope it's not too big :)
								break;
							case LWST_ZOOM_FACTOR:
								{
									float fZoomFactor=0.0f;
									if(!getLWSToken(fLWS,szTokenString,TokenID))
									{
										MessageBox(NULL,L"Unexpected error while reading LWS file!",
											L"LWS import error",MB_TASKMODAL);
										fclose(fLWS);
										return FALSE;
									}
									if(strcmp(szTokenString,"(envelope)")==0)
									{
										//described by envelope
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pCamera->getPropertyByName("ZoomFactor",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										if(!readEnvelopeFromLWS(fLWS,pEnv))
										{
											MessageBox(NULL,L"Failed to read envelope!",
												L"LWS import error!",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
									}
									else
									{
										//numerical value
										if(sscanf(szTokenString,"%f",&fZoomFactor)!=1)
										{
											MessageBox(NULL,L"Failed to scan zoom factor value",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::AutoPtr<LR::Property> pProp;
										try
										{
											pCamera->getPropertyByName("ZoomFactor",pProp);
										}
										catch(LR::Exception &e)
										{
											MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
												L"LWS import error",MB_TASKMODAL);
											fclose(fLWS);
											return FALSE;
										}
										LR::EnvelopePtr pEnv;
										pProp->getPropertyValue(&pEnv);
										pEnv->clear();
										long lIndex=0;
										pEnv->addKey(&lIndex,0,fZoomFactor);
									}
								}
								break;
							default:
								pCamera->setTargetControlFlags(dwHeadingControl ? TRUE : FALSE,
									dwPitchControl ? TRUE : FALSE);
								bGetNextToken=FALSE;
								break;
						}
					}
					//ok, import was successful, add camera to scene
					try
					{
						pImpScene->addTransformNode(pCamera);
					}
					catch(LR::Exception &e)
					{
						MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
							L"LWS import error",MB_TASKMODAL);
						fclose(fLWS);
						return FALSE;
					}
					//and add it to loaded transform nodes set
					LoadedTransformNodes.push_back(node_desc);
				}
				break;	//AddCamera
		}
	}
	fclose(fLWS);
	//ok, file was read, now resolve parenting/targeting dependencies
	unsigned int i=0;
	for(i=0;i<LoadedTransformNodes.size();i++)
	{
		if(LoadedTransformNodes[i].iParentNodeIndex!=-1)
		{
			int iParentNodeIndex=-1;
			switch(LoadedTransformNodes[i].eParentType)
			{
				case LNT_OBJECT:
					iParentNodeIndex=getNthObjectIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iParentNodeIndex);
					break;
				case LNT_LIGHT:
					iParentNodeIndex=getNthLightIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iParentNodeIndex);
					break;
				case LNT_CAMERA:
					iParentNodeIndex=getNthLightIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iParentNodeIndex);
					break;
			}
			if(iParentNodeIndex!=-1)
			{
				LR::AutoPtr<LR::TransformNode> pNode;
				try
				{
					pImpScene->getTransformNodeByName(LoadedTransformNodes[i].NodeName.c_str(),pNode);
				}
				catch(LR::Exception &e)
				{
					MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
						L"LWS import error",MB_TASKMODAL);
					return FALSE;
				}
				pNode->setParentNodeName(LoadedTransformNodes[iParentNodeIndex].NodeName.c_str());
			}
		}
		if(LoadedTransformNodes[i].iTargetNodeIndex!=-1)
		{
			int iTargetNodeIndex=-1;
			switch(LoadedTransformNodes[i].eTargetType)
			{
				case LNT_OBJECT:
					iTargetNodeIndex=getNthObjectIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iTargetNodeIndex);
					break;
				case LNT_LIGHT:
					iTargetNodeIndex=getNthLightIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iTargetNodeIndex);
					break;
				case LNT_CAMERA:
					iTargetNodeIndex=getNthLightIndex(LoadedTransformNodes,
						LoadedTransformNodes[i].iTargetNodeIndex);
					break;
			}
			if(iTargetNodeIndex!=-1)
			{
				LR::AutoPtr<LR::TransformNode> pNode;
				try
				{
					pImpScene->getTransformNodeByName(LoadedTransformNodes[i].NodeName.c_str(),pNode);
				}
				catch(LR::Exception &e)
				{
					MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
						L"LWS import error",MB_TASKMODAL);
					return FALSE;
				}
				pNode->setTargetNodeName(LoadedTransformNodes[iTargetNodeIndex].NodeName.c_str());
			}
		}
	}
	fclose(fLWS);
	std::string strSceneBaseNameExt=extractFileNameFromFullPath(rfImportDesc.szFileName);
	if((!strSceneBaseNameExt.c_str()) || (strSceneBaseNameExt.size()==0))
	{
		MessageBox(NULL,L"Unexpected error while post processing LWS file!",
			L"LWS import error",MB_TASKMODAL);
		return FALSE;
	}
	std::string strSceneBaseName="";
	i=0;
	while((strSceneBaseNameExt[i]) && (strSceneBaseNameExt[i]!='.'))
	{
		strSceneBaseName+=strSceneBaseNameExt[i++];
	}
	strSceneBaseName+=".lrsc2";
	ULONG	uHandle=0;
	try
	{
		globals.filesystem->createTextFile(strSceneBaseName.c_str(),uHandle);
		try
		{
			pImpScene->serializeASCII(uHandle,globals.filesystem,0);
		}
		catch(LR::Exception &e)
		{
			MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
				L"LWS import error",MB_TASKMODAL);
			return FALSE;
		}
	}
	catch(LR::IOException &e)
	{
		MessageBox(NULL,/*e.getDescription()*/L"TODO: get description",
			L"LWS import error (I/O error)",MB_TASKMODAL);
		return FALSE;
	}
	globals.filesystem->closeFile(uHandle);
	return TRUE;
}
