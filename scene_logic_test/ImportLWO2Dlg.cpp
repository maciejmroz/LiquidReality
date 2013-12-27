// ImportLWO2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "ImportLWO2Dlg.h"
#include "globals.h"
#include "LWOImport.h"

// CImportLWO2Dlg dialog

IMPLEMENT_DYNAMIC(CImportLWO2Dlg, CDialog)
CImportLWO2Dlg::CImportLWO2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportLWO2Dlg::IDD, pParent)
	, m_cstrLWO2File(_T(""))
	, m_cstrFLMBaseName(_T(""))
{
}

CImportLWO2Dlg::~CImportLWO2Dlg()
{
}

void CImportLWO2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_cstrLWO2File);
	DDX_Text(pDX, IDC_FLM_BASENAME_EDIT, m_cstrFLMBaseName);
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
}


BEGIN_MESSAGE_MAP(CImportLWO2Dlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE_FOR_LWO_BTN, OnBnClickedBrowseForLwoBtn)
END_MESSAGE_MAP()


// CImportLWO2Dlg message handlers

BOOL CImportLWO2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//LWO name
	OnBnClickedBrowseForLwoBtn();

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

	m_ctrlOverwriteGeometryCheck.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImportLWO2Dlg::OnBnClickedBrowseForLwoBtn()
{
	CFileDialog FileDialog(TRUE,NULL,NULL,0,"Lightwave 7.x LWO files (*.lwo)|*.lwo||");
	if(FileDialog.DoModal()==IDOK) {
		m_cstrLWO2File=FileDialog.GetPathName();
		m_cstrFLMBaseName=FileDialog.GetFileTitle();
		UpdateData(FALSE);
	}
}

void CImportLWO2Dlg::OnOK()
{
	//buld importer structure
	UpdateData();
	LWO2ImportDescStruct ImportDesc;
	ZeroMemory(&ImportDesc,sizeof(LWO2ImportDescStruct));
	ImportDesc.bDeformers=m_ctrlDeformersCheck.GetCheck();
	ImportDesc.bMakeTangentSpace=m_ctrlMakeTangentSpaceCheck.GetCheck();
	ImportDesc.bNPatches=m_ctrlNPatchesCheck.GetCheck();
	ImportDesc.bOutputGeometry=m_ctrlOutputGeometryCheck.GetCheck();
	ImportDesc.bOutputMaterial=m_ctrlOutputMaterialCheck.GetCheck();
	ImportDesc.bOutputMesh=m_ctrlOutputMeshCheck.GetCheck();
	ImportDesc.bOutputTextures=m_ctrlOutputTexturesCheck.GetCheck();
	ImportDesc.bOverwriteGeometry=m_ctrlOverwriteGeometryCheck.GetCheck();
	ImportDesc.bOverwriteImageFiles=m_ctrlOverwriteImagesCheck.GetCheck();
	ImportDesc.bOverwriteMaterial=m_ctrlOverwriteMaterialCheck.GetCheck();
	ImportDesc.bOverwriteMesh=m_ctrlOverwriteMeshCheck.GetCheck();
	ImportDesc.bOverwriteTextures=m_ctrlOverwriteTexturesCheck.GetCheck();
	ImportDesc.bRenderable=m_ctrlRenderableCheck.GetCheck();
	ImportDesc.bSpaceWarps=m_ctrlSpaceWarpsCheck.GetCheck();
	ImportDesc.dwLockabilityMask=0;

	strcpy(ImportDesc.szFileName,m_cstrLWO2File);
	strcpy(ImportDesc.szOutputFLMBaseName,m_cstrFLMBaseName);
	strcpy(ImportDesc.szOutputRootDir,g_cstrFilesystemRoot);

	LWO2ImportResult *pResult=new LWO2ImportResult();
	if(!importLWO2File(ImportDesc,pResult)) {
		AfxMessageBox("Failed to import LWO2 file!!!");
	}
	delete pResult;		//we don't really care about it

	CDialog::OnOK();
}
