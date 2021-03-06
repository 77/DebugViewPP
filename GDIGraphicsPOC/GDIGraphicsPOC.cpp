#include "stdafx.h"
#include "windows.h"
#include <string>

#include "Win32/gdi.h"
#include "gdi.h"

#define WIN32_LEAN_AND_MEAN


#include <Shellapi.h>

#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>

#include <atlbase.h>        // Base ATL classes
#include <atlwin.h>         // ATL windowing classes
#include <atlsplit.h>
#include <atlframe.h>
#include "CobaltFusion/dbgstream.h"

namespace fusion
{

class CMainFrame :public CFrameWindowImpl<CMainFrame>
{
public:
	DECLARE_WND_CLASS(_T("CMainFrame Class"))

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MSG_WM_MOUSEWHEEL(OnMouseWheel)

	END_MSG_MAP()

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		cdbg << "CMainFrame::OnMouseWheel: " << zDelta << "\n";
		return TRUE;
	}

	void DisablePaneHeader(CPaneContainer& panecontainer)
	{
		panecontainer.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON, 0);
		panecontainer.m_cxyHeader = 0;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_split.Create(m_hWnd, rcDefault);
		m_hWndClient = m_split;

		m_top.Create(m_split, L"Top Pane");
		m_bottom.Create(m_split, L"");
		DisablePaneHeader(m_bottom);
		m_split.SetSplitterPanes(m_top, m_bottom, true);
		UpdateLayout();
		m_split.SetSplitterPos(600);
		//m_split.SetSinglePaneMode(SPLIT_PANE_TOP);  // this hides the bottom pane and the splitter

		m_timelineView.Create(m_bottom, rcDefault, gdi::CTimelineView::GetWndClassName(),
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | SS_OWNERDRAW);

		m_bottom.SetClient(m_timelineView);
		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		DestroyWindow();
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		PostQuitMessage(0);
		return 0;
	}

	CHorSplitterWindow m_split;
	CPaneContainer m_top;
	CPaneContainer m_bottom;
	gdi::CTimelineView m_timelineView;

};

} // namespace fusion


CComModule _Module;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
	LPSTR szCmdLine, int nCmdShow)
{
	fusion::CMainFrame wndMain;
	MSG msg;

	// Create & show our main window
	if (NULL == wndMain.Create(NULL, CWindow::rcDefault, _T("WTL Frame")))
	{
		// Bad news, window creation failed
		return 1;
	}

	wndMain.ShowWindow(nCmdShow);
	wndMain.UpdateWindow();

	// Run the message loop
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
