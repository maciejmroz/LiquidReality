#pragma once
#include "afxwin.h"


// CAddTransformNodeDlg dialog

class CAddTransformNodeDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddTransformNodeDlg)

public:
	CAddTransformNodeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddTransformNodeDlg();

// Dialog Data
	enum { IDD = IDD_ADD_TRANSFORM_NODE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CListBox m_ctrlSuperClassList;
	CListBox m_ctrlClassList;
	afx_msg void OnLbnSelchangeSuperclassList();
	// resulting transform node id
	DWORD m_dwNodeID;
};
