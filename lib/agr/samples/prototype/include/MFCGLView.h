// MFCGLView.h : interface of the CMFCGLView class
//


#pragma once

class CMFCGLView : public CView
{
protected: // create from serialization only
	CMFCGLView();
	DECLARE_DYNCREATE(CMFCGLView)

// Attributes
public:
	CWiimoteTestDoc* GetDocument() const;
	void oglInitialize(void);
	
	void DrawScene(void);

// Operations
public:
	// Timer
	UINT_PTR m_unpTimer;
	float m_fPosX, m_fPosY, m_fPosZ, m_fRotX, m_fRotY, m_fLastX, m_fLastY;

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


protected:
      // Window information
      CWnd    *m_hWnd;
      HDC     m_hdc;
      HGLRC   m_hrc;
      int     m_nPixelFormat;
      CRect   m_rect;
      CRect   m_oldWindow;
      CRect   m_originalRect;

// Implementation
public:
	virtual ~CMFCGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in WiimoteTestView.cpp
inline CWiimoteTestDoc* CMFCGLView::GetDocument() const
   { return reinterpret_cast<CWiimoteTestDoc*>(m_pDocument); }
#endif

