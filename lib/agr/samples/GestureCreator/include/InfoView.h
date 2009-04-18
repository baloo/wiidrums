#pragma once


// cInfoView

class cInfoView : public CView
{

protected:
	cInfoView();
	DECLARE_DYNCREATE(cInfoView)

	
// Attributes
public:
	cGestureCreatorDoc* GetDocument() const;

	
// Operations
public:

protected:
	void drawMovementInfo( CDC* pDC );
	void drawGestureInfo( CDC* pDC );
	void drawVocabularyInfo( CDC* pDC );
	void drawGRInfo( CDC* pDC );
	
	void drawMovementInfo( CDC* pDC,
						   const cMovement& movement, 
						   CString movName );

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
		
protected:
	virtual void OnInitialUpdate();

// Implementation
public:
	virtual ~cInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TreeInfoView.cpp
inline cGestureCreatorDoc* cInfoView::GetDocument() const
   { return reinterpret_cast<cGestureCreatorDoc*>(m_pDocument); }
#endif


