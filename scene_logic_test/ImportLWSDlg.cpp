// ImportLWSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "ImportLWSDlg.h"
#include "globals.h"
#include "LWSImport.h"

// CImportLWSDlg dialog

IMPLEMENT_DYNAMIC(CImportLWSDlg, CDialog)
CImportLWSDlg::CImportLWSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportLWSDlg::IDD, pParent)
	, m_cstrLWSFile(_T(""))
{
}

CImportLWSDlg::~CImportLWSDlg()
{
}

void CImportLWSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cstrLWSFile);
	DDX_Control(pDX, IDC_RENDERABLE_CHECK, m_ctrlRenderableCheck);
	DDX_Control(pDX, IDC_NPATCHES_CHECK, m_ctrlNPatchesCheck);
	DDX_Control(pDX, IDC_DEFORMERS_CHECK, m_ctrlDeformersCheck);
	DDX_Control(pDX, IDC_MAX_VS_VERSION_COMBO, m_ctrlMaxVSVersionCombo);
	DDX_Control(pDX, IDC_SPACE_WARP_CHECK, m_ctrlSpaceWarpsCheck);
	DDX_Control(pDX, IDC_OUTPUT_GEOMETRY_CHECK, m_ctrlOutputGeometryCheck);
	DDX_Control(pDX, IDC_OUTPUT_MESH_CHECK, m_ctrlOutputMeshCheck);
	DDX_Control(pDX, IDC_OUTPUT_MATERIAL_CHECK, m_ctrlOutputMaterialCheck);
	DDX_Control(pDX, IDC_OUTPUT_TEXTURES_CHECK, m_ctrlOutputTexturesCheck);
	DDX_Control(pDX, IDC_OVERWRITE_GEOMETRY_CHECK, m_ctrlOverwriteGeometryCheck);
	DDX_Control(pDX, IDC_OVERWRITE_MESH_CHECK, m_ctrlOverwriteMeshCheck);
	DDX_Control(pDX, IDC_OVERWRITE_MATERIAL_CHECK, m_ctrlOverwriteMaterialCheck);
	DDX_Control(pDX, IDC_OVERWRITE_TEXTURES_CHECK, m_ctrlOverwriteTexturesCheck);
	DDX_Control(pDX, IDC_OVERWRITE_IMAGE_FILES_CHECK, m_ctrlOverwriteImagesCheck);
	DDX_Control(pDX, IDC_MAKE_TANGENT_SPACE_CHECK, m_ctrlMakeTangentSpaceCheck);
	DDX_Control(pDX, IDC_OUTPUT_SCENE_CHECK, m_ctrlOutputSceneCheck);
	DDX_Control(pDX, IDC_OVERWRITE_SCENE_FILES_CHECK, m_ctrlOverwriteSceneFileCheck);
}


BEGIN_MESSAGE_MAP(CImportLWSDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_FOR_LWS_BTN, OnBnClickedBrowseForLwsBtn)
END_MESSAGE_MAP()


// CImportLWSDlg message handlers

BOOL CImportLWSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//LWS name
	OnBnClickedBrowseForLwsBtn();

	//vs version
	m_ctrlMaxVSVersionCombo.AddString("0.0 - GF1");
	m_ctrlMaxVSVersionCombo.AddString("1.1\\96 - GF3");
	m_ctrlMaxVSVersionCombo.AddString("1.1\\192 - R8500");
	m_ctrlMaxVSVersionCombo.AddString("2.0\\256 - DX9 hw");
	m_ctrlMaxVSVersionCombo.AddString("3.0\\8192 - DX9 sw");
	m_ctrlMaxVSVersionCombo.SetCurSel(0);
	//default checks
	m_ctrlRenderableCheck.SetCheck(1);
	m_ctrlNPatchesCheck.SetCheck(1);
	m_ctrlOutputGeometryCheck.SetCheck(1);
	m_ctrlOutputMaterialCheck.SetCheck(1);
	m_ctrlOutputTexturesCheck.SetCheck(1);
	m_ctrlOutputMeshCheck.SetCheck(1);
	m_ctrlOutputSceneCheck.SetCheck(1);

	m_ctrlOverwriteGeometryCheck.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImportLWSDlg::OnBnClickedBrowseForLwsBtn()
{
	CFileDialog FileDialog(TRUE,NULL,NULL,0,"Lightwave 7.x LWS files (*.lws)|*.lws||");
	if(FileDialog.DoModal()==IDOK) {
		m_cstrLWSFile=FileDialog.GetPathName();
		UpdateData(FALSE);
	}
}

void CImportLWSDlg::OnOK()
{
	//buld importer structure
	UpdateData();
	LWSImportDescStruct ImportDesc;
	ZeroMemory(&ImportDesc,sizeof(LWSImportDescStruct));
	ImportDesc.bDeformers=m_ctrlDeformersCheck.GetCheck();
	ImportDesc.bMakeTangentSpace=m_ctrlMakeTangentSpaceCheck.GetCheck();
	ImportDesc.bNPatches=m_ctrlNPatchesCheck.GetCheck();
	ImportDesc.bOutputGeometry=m_ctrlOutputGeometryCheck.GetCheck();
	ImportDesc.bOutputMaterial=m_ctrlOutputMaterialCheck.GetCheck();
	ImportDesc.bOutputMesh=m_ctrlOutputMeshCheck.GetCheck();
	ImportDesc.bOutputTextures=m_ctrlOutputTexturesCheck.GetCheck();
	ImportDesc.bOutputScene=m_ctrlOutputSceneCheck.GetCheck();
	ImportDesc.bOverwriteGeometry=m_ctrlOverwriteGeometryCheck.GetCheck();
	ImportDesc.bOverwriteImageFiles=m_ctrlOverwriteImagesCheck.GetCheck();
	ImportDesc.bOverwriteMaterial=m_ctrlOverwriteMaterialCheck.GetCheck();
	ImportDesc.bOverwriteMesh=m_ctrlOverwriteMeshCheck.GetCheck();
	ImportDesc.bOverwriteTextures=m_ctrlOverwriteTexturesCheck.GetCheck();
	ImportDesc.bOverwriteSceneFile=m_ctrlOverwriteSceneFileCheck.GetCheck();
	ImportDesc.bRenderable=m_ctrlRenderableCheck.GetCheck();
	ImportDesc.bSpaceWarps=m_ctrlSpaceWarpsCheck.GetCheck();

	ImportDesc.dwLockabilityMask=0;
	strcpy(ImportDesc.szFileName,m_cstrLWSFile);
	strcpy(ImportDesc.szOutputRootDir,g_cstrFilesystemRoot);

	if(!importLWSFile(ImportDesc)) {
		AfxMessageBox("Failed to import LWS file!!!");
	}

	CDialog::OnOK();
}
