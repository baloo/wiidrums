// WiimoteTestView.h : interface of the CWiimoteTestView class
//


#pragma once

class CWiimoteTestView : public CScrollView
{
protected: // create from serialization only
	CWiimoteTestView();
	DECLARE_DYNCREATE(CWiimoteTestView)

// Attributes
public:
	CWiimoteTestDoc* GetDocument() const;

// Operations
public:
	void saveMoves();
	void loadMoves();

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CWiimoteTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in WiimoteTestView.cpp
inline CWiimoteTestDoc* CWiimoteTestView::GetDocument() const
   { return reinterpret_cast<CWiimoteTestDoc*>(m_pDocument); }
#endif

