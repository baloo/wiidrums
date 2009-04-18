#pragma once



#define ID_BUTTONADDCURRENTMOVE		(6001)
#define ID_BUTTONCREATEGESTURE		(6002)
#define ID_BUTTONCREATESPHERVOC		(6003)
#define ID_BUTTONBAUMWELCH			(6004)

// cButtonView

class cButtonView : public CView
{

protected:
	cButtonView();
	DECLARE_DYNCREATE(cButtonView)

	
// Attributes
public:
	cGestureCreatorDoc* GetDocument() const;
	
	CButton* m_pButtonAddCurrentMove;
	CButton* m_pButtonCreateGesture;
	CButton* m_pButtonCreateSphericalVocabulary;
	CButton* m_pButtonBaumWelch;

	
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

protected:
	virtual void OnInitialUpdate();

// Implementation
public:
	virtual ~cButtonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonAddCurrentMove();
	afx_msg void OnButtonCreateGesture();
	afx_msg void OnButtonCreateSphericalVocabulary();
	afx_msg void OnButtonBaumWelch();
};

#ifndef _DEBUG  // debug version in ButtonView.cpp
inline cGestureCreatorDoc* cButtonView::GetDocument() const
   { return reinterpret_cast<cGestureCreatorDoc*>(m_pDocument); }
#endif


