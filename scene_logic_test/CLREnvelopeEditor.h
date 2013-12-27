// CLREnvelopeEditor.h  : Declaration of ActiveX Control wrapper class(es) created by Microsoft Visual C++

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CLREnvelopeEditor

class CLREnvelopeEditor : public CWnd
{
protected:
	DECLARE_DYNCREATE(CLREnvelopeEditor)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xdebe34df, 0xb53a, 0x4576, { 0xbb, 0xf, 0x9a, 0x34, 0xb6, 0x38, 0x99, 0xfb } };
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

	void setWorkMode(long lMode)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lMode);
	}
	long getEnvelope()
	{
		long result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	void invalidate()
	{
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	long getActiveKey()
	{
		long result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	float getVerticalRange()
	{
		float result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_R4, (void*)&result, NULL);
		return result;
	}
	void setTimeRange(float fStartTime, float fEndTime)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fStartTime, fEndTime);
	}
	void setCurveColor(unsigned char Red, unsigned char Green, unsigned char Blue)
	{
		static BYTE parms[] = VTS_UI1 VTS_UI1 VTS_UI1 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Red, Green, Blue);
	}
	void setVerticalRange(float fMin, float fMax)
	{
		static BYTE parms[] = VTS_R4 VTS_R4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, fMin, fMax);
	}
	void recalcVerticalRange()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
