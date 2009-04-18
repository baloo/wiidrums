// MainFrm.h : interface of the cMainFrame class
//


#pragma once
class cMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	cMainFrame();
	DECLARE_DYNCREATE(cMainFrame)

// Attributes
public:
	CSplitterWnd	m_wndSplitter;
	inline CStatusBar* get_wndStatusBar() {return &m_wndStatusBar; }

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// Implementation
public:
	virtual ~cMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


