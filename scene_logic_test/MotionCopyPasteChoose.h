#pragma once


// CMotionCopyPasteChoose dialog

class CMotionCopyPasteChoose : public CDialog
{
	DECLARE_DYNAMIC(CMotionCopyPasteChoose)

public:
	CMotionCopyPasteChoose(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionCopyPasteChoose();

// Dialog Data
	enum { IDD = IDD_MOTION_COPYPASTE_CHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
