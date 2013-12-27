#pragma once


// CCopyPasteColorChoose dialog

class CCopyPasteColorChoose : public CDialog
{
	DECLARE_DYNAMIC(CCopyPasteColorChoose)

public:
	CCopyPasteColorChoose(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCopyPasteColorChoose();

// Dialog Data
	enum { IDD = IDD_COLOR_COPYPASTE_CHOOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
