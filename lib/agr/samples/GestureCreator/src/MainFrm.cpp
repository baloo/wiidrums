// MainFrm.cpp : implementation of the cMainFrame class
//

#include "stdafx.h"
#include "GestureCreator.h"

#include "MainFrm.h"
#include "GestureCreatorDoc.h"
#include "Movement3DView.h"
#include "TreeInfoView.h"
#include "InfoView.h"
#include "ButtonView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// cMainFrame

IMPLEMENT_DYNCREATE(cMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(cMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// cMainFrame construction/destruction

cMainFrame::cMainFrame()
{
	// TODO: add member initialization code here
}

cMainFrame::~cMainFrame()
{
}


int cMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}


BOOL cMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CSize size;
	CRect rect;

	GetClientRect(&rect);
	size = rect.Size();

	m_wndSplitter.CreateStatic(this,2,2);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(cTreeInfoView),CSize((int)(size.cx*0.3), (int)(size.cy*0.7)),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(cMovement3DView),CSize(0, (int)(size.cy*0.7)),pContext);
	m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(cButtonView),CSize((int)(size.cx*0.3), 0),pContext);
	m_wndSplitter.CreateView(1,1,RUNTIME_CLASS(cInfoView),CSize(0, 0),pContext);

	return true;
}

BOOL cMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.style = FWS_ADDTOTITLE | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU |  WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}


// cMainFrame diagnostics

#ifdef _DEBUG
void cMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void cMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// cMainFrame message handlers

