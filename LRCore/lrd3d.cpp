/*********************************************************
 LiquidReality engine Direct3D subsystem
 (c) 2001-2002 Maciej Mroz
 *********************************************************/

#include "StdAfx.h"
#include "resource.h"
#include <string>
#include <set>
#include <assert.h>

using namespace LR;

ULONG D3DImpl::Release()
{
	m_uRefCount--;
	if(m_uRefCount==0)
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

ULONG D3DImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

//constructor/destructor
D3DImpl::D3DImpl()
{
	//refcount
	m_uRefCount=0;
	//dynamic vb wrappers
	//HVP
	m_DynamicHVPNormalVB.m_dwByteLength=0;
	m_DynamicHVPNormalVB.m_dwBytesUsed=0;
	m_DynamicHVPNormalVB.m_dwDiscardID=0;
	m_DynamicHVPNormalVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPNormalVB.m_pVB=NULL;
	m_DynamicHVPNPatchVB.m_dwByteLength=0;
	m_DynamicHVPNPatchVB.m_dwBytesUsed=0;
	m_DynamicHVPNPatchVB.m_dwDiscardID=0;
	m_DynamicHVPNPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPNPatchVB.m_pVB=NULL;
	m_DynamicHVPRTPatchVB.m_dwByteLength=0;
	m_DynamicHVPRTPatchVB.m_dwBytesUsed=0;
	m_DynamicHVPRTPatchVB.m_dwDiscardID=0;
	m_DynamicHVPRTPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPRTPatchVB.m_pVB=NULL;
	m_DynamicHVPPointVB.m_dwByteLength=0;
	m_DynamicHVPPointVB.m_dwBytesUsed=0;
	m_DynamicHVPPointVB.m_dwDiscardID=0;
	m_DynamicHVPPointVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPPointVB.m_pVB=NULL;
	//SVP
	m_DynamicSVPNormalVB.m_dwByteLength=0;
	m_DynamicSVPNormalVB.m_dwBytesUsed=0;
	m_DynamicSVPNormalVB.m_dwDiscardID=0;
	m_DynamicSVPNormalVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPNormalVB.m_pVB=NULL;
	m_DynamicSVPNPatchVB.m_dwByteLength=0;
	m_DynamicSVPNPatchVB.m_dwBytesUsed=0;
	m_DynamicSVPNPatchVB.m_dwDiscardID=0;
	m_DynamicSVPNPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPNPatchVB.m_pVB=NULL;
	m_DynamicSVPRTPatchVB.m_dwByteLength=0;
	m_DynamicSVPRTPatchVB.m_dwBytesUsed=0;
	m_DynamicSVPRTPatchVB.m_dwDiscardID=0;
	m_DynamicSVPRTPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPRTPatchVB.m_pVB=NULL;
	m_DynamicSVPPointVB.m_dwByteLength=0;
	m_DynamicSVPPointVB.m_dwBytesUsed=0;
	m_DynamicSVPPointVB.m_dwDiscardID=0;
	m_DynamicSVPPointVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPPointVB.m_pVB=NULL;

	m_VBTbl.setAllocStep(100);
	m_IBTbl.setAllocStep(100);
	//d3d initialization
	m_pD3D=NULL;
	m_pD3DDevice=NULL;

	//create Direct3D object
	if(!(m_pD3D=Direct3DCreate9(D3D_SDK_VERSION)))
	{
		throw InvalidOperationExceptionImpl("Failed to create Direct3D");
	}

	D3DDISPLAYMODE				AdapterCurrentDisplayMode;
	//we enumerate only primary display
	//get adapter information
	if(FAILED(m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT,
		/*D3DENUM_NO_WHQL_LEVEL*/0,&m_AdapterInfo)))
	{
		//TODO!!!
		//throw exception?
	}
	//get adapter current display mode
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
		&AdapterCurrentDisplayMode)))
	{
		//TODO!!!
		//throw exception?
	}
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_HALDeviceCaps);
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&m_REFDeviceCaps);
	if((AdapterCurrentDisplayMode.Format!=D3DFMT_X8R8G8B8) &&
		(AdapterCurrentDisplayMode.Format!=D3DFMT_A2R10G10B10))
	{
		MessageBox(NULL,"Display mode must be 8 or 10 bit per component!!!",
			"Engine critical error",MB_TASKMODAL);
	}
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
	m_bWorldTransformChanged=TRUE;
	m_bViewTransformChanged=TRUE;
	m_bProjectionTransformChanged=TRUE;
	//init RT manager
	ZeroMemory(&m_RTPool,D3D_RT_POOL_SIZE*sizeof(D3D_RTDesc));
	m_dwRTPoolSize=0;
	ZeroMemory(&m_DSPool,D3D_DS_POOL_SIZE*sizeof(D3D_DSDesc));
	m_dwDSPoolSize=0;
	m_dwCurrentRTSeed=0;
	m_dwCurrentRT=0;
	m_dwCurrentDS=0;
	//init font renderer to NULL
	m_pFont=NULL;
	//init sprite system
	m_bIn2D=FALSE;
	m_fCurrentSpriteFactor=0.0f;
	m_pCurrentSpriteEffect=NULL;
	m_hCurrentSpriteTechniqueHandle=0;
	m_dwCurrentSpritePSConstantCount=0;
	ZeroMemory(m_CurrentSpritePSContants,32*sizeof(float));
	m_dwCurrentSpriteVSConstantCount=0;
	ZeroMemory(m_CurrentSpriteVSContants,32*sizeof(float));
	m_eSpriteMinFilter=D3DTEXF_POINT;
	m_eSpriteMagFilter=D3DTEXF_POINT;
	m_eSpriteMipFilter=D3DTEXF_NONE;
	m_bSpriteAlphaTestEnable=FALSE;
	m_eSpriteAlphaTestFunc=D3DCMP_GREATEREQUAL;
	m_dwSpriteAlphaTestRef=0x08;
	m_pSpriteVFDecl=NULL;
	m_pVSSpriteVFDecl=NULL;
	m_pSpriteStateBlock=NULL;
	//sampler states - default to trilinear
	m_MipFilter=D3DTEXF_LINEAR;
	m_MinFilter=D3DTEXF_LINEAR;
	m_MagFilter=D3DTEXF_LINEAR;
	m_dwAnisotropy=1;
	//automipmap usage
	m_bDisableAutoMipMaps=TRUE;
	//scrren rt's
	m_dwScreenRTTextureY=512;
	m_dwScreenRTTextureX=512;
	//initialize default aspect ratio
	m_eRenderAspect=AR_4_3;
	m_eDeviceAspect=AR_4_3;
}

D3DImpl::~D3DImpl()
{
	//free internal D3D resources
	if(m_DynamicHVPNormalVB.m_pVB)
	{
		m_DynamicHVPNormalVB.m_pVB->Release();
		ZeroMemory(&m_DynamicHVPNormalVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicHVPNPatchVB.m_pVB)
	{
		m_DynamicHVPNPatchVB.m_pVB->Release();
		ZeroMemory(&m_DynamicHVPNPatchVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicHVPRTPatchVB.m_pVB)
	{
		m_DynamicHVPRTPatchVB.m_pVB->Release();
		ZeroMemory(&m_DynamicHVPRTPatchVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicHVPPointVB.m_pVB)
	{
		m_DynamicHVPPointVB.m_pVB->Release();
		ZeroMemory(&m_DynamicHVPPointVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicSVPNormalVB.m_pVB)
	{
		m_DynamicSVPNormalVB.m_pVB->Release();
		ZeroMemory(&m_DynamicSVPNormalVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicSVPNPatchVB.m_pVB)
	{
		m_DynamicSVPNPatchVB.m_pVB->Release();
		ZeroMemory(&m_DynamicSVPNPatchVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicSVPRTPatchVB.m_pVB)
	{
		m_DynamicSVPRTPatchVB.m_pVB->Release();
		ZeroMemory(&m_DynamicSVPRTPatchVB,sizeof(D3D_DynamicVBWrapper));
	}
	if(m_DynamicSVPPointVB.m_pVB)
	{
		m_DynamicSVPPointVB.m_pVB->Release();
		ZeroMemory(&m_DynamicSVPPointVB,sizeof(D3D_DynamicVBWrapper));
	}
	DWORD dwI=0;
	if(m_dwRTPoolSize>0)
	{
		m_RTPool[0].m_pRTSurface->Release();
		for(dwI=1;dwI<m_dwRTPoolSize;dwI++)
		{
			if(m_RTPool[dwI].m_bCube)
			{
				m_RTPool[dwI].m_pCubeTexture->Release();
			}
			else
			{
				m_RTPool[dwI].m_pTexture->Release();
			}
		}
	}
	ZeroMemory(&m_RTPool,D3D_RT_POOL_SIZE*sizeof(D3D_RTDesc));
	for(dwI=0;dwI<m_dwDSPoolSize;dwI++)
	{
		m_DSPool[dwI].m_pDSSurface->Release();
	}
	ZeroMemory(&m_DSPool,D3D_DS_POOL_SIZE*sizeof(D3D_DSDesc));
	delete m_pFont;
	m_pFont=NULL;
	if(m_pSpriteStateBlock)
	{
		m_pSpriteStateBlock->Release();
		m_pSpriteStateBlock=NULL;
	}
	if(m_pSpriteVFDecl)
	{
		m_pSpriteVFDecl->Release();
		m_pSpriteVFDecl=NULL;
	}
	if(m_pVSSpriteVFDecl)
	{
		m_pVSSpriteVFDecl->Release();
		m_pVSSpriteVFDecl=NULL;
	}
	//effect cache cleanup
	std::map<std::string,ID3DXEffect*>::iterator iter=m_EffectCache.begin();
	while(iter!=m_EffectCache.end())
	{
		(*iter).second->Release();
		iter++;
	}
	m_EffectCache.clear();
	if(m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice=NULL;
	}
	if(m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D=NULL;
	}
}

const D3DCAPS9* D3DImpl::getHardwareCaps()
{
	return &m_HALDeviceCaps;
}

//config input (and _nothing_ more is available!!!)
static PDIRECT3D9				s_pD3D=NULL;
static DeviceCreateStruct2		*s_pCreateStruct2=NULL;
//TODO: move to LRDeviceCreateStruct in 0.3.x
static bool						s_bWidescreen=false;
//config temporaries
static bool						bChangeRefresh=false;
static D3DFORMAT				DisplayModeFmt=D3DFMT_X8R8G8B8;
//config output
static D3DDEVTYPE				s_DevType=D3DDEVTYPE_HAL;
static DWORD					s_dwDevBehavior=D3DCREATE_MIXED_VERTEXPROCESSING;
static bool						s_bLineAntialiasing=false;
static bool						s_bAutoMipMaps=false;
static bool						s_bSaveSettings=false;
static DWORD					s_dwScreenRTTextureY=512;
static DWORD					s_dwScreenRTTextureX=512;
static ASPECT_RATIO			s_eDeviceAspectRatio=AR_4_3;
//no limit,1.4,1.1
static DWORD					s_dwPixelShaderVersionLimit=0;
//aniso max,aniso 8x,aniso 4x,aniso 2x,trilinear
static DWORD					s_dwTextureFiltering=0;
static D3DPRESENT_PARAMETERS	s_PresentParameters=
{
	640,480,			//some very basic default values
	D3DFMT_X8R8G8B8,	//party version settings? :)
	1,
	D3DMULTISAMPLE_NONE,
	0,
	D3DSWAPEFFECT_DISCARD,
	NULL,
	FALSE,
	TRUE,
	D3DFMT_D24S8,
	0,
	D3DPRESENT_RATE_DEFAULT,
	D3DPRESENT_INTERVAL_ONE
};

static void AdvancedConfigDeviceChange(HWND hDlg)
{
	HWND hCombo;
	//init display mode list
	SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_RESETCONTENT,0,0);
	if(s_PresentParameters.Windowed)
	{
		int iPreferredModePos=0;
		//display format (same as desktop format)
		D3DDISPLAYMODE Mode;
		s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Mode);
		if(Mode.Format!=D3DFMT_X8R8G8B8)
		{
			MessageBox(NULL,"Display mode must be 8 bit per component!!!",
				"Engine critical error",MB_TASKMODAL);
		}
		if(s_bWidescreen)
		{
			switch(Mode.Width)
			{
				case 1280:	//slightly different pixel aspect ratio
					if(s_DevType==D3DDEVTYPE_REF)
					{
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
							0,(LPARAM)"320x192");
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"640x384");
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"800x480");
					if(s_pCreateStruct2->dwPreferredScreenWidth==800)
					{
						iPreferredModePos=1;
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"960x576");
					if(s_pCreateStruct2->dwPreferredScreenWidth==960)
					{
						iPreferredModePos=2;
					}
					break;
				default:
					if(s_DevType==D3DDEVTYPE_REF)
					{
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
							0,(LPARAM)"320x180");
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"640x360");
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"800x450");
					if(s_pCreateStruct2->dwPreferredScreenWidth==800)
					{
						iPreferredModePos=1;
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"960x540");
					if(s_pCreateStruct2->dwPreferredScreenWidth==960)
					{
						iPreferredModePos=2;
					}
					break;
			}
		}
		else
		{
			switch(Mode.Width)
			{
				case 1280:	//slightly different pixel aspect ratio
					if(s_DevType==D3DDEVTYPE_REF)
					{
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
							0,(LPARAM)"320x256");
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"640x512");
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"800x640");
					if(s_pCreateStruct2->dwPreferredScreenWidth==800)
					{
						iPreferredModePos=1;
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"960x768");
					if(s_pCreateStruct2->dwPreferredScreenWidth==960)
					{
						iPreferredModePos=2;
					}
					break;
				default:
					if(s_DevType==D3DDEVTYPE_REF)
					{
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
							0,(LPARAM)"320x240");
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"640x480");
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"800x600");
					if(s_pCreateStruct2->dwPreferredScreenWidth==800)
					{
						iPreferredModePos=1;
					}
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"1024x768");
					if(s_pCreateStruct2->dwPreferredScreenWidth==960)
					{
						iPreferredModePos=2;
					}
					break;
			}
		}
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_SETCURSEL,iPreferredModePos,0);
	}
	else
	{
		int iModePos=0;
		int iPreferredModePos=0;
		//display format combo
		DWORD dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
		DWORD dwSelCount=0;
		if(dwModeCount)
		{
			dwSelCount++;
		}
		if(dwSelCount==0)
		{
			MessageBox(NULL,"Seems no 32 bit display modes are available!!!!",
				"Engine critical error",MB_TASKMODAL);
			EndDialog(hDlg,0);
		}
		DisplayModeFmt=D3DFMT_X8R8G8B8;
		//fill mode combo
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_RESETCONTENT,0,0);
		if(bChangeRefresh)
		{
			dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,DisplayModeFmt);
			DWORD dwI;
			D3DDISPLAYMODE	Mode;
			char szModeString[256];
			for(dwI=0;dwI<dwModeCount;dwI++)
			{
				if(FAILED(s_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,DisplayModeFmt,dwI,&Mode)))
				{
					MessageBox(NULL,"Failed to get display mode",
						"Engine critical error",MB_TASKMODAL);
					EndDialog(hDlg,0);
				}
				if((Mode.Width>=640) && (Mode.Height>=480) && (Mode.RefreshRate>=60))
				{
					//build mode string
					sprintf(szModeString,"%dx%dx%d Hz",Mode.Width,Mode.Height,Mode.RefreshRate);
					//add to combo
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,0,(LPARAM)szModeString);
					if(s_pCreateStruct2)
					{
						if(s_pCreateStruct2->dwPreferredScreenWidth==Mode.Width)
						{
							iPreferredModePos=iModePos;
						}
					}
					iModePos++;
				}
			}
		}
		else
		{
			std::set<std::string>			OccurSet;
			std::set<std::string>::iterator	iter;
			dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,DisplayModeFmt);
			DWORD dwI;
			D3DDISPLAYMODE	Mode;
			char szModeString[256];
			for(dwI=0;dwI<dwModeCount;dwI++)
			{
				if(FAILED(s_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,DisplayModeFmt,dwI,&Mode)))
				{
					MessageBox(NULL,"Failed to get display mode","Engine critical error",MB_TASKMODAL);
					EndDialog(hDlg,0);
				}
				//build mode string
				if((Mode.Width>=640) && (Mode.Height>=480))
				{
					sprintf(szModeString,"%dx%d",Mode.Width,Mode.Height);
					iter=OccurSet.find(szModeString);
					if(iter==OccurSet.end())
					{
						//add to combo
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,0,(LPARAM)szModeString);
						if(s_pCreateStruct2)
						{
							if(s_pCreateStruct2->dwPreferredScreenWidth==Mode.Width)
							{
								iPreferredModePos=iModePos;
							}
						}
						iModePos++;
						OccurSet.insert(std::set<std::string>::value_type(szModeString));
					}
				}
			}
		}
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_SETCURSEL,iPreferredModePos,0);
	}
	hCombo=GetDlgItem(hDlg,IDC_DISPLAY_MODE_COMBO);
	int iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
	{
		char szBuffer[16];
		SendMessage(hCombo,CB_GETLBTEXT,iSel,(LPARAM)szBuffer);
		if(bChangeRefresh && (!s_PresentParameters.Windowed))
		{
			sscanf(szBuffer,"%dx%dx%d",&s_PresentParameters.BackBufferWidth,
				&s_PresentParameters.BackBufferHeight,
				&s_PresentParameters.FullScreen_RefreshRateInHz);
			if(s_PresentParameters.Windowed)
			{
				s_PresentParameters.FullScreen_RefreshRateInHz=0;
			}
		}
		else
		{
			sscanf(szBuffer,"%dx%d",&s_PresentParameters.BackBufferWidth,
				&s_PresentParameters.BackBufferHeight);
		}
	}
	if(s_DevType==D3DDEVTYPE_HAL)
	{ //check hardware format conversions etc
		D3DCAPS9 HALCaps;
		s_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,s_DevType,&HALCaps);
		//now check format by format
		DWORD dwNumConfirmedFormats=0;
		if((SUCCEEDED(s_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			DisplayModeFmt,D3DUSAGE_RENDERTARGET,D3DRTYPE_SURFACE,D3DFMT_X8R8G8B8))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayModeFmt,
			D3DFMT_X8R8G8B8,s_PresentParameters.Windowed))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			D3DFMT_X8R8G8B8,DisplayModeFmt))))
		{
			dwNumConfirmedFormats++;
			if(dwNumConfirmedFormats>0)
			{
				s_PresentParameters.BackBufferFormat=D3DFMT_X8R8G8B8;
			}
		}
		if((SUCCEEDED(s_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			DisplayModeFmt,D3DUSAGE_RENDERTARGET,D3DRTYPE_SURFACE,D3DFMT_A2R10G10B10))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,DisplayModeFmt,
			D3DFMT_A2R10G10B10,s_PresentParameters.Windowed))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			D3DFMT_A2R10G10B10,DisplayModeFmt))))
		{
			dwNumConfirmedFormats++;
			if(dwNumConfirmedFormats>0)
			{
				s_PresentParameters.BackBufferFormat=D3DFMT_A2R10G10B10;
			}
		}
		if(!dwNumConfirmedFormats)
		{
			MessageBox(NULL,"Can't find any sensible backbuffer format\n"
				"(or no hardware conversions available),you might consider "
				"switching display format to some other",
				"Engine critical error",MB_TASKMODAL);
		}
	}
	else
	{
		D3DCAPS9 REFCaps;
		s_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,s_DevType,&REFCaps);
		//now check format by format
		DWORD dwNumConfirmedFormats=0;
		if((SUCCEEDED(s_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,
			DisplayModeFmt,D3DUSAGE_RENDERTARGET,D3DRTYPE_SURFACE,D3DFMT_X8R8G8B8))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,DisplayModeFmt,
			D3DFMT_X8R8G8B8,s_PresentParameters.Windowed))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,
			D3DFMT_X8R8G8B8,DisplayModeFmt))))
		{
			dwNumConfirmedFormats++;
			if(dwNumConfirmedFormats>0)
			{
				s_PresentParameters.BackBufferFormat=D3DFMT_X8R8G8B8;
			}
		}
		if((SUCCEEDED(s_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,
			DisplayModeFmt,D3DUSAGE_RENDERTARGET,D3DRTYPE_SURFACE,D3DFMT_A2R10G10B10))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,DisplayModeFmt,
			D3DFMT_A2R10G10B10,s_PresentParameters.Windowed))) &&
			(SUCCEEDED(s_pD3D->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,
			D3DFMT_A2R10G10B10,DisplayModeFmt))))
		{
			dwNumConfirmedFormats++;
			if(dwNumConfirmedFormats>0) s_PresentParameters.BackBufferFormat=D3DFMT_A2R10G10B10;
		}
		if(!dwNumConfirmedFormats)
		{
			MessageBox(NULL,"Can't find any sensible backbuffer format\n"
				"(or no hardware conversions available),you might consider "
				"switching display format to some other",
				"Engine critical error",MB_TASKMODAL);
		}
	}
}

static void AdvancedConfigUpdateResolutionCombo(HWND hDlg)
{
	HWND hCombo;
	//init display mode list
	SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_RESETCONTENT,0,0);
	if(s_PresentParameters.Windowed)
	{
		//display format (same as desktop format)
		D3DDISPLAYMODE Mode;
		s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&Mode);
		switch(Mode.Width)
		{
			case 1280:	//slightly different pixel aspect ratio
				if(s_DevType==D3DDEVTYPE_REF)
				{
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"320x256");
				}
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"640x512");
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"800x640");
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"960x768");
				break;
			default:
				if(s_DevType==D3DDEVTYPE_REF)
				{
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
						0,(LPARAM)"320x240");
				}
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"640x480");
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"800x600");
				SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,
					0,(LPARAM)"1024x768");
				break;
		}
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_SETCURSEL,0,0);
	}
	else
	{
		//display format combo
		DWORD dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
		//fill mode combo
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_RESETCONTENT,0,0);
		if(bChangeRefresh)
		{
			dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,DisplayModeFmt);
			DWORD dwI;
			D3DDISPLAYMODE	Mode;
			char szModeString[256];
			for(dwI=0;dwI<dwModeCount;dwI++)
			{
				if(FAILED(s_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,DisplayModeFmt,dwI,&Mode)))
				{
					MessageBox(NULL,"Failed to get display mode","Engine critical error",MB_TASKMODAL);
					EndDialog(hDlg,0);
				}
				if((Mode.Width>=640) && (Mode.Height>=480) && (Mode.RefreshRate>=60))
				{
					//build mode string
					sprintf(szModeString,"%dx%dx%d Hz",Mode.Width,Mode.Height,Mode.RefreshRate);
					//add to combo
					SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,0,(LPARAM)szModeString);
				}
			}
		}
		else
		{
			std::set<std::string>			OccurSet;
			std::set<std::string>::iterator	iter;
			dwModeCount=s_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,DisplayModeFmt);
			DWORD dwI;
			D3DDISPLAYMODE	Mode;
			char szModeString[256];
			for(dwI=0;dwI<dwModeCount;dwI++)
			{
				if(FAILED(s_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,DisplayModeFmt,dwI,&Mode)))
				{
					MessageBox(NULL,"Failed to get display mode","Engine critical error",MB_TASKMODAL);
					EndDialog(hDlg,0);
				}
				//build mode string
				if((Mode.Width>=640) && (Mode.Height>=480))
				{
					sprintf(szModeString,"%dx%d",Mode.Width,Mode.Height);
					iter=OccurSet.find(szModeString);
					if(iter==OccurSet.end())
					{
						//add to combo
						SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_ADDSTRING,0,(LPARAM)szModeString);
						OccurSet.insert(std::set<std::string>::value_type(szModeString));
					}
				}
			}
		}
		SendDlgItemMessage(hDlg,IDC_DISPLAY_MODE_COMBO,CB_SETCURSEL,0,0);
	}
	hCombo=GetDlgItem(hDlg,IDC_DISPLAY_MODE_COMBO);
	int iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
	{
		char szBuffer[16];
		SendMessage(hCombo,CB_GETLBTEXT,iSel,(LPARAM)szBuffer);
		if(bChangeRefresh && (!s_PresentParameters.Windowed))
		{
			sscanf(szBuffer,"%dx%dx%d",&s_PresentParameters.BackBufferWidth,
				&s_PresentParameters.BackBufferHeight,
				&s_PresentParameters.FullScreen_RefreshRateInHz);
			if(s_PresentParameters.Windowed)
			{
				s_PresentParameters.FullScreen_RefreshRateInHz=0;
			}
		}
		else
		{
			sscanf(szBuffer,"%dx%d",&s_PresentParameters.BackBufferWidth,
				&s_PresentParameters.BackBufferHeight);
		}
	}
}

static BOOL CALLBACK D3DAdvancedConfigDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	HWND			hCombo;
	HWND			hCheckBox;
	if((!s_pCreateStruct2) || (!s_pD3D))
	{
		MessageBox(NULL,"Ooops, unable to display config :(",
			"D3D config error",MB_TASKMODAL);
		EndDialog(hDlg,0);
		return TRUE;
	}
	int iSel=0;
	switch(message)
	{
		case WM_INITDIALOG:
			//check device type settings and init device type combo box
			SendDlgItemMessage(hDlg,IDC_DEVICE_TYPE_COMBO,CB_RESETCONTENT,0,0);
			hCombo=GetDlgItem(hDlg,IDC_DEVICE_TYPE_COMBO);
			SendDlgItemMessage(hDlg,IDC_DEVICE_TYPE_COMBO,CB_ADDSTRING,0,(LPARAM)"HAL");
			SendDlgItemMessage(hDlg,IDC_DEVICE_TYPE_COMBO,CB_ADDSTRING,0,(LPARAM)"REF");
			switch(s_pCreateStruct2->dwDevice)
			{
				case 0:
					s_DevType=D3DDEVTYPE_HAL;
					SendDlgItemMessage(hDlg,IDC_DEVICE_TYPE_COMBO,CB_SETCURSEL,0,0);
					break;
				default:
					s_DevType=D3DDEVTYPE_REF;
					SendDlgItemMessage(hDlg,IDC_DEVICE_TYPE_COMBO,CB_SETCURSEL,0,1);
					break;
			}
			if(!s_pCreateStruct2->bAllowDeviceChange)
			{
				EnableWindow(hCombo,FALSE);
			}
			//init windowed mode check
			s_PresentParameters.Windowed=s_pCreateStruct2->bWindowed;
			hCheckBox=GetDlgItem(hDlg,IDC_WINDOWED_CHECK);
			if(s_PresentParameters.Windowed)
			{
				CheckDlgButton(hDlg,IDC_WINDOWED_CHECK,BST_CHECKED);
			}
			else
			{
				CheckDlgButton(hDlg,IDC_WINDOWED_CHECK,BST_UNCHECKED);
			}
			if(!s_pCreateStruct2->bAllowWindowedChange)
			{
				s_PresentParameters.Windowed=s_PresentParameters.Windowed;
				if(s_PresentParameters.Windowed)
				{
					s_PresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
					s_PresentParameters.FullScreen_RefreshRateInHz=0;
				}
				EnableWindow(hCheckBox,FALSE);
			}
			//init windowed mode dependant checkboxes
			if(s_PresentParameters.Windowed)
			{
				hCheckBox=GetDlgItem(hDlg,IDC_TRIPLE_BUFFER_CHECK);
				EnableWindow(hCheckBox,FALSE);
				hCheckBox=GetDlgItem(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK);
				EnableWindow(hCheckBox,FALSE);
				hCheckBox=GetDlgItem(hDlg,IDC_VSYNC_CHECK);
				EnableWindow(hCheckBox,FALSE);
			}
			else
			{
				if(s_pCreateStruct2)
				{
					if(!s_pCreateStruct2->bDefaultVsync)
					{
						s_PresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
					}
					else
					{
						s_PresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
					}
				}
				if(s_PresentParameters.PresentationInterval==D3DPRESENT_INTERVAL_ONE)
				{
					CheckDlgButton(hDlg,IDC_VSYNC_CHECK,BST_CHECKED);
				}
				else
				{
					CheckDlgButton(hDlg,IDC_VSYNC_CHECK,BST_UNCHECKED);
				}
				if(bChangeRefresh)
				{
					CheckDlgButton(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK,BST_CHECKED);
				}
				else
				{
					CheckDlgButton(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK,BST_UNCHECKED);
				}
				if(((s_PresentParameters.BackBufferCount==2) ? TRUE : FALSE))
				{
					CheckDlgButton(hDlg,IDC_TRIPLE_BUFFER_CHECK,BST_CHECKED);
				}
				else
				{
					CheckDlgButton(hDlg,IDC_TRIPLE_BUFFER_CHECK,BST_UNCHECKED);
				}
			}
			//init all stuff
			AdvancedConfigDeviceChange(hDlg);
			//line antialias checkbox
			hCheckBox=GetDlgItem(hDlg,IDC_LINE_ANTIALIAS_CHECK);
			if(s_DevType==D3DDEVTYPE_REF)
			{
				//REF caps are fixed ...
				EnableWindow(hCheckBox,TRUE);
				CheckDlgButton(hDlg,IDC_LINE_ANTIALIAS_CHECK,BST_CHECKED);
				s_bLineAntialiasing=TRUE;
			}
			else
			{
				//we definitely need to check HAL caps
				D3DCAPS9 Caps;
				s_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&Caps);
				if(Caps.LineCaps&D3DLINECAPS_ANTIALIAS)
				{
					EnableWindow(hCheckBox,TRUE);
					CheckDlgButton(hDlg,IDC_LINE_ANTIALIAS_CHECK,BST_CHECKED);
					s_bLineAntialiasing=TRUE;
				}
				else
				{
					EnableWindow(hCheckBox,FALSE);
					CheckDlgButton(hDlg,IDC_LINE_ANTIALIAS_CHECK,BST_UNCHECKED);
					s_bLineAntialiasing=FALSE;
				}
			}
			//automipmaps
			if(s_pCreateStruct2)
			{
				if(s_pCreateStruct2->bDefaultAutoMipMaps)
				{
					s_bAutoMipMaps=TRUE;
				}
			}
			if(s_bAutoMipMaps)
			{
				CheckDlgButton(hDlg,IDC_AUTOMIPMAP_CHECK,BST_CHECKED);
			}
			else
			{
				CheckDlgButton(hDlg,IDC_AUTOMIPMAP_CHECK,BST_UNCHECKED);
			}
			//rendertarget resolution is pretty much independent of anything else
			SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_RESETCONTENT,0,0);
			hCombo=GetDlgItem(hDlg,IDC_RT_RESOLUTION_COMBO);
			SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_ADDSTRING,0,(LPARAM)"512x512");
			SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_ADDSTRING,0,(LPARAM)"1024x1024");
			SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_ADDSTRING,0,(LPARAM)"2048x2048");
			if(s_pCreateStruct2)
			{
				switch(s_pCreateStruct2->dwPreferredRTSize)
				{
					case 2048:
						SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_SETCURSEL,2,0);
						s_dwScreenRTTextureX=s_dwScreenRTTextureY=2048;
						break;
					case 1024:
						SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_SETCURSEL,1,0);
						s_dwScreenRTTextureX=s_dwScreenRTTextureY=1024;
						break;
					default:
						SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_SETCURSEL,0,0);
						s_dwScreenRTTextureX=s_dwScreenRTTextureY=512;
						break;
				}
			}
			else
			{
				SendDlgItemMessage(hDlg,IDC_RT_RESOLUTION_COMBO,CB_SETCURSEL,0,0);
			}
			//same goes for device aspect ratio
			SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_RESETCONTENT,0,0);
			hCombo=GetDlgItem(hDlg,IDC_DEVICE_ASPECT_COMBO);
			SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_ADDSTRING,0,(LPARAM)"4:3");
			SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_ADDSTRING,0,(LPARAM)"16:9");
			if(s_pCreateStruct2)
			{
				switch(s_pCreateStruct2->dwDefaultDeviceAspect)
				{
					case 1:
						SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_SETCURSEL,1,0);
						break;
					default:
						SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_SETCURSEL,0,0);
						break;
				}
			}
			else
			{
				SendDlgItemMessage(hDlg,IDC_DEVICE_ASPECT_COMBO,CB_SETCURSEL,0,0);
			}
			//pixel shader version limit
			SendDlgItemMessage(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO,CB_RESETCONTENT,0,0);
			hCombo=GetDlgItem(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO);
			SendDlgItemMessage(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO,CB_ADDSTRING,0,(LPARAM)"No limit");
			SendDlgItemMessage(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO,CB_ADDSTRING,0,(LPARAM)"1.4");
			SendDlgItemMessage(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO,CB_ADDSTRING,0,(LPARAM)"1.1");
			SendDlgItemMessage(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO,CB_SETCURSEL,0,0);
			//texture filering settings
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_RESETCONTENT,0,0);
			hCombo=GetDlgItem(hDlg,IDC_TX_FILTERING_COMBO);
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_ADDSTRING,0,(LPARAM)"Anisotropic Max");
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_ADDSTRING,0,(LPARAM)"Anisotropic 8x");
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_ADDSTRING,0,(LPARAM)"Anisotropic 4x");
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_ADDSTRING,0,(LPARAM)"Anisotropic 2x");
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_ADDSTRING,0,(LPARAM)"Trilinear");
			SendDlgItemMessage(hDlg,IDC_TX_FILTERING_COMBO,CB_SETCURSEL,0,0);
			return TRUE;
			break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDOK:
				//read data from dialog
				if(s_bSaveSettings && s_pCreateStruct2)
				{
					char szResolutionStr[64];
					char szRTResolutionStr[64];
					char szDevAspectStr[64];
					char szVsyncStr[64];
					char szAutoMipMapsStr[64];

					sprintf(szResolutionStr,"%d",s_PresentParameters.BackBufferWidth);
					sprintf(szRTResolutionStr,"%d",s_dwScreenRTTextureX);
					switch(s_eDeviceAspectRatio)
					{
						case AR_16_9:
							sprintf(szDevAspectStr,"%d",1);
							break;
						default:
							sprintf(szDevAspectStr,"%d",0);
							break;
					}
					switch(s_PresentParameters.PresentationInterval)
					{
						case D3DPRESENT_INTERVAL_IMMEDIATE:
							sprintf(szVsyncStr,"%d",0);
							break;
						default:
							sprintf(szVsyncStr,"%d",1);
							break;
					}
					if(s_bAutoMipMaps)
					{
						sprintf(szAutoMipMapsStr,"%d",1);
					}
					else
					{
						sprintf(szAutoMipMapsStr,"%d",0);
					}
					//write to registry
					HKEY hLiquidReality=NULL;
					RegCreateKey(HKEY_CURRENT_USER,
						s_pCreateStruct2->szProgramRegKey,
						&hLiquidReality);
					if(hLiquidReality)
					{
						const char *szTmp=NULL;
						szTmp=szResolutionStr;
						RegSetValueEx(hLiquidReality,"ScreenWidth",0,REG_SZ,
							(CONST BYTE*)(LPCSTR)szTmp,strlen(szTmp)+1);
						szTmp=szRTResolutionStr;
						RegSetValueEx(hLiquidReality,"RTSize",0,REG_SZ,
							(CONST BYTE*)(LPCSTR)szTmp,strlen(szTmp)+1);
						szTmp=szDevAspectStr;
						RegSetValueEx(hLiquidReality,"DeviceAspect",0,REG_SZ,
							(CONST BYTE*)(LPCSTR)szTmp,strlen(szTmp)+1);
						szTmp=szVsyncStr;
						RegSetValueEx(hLiquidReality,"VSync",0,REG_SZ,
							(CONST BYTE*)(LPCSTR)szTmp,strlen(szTmp)+1);
						szTmp=szAutoMipMapsStr;
						RegSetValueEx(hLiquidReality,"AutoMipmaps",0,REG_SZ,
							(CONST BYTE*)(LPCSTR)szTmp,strlen(szTmp)+1);
						RegCloseKey(hLiquidReality);
					}
				}
				EndDialog(hDlg,1);
				return TRUE;
				break;
			case IDCANCEL:
				EndDialog(hDlg,0);
				return TRUE;
				break;
			case IDC_DEVICE_TYPE_COMBO:
				if(HIWORD(wParam)==CBN_SELCHANGE)
				{
					hCombo=GetDlgItem(hDlg,IDC_DEVICE_TYPE_COMBO);
					iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
					if(iSel==0)
					{
						s_DevType=D3DDEVTYPE_HAL;
					}
					else
					{
						s_DevType=D3DDEVTYPE_REF;
					}
					//reinit all ....
					AdvancedConfigDeviceChange(hDlg);
				}
				break;
			case IDC_RT_RESOLUTION_COMBO:
				if(HIWORD(wParam)==CBN_SELCHANGE)
				{
					hCombo=GetDlgItem(hDlg,IDC_RT_RESOLUTION_COMBO);
					iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
					switch(iSel)
					{
						case 1:
							s_dwScreenRTTextureX=1024;
							s_dwScreenRTTextureY=1024;
							break;
						case 2:
							s_dwScreenRTTextureX=2048;
							s_dwScreenRTTextureY=2048;
							break;
						default:
							s_dwScreenRTTextureX=512;
							s_dwScreenRTTextureY=512;
							break;
					}
				}
				break;
			case IDC_DEVICE_ASPECT_COMBO:
				if(HIWORD(wParam)==CBN_SELCHANGE)
				{
					hCombo=GetDlgItem(hDlg,IDC_DEVICE_ASPECT_COMBO);
					iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
					switch(iSel)
					{
						case 1:
							s_eDeviceAspectRatio=AR_16_9;
							break;
						default:
							s_eDeviceAspectRatio=AR_4_3;
							break;
					}
				}
				break;
			case IDC_PIXEL_SHADER_VERSION_COMBO:
				if(HIWORD(wParam)==CBN_SELCHANGE)
				{
					hCombo=GetDlgItem(hDlg,IDC_PIXEL_SHADER_VERSION_COMBO);
					iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
					s_dwPixelShaderVersionLimit=iSel;
				}
				break;
			case IDC_WINDOWED_CHECK:
				s_PresentParameters.Windowed=IsDlgButtonChecked(hDlg,IDC_WINDOWED_CHECK);
				//init windowed mode dependant checkboxes
				if(s_PresentParameters.Windowed)
				{
					hCheckBox=GetDlgItem(hDlg,IDC_TRIPLE_BUFFER_CHECK);
					EnableWindow(hCheckBox,FALSE);
					hCheckBox=GetDlgItem(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK);
					EnableWindow(hCheckBox,FALSE);
					hCheckBox=GetDlgItem(hDlg,IDC_VSYNC_CHECK);
					EnableWindow(hCheckBox,FALSE);
					s_PresentParameters.PresentationInterval=D3DPRESENT_INTERVAL_ONE;
					s_PresentParameters.FullScreen_RefreshRateInHz=0;
				}
				else
				{
					hCheckBox=GetDlgItem(hDlg,IDC_TRIPLE_BUFFER_CHECK);
					EnableWindow(hCheckBox,TRUE);
					hCheckBox=GetDlgItem(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK);
					EnableWindow(hCheckBox,TRUE);
					hCheckBox=GetDlgItem(hDlg,IDC_VSYNC_CHECK);
					EnableWindow(hCheckBox,TRUE);
					if(s_PresentParameters.PresentationInterval==D3DPRESENT_INTERVAL_ONE)
					{
						CheckDlgButton(hDlg,IDC_VSYNC_CHECK,BST_CHECKED);
					}
					else
					{
						CheckDlgButton(hDlg,IDC_VSYNC_CHECK,BST_UNCHECKED);
					}
					if(bChangeRefresh)
					{
						CheckDlgButton(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK,BST_CHECKED);
					}
					else
					{
						CheckDlgButton(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK,BST_UNCHECKED);
					}
					if(((s_PresentParameters.BackBufferCount==2) ? TRUE : FALSE))
					{
						CheckDlgButton(hDlg,IDC_TRIPLE_BUFFER_CHECK,BST_CHECKED);
					}
					else
					{
						CheckDlgButton(hDlg,IDC_TRIPLE_BUFFER_CHECK,BST_UNCHECKED);
					}
				}
				//init all stuff
				AdvancedConfigDeviceChange(hDlg);
				break;
			case IDC_TRIPLE_BUFFER_CHECK:
				s_PresentParameters.BackBufferCount=(IsDlgButtonChecked(hDlg,IDC_TRIPLE_BUFFER_CHECK)) ? 2 : 1;
				break;
			case IDC_CHANGE_REFRESH_RATE_CHECK:
				bChangeRefresh=IsDlgButtonChecked(hDlg,IDC_CHANGE_REFRESH_RATE_CHECK) ? true : false;
				AdvancedConfigUpdateResolutionCombo(hDlg);
				break;
			case IDC_VSYNC_CHECK:
				s_PresentParameters.PresentationInterval=(IsDlgButtonChecked(hDlg,IDC_VSYNC_CHECK)) ?
					D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
				break;
			case IDC_TX_FILTERING_COMBO:
				hCombo=GetDlgItem(hDlg,IDC_TX_FILTERING_COMBO);
				iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
				s_dwTextureFiltering=iSel;
				break;
			case IDC_DISPLAY_MODE_COMBO:
				hCombo=GetDlgItem(hDlg,IDC_DISPLAY_MODE_COMBO);
				iSel=SendMessage(hCombo,CB_GETCURSEL,0,0);
				{
					char szBuffer[16];
					SendMessage(hCombo,CB_GETLBTEXT,iSel,(LPARAM)szBuffer);
					if(bChangeRefresh && (!s_PresentParameters.Windowed))
					{
						sscanf(szBuffer,"%dx%dx%d",&s_PresentParameters.BackBufferWidth,
							&s_PresentParameters.BackBufferHeight,
							&s_PresentParameters.FullScreen_RefreshRateInHz);
						if(s_PresentParameters.Windowed)
						{
							s_PresentParameters.FullScreen_RefreshRateInHz=0;
						}
					}
					else
					{
						sscanf(szBuffer,"%dx%d",&s_PresentParameters.BackBufferWidth,
							&s_PresentParameters.BackBufferHeight);
					}
				}
				break;
			case IDC_LINE_ANTIALIAS_CHECK:
				s_bLineAntialiasing=(IsDlgButtonChecked(hDlg,IDC_LINE_ANTIALIAS_CHECK)) ? TRUE : FALSE;
				break;
			case IDC_AUTOMIPMAP_CHECK:
				s_bAutoMipMaps=(IsDlgButtonChecked(hDlg,IDC_AUTOMIPMAP_CHECK)) ? TRUE : FALSE;
				break;
			case IDC_SAVE_SETTINGS_CHECK:
				s_bSaveSettings=(IsDlgButtonChecked(hDlg,IDC_SAVE_SETTINGS_CHECK)) ? TRUE : FALSE;
				break;
		}
		break;
	}
	return FALSE;
}

void D3DImpl::getSelectedModeParams(DWORD *pdwWidth,DWORD *pdwHeight,bool *pbWindowed)
{
	*pdwWidth=s_PresentParameters.BackBufferWidth;
	*pdwHeight=s_PresentParameters.BackBufferHeight;
	*pbWindowed=s_PresentParameters.Windowed ? true : false;
}

/*HRESULT	D3DImpl::displayAdvancedConfig(DeviceCreateStruct *pCreateStruct)
{
	int result;
	s_pD3D=m_pD3D;
	s_pCreateStruct=pCreateStruct;
	s_bWidescreen=(m_eRenderAspect==AR_16_9) ? true : false;
	result=DialogBox(NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDD_ENGINE_CONFIG),
		NULL,
		D3DAdvancedConfigDlgProc);
	return S_OK;
}*/

HRESULT	D3DImpl::displayAdvancedConfig2(DeviceCreateStruct2 *pCreateStruct)
{
	int result;
	s_pD3D=m_pD3D;
	s_pCreateStruct2=pCreateStruct;
	s_bWidescreen=(m_eRenderAspect==AR_16_9) ? true : false;
	if(strlen(pCreateStruct->szProgramRegKey)>0)
	{
		//try to read settings from registry
		HKEY hLiquidReality=NULL;
		RegCreateKey(HKEY_CURRENT_USER,
			pCreateStruct->szProgramRegKey,
			&hLiquidReality);
		if(hLiquidReality)
		{
			char szRegStr[256];
			szRegStr[0]=0;
			DWORD dwStrSize=256;
			DWORD dwDummy=0;
			RegQueryValueEx(hLiquidReality,"ScreenWidth",NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize)
			{
				pCreateStruct->dwPreferredScreenWidth=atoi(szRegStr);
			}
			RegQueryValueEx(hLiquidReality,"RTSize",NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize)
			{
				pCreateStruct->dwPreferredRTSize=atoi(szRegStr);
			}
			RegQueryValueEx(hLiquidReality,"DeviceAspect",NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize)
			{
				pCreateStruct->dwDefaultDeviceAspect=atoi(szRegStr);
			}
			RegQueryValueEx(hLiquidReality,"VSync",NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize)
			{
				pCreateStruct->bDefaultVsync=(atoi(szRegStr)!=0) ? TRUE : FALSE;
			}
			RegQueryValueEx(hLiquidReality,"AutoMipmaps",NULL,&dwDummy,
				(LPBYTE)szRegStr,&dwStrSize);
			if(dwStrSize)
			{
				pCreateStruct->bDefaultAutoMipMaps=(atoi(szRegStr)!=0) ? TRUE : FALSE;
			}
			RegCloseKey(hLiquidReality);
		}
	}
	result=DialogBox(NMainDllGlobals::g_hInstance,
		MAKEINTRESOURCE(IDD_ENGINE_CONFIG),
		NULL,
		D3DAdvancedConfigDlgProc);
	return (result==IDOK) ? S_OK : E_FAIL;
}


void D3DImpl::setDeviceWindowHandle(HWND hWnd)
{
	s_PresentParameters.hDeviceWindow=hWnd;
}

//sprite vertex is rhw+uv which is 6 floats
#define SPRITE_VERTEX_SIZE		24

static D3DVERTEXELEMENT9 sSpriteVF[]=
{
	{0,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITIONT,0},
	{0,16,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	D3DDECL_END()
};

static D3DVERTEXELEMENT9 sVSSpriteVF[]=
{
	{0,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
	{0,16,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	D3DDECL_END()
};


void D3DImpl::createRenderingDevice() 
{
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,s_DevType,
		s_PresentParameters.hDeviceWindow,s_dwDevBehavior,&s_PresentParameters,
		&m_pD3DDevice)))
	{
		throw InvalidOperationExceptionImpl("Failed to create rendering device!!!");
	}
	DEBUG_STRING("D3D (INFO): Created rendering device\n");
	if(s_DevType==D3DDEVTYPE_HAL)
	{
		m_DevCaps=m_HALDeviceCaps;
	}
	else
	{
		m_DevCaps=m_REFDeviceCaps;
	}
	D3D_RTDesc rt_desc;
	ZeroMemory(&rt_desc,sizeof(D3D_RTDesc));
	if(FAILED(m_pD3DDevice->GetRenderTarget(0,&rt_desc.m_pRTSurface)))
	{
		m_pD3DDevice->Release();
		m_pD3DDevice=NULL;
		throw InvalidOperationExceptionImpl("Failed to get current render target!!!");
	}
	rt_desc.m_dwHeight=s_PresentParameters.BackBufferHeight;
	rt_desc.m_dwWidth=s_PresentParameters.BackBufferWidth;
	rt_desc.m_Format=s_PresentParameters.BackBufferFormat;
	rt_desc.m_bBackbuffer=TRUE;
	m_RTPool[m_dwRTPoolSize++]=rt_desc;
	D3D_DSDesc ds_desc;
	ZeroMemory(&ds_desc,sizeof(D3D_DSDesc));
	if(FAILED(m_pD3DDevice->GetDepthStencilSurface(&ds_desc.m_pDSSurface)))
	{
		m_RTPool[0].m_pRTSurface->Release();
		m_dwRTPoolSize=0;
		m_pD3DDevice->Release();
		m_pD3DDevice=NULL;
		throw InvalidOperationExceptionImpl("Failed to get current depth stencil surface!!!");
	}
	ds_desc.m_dwHeight=s_PresentParameters.BackBufferHeight;
	ds_desc.m_dwWidth=s_PresentParameters.BackBufferWidth;
	ds_desc.m_Format=s_PresentParameters.AutoDepthStencilFormat;
	m_DSPool[m_dwDSPoolSize++]=ds_desc;
	//set some sensible defaults
	m_MipFilter=D3DTEXF_LINEAR;
	m_MinFilter=D3DTEXF_ANISOTROPIC;
	m_MagFilter=D3DTEXF_ANISOTROPIC;
	switch(s_dwTextureFiltering)
	{
		case 0:
			m_dwAnisotropy=m_DevCaps.MaxAnisotropy;
			break;
		case 1:
			m_dwAnisotropy=8;
			break;
		case 2:
			m_dwAnisotropy=4;
			break;
		case 3:
			m_dwAnisotropy=2;
			break;
		case 4:
			m_dwAnisotropy=1;
			m_MinFilter=D3DTEXF_LINEAR;
			m_MagFilter=D3DTEXF_LINEAR;
			break;
	}
	if(m_dwAnisotropy>m_DevCaps.MaxAnisotropy)
	{
		m_dwAnisotropy=m_DevCaps.MaxAnisotropy;
	}
	setDefaultFilteringState();
	//init font renderer
	m_pFont=new CD3DFont(_T("Arial"),16,D3DFONT_BOLD);
	m_pFont->InitDeviceObjects(m_pD3DDevice);
	m_pFont->RestoreDeviceObjects();
	//init line antialiasing state
	if(s_bLineAntialiasing)
	{
		m_pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE,TRUE);
	}
	//init default dynamic VB
	createDynamicVB(0);
	m_pD3DDevice->BeginStateBlock();
	//TODO: fill it with some sensible state
	m_pD3DDevice->EndStateBlock(&m_pSpriteStateBlock);
	//create sprite vertex declaration
	try
	{
		createVertexDecl(sSpriteVF,&m_pSpriteVFDecl);
		createVertexDecl(sVSSpriteVF,&m_pVSSpriteVFDecl);
	}
	catch(Exception &)
	{
		if(m_pSpriteVFDecl)
		{
			m_pSpriteVFDecl->Release();
			m_pSpriteVFDecl=NULL;
		}
		m_RTPool[0].m_pRTSurface->Release();
		m_dwRTPoolSize=0;
		m_pD3DDevice->Release();
		m_pD3DDevice=NULL;
		throw;
	}
	m_dwCurrentSpriteVSVersion=D3DVS_VERSION(0,0);
	m_dwCurrentSpritePSConstantCount=0;
	m_dwCurrentSpriteVSConstantCount=0;
	//init feedback subsystem
	m_bLastFrameFeedbackRequest=FALSE;
	m_bIsRenderingFeedback=FALSE;
	m_dwThisFrameRTHandle=0xffffffff;
	m_dwLastFrameRTHandle=0xffffffff;
	m_dwFeedbackDSHandle=0xffffffff;
	//automipmaps
	m_bDisableAutoMipMaps=!s_bAutoMipMaps;
	//scrren rt
	m_dwScreenRTTextureX=s_dwScreenRTTextureX;
	m_dwScreenRTTextureY=s_dwScreenRTTextureY;
	//device aspect ratio
	m_eDeviceAspect=s_eDeviceAspectRatio;
	switch(s_dwPixelShaderVersionLimit)
	{
		case 1:
			if(m_DevCaps.PixelShaderVersion>D3DPS_VERSION(1,4))
			{
				m_DevCaps.PixelShaderVersion=D3DPS_VERSION(1,4);
			}
			if(m_HALDeviceCaps.PixelShaderVersion>D3DPS_VERSION(1,4))
			{
				m_HALDeviceCaps.PixelShaderVersion=D3DPS_VERSION(1,4);
			}
			break;
		case 2:
			if(m_DevCaps.PixelShaderVersion>D3DPS_VERSION(1,1))
			{
				m_DevCaps.PixelShaderVersion=D3DPS_VERSION(1,1);
			}
			if(m_HALDeviceCaps.PixelShaderVersion>D3DPS_VERSION(1,1))
			{
				m_HALDeviceCaps.PixelShaderVersion=D3DPS_VERSION(1,1);
			}
			break;
	}
}

/***********************
 VB/IB creation stuff
************************/

/*
 *	VertexBuffer class
 */

ULONG VertexBufferImpl::Release()
{
	m_uRefCount--;
	if(m_uRefCount==0)
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

ULONG VertexBufferImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

VertexBufferImpl::VertexBufferImpl(VBCreateStruct *pCreateStruct,VertexStream **ppVStreamTbl)
{
	m_uRefCount=0;
	DWORD dwI=0;
	D3DVERTEXELEMENT9 DeclEnd=D3DDECL_END();
	for(dwI=0;dwI<MAX_STREAM_DECL_COMPONENTS;dwI++)
	{
		m_D3DDecl[dwI]=DeclEnd;
	}
	for(dwI=0;dwI<MAX_VERTEX_STREAMS;dwI++)
	{
		m_StreamTbl[dwI]=NULL;
	}
	m_pD3DVDecl=NULL;
	//now init
	m_bUsagePoints=pCreateStruct->bUsagePoints;
	m_dwSize=pCreateStruct->dwSize;
	m_dwStreamCount=pCreateStruct->dwStreamCount;
	for(dwI=0;dwI<m_dwStreamCount;dwI++)
	{
		m_StreamTbl[dwI]=ppVStreamTbl[dwI];
		m_StreamTbl[dwI]->AddRef();
	}
	//calculate stream decl - currently multiple usage indices are allowed only
	//on POSITION,NORMAL and TEXCOORD
	//TODO: support for TANGENT usages
	const D3DVERTEXELEMENT9	*pTempDecl=NULL;
	DWORD dwJ=0;
	DWORD dwPositionIndex=0;
	DWORD dwNormalIndex=0;
	DWORD dwTexcoordIndex=0;
	DWORD dwStreamIndex=0;
	for(dwI=0;dwI<m_dwStreamCount;dwI++) {
		//TODO: exception used as control flow statement
		try
		{
			pTempDecl=m_StreamTbl[dwI]->getD3DVertexDecl();
		}
		catch(Exception &)
		{
			continue;		//not renderable
		}
		while((*pTempDecl).Stream!=0xff)
		{
			m_D3DDecl[dwJ]=*pTempDecl;
			switch(m_D3DDecl[dwJ].Usage)
			{
				case D3DDECLUSAGE_POSITION:
					m_D3DDecl[dwJ].UsageIndex=(BYTE)(dwPositionIndex++);
					break;
				case D3DDECLUSAGE_TEXCOORD:
					m_D3DDecl[dwJ].UsageIndex=(BYTE)(dwTexcoordIndex++);
					break;
				case D3DDECLUSAGE_NORMAL:
					m_D3DDecl[dwJ].UsageIndex=(BYTE)(dwNormalIndex++);
					break;
			}
			m_D3DDecl[dwJ].Stream=(BYTE)dwStreamIndex;
			pTempDecl++;
			dwJ++;
		}
		dwStreamIndex++;
	}
	m_D3DDecl[dwJ]=DeclEnd;
	//create stream decl object
	NMainDllGlobals::g_pD3D->createVertexDecl(&(m_D3DDecl[0]),&m_pD3DVDecl);
}

VertexBufferImpl::~VertexBufferImpl()
{
	m_uRefCount=0;
	DWORD dwI=0;
	D3DVERTEXELEMENT9 DeclEnd=D3DDECL_END();
	for(dwI=0;dwI<MAX_STREAM_DECL_COMPONENTS;dwI++)
	{
		m_D3DDecl[dwI]=DeclEnd;
	}
	for(dwI=0;dwI<MAX_VERTEX_STREAMS;dwI++)
	{
		if(m_StreamTbl[dwI])
		{
			m_StreamTbl[dwI]->Release();
			m_StreamTbl[dwI]=NULL;
		}
	}
	if(m_pD3DVDecl)
	{
		m_pD3DVDecl->Release();
		m_pD3DVDecl=NULL;
	}
}

DWORD VertexBufferImpl::getStreamCount()
{
	return m_dwStreamCount;
}

void VertexBufferImpl::getVertexStream(DWORD dwIndex,AutoPtr<VertexStream> &ppStream)
{
	if(dwIndex>=m_dwStreamCount)
	{
		throw IndexOutOfBoundsExceptionImpl(0,m_dwStreamCount,dwIndex);
	}
	m_StreamTbl[dwIndex]->AddRef();
	ppStream=m_StreamTbl[dwIndex];
}

void VertexBufferImpl::getD3DStreamDecl(IDirect3DVertexDeclaration9 **ppDecl)
{
	*ppDecl=m_pD3DVDecl;
	m_pD3DVDecl->AddRef();
}

/*
 *	VertexStream class
 */

ULONG VertexStreamImpl::Release()
{
	m_uRefCount--;
	if(m_uRefCount==0)
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

ULONG VertexStreamImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

static DWORD getVertexSizeFromDecl(const D3DVERTEXELEMENT9 *Decl)
{
	D3DVERTEXELEMENT9	EndElem=D3DDECL_END();
	DWORD dwI=0;
	DWORD dwVertexSize=0;
	while(memcmp(&Decl[dwI],&EndElem,sizeof(D3DVERTEXELEMENT9))!=0) dwI++;
	dwVertexSize=Decl[dwI-1].Offset;
	switch(Decl[dwI-1].Type)
	{
		case D3DDECLTYPE_FLOAT3:
			dwVertexSize+=12;
			break;
		case D3DDECLTYPE_FLOAT2:
		case D3DDECLTYPE_FLOAT16_4:
		case D3DDECLTYPE_SHORT4:
		case D3DDECLTYPE_SHORT4N:
		case D3DDECLTYPE_USHORT4N:
			dwVertexSize+=8;
			break;
		case D3DDECLTYPE_FLOAT4:
			dwVertexSize+=16;
			break;
		default:
			dwVertexSize+=4;
			break;
	}
	return dwVertexSize;
}

VertexStreamImpl::VertexStreamImpl(StreamDesc *pDesc,DWORD dwLength,DWORD dwOffset,
							   DWORD dwUsage,PDIRECT3DVERTEXBUFFER9 pVB)
{
	m_uRefCount=0;
	m_pVB=pVB;
	if(m_pVB)
	{
		m_pVB->AddRef();
	}
	m_pVBWrapper=NULL;
	m_dwLength=dwLength;
	m_dwOffset=dwOffset;
	m_dwUsage=dwUsage;
	m_dwFlags=pDesc->dwFlags;
	m_eMode=pDesc->eMode;
	DWORD dwI;
	for(dwI=0;dwI<MAX_STREAM_DECL_COMPONENTS;dwI++)
	{
		m_InternalDecl[dwI]=pDesc->InternalDecl[dwI];
		m_ExternalDecl[dwI]=pDesc->ExternalDecl[dwI];
	}
	m_dwInternalVertexSize=getVertexSizeFromDecl(&m_InternalDecl[0]);
	m_dwExternalVertexSize=getVertexSizeFromDecl(&m_ExternalDecl[0]);
	if(m_dwFlags&SF_LOCKABLE)
	{
		m_pBaseSysmemCopy=new BYTE[m_dwExternalVertexSize*m_dwLength];
	}
	else
	{
		m_pBaseSysmemCopy=NULL;
	}
	if(m_dwFlags&SF_DEFORMER)
	{
		m_pDeformerSysmemCopy=new BYTE[m_dwExternalVertexSize*m_dwLength];
	}
	else
	{
		m_pDeformerSysmemCopy=NULL;
	}
	m_bIsLocked=FALSE;
	m_bBaseSysmemCopyValid=FALSE;
	m_bDeformerSysmemCopyValid=FALSE;
	m_bD3DVBValid=FALSE;
	m_dwDiscardID=0;
}

VertexStreamImpl::VertexStreamImpl(StreamDesc *pDesc,DWORD dwLength,DWORD dwUsage,
							   D3D_DynamicVBWrapper *pVBWrapper)
{
	m_uRefCount=0;
	m_pVB=NULL;
	m_pVBWrapper=pVBWrapper;
	if(m_pVBWrapper)
	{
		m_pVBWrapper->m_pVB->AddRef();
	}
	m_dwLength=dwLength;
	m_dwOffset=0;
	m_dwUsage=dwUsage;
	m_dwFlags=pDesc->dwFlags;
	m_eMode=pDesc->eMode;
	DWORD dwI;
	for(dwI=0;dwI<MAX_STREAM_DECL_COMPONENTS;dwI++)
	{
		m_InternalDecl[dwI]=pDesc->InternalDecl[dwI];
		m_ExternalDecl[dwI]=pDesc->ExternalDecl[dwI];
	}
	m_dwInternalVertexSize=getVertexSizeFromDecl(&m_InternalDecl[0]);
	m_dwExternalVertexSize=getVertexSizeFromDecl(&m_ExternalDecl[0]);
	if(m_dwFlags&SF_LOCKABLE)
	{
		m_pBaseSysmemCopy=new BYTE[m_dwExternalVertexSize*m_dwLength];
	}
	else
	{
		m_pBaseSysmemCopy=NULL;
	}
	if(m_dwFlags&SF_DEFORMER)
	{
		m_pDeformerSysmemCopy=new BYTE[m_dwExternalVertexSize*m_dwLength];
	}
	else
	{
		m_pDeformerSysmemCopy=NULL;
	}
	m_bIsLocked=FALSE;
	m_bBaseSysmemCopyValid=FALSE;
	m_bDeformerSysmemCopyValid=FALSE;
	m_bD3DVBValid=FALSE;
	m_dwDiscardID=0;
}

VertexStreamImpl::~VertexStreamImpl()
{
	if(m_pVB)
	{
		m_pVB->Release();
		m_pVB=NULL;
	}
	if(m_pVBWrapper)
	{
		m_pVBWrapper->m_pVB->Release();
		m_pVBWrapper=NULL;
	}
	m_dwLength=0;
	m_dwOffset=0;
	m_dwUsage=0;
	m_dwFlags=0;
	DWORD dwI;
	D3DVERTEXELEMENT9 EndElem=D3DDECL_END();
	for(dwI=0;dwI<MAX_STREAM_DECL_COMPONENTS;dwI++)
	{
		m_InternalDecl[dwI]=EndElem;
		m_ExternalDecl[dwI]=EndElem;
	}
	if(m_pBaseSysmemCopy)
	{
		delete [] m_pBaseSysmemCopy;
		m_pBaseSysmemCopy=NULL;
	}
	if(m_pDeformerSysmemCopy)
	{
		delete [] m_pDeformerSysmemCopy;
		m_pDeformerSysmemCopy=NULL;
	}
}

//stream copy with on-the-fly conversion between:
//DEC3N-FLOAT3 (compressed normals etc)
//FLOAT2/4-FLOAT16_2/4 (usually texture coordinates)
static HRESULT copyStreamWithFormatConversion(const D3DVERTEXELEMENT9 *SrcDecl,
											  const D3DVERTEXELEMENT9 *DstDecl,
											  BYTE *pbSrcData,
											  BYTE *pbDstData,
											  DWORD dwVertexCount)
{
	DWORD dwI=0;
	DWORD dwJ=0;
	DWORD dwSrcVertexSize=getVertexSizeFromDecl(SrcDecl);
	DWORD dwDstVertexSize=getVertexSizeFromDecl(DstDecl);
	for(dwI=0;dwI<dwVertexCount;dwI++)
	{
		dwJ=0;
		while(DstDecl[dwJ].Stream!=0xff)
		{
			if(DstDecl[dwJ].Type==SrcDecl[dwJ].Type)
			{
				if(DstDecl[dwJ+1].Stream!=0xff)
				{
					memcpy(pbDstData,pbSrcData,DstDecl[dwJ+1].Offset-DstDecl[dwJ].Offset);
					pbDstData+=DstDecl[dwJ+1].Offset-DstDecl[dwJ].Offset;
					pbSrcData+=SrcDecl[dwJ+1].Offset-SrcDecl[dwJ].Offset;
				}
				else
				{
					memcpy(pbDstData,pbSrcData,dwDstVertexSize-DstDecl[dwJ].Offset);
					pbDstData+=dwDstVertexSize-DstDecl[dwJ].Offset;
					pbSrcData+=dwSrcVertexSize-SrcDecl[dwJ].Offset;
				}
			}
			else
			{ //conversion
				if((DstDecl[dwJ].Type==D3DDECLTYPE_DEC3N) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_FLOAT3))
				{
					short x,y,z;
					x=(short)(((float*)pbSrcData)[0]*16384.0f);
					y=(short)(((float*)pbSrcData)[1]*16384.0f);
					z=(short)(((float*)pbSrcData)[2]*16384.0f);
					*((int*)pbDstData)=((((int)((x & 0xffc0) >> 6)) << 22) | (((int)((x & 0xffc0) >> 6)) << 12) |
						(((int)((x & 0xffc0) >> 6)) << 2)) ^ 3;
					pbDstData+=4;
					pbSrcData+=12;
				}
				else if((DstDecl[dwJ].Type==D3DDECLTYPE_FLOAT3) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_DEC3N))
				{
					float x,y,z;
					x=((float)(((*((int*)pbSrcData)) >> 16) & 0xffc0))/16384.0f;
					y=((float)(((*((int*)pbSrcData)) >> 6) & 0xffc0))/16384.0f;
					z=((float)(((*((int*)pbSrcData)) << 4) & 0xffc0))/16384.0f;
					((float*)pbDstData)[0]=x;
					((float*)pbDstData)[1]=y;
					((float*)pbDstData)[2]=z;
					pbDstData+=12;
					pbSrcData+=4;
				}
				else if((DstDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_2) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_FLOAT2))
				{
					((D3DXFLOAT16*)pbDstData)[0]=D3DXFLOAT16(((float*)pbSrcData)[0]);
					((D3DXFLOAT16*)pbDstData)[1]=D3DXFLOAT16(((float*)pbSrcData)[1]);
					pbDstData+=4;
					pbSrcData+=8;
				}
				else if((DstDecl[dwJ].Type==D3DDECLTYPE_FLOAT2) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_2))
				{
					((float*)pbDstData)[0]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[0]);
					((float*)pbDstData)[1]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[1]);
					pbDstData+=8;
					pbSrcData+=4;
				}
				else if((DstDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_4) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_FLOAT4))
				{
					((D3DXFLOAT16*)pbDstData)[0]=D3DXFLOAT16(((float*)pbSrcData)[0]);
					((D3DXFLOAT16*)pbDstData)[1]=D3DXFLOAT16(((float*)pbSrcData)[1]);
					((D3DXFLOAT16*)pbDstData)[2]=D3DXFLOAT16(((float*)pbSrcData)[2]);
					((D3DXFLOAT16*)pbDstData)[3]=D3DXFLOAT16(((float*)pbSrcData)[3]);
					pbDstData+=6;
					pbSrcData+=12;
				}
				else if((DstDecl[dwJ].Type==D3DDECLTYPE_FLOAT4) &&
					(SrcDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_4))
				{
					((float*)pbDstData)[0]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[0]);
					((float*)pbDstData)[1]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[1]);
					((float*)pbDstData)[2]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[2]);
					((float*)pbDstData)[3]=(FLOAT)(((D3DXFLOAT16*)pbSrcData)[3]);
					pbDstData+=12;
					pbSrcData+=6;
				}
				else
				{
					MessageBox(NULL,"No applicable vertex format conversion found\nProbably internal engine error",
						"Engine critical error",MB_TASKMODAL);
					return E_FAIL;
				}
			}
			dwJ++;
		}
	}
	return S_OK;
}

void VertexStreamImpl::loadStream(void* pStreamData,DWORD dwVertexCount,const D3DVERTEXELEMENT9 *Decl)
{
	if(m_eMode!=SM_STATIC)
	{
		throw InvalidOperationExceptionImpl("Stream is not static");
	}
	if(dwVertexCount!=m_dwLength)
	{
		throw InvalidOperationExceptionImpl("Stream data has wrong size");
	}
	BYTE *pbData=NULL;
	if(m_dwFlags&SF_RENDERABLE)
	{
		if(FAILED(m_pVB->Lock(m_dwOffset*m_dwInternalVertexSize,
			dwVertexCount*m_dwInternalVertexSize,(void**)&pbData,0)))
		{
			throw InvalidOperationExceptionImpl("Failed to lock vertex buffer (loadStream)");
		}
		DWORD dwI=0;
		while(Decl[dwI].Stream!=0xff)
		{
			dwI++;
		}
		if(memcmp(Decl,m_InternalDecl,(dwI+1)*sizeof(D3DVERTEXELEMENT9))!=0)
		{
			if(FAILED(copyStreamWithFormatConversion(Decl,m_InternalDecl,(BYTE*)pStreamData,pbData,dwVertexCount)))
			{
				m_pVB->Unlock();
				throw InvalidOperationExceptionImpl("Failed to copy vertex stream (with format conversion)");
			}
		}
		else
		{
			memcpy((void*)pbData,pStreamData,dwVertexCount*m_dwInternalVertexSize);
		}
		m_bD3DVBValid=TRUE;
		m_pVB->Unlock();
	}
	if(m_dwFlags&SF_LOCKABLE)
	{
		DWORD dwI=0;
		while(Decl[dwI].Offset!=0xff) dwI++;
		if(memcmp(Decl,m_ExternalDecl,(dwI+1)*sizeof(D3DVERTEXELEMENT9))!=0)
		{
			//TODO: should sysmem copy be in converted format?
			if(FAILED(copyStreamWithFormatConversion(Decl,m_ExternalDecl,(BYTE*)pStreamData,
														(BYTE*)m_pBaseSysmemCopy,dwVertexCount)))
			{
				throw InvalidOperationExceptionImpl("Failed to copy vertex stream (with format conversion)");
			}
		}
		else
		{
			memcpy(m_pBaseSysmemCopy,pStreamData,dwVertexCount*m_dwExternalVertexSize);
		}
	}
}

void VertexStreamImpl::BaseWriteLock(void** ppStreamData,DWORD dwVertexCount,DWORD dwVertexSize)
{
	if(!(m_dwFlags&SF_DYNAMIC))
	{
		throw InvalidOperationExceptionImpl("Stream is not dynamic");
	}
	if(m_bIsLocked)
	{
		throw InvalidOperationExceptionImpl("Stream is already locked");
	}
	if(m_dwExternalVertexSize!=dwVertexSize)
	{
		throw InvalidParameterExceptionImpl("Invalid vertex size");
	}
	if(m_dwLength>dwVertexCount)
	{
		throw InvalidParameterExceptionImpl("Attempting to lock invalid vertex count");
	}
	m_bBaseSysmemCopyValid=TRUE;
	m_bDeformerSysmemCopyValid=FALSE;
	m_bIsLocked=TRUE;
	*ppStreamData=m_pBaseSysmemCopy;
	m_bD3DVBValid=FALSE;
}

void VertexStreamImpl::unlock()
{
	if(!m_bIsLocked)
	{
		throw InvalidOperationExceptionImpl("Stream is not locked");
	}
	m_bIsLocked=FALSE;
}

DWORD VertexStreamImpl::getVertexSize()
{
	return m_dwExternalVertexSize;
}

DWORD VertexStreamImpl::getD3DVertexSize()
{
	return m_dwInternalVertexSize;
}

const D3DVERTEXELEMENT9* VertexStreamImpl::getVertexDecl()
{
	return &m_ExternalDecl[0];
}

const D3DVERTEXELEMENT9* VertexStreamImpl::getD3DVertexDecl()
{
	if(!(m_dwFlags&SF_RENDERABLE))
	{
		throw InvalidOperationExceptionImpl("Stream is not renderable");
	}
	return &m_InternalDecl[0];
}

void VertexStreamImpl::getD3DVB(PDIRECT3DVERTEXBUFFER9 *pVB,DWORD *pdwOffset,
								DWORD *pdwVertexCount)
{
	if(!(m_dwFlags&SF_RENDERABLE))
	{
		throw InvalidOperationExceptionImpl("Stream is not renderable");
	}
	if(m_pVB)
	{
		//update if neccessary
		if(!m_bD3DVBValid)
		{
			if(m_eMode==SM_STATIC)
			{
				throw InvalidOperationExceptionImpl("Data invalid in static vb");
			}
			if((!m_bBaseSysmemCopyValid) &&
				(!m_bDeformerSysmemCopyValid))
			{
				throw InvalidOperationExceptionImpl("No valid source data for dynamic vb");
			}
			BYTE *pVBData=NULL;
			if(FAILED(m_pVB->Lock(m_dwOffset*m_dwInternalVertexSize,
				m_dwLength*m_dwInternalVertexSize,(void**)(&pVBData),D3DLOCK_DISCARD)))
			{
				throw InvalidOperationExceptionImpl("Failed to lock vertex buffer!!!");
			}
			DEBUG_STRING("D3D (INFO): Dynamic VB locked with D3DLOCK_DISCARD\n");
			if(m_bDeformerSysmemCopyValid)
			{
				memcpy(pVBData,m_pDeformerSysmemCopy,m_dwLength*m_dwInternalVertexSize);
			}
			else
			{
				memcpy(pVBData,m_pBaseSysmemCopy,m_dwLength*m_dwInternalVertexSize);
			}
			m_pVB->Unlock();
			m_bD3DVBValid=TRUE;
		}
		*pVB=m_pVB;
		m_pVB->AddRef();
		*pdwOffset=m_dwOffset;
		*pdwVertexCount=m_dwLength;
		return;
	}
	if(m_pVBWrapper)
	{
		//first check DiscardID change
		if(m_pVBWrapper->m_dwDiscardID!=m_dwDiscardID)
		{
			m_bD3DVBValid=FALSE;
		}
		//update if neccessary
		if(!m_bD3DVBValid)
		{
			if(m_eMode!=SM_DYNAMIC)
			{
				throw InvalidOperationExceptionImpl("Data invalid in static vb");
			}
			if((!m_bBaseSysmemCopyValid) &&
				(!m_bDeformerSysmemCopyValid))
			{
				throw InvalidOperationExceptionImpl("No valid source data for dynamic vb");
			}
			BYTE *pVBData=NULL;
			if(((m_pVBWrapper->m_dwByteLength-m_pVBWrapper->m_dwBytesUsed)/
				m_dwInternalVertexSize)>m_dwLength)
			{	//discard
				if(FAILED(m_pVBWrapper->m_pVB->Lock(0,m_dwLength*m_dwInternalVertexSize,
					(void**)(&pVBData),D3DLOCK_DISCARD)))
				{
					throw InvalidOperationExceptionImpl("Failed to lock vertex buffer!!!");
				}
				else if(m_bDeformerSysmemCopyValid)
				{
					memcpy(pVBData,m_pDeformerSysmemCopy,m_dwLength*m_dwInternalVertexSize);
				}
				else
				{
					memcpy(pVBData,m_pBaseSysmemCopy,m_dwLength*m_dwInternalVertexSize);
				}
				m_pVBWrapper->m_pVB->Unlock();
				m_bD3DVBValid=TRUE;
				m_pVBWrapper->m_dwBytesUsed=m_dwInternalVertexSize*m_dwLength;
				m_pVBWrapper->m_dwDiscardID=++m_dwDiscardID;
				m_dwOffset=0;
			}
			else
			{										//nooverwrite
				if(FAILED(m_pVBWrapper->m_pVB->Lock(0,m_pVBWrapper->m_dwByteLength,
					(void**)(&pVBData),D3DLOCK_NOOVERWRITE)))
				{
					throw InvalidOperationExceptionImpl("Failed to lock vertex buffer!!!");
				}
				m_dwOffset=(m_pVBWrapper->m_dwBytesUsed/m_dwInternalVertexSize);
				DWORD dwByteOffset=m_dwOffset*m_dwInternalVertexSize;
				if(dwByteOffset<m_pVBWrapper->m_dwBytesUsed)
				{
					m_dwOffset++;
					dwByteOffset+=m_dwInternalVertexSize;
				}
				if(m_bDeformerSysmemCopyValid)
				{
					memcpy(pVBData+dwByteOffset,m_pDeformerSysmemCopy,
						m_dwLength*m_dwInternalVertexSize);
				}
				else
				{
					memcpy(pVBData+dwByteOffset,m_pBaseSysmemCopy,
						m_dwLength*m_dwInternalVertexSize);
				}
				m_pVBWrapper->m_pVB->Unlock();
				m_bD3DVBValid=TRUE;
				m_pVBWrapper->m_dwBytesUsed=dwByteOffset+m_dwLength*m_dwInternalVertexSize;
			}
		}
		*pVB=m_pVBWrapper->m_pVB;
		m_pVBWrapper->m_pVB->AddRef();
		*pdwOffset=m_dwOffset;
		*pdwVertexCount=m_dwLength;
		return;
	}
	throw InvalidOperationExceptionImpl("Target D3D VB does not exist");
}

/*
 *	Index buffer class
 */

IndexBufferImpl::IndexBufferImpl(IBCreateStruct *pCreateStruct,DWORD dwOffset,PDIRECT3DINDEXBUFFER9 pIB)
{
	m_uRefCount=0;
	m_pIB=pIB;
	if(m_pIB)
	{
		m_pIB->AddRef();
	}
	m_pIBWrapper=NULL;
	m_dwSize=pCreateStruct->dwSize;
	m_dwOffset=dwOffset;
	m_dwFlags=pCreateStruct->dwFlags;
	m_bSysmemCopyValid=FALSE;
	m_bUsagePoints=pCreateStruct->bUsagePoints;
	m_Format=pCreateStruct->Format;
	m_bIsLocked=FALSE;
	m_bD3DIBValid=FALSE;
	m_dwDiscardID=0;
	if(m_dwFlags&IBF_LOCKABLE)
	{
		DWORD dwIndexSize=(m_Format==D3DFMT_INDEX16) ? 2 : 4;
		m_pSysmemCopy=(void*)new BYTE[m_dwSize*dwIndexSize];
	}
	else
	{
		m_pSysmemCopy=NULL;
	}
}

IndexBufferImpl::IndexBufferImpl(IBCreateStruct *pCreateStruct,D3D_DynamicIBWrapper *pIBWrapper)
{
	m_uRefCount=0;
	m_pIB=NULL;
	m_pIBWrapper=pIBWrapper;
	if(m_pIBWrapper)
	{
		m_pIBWrapper->m_pIB->AddRef();
	}
	m_dwSize=pCreateStruct->dwSize;
	m_dwOffset=pIBWrapper->m_dwElementsUsed;
	m_dwFlags=pCreateStruct->dwFlags;
	m_bSysmemCopyValid=FALSE;
	m_bUsagePoints=pCreateStruct->bUsagePoints;
	m_Format=pCreateStruct->Format;
	m_bIsLocked=FALSE;
	m_bD3DIBValid=FALSE;
	m_dwDiscardID=0;
	DWORD dwIndexSize=(m_Format==D3DFMT_INDEX16) ? 2 : 4;
	if(m_dwFlags&IBF_LOCKABLE)
	{
		DWORD dwIndexSize=(m_Format==D3DFMT_INDEX16) ? 2 : 4;
		m_pSysmemCopy=(void*)new BYTE[m_dwSize*dwIndexSize];
	}
	else
	{
		m_pSysmemCopy=NULL;
	}
}

IndexBufferImpl::~IndexBufferImpl()
{
	if(m_pIB)
	{
		m_pIB->Release();
		m_pIB=NULL;
	}
}

ULONG IndexBufferImpl::AddRef()
{
	m_uRefCount++;
	return m_uRefCount;
}

ULONG IndexBufferImpl::Release()
{
	m_uRefCount--;
	if(m_uRefCount==0)
	{
		delete this;
		return 0;
	}
	return m_uRefCount;
}

void IndexBufferImpl::loadIB(void* pIBData,DWORD dwIndexCount,D3DFORMAT IndexFmt)
{
	if(dwIndexCount!=m_dwSize)
	{
		throw InvalidParameterExceptionImpl("dwIndexCount parameter does not equal real size");
	}
	if(IndexFmt!=m_Format)
	{
		throw InvalidParameterExceptionImpl("Invalid index data format");
	}
	if(m_dwFlags&IBF_DYNAMIC)
	{
		throw InvalidParameterExceptionImpl("Index buffer is dynamic");
	}
	BYTE *pbData=NULL;
	DWORD dwIndexSize=(m_Format==D3DFMT_INDEX16) ? 2 : 4;
	if(m_dwFlags&IBF_RENDERABLE)
	{
		if(FAILED(m_pIB->Lock(m_dwOffset*dwIndexSize,m_dwSize*dwIndexSize,(void**)&pbData,0)))
		{
			throw InvalidOperationExceptionImpl("Failed to lock index buffer (loadIB)");
		}
		memcpy((void*)pbData,pIBData,dwIndexCount*dwIndexSize);
		m_bD3DIBValid=TRUE;
		m_pIB->Unlock();
	}
	if(m_dwFlags&IBF_LOCKABLE)
	{
		memcpy((void*)pIBData,m_pSysmemCopy,dwIndexCount*dwIndexSize);
	}
}

void IndexBufferImpl::lock(void** ppIBData,DWORD dwIndexCount,D3DFORMAT IndexFmt)
{
	if(!(m_dwFlags&IBF_LOCKABLE))
	{
		throw InvalidOperationExceptionImpl("Index buffer not lockable");
	}
	if(dwIndexCount>m_dwSize)
	{
		throw InvalidParameterExceptionImpl("Trying to perform lock beyond buffer bounds");
	}
	if(m_Format!=IndexFmt)
	{
		throw InvalidParameterExceptionImpl("Invalid index data format");
	}
	m_bSysmemCopyValid=TRUE;
	m_bIsLocked=TRUE;
	m_bD3DIBValid=FALSE;
	*ppIBData=m_pSysmemCopy;
}

void IndexBufferImpl::unlock()
{
	if(!m_bIsLocked)
	{
		throw InvalidOperationExceptionImpl("Index buffer not locked");
	}
	m_bIsLocked=FALSE;
}

DWORD IndexBufferImpl::getIndexSize()
{
	return (m_Format==D3DFMT_INDEX16) ? 2 : 4;
}

void IndexBufferImpl::getD3DIB(PDIRECT3DINDEXBUFFER9 *pIB,DWORD *pdwOffset,DWORD *pdwIndexCount)
{
	if(!(m_dwFlags&IBF_RENDERABLE))
	{
		throw InvalidOperationExceptionImpl("Index buffer not renderable");
	}
	DWORD	dwIndexSize=(m_Format==D3DFMT_INDEX16) ? 2 : 4;
	if(m_pIB)
	{
		if(!m_bD3DIBValid)
		{
			if(!(m_dwFlags&IBF_DYNAMIC))
			{
				throw InvalidOperationExceptionImpl("Index buffer is static");
			}
			if(!m_bSysmemCopyValid)
			{
				throw InvalidOperationExceptionImpl("No valid source data for dynamic index buffer");
			}
			BYTE *pbData=NULL;
			if(FAILED(m_pIB->Lock(m_dwOffset*dwIndexSize,m_dwSize*dwIndexSize,
				(void**)&pbData,D3DLOCK_DISCARD)))
			{
				throw InvalidOperationExceptionImpl("Failed to lock index buffer (loadIB)");
			}
			memcpy((void*)pbData,m_pSysmemCopy,m_dwSize*dwIndexSize);
			m_bD3DIBValid=TRUE;
			m_pIB->Unlock();
		}
		*pIB=m_pIB;
		m_pIB->AddRef();
		*pdwOffset=m_dwOffset;
		*pdwIndexCount=m_dwSize;
		return;
	}
	if(m_pIBWrapper)
	{
		//first check DiscardID change
		if(m_pIBWrapper->m_dwDiscardID!=m_dwDiscardID)
		{
			m_bD3DIBValid=FALSE;
		}
		//update if neccessary
		if(!m_bD3DIBValid)
		{
			if(!(m_dwFlags&IBF_DYNAMIC))
			{
				throw InvalidOperationExceptionImpl("Index buffer is static");
			}
			if(!m_bSysmemCopyValid)
			{
				throw InvalidOperationExceptionImpl("No valid source data for dynamic index buffer");
			}
			BYTE *pbData=NULL;
			if((m_pIBWrapper->m_dwElementLenghth-m_pIBWrapper->m_dwElementsUsed)>m_dwSize)
			{ //discard
				if(FAILED(m_pIBWrapper->m_pIB->Lock(0,m_dwSize*dwIndexSize,(void**)&pbData,D3DLOCK_DISCARD)))
				{
					throw InvalidOperationExceptionImpl("Failed to lock index buffer (loadIB)");
				}
				memcpy((void*)pbData,m_pSysmemCopy,m_dwSize*dwIndexSize);
				m_pIBWrapper->m_pIB->Unlock();
				m_bD3DIBValid=TRUE;
				m_pIBWrapper->m_dwElementsUsed=m_dwSize;
				m_pIBWrapper->m_dwDiscardID=++m_dwDiscardID;
				m_dwOffset=0;
			}
			else
			{	//nooverwrite
				if(FAILED(m_pIBWrapper->m_pIB->Lock(0,m_dwSize*dwIndexSize,(void**)&pbData,D3DLOCK_NOOVERWRITE)))
				{
					throw InvalidOperationExceptionImpl("Failed to lock index buffer (loadIB)");
				}
				m_dwOffset=m_pIBWrapper->m_dwElementsUsed;
				memcpy((void*)(pbData+m_dwSize*dwIndexSize),m_pSysmemCopy,m_dwSize*dwIndexSize);
				m_pIBWrapper->m_pIB->Unlock();
				m_bD3DIBValid=TRUE;
				m_pIBWrapper->m_dwElementsUsed+=m_dwSize;
			}
		}
		//fill ib data
		*pIB=m_pIBWrapper->m_pIB;
		m_pIBWrapper->m_pIB->AddRef();
		*pdwOffset=m_dwOffset;
		*pdwIndexCount=m_dwSize;
		return;
	}
	throw InvalidOperationExceptionImpl("Target D3D IB does not exist");
}

/*
 *	D3D continued
 */

#define STATIC_VB_SIZE			2000000
#define DYNAMIC_VB_SIZE			5000000		//guarantees that anything will fit
#define STATIC_VB_POOL			D3DPOOL_MANAGED

void D3DImpl::createVertexBuffer(VBCreateStruct *pCS,
								 AutoPtr<VertexBuffer> &pBuffer)
{
	DWORD dwI;
	VertexStream *StreamTbl[MAX_VERTEX_STREAMS];
	for(dwI=0;dwI<MAX_VERTEX_STREAMS;dwI++)
	{
		StreamTbl[dwI]=NULL;
	}
	for(dwI=0;dwI<pCS->dwStreamCount;dwI++)
	{
		if(pCS->streams[dwI].eMode==SM_STATIC)
		{	//check for declaration changes
			DWORD dwJ=0;
			while(pCS->streams[dwI].ExternalDecl[dwJ].Stream!=0xff)
			{
				memcpy(&pCS->streams[dwI].InternalDecl[dwJ],
					&pCS->streams[dwI].ExternalDecl[dwJ],sizeof(D3DVERTEXELEMENT9));
				//check what cannot be done in hardware
				if((pCS->streams[dwI].ExternalDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_2) &&
					!(m_DevCaps.DeclTypes&D3DDTCAPS_FLOAT16_2))
				{
					pCS->streams[dwI].InternalDecl[dwJ].Type=D3DDECLTYPE_FLOAT2;
				}
				else if((pCS->streams[dwI].ExternalDecl[dwJ].Type==D3DDECLTYPE_FLOAT16_4) &&
					!(m_DevCaps.DeclTypes&D3DDTCAPS_FLOAT16_4))
				{
					pCS->streams[dwI].InternalDecl[dwJ].Type=D3DDECLTYPE_FLOAT4;
				}
				else if((pCS->streams[dwI].ExternalDecl[dwJ].Type==D3DDECLTYPE_DEC3N) &&
					!(m_DevCaps.DeclTypes&D3DDTCAPS_DEC3N))
				{
					pCS->streams[dwI].InternalDecl[dwJ].Type=D3DDECLTYPE_FLOAT3;
				}
				dwJ++;
			}
			memcpy(&pCS->streams[dwI].InternalDecl[dwJ],
				&pCS->streams[dwI].ExternalDecl[dwJ],sizeof(D3DVERTEXELEMENT9));
			//recalculate component offsets in internal declaration
			dwJ=0;
			DWORD dwOffset=0;
			while(pCS->streams[dwI].InternalDecl[dwJ].Stream!=0xff)
			{
				pCS->streams[dwI].InternalDecl[dwJ].Offset=(BYTE)dwOffset;
				switch(pCS->streams[dwI].InternalDecl[dwJ].Type)
				{
					case D3DDECLTYPE_FLOAT3:
						dwOffset+=12;
						break;
					case D3DDECLTYPE_FLOAT2:
					case D3DDECLTYPE_FLOAT16_4:
					case D3DDECLTYPE_SHORT4:
					case D3DDECLTYPE_SHORT4N:
					case D3DDECLTYPE_USHORT4N:
						dwOffset+=8;
						break;
					case D3DDECLTYPE_FLOAT4:
						dwOffset+=16;
						break;
					default:
						dwOffset+=4;
						break;
				}
				dwJ++;
			}
		}
		else
		{	//just copy external to internal
			memcpy(&pCS->streams[dwI].InternalDecl,
				&pCS->streams[dwI].ExternalDecl,
				sizeof(D3DVERTEXELEMENT9)*MAX_STREAM_DECL_COMPONENTS);
		}
		//determine usage
		DWORD dwUsage=D3DUSAGE_WRITEONLY;
		if(pCS->bUsagePoints)
		{
			dwUsage|=D3DUSAGE_POINTS;
		}
		if(pCS->streams[dwI].eMode==SM_DYNAMIC)
		{
			dwUsage|=D3DUSAGE_DYNAMIC;
		}
		if(pCS->streams[dwI].eMode==SM_DYNAMIC_MULTIINST)
		{
			dwUsage|=D3DUSAGE_DYNAMIC;
		}
		D3DPOOL Pool=STATIC_VB_POOL;
		if(dwUsage&D3DUSAGE_DYNAMIC)
		{
			Pool=D3DPOOL_DEFAULT;
		}
		//create
		if(pCS->streams[dwI].dwFlags&SF_RENDERABLE)
		{
			PDIRECT3DVERTEXBUFFER9 pVB=NULL;
			HANDLE	hTmp=0;
			if(FAILED(m_pD3DDevice->CreateVertexBuffer(
				pCS->dwSize*getVertexSizeFromDecl(&pCS->streams[dwI].InternalDecl[0]),
				dwUsage,0,Pool,&pVB,NULL)))
			{
				throw InvalidOperationExceptionImpl("Failed to create vertex buffer");
			}
#ifdef _DEBUG
			{
				DEBUG_STRING("D3D (INFO): Created vertex buffer\n");
				char szDebugMsg[256];
				sprintf(szDebugMsg,"\tSize: %d bytes\n",
					pCS->dwSize*getVertexSizeFromDecl(&pCS->streams[dwI].InternalDecl[0]));
				DEBUG_STRING(szDebugMsg);
				if(pCS->bUsagePoints)
				{
					DEBUG_STRING("\tD3DUSAGE_POINTS\n");
				}
				if(dwUsage&D3DUSAGE_DYNAMIC)
				{
					DEBUG_STRING("\tD3DUSAGE_DYNAMIC\n");
				}
				if(Pool==D3DPOOL_DEFAULT)
				{
					DEBUG_STRING("\tPool: DEFAULT\n");
				}
				else if(Pool==D3DPOOL_MANAGED)
				{
					DEBUG_STRING("\tPool: MANAGED\n");
				}
			}
#endif
			StreamTbl[dwI]=new VertexStreamImpl(&pCS->streams[dwI],pCS->dwSize,0,dwUsage,pVB);
			m_VBTbl.addElement(pVB);
		}
		else
		{
			StreamTbl[dwI]=new VertexStreamImpl(&pCS->streams[dwI],pCS->dwSize,0,dwUsage,NULL);
		}
	}
	VertexBuffer *pVirtualVB=new VertexBufferImpl(pCS,&(StreamTbl[0]));
	pVirtualVB->AddRef();
	//send it back to caller
	pBuffer=pVirtualVB;
}

void D3DImpl::createIndexBuffer(IBCreateStruct *pCS,
								AutoPtr<IndexBuffer> &pBuffer)
{
	//determine usage
	DWORD dwUsage=D3DUSAGE_WRITEONLY;
	if(pCS->bUsagePoints)
	{
		dwUsage|=D3DUSAGE_POINTS;
	}
	if(pCS->dwFlags&IBF_DYNAMIC)
	{
		dwUsage|=D3DUSAGE_DYNAMIC;
	}
	D3DPOOL Pool=D3DPOOL_MANAGED;
	if(dwUsage&D3DUSAGE_DYNAMIC)
	{
		Pool=D3DPOOL_DEFAULT;
	}
	//create
	if(pCS->dwFlags&IBF_RENDERABLE)
	{
		PDIRECT3DINDEXBUFFER9 pIB=NULL;
		if(FAILED(m_pD3DDevice->CreateIndexBuffer(
			pCS->dwSize*((pCS->Format==D3DFMT_INDEX16) ? 2 : 4),
			dwUsage,pCS->Format,Pool,&pIB,NULL)))
		{
			throw InvalidOperationExceptionImpl("Failed to index vertex buffer");
		}
		IndexBuffer *pVirtualIB=new IndexBufferImpl(pCS,0,pIB);
		m_IBTbl.addElement(pIB);
		pVirtualIB->AddRef();
		pBuffer=pVirtualIB;
	}
	else
	{
		IndexBuffer *pVirtualIB=new IndexBufferImpl(pCS,0,NULL);
		pVirtualIB->AddRef();
		pBuffer=pVirtualIB;
	}
}

void D3DImpl::createVertexDecl(const D3DVERTEXELEMENT9 *pVertexElements,
							 IDirect3DVertexDeclaration9** ppDecl)
{
	if(FAILED(m_pD3DDevice->CreateVertexDeclaration(pVertexElements,ppDecl)))
	{
		throw InvalidOperationExceptionImpl(
			"Failed to create vertex declaration");
	}
}

void D3DImpl::createDynamicVB(DWORD dwFlags)
{
	dwFlags&=D3DUSAGE_POINTS;
	dwFlags|=D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
	if(dwFlags&D3DUSAGE_POINTS)
	{
		if(!(m_DynamicHVPPointVB.m_pVB))
		{
			if(FAILED(m_pD3DDevice->CreateVertexBuffer(DYNAMIC_VB_SIZE,
				dwFlags,0,D3DPOOL_DEFAULT,&m_DynamicHVPPointVB.m_pVB,
				NULL)))
			{
				throw InvalidOperationExceptionImpl("Failed to create vertex buffer!!!");
			}
			m_DynamicHVPPointVB.m_dwDiscardID=0;
			m_DynamicHVPPointVB.m_dwBytesUsed=0;
			m_DynamicHVPPointVB.m_dwByteLength=DYNAMIC_VB_SIZE;
		}
		return;
	}
	if(!(m_DynamicHVPNormalVB.m_pVB))
	{
		if(FAILED(m_pD3DDevice->CreateVertexBuffer(DYNAMIC_VB_SIZE,
			dwFlags,0,D3DPOOL_DEFAULT,&m_DynamicHVPNormalVB.m_pVB,
			NULL)))
		{
			throw InvalidOperationExceptionImpl("Failed to create vertex buffer!!!");
		}
		m_DynamicHVPNormalVB.m_dwDiscardID=0;
		m_DynamicHVPNormalVB.m_dwBytesUsed=0;
		m_DynamicHVPNormalVB.m_dwByteLength=DYNAMIC_VB_SIZE;
	}
}

void D3DImpl::setViewMatrix(D3DXMATRIXA16 *pViewMatrix)
{
	m_mtxView=*pViewMatrix;
	m_bViewTransformChanged=TRUE;
}

void D3DImpl::applyViewMatrix(bool bForce)
{
	if(m_bViewTransformChanged)
	{
		m_pD3DDevice->SetTransform(D3DTS_VIEW,&m_mtxView);
		m_bViewTransformChanged=false;
		return;
	}
	if(bForce)
	{
		m_pD3DDevice->SetTransform(D3DTS_VIEW,&m_mtxView);
	}
}

void D3DImpl::getViewMatrix(D3DXMATRIXA16 *pViewMatrix)
{
	*pViewMatrix=m_mtxView;
}

void D3DImpl::setWorldMatrix(D3DXMATRIXA16 *pWorldMatrix)
{
	m_mtxWorld=*pWorldMatrix;
	m_bWorldTransformChanged=TRUE;
}

void D3DImpl::applyWorldMatrix(bool bForce)
{
	if(m_bWorldTransformChanged)
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);
		m_bWorldTransformChanged=FALSE;
		return;
	}
	if(bForce)
	{
		m_pD3DDevice->SetTransform(D3DTS_WORLD,&m_mtxWorld);
	}
}

void D3DImpl::getWorldMatrix(D3DXMATRIXA16 *pWorldMatrix)
{
	*pWorldMatrix=m_mtxWorld;
}

void D3DImpl::setProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix)
{
	m_mtxProjection=*pProjectionMatrix;
	m_bProjectionTransformChanged=true;
}

void D3DImpl::applyProjectionMatrix(bool bForce)
{
	if(m_bProjectionTransformChanged)
	{
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);
		m_bProjectionTransformChanged=false;
		return;
	}
	if(bForce)
	{
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION,&m_mtxProjection);
	}
}

void D3DImpl::getProjectionMatrix(D3DXMATRIXA16 *pProjectionMatrix)
{
	*pProjectionMatrix=m_mtxProjection;
}

void D3DImpl::drawVB(VertexBuffer *pVB,IndexBuffer *pIB,
					 PRIMITIVE_TYPE eType,DWORD dwPrimitiveCount)
{
	if(!pVB)
	{
		throw InvalidParameterExceptionImpl("pVB is NULL");
	}
	if(!pIB)
	{
		throw InvalidParameterExceptionImpl("pIB is NULL");
	}
	DWORD dwStreamCount=0;
	DWORD dwI=0;
	IDirect3DVertexDeclaration9 *pDecl=NULL;
	PDIRECT3DINDEXBUFFER9 pD3DIB=NULL;
	DWORD dwIndexOffset=0;
	DWORD dwIndexCount=0;
	DWORD dwIndexSize=0;
	DWORD dwStreamOffset=0;
	DWORD dwVertexCount=0;
	switch(eType)
	{
		case PT_INDEXED_TRIANGLES:
			dwStreamCount=pVB->getStreamCount();
			for(dwI=0;dwI<dwStreamCount;dwI++)
			{
				AutoPtr<VertexStream> pStream;
				pVB->getVertexStream(dwI,pStream);
				PDIRECT3DVERTEXBUFFER9 pD3DVB=NULL;
				dwStreamOffset=0;
				dwVertexCount=0;
				pStream->getD3DVB(&pD3DVB,&dwStreamOffset,&dwVertexCount);
				DWORD dwStreamVertexSize=0;
				dwStreamVertexSize=pStream->getD3DVertexSize();
				if(dwStreamCount==1)
				{
					m_pD3DDevice->SetStreamSource(dwI,pD3DVB,0,dwStreamVertexSize);
				}
				else
				{
					m_pD3DDevice->SetStreamSource(dwI,pD3DVB,
						dwStreamOffset*dwStreamVertexSize,dwStreamVertexSize);
				}
				pD3DVB->Release();
			}
			//TODO: better stream clearing
			for(dwI=dwStreamCount;dwI<MAX_VERTEX_STREAMS;dwI++)
			{
				m_pD3DDevice->SetStreamSource(dwI,NULL,0,0);
			}
			pVB->getD3DStreamDecl(&pDecl);
			m_pD3DDevice->SetVertexDeclaration(pDecl);
			pDecl->Release();
			pIB->getD3DIB(&pD3DIB,&dwIndexOffset,&dwIndexCount);
			dwIndexSize=pIB->getIndexSize();
			m_pD3DDevice->SetIndices(pD3DIB);
			pD3DIB->Release();
			if(dwStreamCount==1)
			{
				if(FAILED(m_pD3DDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,dwStreamOffset,0,dwVertexCount,
					dwIndexOffset,dwPrimitiveCount)))
				{
					throw InvalidOperationExceptionImpl("DrawIndexedPrimitive failed");
				}
			}
			else
			{
				if(FAILED(m_pD3DDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,0,0,dwVertexCount,
					dwIndexOffset,dwPrimitiveCount)))
				{
					throw InvalidOperationExceptionImpl("DrawIndexedPrimitive failed");
				}
			}
			break;
		case PT_POINTS:
			throw InvalidOperationExceptionImpl("Point primitive not implemented yet");
			break;
		default:
			throw InvalidOperationExceptionImpl("Unknown primitive type");
			break;
	}
}

//TODO: it might be good idea to look for failure and throw an exception BUT:
//a) need to check in D3D spec if SRT can fail at all
//b) need to add exception handling code around all calls to this method
void D3DImpl::setRenderState(D3DRENDERSTATETYPE eState,DWORD dwValue)
{
	m_pD3DDevice->SetRenderState(eState,dwValue);
}

//TODO: same note as in setRenderState
void D3DImpl::setSamplerState(DWORD dwSampler,D3DSAMPLERSTATETYPE eType,DWORD dwValue)
{
	m_pD3DDevice->SetSamplerState(dwSampler,eType,dwValue);
}

//TODO: same note as in setRenderState
void D3DImpl::setMaterial(D3DMATERIAL9 *pMaterial)
{
	m_pD3DDevice->SetMaterial(pMaterial);
}

//TODO: same note as in setRenderState
void D3DImpl::setLight(DWORD dwLightIndex,D3DLIGHT9 *pLight)
{
	m_pD3DDevice->SetLight(dwLightIndex,pLight);
}

void D3DImpl::setLightEnable(DWORD dwLightIndex,bool bEnable)
{
	if(FAILED(m_pD3DDevice->LightEnable(dwLightIndex,bEnable ? TRUE : FALSE)))
	{
		throw InvalidParameterExceptionImpl("Invalid light index");
	}
}

/*
 *	Rendertarget manager
 */

void D3DImpl::precacheRenderTargets(DWORD dwWidth,DWORD dwHeight,D3DFORMAT eFormat,
										 bool bCubeTexture,DWORD dwCount)
{
	if((m_dwRTPoolSize+dwCount)>D3D_RT_POOL_SIZE)
	{
		dwCount=D3D_RT_POOL_SIZE-m_dwRTPoolSize;
	}
	if(dwCount==0)
	{
		return;
	}
	if(bCubeTexture)
	{
		if(dwWidth!=dwHeight)
		{
			//TODO: really?
			throw InvalidOperationExceptionImpl("D3D (ERROR): cube map faces must be square\n");
		}
	}
	D3DDISPLAYMODE mode;
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&mode);
	HRESULT hr;
	if(bCubeTexture)
	{
		hr=m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,s_DevType,mode.Format,
			D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP,
			D3DRTYPE_CUBETEXTURE,eFormat);
	}
	else
	{
		hr=m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,s_DevType,mode.Format,
			D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP,
			D3DRTYPE_TEXTURE,eFormat);
	}
	if(FAILED(hr))
	{
		throw InvalidOperationExceptionImpl("D3D (ERROR): Requested format cannot be used\n"
			"as rendertarget on this device\n");
	}
	//temporarily disable autogenmipmaps
	if(m_bDisableAutoMipMaps)
	{
		hr=D3DOK_NOAUTOGEN;
	}
	if(hr==D3DOK_NOAUTOGEN)
	{
		DWORD dwI=0;
		for(dwI=0;dwI<dwCount;dwI++)
		{
			D3D_RTDesc RTDesc;
			ZeroMemory(&RTDesc,sizeof(D3D_RTDesc));
			if(bCubeTexture)
			{
				hr=m_pD3DDevice->CreateCubeTexture(dwWidth,1,D3DUSAGE_RENDERTARGET,
					eFormat,D3DPOOL_DEFAULT,&(RTDesc.m_pCubeTexture),NULL);
				RTDesc.m_bCube=TRUE;
			}
			else
			{
				hr=m_pD3DDevice->CreateTexture(dwWidth,dwHeight,1,D3DUSAGE_RENDERTARGET,
					eFormat,D3DPOOL_DEFAULT,&(RTDesc.m_pTexture),NULL);
				RTDesc.m_bCube=FALSE;
			}
			if(FAILED(hr))
			{
				throw InvalidOperationExceptionImpl("D3D (ERROR): Failed to create rendertarget\n");
			}
			else
			{
				RTDesc.m_bLocked=FALSE;
				RTDesc.m_dwHeight=dwHeight;
				RTDesc.m_dwWidth=dwWidth;
				RTDesc.m_dwLockDuration=0;
				RTDesc.m_dwRandomSeed=m_dwCurrentRTSeed++;
				RTDesc.m_Format=eFormat;
				RTDesc.m_pRTSurface=NULL;
				RTDesc.m_bBackbuffer=FALSE;
				m_RTPool[m_dwRTPoolSize++]=RTDesc;
			}
		}
	}
	else
	{
		DWORD dwI=0;
		for(dwI=0;dwI<dwCount;dwI++)
		{
			D3D_RTDesc RTDesc;
			ZeroMemory(&RTDesc,sizeof(D3D_RTDesc));
			if(bCubeTexture)
			{
				hr=m_pD3DDevice->CreateCubeTexture(dwWidth,0,
					D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP,
					eFormat,D3DPOOL_DEFAULT,&(RTDesc.m_pCubeTexture),NULL);
				RTDesc.m_bCube=TRUE;
			}
			else
			{
				hr=m_pD3DDevice->CreateTexture(dwWidth,dwHeight,0,
					D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP,
					eFormat,D3DPOOL_DEFAULT,&(RTDesc.m_pTexture),NULL);
				RTDesc.m_bCube=FALSE;
			}
			if(FAILED(hr))
			{
				throw InvalidOperationExceptionImpl("D3D (ERROR): Failed to create rendertarget\n");
			}
			else
			{
				RTDesc.m_bLocked=FALSE;
				RTDesc.m_dwHeight=dwHeight;
				RTDesc.m_dwWidth=dwWidth;
				RTDesc.m_dwLockDuration=0;
				RTDesc.m_dwRandomSeed=m_dwCurrentRTSeed++;
				RTDesc.m_Format=eFormat;
				RTDesc.m_pRTSurface=NULL;
				RTDesc.m_bBackbuffer=FALSE;
				m_RTPool[m_dwRTPoolSize++]=RTDesc;
			}
		}
	}
}

void D3DImpl::precacheDepthStencils(DWORD dwWidth,DWORD dwHeight,DWORD dwCount)
{
	if((m_dwDSPoolSize+dwCount)>D3D_DS_POOL_SIZE)
	{
		dwCount=D3D_DS_POOL_SIZE-m_dwDSPoolSize;
	}
	if(dwCount==0)
	{
		return;
	}
	DWORD dwI=0;
	for(dwI=0;dwI<dwCount;dwI++)
	{
		D3D_DSDesc DSDesc;
		ZeroMemory(&DSDesc,sizeof(D3D_DSDesc));
		HRESULT hr=m_pD3DDevice->CreateDepthStencilSurface(dwWidth,dwHeight,
			D3DFMT_D24S8,D3DMULTISAMPLE_NONE,0,FALSE,&DSDesc.m_pDSSurface,
			NULL);
		if(FAILED(hr))
		{
			 throw InvalidOperationExceptionImpl("Failed to create DepthStencil surface\n");
		}
		DSDesc.m_bLocked=FALSE;
		DSDesc.m_dwHeight=dwHeight;
		DSDesc.m_dwWidth=dwWidth;
		DSDesc.m_Format=D3DFMT_D24S8;
		DSDesc.m_dwRandomSeed=m_dwCurrentRTSeed++;
		m_DSPool[m_dwDSPoolSize++]=DSDesc;
	}
}

DWORD D3DImpl::getCurrentRenderTarget()
{
	return m_dwCurrentRT;
}

DWORD D3DImpl::getCurrentDepthStencil()
{
	return m_dwCurrentDS;
}

void D3DImpl::getCurrentRenderTargetSizes(DWORD *pdwWidth,DWORD *pdwHeight)
{
	*pdwWidth=m_RTPool[m_dwCurrentRT].m_dwWidth;
	*pdwHeight=m_RTPool[m_dwCurrentRT].m_dwHeight;
}

/*D3DFORMAT D3DImpl::getCurrentRenderTargetFormat()
{
	return m_RTPool[m_dwCurrentRT].m_Format;
}*/

/*void D3DImpl::getCurrentDepthStencilSizes(DWORD *pdwWidth,DWORD *pdwHeight)
{
	*pdwWidth=m_DSPool[m_dwCurrentDS].m_dwWidth;
	*pdwHeight=m_DSPool[m_dwCurrentDS].m_dwHeight;
}*/

/*D3DFORMAT D3DImpl::getCurrentDepthStencilFormat()
{
	return m_DSPool[m_dwCurrentDS].m_Format;
}*/

//assumes SetRT calls Release() on previous RT and AddRef() on new RT
void D3DImpl::setRenderTarget(DWORD dwRTHandle,DWORD dwCubeFace)
{
	if(dwRTHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid rendertarget handle");
	}
	PDIRECT3DSURFACE9 pTexLevel=NULL;
	if(m_RTPool[dwRTHandle].m_bCube)
	{
		if(FAILED(m_RTPool[dwRTHandle].m_pCubeTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)dwCubeFace,0,&pTexLevel)))
		{
			throw InvalidOperationExceptionImpl("Failed to get cube texture surface level");
		}
	}
	else
	{
#ifdef _DEBUG
		if(dwCubeFace!=0)
		{
			DEBUG_STRING("D3D (WARN): dwCubeFace!=0 on setting 2D rendertaget\n");
		}
#endif
		if(dwRTHandle==0)
		{
			pTexLevel=m_RTPool[dwRTHandle].m_pRTSurface;
			pTexLevel->AddRef();	//just for consistency
		}
		else if(FAILED(m_RTPool[dwRTHandle].m_pTexture->GetSurfaceLevel(0,&pTexLevel)))
		{
			throw InvalidOperationExceptionImpl("Failed to get texture surface level");
		}
	}
	if(FAILED(m_pD3DDevice->SetRenderTarget(0,pTexLevel)))
	{
		pTexLevel->Release();
		throw InvalidOperationExceptionImpl("SetRenderTarget failed");
	}
	m_dwCurrentRT=dwRTHandle;
	pTexLevel->Release();
}

void D3DImpl::setDepthStencil(DWORD dwDSHandle)
{
	if(dwDSHandle>=m_dwDSPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid depthstencil surface handle");
	}
	if(FAILED(m_pD3DDevice->SetDepthStencilSurface(m_DSPool[dwDSHandle].m_pDSSurface)))
	{
		throw InvalidOperationExceptionImpl("SetDepthStencilSurface failed");
	}
	m_dwCurrentDS=dwDSHandle;
}

//get by handle ignores any locks and assumes caller WILL/DID CHECK THE SEED!!!
void D3DImpl::getRenderTargetByHandle(DWORD dwHandle,PDIRECT3DTEXTURE9 *pRTTexture)
{
	if(dwHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid rendertarget handle");
	}
	//just to be sure fail if called for backbuffer surface
	if(dwHandle==0)
	{
		throw InvalidOperationExceptionImpl("This operation is not allowed on backbuffer surface");
	}
	*pRTTexture=m_RTPool[dwHandle].m_pTexture;
	(*pRTTexture)->AddRef();
}

/*HRESULT	D3DImpl::getDepthStencilByHandle(DWORD dwHandle,PDIRECT3DSURFACE9 *ppDSSurface)
{
	if(dwHandle>=m_dwDSPoolSize)
	{
		return E_FAIL;
	}
	*ppDSSurface=m_DSPool[dwHandle].m_pDSSurface;
	(*ppDSSurface)->AddRef();
	return S_OK;
}*/

void D3DImpl::getRenderTarget(DWORD dwWidth,DWORD dwHeight,D3DFORMAT eFormat,
									bool bCubeTexture,PDIRECT3DTEXTURE9 *pRTTexture,
									DWORD *pdwHandle)
{
	DWORD dwI=0;
	DWORD dwMinFreeID=0xffffffff;
	DWORD dwMinFreeIndex=0xffffffff;
	for(dwI=1;dwI<m_dwRTPoolSize;dwI++)
	{
		if((!m_RTPool[dwI].m_bLocked) &&
			(m_RTPool[dwI].m_bCube==bCubeTexture) &&
			(m_RTPool[dwI].m_dwHeight==dwHeight) &&
			(m_RTPool[dwI].m_dwWidth==dwWidth) &&
			(m_RTPool[dwI].m_Format==eFormat) &&
			(!m_RTPool[dwI].m_bBackbuffer))
		{
			if(m_RTPool[dwI].m_dwRandomSeed<dwMinFreeID)
			{
				dwMinFreeID=m_RTPool[dwI].m_dwRandomSeed;
				dwMinFreeIndex=dwI;
			}
		}
	}
	if(dwMinFreeIndex!=0xffffffff)
	{
		m_RTPool[dwMinFreeIndex].m_dwRandomSeed=m_dwCurrentRTSeed++;
		*pRTTexture=m_RTPool[dwMinFreeIndex].m_pTexture;
		(*pRTTexture)->AddRef();
		*pdwHandle=dwMinFreeIndex;
	}
	else
	{
		//create new render target
#ifdef _DEBUG
		char szMsg[1024];
		sprintf(szMsg,"D3D (WARN): Forced to create new rendeterget on the fly (%dx%dx%d)!!!\n",
			bCubeTexture ? 6 : 1,dwWidth,dwHeight);
		DEBUG_STRING(szMsg);
#endif
		precacheRenderTargets(dwWidth,dwHeight,eFormat,bCubeTexture,1);
		m_RTPool[m_dwRTPoolSize-1].m_dwRandomSeed=m_dwCurrentRTSeed++;
		*pRTTexture=m_RTPool[m_dwRTPoolSize-1].m_pTexture;
		(*pRTTexture)->AddRef();
		*pdwHandle=m_dwRTPoolSize-1;
	}
}

void D3DImpl::getDepthStencil(DWORD dwWidth,DWORD dwHeight,
							  PDIRECT3DSURFACE9 *ppDSSurface,DWORD *pdwHandle)
{
	DWORD dwI=0;
	DWORD dwMinFreeID=0xffffffff;
	DWORD dwMinFreeIndex=0xffffffff;
	for(dwI=0;dwI<m_dwDSPoolSize;dwI++)
	{
		if((!m_DSPool[dwI].m_bLocked) &&
			(m_DSPool[dwI].m_dwHeight==dwHeight) &&
			(m_DSPool[dwI].m_dwWidth==dwWidth))
		{
			if(m_DSPool[dwI].m_dwRandomSeed<dwMinFreeID)
			{
				dwMinFreeID=m_DSPool[dwI].m_dwRandomSeed;
				dwMinFreeIndex=dwI;
			}
		}
	}
	if(dwMinFreeIndex!=0xffffffff)
	{
		*ppDSSurface=m_DSPool[dwMinFreeIndex].m_pDSSurface;
		(*ppDSSurface)->AddRef();
		*pdwHandle=dwMinFreeIndex;
		return;
	}
	//create new depth stencil surface
#ifdef _DEBUG
	char szMsg[1024];
	sprintf(szMsg,"D3D (WARN): Forced to create new depthstencil surface on the fly (%dx%d)!!!\n",
		dwWidth,dwHeight);
	DEBUG_STRING(szMsg);
#endif
	precacheDepthStencils(dwWidth,dwHeight,1);
	m_DSPool[m_dwDSPoolSize-1].m_dwRandomSeed=m_dwCurrentRTSeed++;
	*ppDSSurface=m_DSPool[m_dwDSPoolSize-1].m_pDSSurface;
	(*ppDSSurface)->AddRef();
	*pdwHandle=m_dwDSPoolSize-1;
}

DWORD D3DImpl::getRenderTargetSeed(DWORD dwHandle)
{
	if(dwHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid rendertarget handle");
	}
	//just to be sure fail if called for backbuffer surface
	if(dwHandle==0)
	{
		throw InvalidOperationExceptionImpl("This operation is not allowed on backbuffer surface");
	}
	return m_RTPool[dwHandle].m_dwRandomSeed;
}

/*HRESULT D3DImpl::changeRenderTargetSeed(DWORD dwHandle)
{
	if(dwHandle>=m_dwRTPoolSize)
	{
		return E_FAIL;
	}
	//just to be sure fail if called for backbuffer surface
	if(dwHandle==0)
	{
		return E_FAIL;
	}
	m_RTPool[dwHandle].m_dwRandomSeed=m_dwCurrentRTSeed++;
	return S_OK;
}*/

void D3DImpl::lockRenderTarget(DWORD dwRTHandle,DWORD dwLockDuration)
{
	if(dwRTHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid rendertarget handle");
	}
	//we can't lock anything twice
	if(m_RTPool[dwRTHandle].m_bLocked)
	{
		throw InvalidOperationExceptionImpl("Rendertarget already locked");
	}
	//we can't lock backbuffer surface (it makes no sense
	//- it is not texture so it cannot be returned by getRT call)
	if(dwRTHandle==0)
	{
		throw InvalidOperationExceptionImpl("This operation is not allowed on backbuffer surface");
	}
	m_RTPool[dwRTHandle].m_bLocked=TRUE;
	m_RTPool[dwRTHandle].m_dwLockDuration=dwLockDuration;
}

void D3DImpl::unlockRenderTarget(DWORD dwRTHandle)
{
	if(dwRTHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid rendertarget handle");
	}
	if(!m_RTPool[dwRTHandle].m_bLocked)
	{
		throw InvalidOperationExceptionImpl("Rendertarget not locked");
	}
	m_RTPool[dwRTHandle].m_bLocked=FALSE;
	m_RTPool[dwRTHandle].m_dwLockDuration=0;
}

void D3DImpl::lockDepthStencil(DWORD dwDSHandle)
{
	if(dwDSHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid depthstencil surface handle");
	}
	//we can't lock anything twice
	if(m_DSPool[dwDSHandle].m_bLocked)
	{
		throw InvalidOperationExceptionImpl("Depthstencil surface already locked");
	}
	m_DSPool[dwDSHandle].m_bLocked=TRUE;
}

void D3DImpl::unlockDepthStencil(DWORD dwDSHandle)
{
	if(dwDSHandle>=m_dwRTPoolSize)
	{
		throw InvalidParameterExceptionImpl("Invalid depthstencil surface handle");
	}
	if(!m_DSPool[dwDSHandle].m_bLocked)
	{
		throw InvalidOperationExceptionImpl("Depthstencil surface not locked");
	}
	m_DSPool[dwDSHandle].m_bLocked=FALSE;
}

bool D3DImpl::isRenderTargetLocked(DWORD dwRTHandle)
{
	if(dwRTHandle>=m_dwRTPoolSize)
	{
		return true;
	}
	return m_RTPool[dwRTHandle].m_bLocked;
}

/*HRESULT D3DImpl::checkRenderTargetLockDuration(DWORD dwHandle,DWORD *pdwLockDuration)
{
	if(dwHandle>=m_dwRTPoolSize)
	{
		return E_FAIL;
	}
	if(m_RTPool[dwHandle].m_bLocked)
	{
		*pdwLockDuration=m_RTPool[dwHandle].m_dwLockDuration;
	}
	else
	{
		//TODO: should probably put some assert here
		*pdwLockDuration=0;
	}
	return S_OK;
}*/

/*HRESULT D3DImpl::incRenderTargetLockDuration(DWORD dwHandle,DWORD dwIncValue)
{
	if(dwHandle>=m_dwRTPoolSize)
	{
		return E_FAIL;
	}
	m_RTPool[dwHandle].m_dwLockDuration+=dwIncValue;
	return S_OK;
}*/

void D3DImpl::getFeedbackTextureHandle(DWORD &dwRTHandle)
{
	dwRTHandle=m_dwLastFrameRTHandle;
	m_bLastFrameFeedbackRequest=TRUE;
}

void D3DImpl::getScreenRTTextureSizes(DWORD *pdwWidth,DWORD *pdwHeight)
{
	*pdwWidth=m_dwScreenRTTextureX;
	*pdwHeight=m_dwScreenRTTextureY;
}

void D3DImpl::beginFrame(D3DCOLOR ClearCol,bool bGenFeedbackTexture)
{
	if(bGenFeedbackTexture || m_bLastFrameFeedbackRequest ||
		((m_eDeviceAspect!=m_eRenderAspect) && !s_PresentParameters.Windowed))
	{
		m_bIsRenderingFeedback=TRUE;
		//circulate feedback buffers
		DWORD dwTmp=0;
		dwTmp=m_dwThisFrameRTHandle;
		m_dwThisFrameRTHandle=m_dwLastFrameRTHandle;
		m_dwLastFrameRTHandle=dwTmp;
	}
	else
	{
		m_bIsRenderingFeedback=FALSE;
		//free feedback textures if any were used
		if(m_dwThisFrameRTHandle!=0xffffffff)
		{
			unlockRenderTarget(m_dwThisFrameRTHandle);
			m_dwThisFrameRTHandle=0xffffffff;
		}
		if(m_dwLastFrameRTHandle!=0xffffffff)
		{
			unlockRenderTarget(m_dwLastFrameRTHandle);
			m_dwLastFrameRTHandle=0xffffffff;
		}
	}
	if(bGenFeedbackTexture)
	{
		m_bLastFrameFeedbackRequest=TRUE;
	}
	else
	{
		m_bLastFrameFeedbackRequest=FALSE;
	}
	m_DynamicSVPNormalVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPNPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPRTPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicSVPPointVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPNormalVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPNPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPRTPatchVB.m_bDiscardedInThisFrame=FALSE;
	m_DynamicHVPPointVB.m_bDiscardedInThisFrame=FALSE;
	m_pD3DDevice->BeginScene();
	m_pD3DDevice->Clear(0,NULL,D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		ClearCol,1.0f,0);
	if(m_bIsRenderingFeedback)
	{
		//switch render target
		if(m_dwThisFrameRTHandle==0xffffffff)
		{
			PDIRECT3DTEXTURE9 pTmpTex=NULL;
			getRenderTarget(m_dwScreenRTTextureX,m_dwScreenRTTextureY,
				D3DFMT_A8R8G8B8,FALSE,&pTmpTex,&m_dwThisFrameRTHandle);
			pTmpTex->Release();
			lockRenderTarget(m_dwThisFrameRTHandle);
		}
		//TODO: what is the reasoning of code below?
		PDIRECT3DSURFACE9 pTmpSurf=NULL;
		getDepthStencil(m_dwScreenRTTextureX,m_dwScreenRTTextureY,&pTmpSurf,
			&m_dwFeedbackDSHandle);
		pTmpSurf->Release();
		setRenderTarget(m_dwThisFrameRTHandle,0);
		setDepthStencil(m_dwFeedbackDSHandle);
		lockDepthStencil(m_dwFeedbackDSHandle);
	}
	m_pD3DDevice->Clear(0,NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,ClearCol,1.0f,0);
}

void D3DImpl::clearCurrentRenderTarget(D3DCOLOR ClearCol)
{
	m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,ClearCol,1.0f,0);
}

void D3DImpl::clearCurrentDepthStencil()
{
	m_pD3DDevice->Clear(0,NULL,D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0,0,0,0),1.0f,0);
}

void D3DImpl::endFrame()
{
	DWORD dwI=0;
	for(dwI=0;dwI<m_dwRTPoolSize;dwI++)
	{
		if((m_RTPool[dwI].m_dwLockDuration>0) && (m_RTPool[dwI].m_dwLockDuration!=0xffffffff))
		{
			m_RTPool[dwI].m_dwLockDuration--;
			if(m_RTPool[dwI].m_dwLockDuration==0)
			{
				m_RTPool[dwI].m_bLocked=FALSE;
			}
		}
	}
	if(m_bIsRenderingFeedback)
	{
		//some more complicated handling is needed :)
		setRenderTarget(0,0);
		setDepthStencil(0);
		unlockDepthStencil(m_dwFeedbackDSHandle);
		PDIRECT3DTEXTURE9 pTmpTex=NULL;
		getRenderTargetByHandle(m_dwThisFrameRTHandle,&pTmpTex);
		begin2D();
		setSpriteAlphaTestMode(FALSE);
		setSpriteFilter(D3DTEXF_LINEAR,D3DTEXF_LINEAR,D3DTEXF_LINEAR);		//TODO: support it!!!
		setSpriteFactor(0.1f);
		if((m_eDeviceAspect!=m_eRenderAspect) && !s_PresentParameters.Windowed)
		{
			//right now the only possible adjustments are 4:3 rt on 16:9 display
			//and 16:9 rt on 4:3 display
			assert(m_eDeviceAspect==AR_4_3);		//support only one option now but
													//be careful if releasing 4:3 stuff to public!!!!
			drawQuad(LRSM_REPLACE,pTmpTex,NULL,NULL,NULL,NULL,NULL,
				m_dwScreenRTTextureX,m_dwScreenRTTextureY,0.0f,0.125f,1.0f,0.875f);
		}
		else
		{
			drawFullscreenQuad(LRSM_REPLACE,pTmpTex,NULL,NULL,NULL,NULL,NULL,
				m_dwScreenRTTextureX,m_dwScreenRTTextureY);
		}
		pTmpTex->AddRef();
		end2D();
		m_bIsRenderingFeedback=FALSE;
	}
#ifdef _DEBUG
	if((m_dwCurrentRT!=0) || (m_dwCurrentDS!=0))
	{
		DEBUG_STRING("D3D (ERROR): Render target/depth stencil switching error (base surfaces not restored)\n");
	}
#endif
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}

void D3DImpl::createTextureFromFileInMemory(LPVOID pSrcData,UINT uiSrcDataSize,
												  PDIRECT3DTEXTURE9 *pOutTexture)
{
	DEBUG_STRING("D3D (INFO): Creating new (static,managed) texture\n");
	HRESULT hr=D3DXCreateTextureFromFileInMemoryEx(m_pD3DDevice,pSrcData,
		uiSrcDataSize,0,0,0,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,D3DX_DEFAULT,
		D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0),NULL,NULL,pOutTexture);
	if(FAILED(hr))
	{
		throw InvalidOperationExceptionImpl("Failed to create texture");
	}
	m_CreatedTextures.push_back(*pOutTexture);
}

void D3DImpl::createEffectFromFileInMemory(LPCSTR pSrcData,UINT uiSrcDataSize,
												 LPD3DXEFFECT *ppEffect)
{
	LPD3DXBUFFER pCompilationErrors=NULL;
	HRESULT hr=D3DXCreateEffect(m_pD3DDevice,pSrcData,uiSrcDataSize,NULL,NULL,
		0,NULL,ppEffect,&pCompilationErrors);
	if(FAILED(hr))
	{
		throw InvalidOperationExceptionImpl("Failed to create effect");
	}
}

void D3DImpl::createEffectFromResource(const char *pszOriginalFileName,HMODULE hSrcModule,
								 LPCSTR pSrcResource,LPD3DXEFFECT *ppEffect)
{
	if(!pszOriginalFileName)
	{
		throw InvalidParameterExceptionImpl("pszOriginalFileName must not be NULL");
	}
	//cache lookup
	if(m_EffectCache.find(pszOriginalFileName)!=m_EffectCache.end())
	{
		*ppEffect=m_EffectCache[pszOriginalFileName];
		(*ppEffect)->AddRef();
		return;
	}
	//load if not present
	HRSRC hrsrc=::FindResource(hSrcModule,pSrcResource,"FXFILE");
	if(!hrsrc)
	{
		throw InvalidOperationExceptionImpl("Resource not found");
	}
	HGLOBAL hgres=::LoadResource(hSrcModule,hrsrc);
	if(!hgres)
	{
		throw InvalidOperationExceptionImpl("Failed to load resource");
	}
	try
	{
		createEffectFromFileInMemory((LPCSTR)::LockResource(hgres),
			::SizeofResource(hSrcModule,hrsrc),ppEffect);
	}
	catch(Exception&)
	{
		FreeResource(hgres);
	}
	FreeResource(hgres);
	//add effect to cache
	(*ppEffect)->AddRef();
	m_EffectCache.insert(std::map<std::string,ID3DXEffect*>::value_type(pszOriginalFileName,*ppEffect));
}

void D3DImpl::createEffectFromFile(const char *pszFileName,AutoPtr<fs> &pfs,
										 LPD3DXEFFECT *ppEffect)
{
	if(!pszFileName)
	{
		throw InvalidParameterExceptionImpl("pszFileName must not be NULL");
	}
	//cache lookup
	if(m_EffectCache.find(pszFileName)!=m_EffectCache.end())
	{
		*ppEffect=m_EffectCache[pszFileName];
		(*ppEffect)->AddRef();
		return;
	}
	if(pfs.isNull())
	{
		throw InvalidParameterExceptionImpl("pfs must not be NULL");
	}
	ULONG uVFileHandle=0;
	unsigned char *pMemBuf=NULL;
	pfs->openFileAsPtr(pszFileName,uVFileHandle,&pMemBuf);
	UINT uiFileSize=pfs->getFileSize(uVFileHandle);
	try
	{
		createEffectFromFileInMemory((LPCSTR)pMemBuf,uiFileSize,ppEffect);
	}
	catch(Exception &)
	{
		pfs->closeFile(uVFileHandle);
		throw;
	}
	pfs->closeFile(uVFileHandle);
	//add effect to cache
	(*ppEffect)->AddRef();
	m_EffectCache.insert(std::map<std::string,ID3DXEffect*>::value_type(
		pszFileName,*ppEffect));
}

void D3DImpl::setTextureStageState(DWORD dwStage,
								   D3DTEXTURESTAGESTATETYPE eType,
								   DWORD dwValue)
{
	m_pD3DDevice->SetTextureStageState(dwStage,eType,dwValue);
}

void D3DImpl::setTextureTransformMatrix(D3DTRANSFORMSTATETYPE eTexture,
											 D3DXMATRIXA16 *pMatrix)
{
	if((eTexture<D3DTS_TEXTURE0) ||
		(eTexture>D3DTS_TEXTURE7))
	{
		throw InvalidParameterExceptionImpl("Invalid texture transform id");
	}
	m_pD3DDevice->SetTransform(eTexture,pMatrix);
}

void D3DImpl::setPixelShaderConstant(UINT uiStartRegister,
					const float *pfConstantData,UINT uiVector4fCount)
{
	m_pD3DDevice->SetPixelShaderConstantF(uiStartRegister,
		pfConstantData,uiVector4fCount);
}

void D3DImpl::setVertexShaderConstant(UINT uiStartRegister,
					const float *pfConstantData,UINT uiVector4fCount)
{
	m_pD3DDevice->SetVertexShaderConstantF(uiStartRegister,
		pfConstantData,uiVector4fCount);
}

void D3DImpl::setDefaultFilteringState()
{
	DWORD dwTmp=0;
	DWORD dwTexCount=m_DevCaps.MaxSimultaneousTextures;		//TODO: using this constant is not ok, on ps2.0+ hw!
	for(dwTmp=0;dwTmp<dwTexCount;dwTmp++) {
		m_pD3DDevice->SetSamplerState(dwTmp,D3DSAMP_MAGFILTER,m_MagFilter);
		m_pD3DDevice->SetSamplerState(dwTmp,D3DSAMP_MINFILTER,m_MinFilter);
		m_pD3DDevice->SetSamplerState(dwTmp,D3DSAMP_MIPFILTER,m_MipFilter);
		m_pD3DDevice->SetSamplerState(dwTmp,D3DSAMP_MAXANISOTROPY,m_dwAnisotropy);
	}
}

void D3DImpl::drawText(DWORD dwXPos,DWORD dwYPos,
					D3DCOLOR color,const char *pszText)
{
	m_pFont->DrawText((float)dwXPos,(float)dwYPos,color,pszText);
}

void D3DImpl::createOffscreenD3DSurface(UINT Width,UINT Height,
					D3DFORMAT Format,PDIRECT3DSURFACE9* ppSurface)
{
	if(FAILED(m_pD3DDevice->CreateOffscreenPlainSurface(Width,Height,
		Format,D3DPOOL_SCRATCH,ppSurface,NULL)))
	{
		throw InvalidOperationExceptionImpl("Failed to create offscreen surface");
	}
}

void D3DImpl::loadSurfaceFromFileInMemory(PDIRECT3DSURFACE9 pDestSurface,
											   LPCVOID pSrcData,
											   UINT uiSrcDataSize)
{
	if(FAILED(D3DXLoadSurfaceFromFileInMemory(pDestSurface,NULL,NULL,pSrcData,
		uiSrcDataSize,NULL,D3DX_DEFAULT,D3DCOLOR_RGBA(0,0,0,0),NULL)))
	{
		throw InvalidOperationExceptionImpl("Failed to load surface data");
	}
}

void D3DImpl::getImageInfoFromFileInMemory(LPCVOID pSrcData,
												UINT uiSrcDataSize,
												D3DXIMAGE_INFO *pInfo)
{
	if(FAILED(D3DXGetImageInfoFromFileInMemory(pSrcData,uiSrcDataSize,pInfo)))
	{
		throw InvalidOperationExceptionImpl("Failed to get image info");
	}
}

void D3DImpl::createTextureFromSurface(D3DFORMAT Format,
					PDIRECT3DSURFACE9 pSurf,PDIRECT3DTEXTURE9 *ppTexture)
{
	//this one is little bit harder :)
	D3DSURFACE_DESC dsc;
	ZeroMemory(&dsc,sizeof(D3DSURFACE_DESC));
	pSurf->GetDesc(&dsc);
	PDIRECT3DTEXTURE9 pTex=NULL;
	HRESULT hr=S_OK;
	if(FAILED(hr=m_pD3DDevice->CreateTexture(dsc.Width,dsc.Height,
		0,0,Format,D3DPOOL_MANAGED,&pTex,NULL)))
	{
		throw InvalidOperationExceptionImpl("Failed to create texture");
	}
	PDIRECT3DSURFACE9 pTexSurf=NULL;
	if(FAILED(hr=pTex->GetSurfaceLevel(0,&pTexSurf)))
	{
		pTex->Release();
		throw InvalidOperationExceptionImpl("GetSurfaceLevel failed");
	}
	if(FAILED(hr=D3DXLoadSurfaceFromSurface(pTexSurf,NULL,NULL,pSurf,NULL,
		NULL,D3DX_DEFAULT,D3DCOLOR_ARGB(0,0,0,0))))
	{
		pTexSurf->Release();
		pTex->Release();
		throw InvalidOperationExceptionImpl("LoadSurfaceFromSurface failed");
	}
	pTexSurf->Release();
	if(FAILED(hr=D3DXFilterTexture(pTex,NULL,0,D3DX_DEFAULT)))
	{
		pTex->Release();
		throw InvalidOperationExceptionImpl("FilterTexture failed");
	}
	*ppTexture=pTex;
	m_CreatedTextures.push_back(pTex);
}

void D3DImpl::begin2D()
{
	if(m_bIn2D)
	{
		throw InvalidOperationExceptionImpl("begin2D() called inside begin/end2D block");
	}
	m_bIn2D=TRUE;
}

void D3DImpl::end2D()
{
	if(!m_bIn2D)
	{
		throw InvalidOperationExceptionImpl("end2D() called outside begin/end2D block");
	}
	DWORD dwCurRTWidth=512;
	DWORD dwCurRTHeight=512;
	getCurrentRenderTargetSizes(&dwCurRTWidth,&dwCurRTHeight);
	//here is where all real work is done, it's quite complicated as
	//code tries to minimize VB locks/unlocks while keeping track of
	//vertex processing mode - definitely CAN be further optimized :)
	DWORD dwHardwareVSVersion=m_DevCaps.VertexShaderVersion;
	DWORD dwStartSprite=0;
	DWORD dwSpriteCount=m_SpriteVec.size();
	//capture existing state
	m_pSpriteStateBlock->Capture();
	//init common renderstates (and their cache)
	//uncached
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_TEXCOORDINDEX,1|D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(2,D3DTSS_TEXCOORDINDEX,2|D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(3,D3DTSS_TEXCOORDINDEX,3|D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(4,D3DTSS_TEXCOORDINDEX,2|D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(5,D3DTSS_TEXCOORDINDEX,3|D3DTSS_TCI_PASSTHRU);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState(2,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState(3,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState(4,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetTextureStageState(5,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(1,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(1,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(2,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(2,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(3,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(3,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(4,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(4,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(5,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
	m_pD3DDevice->SetSamplerState(5,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
	//cached
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,0);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,D3DBLENDOP_ADD);
	m_pD3DDevice->SetVertexShader(NULL);
	m_pD3DDevice->SetPixelShader(NULL);
	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetTexture(1,NULL);
	m_pD3DDevice->SetTexture(2,NULL);
	m_pD3DDevice->SetTexture(3,NULL);
	bool				bZTestEnabled=false;
	LPD3DXEFFECT		pCurrentEffect=NULL;
	D3DXHANDLE			hCurrentTechnique=NULL;		//important - only pair (fx,technique) is useful identifier!!!
	bool				bLigtingEnabled=false;
	bool				bFogEnabled=false;
	bool				bAlphaTestEnabled=false;
	DWORD				dwAlphaRef=0;
	bool				bAlphaBlendEnabled=false;
	D3DBLEND			eSrcBlend=D3DBLEND_SRCALPHA;
	D3DBLEND			eDstBlend=D3DBLEND_INVSRCALPHA;
	D3DCMPFUNC			eAlphaFunc=D3DCMP_GREATEREQUAL;
	D3DBLENDOP			eBlendOp=D3DBLENDOP_ADD;
	PDIRECT3DTEXTURE9	pTex0=NULL;
	PDIRECT3DTEXTURE9	pTex1=NULL;
	PDIRECT3DTEXTURE9	pTex2=NULL;
	PDIRECT3DTEXTURE9	pTex3=NULL;
	PDIRECT3DTEXTURE9	pTex4=NULL;
	PDIRECT3DTEXTURE9	pTex5=NULL;
	//stages beyond first can only be controlled through effect files
	//so there's no need or way to cache them
	D3DTEXTUREOP		eTx1ColorOp=D3DTOP_DISABLE;
	D3DTEXTUREOP		eTx1AlphaOp=D3DTOP_DISABLE;
	DWORD				dwTx1ColorArg1=D3DTA_TEXTURE;
	DWORD				dwTx1AlphaArg1=D3DTA_TEXTURE;
	DWORD				dwTx1AlphaArg2=D3DTA_TEXTURE;
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,dwTx1AlphaArg2);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
	m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
	//we don't know what's there ... TODO: do it in further setups, too
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
	m_pD3DDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	DWORD				dwCurrentTextureFactor=0;
	m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
	//draw everything
	while(dwStartSprite<dwSpriteCount)
	{
		//see what kind of vertex processing current start sprite uses
		D3D_SpriteDesc &desc=m_SpriteVec[dwStartSprite];
		//see how many following sprites use the same vertex processing mode
		DWORD dwEndSprite=dwStartSprite+1;
		while(dwEndSprite<dwSpriteCount)
		{
			dwEndSprite++;
		}
		//see how much available space is there in appropriate dynamic vb
		D3D_DynamicVBWrapper *pDynVB=NULL;
		if(!m_DynamicHVPNormalVB.m_pVB)
		{
			DEBUG_STRING("D3D (WARNING): Creating dynamic vertex buffer on the fly!!!\n");
			createDynamicVB(0);
		}
		pDynVB=&m_DynamicHVPNormalVB;
		m_pD3DDevice->SetStreamSource(0,pDynVB->m_pVB,0,SPRITE_VERTEX_SIZE);
		DWORD dwStartByte=(pDynVB->m_dwBytesUsed%SPRITE_VERTEX_SIZE) ?
						((pDynVB->m_dwBytesUsed%SPRITE_VERTEX_SIZE)+1)*SPRITE_VERTEX_SIZE :
						pDynVB->m_dwBytesUsed;
		DWORD dwBytesFree=pDynVB->m_dwByteLength-dwStartByte;
		DWORD dwSpriteCapacity=dwBytesFree/(6*SPRITE_VERTEX_SIZE);
		bool bDiscard=false;
		DWORD dwSpritesToPush=dwEndSprite-dwStartSprite;
		//lock
		BYTE *pVBData=NULL;
		if(!pDynVB->m_bDiscardedInThisFrame)
		{
//			dwSpriteCapacity=0;
		}
		if(dwSpriteCapacity==0)
		{
			dwStartByte=0;
			dwBytesFree=pDynVB->m_dwByteLength;
			dwSpriteCapacity=dwBytesFree/(6*SPRITE_VERTEX_SIZE);
			if(dwSpritesToPush>dwSpriteCapacity)
			{
				dwSpritesToPush=dwSpriteCapacity;
			}
			pDynVB->m_pVB->Lock(0,dwSpritesToPush*6*SPRITE_VERTEX_SIZE,
				(void**)&pVBData,D3DLOCK_DISCARD);
			pDynVB->m_dwBytesUsed=dwSpritesToPush*6*SPRITE_VERTEX_SIZE;
			pDynVB->m_dwDiscardID++;
			pDynVB->m_bDiscardedInThisFrame=true;
			dwStartByte=0;
		}
		else
		{
			if(dwSpritesToPush>dwSpriteCapacity)
			{
				dwSpritesToPush=dwSpriteCapacity;
			}
			pDynVB->m_pVB->Lock(pDynVB->m_dwBytesUsed,
				dwSpritesToPush*6*SPRITE_VERTEX_SIZE+(pDynVB->m_dwBytesUsed-dwStartByte),
				(void**)&pVBData,D3DLOCK_NOOVERWRITE);
			pVBData+=pDynVB->m_dwBytesUsed-dwStartByte;
			pDynVB->m_dwBytesUsed+=dwSpritesToPush*6*SPRITE_VERTEX_SIZE+(pDynVB->m_dwBytesUsed-dwStartByte);
		}
		//fill - each sprite has 6 vertices (2 non-indexed triangles)
		unsigned int i=0;
		float *pfVertices=(float*)pVBData;
		float fAdjustX=0.5f/dwCurRTWidth;
		float fAdjustY=0.5f/dwCurRTHeight;
		for(i=0;i<dwSpritesToPush;i++)
		{
			if(m_SpriteVec[dwStartSprite+i].m_dwVertexShaderVersionUsed==D3DVS_VERSION(0,0))
			{
				//triangle 1 - {(0,0);(1,1);(0,1)}, adjustment is (0.5f/rt_size)
				//0,0
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x0*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y0*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
				//1,1
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x1*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y1*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//0,1
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x0*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y1*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//triangle 2 - {(0,0);(1,1);(0,1)}
				//1,1
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x1*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y1*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//0,0
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x0*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y0*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
				//1,0
				*pfVertices++=m_SpriteVec[dwStartSprite+i].x1*dwCurRTWidth-0.5f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].y0*dwCurRTHeight-0.5f;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
			}
			else
			{
				//triangle 1 - {(0,0);(1,1);(0,1)}
				//0,0
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x0-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y0+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
				//1,1
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x1-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y1+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//0,1
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x0-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y1+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//triangle 2 - {(0,0);(1,1);(0,1)}
				//1,1
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x1-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y1+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty1;
				//0,0
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x0-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y0+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx0;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
				//1,0
				*pfVertices++=(m_SpriteVec[dwStartSprite+i].x1-0.5f)*2.0f-fAdjustX;
				*pfVertices++=(-m_SpriteVec[dwStartSprite+i].y0+0.5f)*2.0f+fAdjustY;
				*pfVertices++=0.0f;
				*pfVertices++=1.0f;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].tx1;
				*pfVertices++=m_SpriteVec[dwStartSprite+i].ty0;
			}
		}
		//unlock
		pDynVB->m_pVB->Unlock();
		//render
		//TODO: batch draw primitive calls!!! (this actually is done by d3d runtime
		//as long as no renderstate changes appear between DrawPrimitive calls so
		//current approach is to simply cache state changes)
		for(i=0;i<dwSpritesToPush;i++)
		{
			D3D_SpriteDesc &desc=m_SpriteVec[dwStartSprite+i];
			//perform renderstate setup
			switch(desc.m_eSpriteMode)
			{
				case LRSM_REPLACE:
					if(pCurrentEffect)
					{
						//full render state setup
						pCurrentEffect->EndPass();
						hCurrentTechnique=NULL;
						pCurrentEffect->End();
						pCurrentEffect=NULL;
						m_pD3DDevice->SetVertexShader(NULL);
						m_pD3DDevice->SetPixelShader(NULL);
						bZTestEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,bZTestEnabled);
						bFogEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,bFogEnabled);
						bLigtingEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,bLigtingEnabled);
						if(desc.m_bAlphaTestEnable)
						{
							bAlphaTestEnabled=true;
							dwAlphaRef=desc.m_dwAlphaTestRef;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						else
						{
							bAlphaTestEnabled=false;
							dwAlphaRef=0;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
						bAlphaBlendEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						eSrcBlend=D3DBLEND_SRCALPHA;
						m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,eSrcBlend);
						eDstBlend=D3DBLEND_INVSRCALPHA;
						m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,eDstBlend);
						eBlendOp=D3DBLENDOP_ADD;
						m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,eBlendOp);
						pTex0=desc.m_pTexture1;
						pTex1=desc.m_pTexture2;
						pTex2=desc.m_pTexture3;
						pTex3=desc.m_pTexture4;
						pTex4=desc.m_pTexture5;
						pTex5=desc.m_pTexture6;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							eTx1ColorOp=D3DTOP_DISABLE;
							eTx1AlphaOp=D3DTOP_DISABLE;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						else
						{
							dwTx1ColorArg1=D3DTA_TEXTURE;
							dwTx1AlphaArg1=D3DTA_TEXTURE;
							eTx1ColorOp=D3DTOP_SELECTARG1;
							eTx1AlphaOp=D3DTOP_SELECTARG1;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						m_pD3DDevice->SetTexture(1,pTex1);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(2,pTex2);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(3,pTex3);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(4,pTex3);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(5,pTex3);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						dwCurrentTextureFactor=0;
						m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
					}
					else
					{
						//cached setup
						if(desc.m_bAlphaTestEnable)
						{
							if(!bAlphaTestEnabled)
							{
								bAlphaTestEnabled=TRUE;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
							if(desc.m_dwAlphaTestRef!=dwAlphaRef)
							{
								dwAlphaRef=desc.m_dwAlphaTestRef;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
							}
							if(eAlphaFunc!=D3DCMP_GREATEREQUAL)
							{
								eAlphaFunc=D3DCMP_GREATEREQUAL;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
							}
						}
						else
						{
							if(bAlphaTestEnabled)
							{
								bAlphaTestEnabled=false;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
						}
						if(bAlphaBlendEnabled)
						{
							bAlphaBlendEnabled=false;
							m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						}
						//setup only first texture
						pTex0=desc.m_pTexture1;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							if(eTx1ColorOp!=D3DTOP_DISABLE)
							{
								eTx1ColorOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_DISABLE)
							{
								eTx1AlphaOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
						else
						{
							if(dwTx1ColorArg1!=D3DTA_TEXTURE)
							{
								dwTx1ColorArg1=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							}
							if(dwTx1AlphaArg1!=D3DTA_TEXTURE)
							{
								dwTx1AlphaArg1=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							}
							if(eTx1ColorOp!=D3DTOP_SELECTARG1)
							{
								eTx1ColorOp=D3DTOP_SELECTARG1;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_SELECTARG1)
							{
								eTx1AlphaOp=D3DTOP_SELECTARG1;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
					}
					break;
				case LRSM_ADD:
					if(pCurrentEffect)
					{
						//full render state setup
						pCurrentEffect->EndPass();
						hCurrentTechnique=NULL;
						pCurrentEffect->EndPass();
						hCurrentTechnique=NULL;
						pCurrentEffect->End();
						pCurrentEffect=NULL;
						m_pD3DDevice->SetVertexShader(NULL);
						m_pD3DDevice->SetPixelShader(NULL);
						bZTestEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,bZTestEnabled);
						bFogEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,bFogEnabled);
						bLigtingEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,bLigtingEnabled);
						if(desc.m_bAlphaTestEnable)
						{
							bAlphaTestEnabled=true;
							dwAlphaRef=desc.m_dwAlphaTestRef;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						else
						{
							bAlphaTestEnabled=false;
							dwAlphaRef=0;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
						bAlphaBlendEnabled=true;
						m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						eSrcBlend=D3DBLEND_SRCALPHA;
						m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,eSrcBlend);
						eDstBlend=D3DBLEND_ONE;
						m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,eDstBlend);
						eBlendOp=D3DBLENDOP_ADD;
						m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,eBlendOp);
						pTex0=desc.m_pTexture1;
						pTex1=desc.m_pTexture2;
						pTex2=desc.m_pTexture3;
						pTex3=desc.m_pTexture4;
						pTex4=desc.m_pTexture5;
						pTex5=desc.m_pTexture6;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							eTx1ColorOp=D3DTOP_DISABLE;
							eTx1AlphaOp=D3DTOP_DISABLE;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						else
						{
							dwTx1ColorArg1=D3DTA_TEXTURE;
							dwTx1AlphaArg1=D3DTA_TFACTOR;
							dwTx1AlphaArg2=D3DTA_TEXTURE;
							eTx1ColorOp=D3DTOP_SELECTARG1;
							eTx1AlphaOp=D3DTOP_MODULATE;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,dwTx1AlphaArg2);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						m_pD3DDevice->SetTexture(1,pTex1);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(2,pTex2);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(3,pTex3);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(4,pTex3);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(5,pTex3);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						dwCurrentTextureFactor=D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
							desc.m_fSpriteFactor,desc.m_fSpriteFactor);
						m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
					}
					else
					{
						//cached setup
						if(desc.m_bAlphaTestEnable)
						{
							if(!bAlphaTestEnabled)
							{
								bAlphaTestEnabled=true;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
							if(desc.m_dwAlphaTestRef!=dwAlphaRef)
							{
								dwAlphaRef=desc.m_dwAlphaTestRef;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
							}
							if(eAlphaFunc!=D3DCMP_GREATEREQUAL)
							{
								eAlphaFunc=D3DCMP_GREATEREQUAL;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
							}
						}
						else
						{
							if(bAlphaTestEnabled)
							{
								bAlphaTestEnabled=false;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
						}
						if(!bAlphaBlendEnabled)
						{
							bAlphaBlendEnabled=true;
							m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						}
						if(eSrcBlend!=D3DBLEND_SRCALPHA)
						{
							eSrcBlend=D3DBLEND_SRCALPHA;
							m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,eSrcBlend);
						}
						if(eDstBlend!=D3DBLEND_ONE)
						{
							eDstBlend=D3DBLEND_ONE;
							m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,eDstBlend);
						}
						//setup only first texture
						pTex0=desc.m_pTexture1;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							if(eTx1ColorOp!=D3DTOP_DISABLE)
							{
								eTx1ColorOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_DISABLE)
							{
								eTx1AlphaOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
						else
						{
							if(dwTx1ColorArg1!=D3DTA_TEXTURE)
							{
								dwTx1ColorArg1=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							}
							if(dwTx1AlphaArg1!=D3DTA_TFACTOR)
							{
								dwTx1AlphaArg1=D3DTA_TFACTOR;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							}
							if(dwTx1AlphaArg2!=D3DTA_TEXTURE)
							{
								dwTx1AlphaArg2=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,dwTx1AlphaArg2);
							}
							if(eTx1ColorOp!=D3DTOP_SELECTARG1)
							{
								eTx1ColorOp=D3DTOP_SELECTARG1;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_MODULATE)
							{
								eTx1AlphaOp=D3DTOP_MODULATE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
						if(dwCurrentTextureFactor!=
							D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
							desc.m_fSpriteFactor,desc.m_fSpriteFactor))
						{
							dwCurrentTextureFactor=D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
								desc.m_fSpriteFactor,desc.m_fSpriteFactor);
							m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
						}
					}
					break;
				case LRSM_BLEND:
					if(pCurrentEffect)
					{
						//full render state setup
						pCurrentEffect->EndPass();
						hCurrentTechnique=NULL;
						pCurrentEffect->End();
						pCurrentEffect=NULL;
						m_pD3DDevice->SetVertexShader(NULL);
						m_pD3DDevice->SetPixelShader(NULL);
						bZTestEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,bZTestEnabled);
						bFogEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,bFogEnabled);
						bLigtingEnabled=false;
						m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,bLigtingEnabled);
						if(desc.m_bAlphaTestEnable)
						{
							bAlphaTestEnabled=true;
							dwAlphaRef=desc.m_dwAlphaTestRef;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						else
						{
							bAlphaTestEnabled=false;
							dwAlphaRef=0;
							eAlphaFunc=D3DCMP_GREATEREQUAL;
						}
						m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
						m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
						bAlphaBlendEnabled=true;
						m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						eSrcBlend=D3DBLEND_SRCALPHA;
						m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,eSrcBlend);
						eDstBlend=D3DBLEND_INVSRCALPHA;
						m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,eDstBlend);
						eBlendOp=D3DBLENDOP_ADD;
						m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,eBlendOp);
						pTex0=desc.m_pTexture1;
						pTex1=desc.m_pTexture2;
						pTex2=desc.m_pTexture3;
						pTex3=desc.m_pTexture4;
						pTex4=desc.m_pTexture5;
						pTex5=desc.m_pTexture6;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							eTx1ColorOp=D3DTOP_DISABLE;
							eTx1AlphaOp=D3DTOP_DISABLE;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						else
						{
							dwTx1ColorArg1=D3DTA_TEXTURE;
							dwTx1AlphaArg1=D3DTA_TFACTOR;
							dwTx1AlphaArg2=D3DTA_TEXTURE;
							eTx1ColorOp=D3DTOP_SELECTARG1;
							eTx1AlphaOp=D3DTOP_MODULATE;
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,dwTx1AlphaArg2);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
						}
						m_pD3DDevice->SetTexture(1,pTex1);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(2,pTex2);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(2,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(3,pTex3);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(3,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(4,pTex3);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(4,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTexture(5,pTex3);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_COLOROP,D3DTOP_DISABLE);
						m_pD3DDevice->SetTextureStageState(5,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
						dwCurrentTextureFactor=D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
							desc.m_fSpriteFactor,desc.m_fSpriteFactor);
						m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
					}
					else
					{
						//cached setup
						if(desc.m_bAlphaTestEnable)
						{
							if(!bAlphaTestEnabled)
							{
								bAlphaTestEnabled=TRUE;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
							if(desc.m_dwAlphaTestRef!=dwAlphaRef)
							{
								dwAlphaRef=desc.m_dwAlphaTestRef;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF,dwAlphaRef);
							}
							if(eAlphaFunc!=D3DCMP_GREATEREQUAL)
							{
								eAlphaFunc=D3DCMP_GREATEREQUAL;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,eAlphaFunc);
							}
						}
						else
						{
							if(bAlphaTestEnabled)
							{
								bAlphaTestEnabled=false;
								m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,bAlphaTestEnabled);
							}
						}
						if(!bAlphaBlendEnabled)
						{
							bAlphaBlendEnabled=true;
							m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,bAlphaBlendEnabled);
						}
						if(eSrcBlend!=D3DBLEND_SRCALPHA)
						{
							eSrcBlend=D3DBLEND_SRCALPHA;
							m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,eSrcBlend);
						}
						if(eDstBlend!=D3DBLEND_INVSRCALPHA)
						{
							eDstBlend=D3DBLEND_INVSRCALPHA;
							m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,eDstBlend);
						}
						//setup only first texture
						pTex0=desc.m_pTexture1;
						m_pD3DDevice->SetTexture(0,pTex0);
						if(!pTex0)
						{
							//TODO: this (supposedly) is an error
							if(eTx1ColorOp!=D3DTOP_DISABLE)
							{
								eTx1ColorOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_DISABLE)
							{
								eTx1AlphaOp=D3DTOP_DISABLE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
						else
						{
							if(dwTx1ColorArg1!=D3DTA_TEXTURE)
							{
								dwTx1ColorArg1=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,dwTx1ColorArg1);
							}
							if(dwTx1AlphaArg1!=D3DTA_TFACTOR)
							{
								dwTx1AlphaArg1=D3DTA_TFACTOR;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,dwTx1AlphaArg1);
							}
							if(dwTx1AlphaArg2!=D3DTA_TEXTURE)
							{
								dwTx1AlphaArg2=D3DTA_TEXTURE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,dwTx1AlphaArg2);
							}
							if(eTx1ColorOp!=D3DTOP_SELECTARG1)
							{
								eTx1ColorOp=D3DTOP_SELECTARG1;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,eTx1ColorOp);
							}
							if(eTx1AlphaOp!=D3DTOP_MODULATE)
							{
								eTx1AlphaOp=D3DTOP_MODULATE;
								m_pD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,eTx1AlphaOp);
							}
						}
						if(dwCurrentTextureFactor!=
							D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
							desc.m_fSpriteFactor,desc.m_fSpriteFactor))
						{
							dwCurrentTextureFactor=D3DCOLOR_COLORVALUE(desc.m_fSpriteFactor,desc.m_fSpriteFactor,
								desc.m_fSpriteFactor,desc.m_fSpriteFactor);
							m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,dwCurrentTextureFactor);
						}
					}
					break;
				case LRSM_CUSTOM_EFFECT:
					if(pCurrentEffect &&
						((pCurrentEffect!=desc.m_pSpriteEffect) ||
						(hCurrentTechnique!=desc.m_hSpriteTechniqueHandle)))
					{
						//switch effect&set constants
						pCurrentEffect->EndPass();
						pCurrentEffect->End();
						m_pD3DDevice->SetVertexShader(NULL);
						m_pD3DDevice->SetPixelShader(NULL);
					}
					if((pCurrentEffect!=desc.m_pSpriteEffect) || (hCurrentTechnique!=desc.m_hSpriteTechniqueHandle))
					{
						//begin effect and set constants
						pCurrentEffect=desc.m_pSpriteEffect;
						hCurrentTechnique=desc.m_hSpriteTechniqueHandle;
						pCurrentEffect->SetTechnique(desc.m_hSpriteTechniqueHandle);
						UINT dwPasses=0;
						pCurrentEffect->Begin(&dwPasses,0);
					}
					pCurrentEffect->BeginPass(0);
					pCurrentEffect->CommitChanges();
					if(desc.m_dwSpritePSConstantCount)
					{
						setPixelShaderConstant(0,desc.m_SpritePSContants,desc.m_dwSpritePSConstantCount);
					}
					if(desc.m_dwSpriteVSConstantCount)
					{
						setVertexShaderConstant(0,desc.m_SpriteVSContants,desc.m_dwSpriteVSConstantCount);
					}
					//setup textures
					if(pTex0!=desc.m_pTexture1)
					{
						pTex0=desc.m_pTexture1;
						m_pD3DDevice->SetTexture(0,pTex0);
					}
					if(pTex1!=desc.m_pTexture2)
					{
						pTex1=desc.m_pTexture2;
						m_pD3DDevice->SetTexture(1,pTex1);
					}
					if(pTex2!=desc.m_pTexture3)
					{
						pTex2=desc.m_pTexture3;
						m_pD3DDevice->SetTexture(2,pTex2);
					}
					if(pTex3!=desc.m_pTexture4)
					{
						pTex3=desc.m_pTexture4;
						m_pD3DDevice->SetTexture(3,pTex3);
					}
					if(pTex4!=desc.m_pTexture5)
					{
						pTex4=desc.m_pTexture5;
						m_pD3DDevice->SetTexture(4,pTex4);
					}
					if(pTex5!=desc.m_pTexture6)
					{
						pTex5=desc.m_pTexture6;
						m_pD3DDevice->SetTexture(5,pTex5);
					}
					break;
			}
			//set sprite vertex decl as current but choose the right one
			if(m_SpriteVec[dwStartSprite].m_dwVertexShaderVersionUsed!=D3DVS_VERSION(0,0))
			{
				m_pD3DDevice->SetVertexDeclaration(m_pVSSpriteVFDecl);
			}
			else
			{
				m_pD3DDevice->SetVertexDeclaration(m_pSpriteVFDecl);
			}
			//finally push it to hw :)
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,
				(dwStartSprite+i)*6+dwStartByte/(SPRITE_VERTEX_SIZE),2);
		}
		//set new start sprite to reflect what we just rendered
		dwStartSprite+=dwSpritesToPush;
	}
	//restore renderstates
	if(pCurrentEffect)
	{
		pCurrentEffect->EndPass();
		pCurrentEffect->End();
		pCurrentEffect=NULL;
	}
	m_pSpriteStateBlock->Apply();
	//clear everything
	unsigned int i=0;
	for(i=0;i<6;i++)
	{
		m_pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(i,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
		//TODO: restore texture filtering state
	}
	if(m_pCurrentSpriteEffect)
	{
		m_pCurrentSpriteEffect->Release();
		m_pCurrentSpriteEffect=NULL;
	}
	m_hCurrentSpriteTechniqueHandle=NULL;
	unsigned int uiSpriteCount=m_SpriteVec.size();
	for(i=0;i<uiSpriteCount;i++)
	{
		D3D_SpriteDesc &rfSprite=m_SpriteVec[i];
		if(rfSprite.m_eSpriteMode==LRSM_CUSTOM_EFFECT)
		{
			rfSprite.m_pSpriteEffect->Release();
		}
		ULONG uTmpRefCount=0;
		if(rfSprite.m_pTexture1)
		{
			uTmpRefCount=rfSprite.m_pTexture1->Release();
		}
		if(rfSprite.m_pTexture2)
		{
			uTmpRefCount=rfSprite.m_pTexture2->Release();
		}
		if(rfSprite.m_pTexture3)
		{
			uTmpRefCount=rfSprite.m_pTexture3->Release();
		}
		if(rfSprite.m_pTexture4)
		{
			uTmpRefCount=rfSprite.m_pTexture4->Release();
		}
		if(rfSprite.m_pTexture5)
		{
			uTmpRefCount=rfSprite.m_pTexture5->Release();
		}
		if(rfSprite.m_pTexture6)
		{
			uTmpRefCount=rfSprite.m_pTexture6->Release();
		}
	}
	m_dwCurrentSpriteVSVersion=D3DVS_VERSION(0,0);
	m_dwCurrentSpritePSConstantCount=0;
	m_dwCurrentSpriteVSConstantCount=0;
	m_SpriteVec.clear();
	m_bIn2D=FALSE;
}

void D3DImpl::setSpriteFactor(float fFactor)
{
	m_fCurrentSpriteFactor=fFactor;
}

void D3DImpl::setSpriteTechnique(LPD3DXEFFECT pEffect,D3DXHANDLE hTechnique,
								 DWORD dwVSVersion,DWORD dwPSVector4fCount,
								 const float *pfPSConstants,
								 DWORD dwVSVector4fCount,
								 const float *pfVSConstants)
{
#ifdef _DEBUG
	if(!pEffect)
	{
		throw InvalidOperationExceptionImpl("Sprite effect must not be NULL");
	}
	if(dwPSVector4fCount>8)
	{
		throw InvalidOperationExceptionImpl("Sprite pixel shader constant count must not be greater than 8");
	}
	if(dwVSVector4fCount>8)
	{
		throw InvalidOperationExceptionImpl("Sprite vertex shader constant count must not be greater than 8");
	}
#endif
	if(m_pCurrentSpriteEffect)
	{
		m_pCurrentSpriteEffect->Release();
	}
	m_pCurrentSpriteEffect=pEffect;
	m_pCurrentSpriteEffect->AddRef();
	m_hCurrentSpriteTechniqueHandle=hTechnique;
	m_dwCurrentSpritePSConstantCount=dwPSVector4fCount;
	if(pfPSConstants)
	{
		memcpy(m_CurrentSpritePSContants,pfPSConstants,dwPSVector4fCount*4*sizeof(float));
	}
	m_dwCurrentSpriteVSConstantCount=dwVSVector4fCount;
	if(pfVSConstants)
	{
		memcpy(m_CurrentSpriteVSContants,pfVSConstants,dwVSVector4fCount*4*sizeof(float));
	}
	m_dwCurrentSpriteVSVersion=dwVSVersion;
}

void D3DImpl::setSpriteFilter(D3DTEXTUREFILTERTYPE eMinFilter,D3DTEXTUREFILTERTYPE eMagFilter,
									D3DTEXTUREFILTERTYPE eMipFilter)
{
	m_eSpriteMinFilter=eMinFilter;
	m_eSpriteMagFilter=eMagFilter;
	m_eSpriteMipFilter=eMipFilter;
}

void D3DImpl::setSpriteAlphaTestMode(bool bEnable,D3DCMPFUNC eFunc,DWORD dwAlphaRef)
{
	m_bSpriteAlphaTestEnable=bEnable;
	m_eSpriteAlphaTestFunc=eFunc;
	m_dwSpriteAlphaTestRef=dwAlphaRef;
}

void D3DImpl::drawSubQuad(LR_SPRITE_MODE eSpriteMode,TexturePtr rfTexture,
							float x0,float y0,float x1,float y1,
							float tx0,float ty0,float tx1,float ty1)
{
#ifdef _DEBUG
	if(!m_bIn2D)
	{
		throw InvalidOperationExceptionImpl("2D drawing function called outside begin2D()/end2D()");
	}
	if(!rfTexture)
	{
		throw InvalidParameterExceptionImpl("drawSubQuad - texture parameter is NULL");
	}
#endif
	DWORD dwSizeX=0;
	DWORD dwSizeY=0;
	PDIRECT3DTEXTURE9 pTex=NULL;
	rfTexture->getSizes(&dwSizeX,&dwSizeY);
	pTex=rfTexture->getD3DTexture();
	drawSubQuad(eSpriteMode,pTex,NULL,NULL,NULL,NULL,NULL,dwSizeX,dwSizeY,x0,
		y0,x1,y1,tx0,ty0,tx1,ty1);
}

/*
 *	internal sprite API
 */

void D3DImpl::drawFullscreenQuad(LR_SPRITE_MODE eSpriteMode,
								 PDIRECT3DTEXTURE9 pTex1,
								 PDIRECT3DTEXTURE9 pTex2,
								 PDIRECT3DTEXTURE9 pTex3,
								 PDIRECT3DTEXTURE9 pTex4,
								 PDIRECT3DTEXTURE9 pTex5,
								 PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
								 DWORD dwTxHeight)
{
	drawSubQuad(eSpriteMode,pTex1,pTex2,pTex3,pTex4,pTex5,pTex6,dwTxWidth,
		dwTxHeight,0.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f);
}

void D3DImpl::drawQuad(LR_SPRITE_MODE eSpriteMode,PDIRECT3DTEXTURE9 pTex1,
							PDIRECT3DTEXTURE9 pTex2,PDIRECT3DTEXTURE9 pTex3,
							PDIRECT3DTEXTURE9 pTex4,PDIRECT3DTEXTURE9 pTex5,
							PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
							DWORD dwTxHeight,float x0,float y0,float x1,
							float y1)
{
	drawSubQuad(eSpriteMode,pTex1,pTex2,pTex3,pTex4,pTex5,pTex6,
		dwTxWidth,dwTxHeight,x0,y0,x1,y1,0.0f,0.0f,1.0f,1.0f);
}

void D3DImpl::drawSubQuad(LR_SPRITE_MODE eSpriteMode,PDIRECT3DTEXTURE9 pTex1,
						  PDIRECT3DTEXTURE9 pTex2,PDIRECT3DTEXTURE9 pTex3,
						  PDIRECT3DTEXTURE9 pTex4,PDIRECT3DTEXTURE9 pTex5,
						  PDIRECT3DTEXTURE9 pTex6,DWORD dwTxWidth,
						  DWORD dwTxHeight,float x0,float y0,float x1,float y1,
						  float tx0,float ty0,float tx1,float ty1)
{
	//enqueue draw request
	D3D_SpriteDesc desc;
	if(eSpriteMode==LRSM_CUSTOM_EFFECT)
	{
		if(!m_pCurrentSpriteEffect)
		{
			throw InvalidParameterExceptionImpl("Sprite effect is NULL!");
		}
	}
	desc.m_eSpriteMode=eSpriteMode;
	if(eSpriteMode==LRSM_CUSTOM_EFFECT)
	{
		desc.m_pSpriteEffect=m_pCurrentSpriteEffect;
		desc.m_pSpriteEffect->AddRef();
		desc.m_dwSpritePSConstantCount=m_dwCurrentSpritePSConstantCount;
		if(m_dwCurrentSpritePSConstantCount)
		{
			memcpy(desc.m_SpritePSContants,m_CurrentSpritePSContants,
				m_dwCurrentSpritePSConstantCount*4*sizeof(float));
		}
		desc.m_dwSpriteVSConstantCount=m_dwCurrentSpriteVSConstantCount;
		if(m_dwCurrentSpriteVSConstantCount)
		{
			memcpy(desc.m_SpriteVSContants,m_CurrentSpriteVSContants,
				m_dwCurrentSpriteVSConstantCount*4*sizeof(float));
		}
		desc.m_fSpriteFactor=0.0f;
		desc.m_hSpriteTechniqueHandle=m_hCurrentSpriteTechniqueHandle;
	}
	else
	{
		desc.m_pSpriteEffect=NULL;
		desc.m_dwSpritePSConstantCount=0;
		desc.m_dwSpriteVSConstantCount=0;
		desc.m_fSpriteFactor=m_fCurrentSpriteFactor;
		desc.m_hSpriteTechniqueHandle=NULL;
	}
	//no need to AddRef textures, it's already been done!!!
	//TODO: should this behavior change???
	desc.m_pTexture1=pTex1;
	desc.m_pTexture2=pTex2;
	desc.m_pTexture3=pTex3;
	desc.m_pTexture4=pTex4;
	desc.m_pTexture5=pTex5;
	desc.m_pTexture6=pTex6;
	//vs version
	desc.m_dwVertexShaderVersionUsed=m_dwCurrentSpriteVSVersion;
	//texture filter settings
	desc.m_eMinFilter=m_eSpriteMinFilter;
	desc.m_eMagFilter=m_eSpriteMagFilter;
	desc.m_eMipFilter=m_eSpriteMipFilter;
	//alpha testing settings
	desc.m_bAlphaTestEnable=m_bSpriteAlphaTestEnable;
	desc.m_eAlphaTestFunc=m_eSpriteAlphaTestFunc;
	desc.m_dwAlphaTestRef=m_dwSpriteAlphaTestRef;
	//coordinates
	desc.x0=x0;
	desc.x1=x1;
	desc.y0=y0;
	desc.y1=y1;
	desc.tx0=tx0;
	desc.tx1=tx1;
	desc.ty0=ty0;
	desc.ty1=ty1;
	m_SpriteVec.push_back(desc);
}

void D3DImpl::setRenderAspectRatio(ASPECT_RATIO eRatio)
{
	m_eRenderAspect=eRatio;
}

ASPECT_RATIO D3DImpl::getRenderAspectRatio()
{
	return m_eRenderAspect;
}

float D3DImpl::getRenderAspectRatioNumeric()
{
	switch(m_eRenderAspect)
	{
		case AR_16_9:
			return 1.777778f;
			break;
		default:
			return 1.333333f;
			break;
	}
}

void D3DImpl::preloadTextures()
{
	if((!m_pD3D) || (!m_pD3DDevice))
	{
		return;
	}
	std::vector<PDIRECT3DTEXTURE9>::iterator iter=m_CreatedTextures.begin();
	while(iter!=m_CreatedTextures.end())
	{
		PDIRECT3DTEXTURE9 pTmpTex=*iter;
		pTmpTex->AddRef();
		m_pD3DDevice->BeginScene();
		m_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,
			D3DCOLOR_RGBA(0,0,0,0),1.0f,0);
		begin2D();
		drawSubQuad(LRSM_REPLACE,
			pTmpTex,NULL,NULL,
			NULL,NULL,NULL,
			512,512,
			0.0f,0.0f,0.1f,0.1f,
			0.0f,0.0f,1.0f,1.0f);
		end2D();
		m_pD3DDevice->EndScene();
		iter++;
	}
}

HRESULT WINAPI CreateD3D(LPVOID* ppv)
{
	D3D	*p_d3d=NULL;
	p_d3d=new D3DImpl();
	p_d3d->AddRef();
	*ppv=(LPVOID)p_d3d;
	return S_OK;
}
