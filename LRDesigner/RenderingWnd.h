//basic CWindow derived class used to handle renderign window

#ifndef _RENDERING_WND_H_INCLUDED_
#define _RENDERING_WND_H_INCLUDED_

class CRenderingWindow : public CWindowImpl<CRenderingWindow> {
	BEGIN_MSG_MAP(CRenderingWindow)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	END_MSG_MAP()

	LRESULT OnDestroy( UINT, WPARAM, LPARAM, BOOL& ){
		PostQuitMessage( 0 );
		return 0;
	}

	LRESULT OnSysCommand( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		switch (wParam){
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				break;
			default:
				bHandled=FALSE;
				break;
		}
		return 0;
	}

	LRESULT OnCreate( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		if(m_bFullscreen) {
			SetWindowPos(HWND_TOPMOST,0,0,m_dwWidth,m_dwHeight,0);
		}
		return 0;
	}

	LRESULT OnGetMinMaxInfo( UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		LPMINMAXINFO pMinMax=(LPMINMAXINFO)lParam;
		pMinMax->ptMaxSize.x=m_dwWidth;
		pMinMax->ptMaxSize.y=m_dwHeight;
		pMinMax->ptMaxTrackSize.x=m_dwWidth;
		pMinMax->ptMaxTrackSize.y=m_dwHeight;
		pMinMax->ptMinTrackSize.x=m_dwWidth;
		pMinMax->ptMinTrackSize.y=m_dwHeight;
		return 0;
	}
protected:
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	BOOL	m_bFullscreen;
public:
	CRenderingWindow() :
		m_dwWidth(640),
		m_dwHeight(480),
		m_bFullscreen(FALSE) {
	}

	void init(DWORD dwWidth,DWORD dwHeight,BOOL bFullscreen) {
		m_dwWidth=dwWidth;
		m_dwHeight=dwHeight;
		m_bFullscreen=bFullscreen;
	}

	static ATL::CWndClassInfo& GetWndClassInfo()
	{
		static ATL::CWndClassInfo wc=
		{
			{ sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, StartWindowProc,
			0, 0, NULL, NULL, NULL, NULL, NULL, _T("LRRenderingWnd"), NULL },
			NULL, NULL, IDC_ARROW, TRUE, 0, _T("")
		};
		return wc;
	}
};

#endif