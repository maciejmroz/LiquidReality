#pragma once


// CSaveMeshAsDlg dialog

class CSaveMeshAsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveMeshAsDlg)

public:
	CSaveMeshAsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveMeshAsDlg();

// Dialog Data
	enum { IDD = IDD_SAVE_MESH_AS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_cstrMeshName;
protected:
	virtual void OnOK();
};
