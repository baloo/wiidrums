// Movement3DView.h : interface of the cMovement3DView class
//


#pragma once

class cMovement3DView : public CView
{
protected: // create from serialization only
	cMovement3DView();
	DECLARE_DYNCREATE(cMovement3DView)

// Attributes
public:
	cGestureCreatorDoc* GetDocument() const;
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
	virtual ~cMovement3DView();
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
inline cGestureCreatorDoc* cMovement3DView::GetDocument() const
   { return reinterpret_cast<cGestureCreatorDoc*>(m_pDocument); }
#endif

