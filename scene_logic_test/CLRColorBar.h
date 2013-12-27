// CLRColorBar.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CLRColorBar

class CLRColorBar : public CWnd
{
protected:
	DECLARE_DYNCREATE(CLRColorBar)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xaeb9981e, 0x773a, 0x4ca2, { 0x83, 0xcb, 0x8b, 0x74, 0xb3, 0x30, 0x54, 0xb } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:

// Operations
public:

	void setColor(LPUNKNOWN pILRColor)
	{
		static BYTE parms[] = VTS_UNKNOWN ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pILRColor);
	}
	void setRange(float fStartTime, float fEndTime)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fStartTime, fEndTime);
	}
	void invalidate()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
