// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "WiimoteTest.h"

#include "MainFrm.h"
#include ".\mainfrm.h"


#include "WiiAccelerometer.h"

#include "WiimoteTestDoc.h"
#include "WiimoteTestView.h"
#include "MFCGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CSize size;
	CRect rect;

	GetClientRect(&rect);
	size = rect.Size();

	m_wndSplitter.CreateStatic(this,1,2);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CWiimoteTestView),CSize((int)(size.cx*0.4), 0),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CMFCGLView),CSize(0, 0),pContext);

	return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = FWS_ADDTOTITLE | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU |  WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

