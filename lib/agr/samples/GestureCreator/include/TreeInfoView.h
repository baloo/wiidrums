// TreeInfoView.h : interface of the cTreeInfoView class
//


#pragma once


class cTreeInfoView : public CTreeView
{
protected: // create from serialization only
	cTreeInfoView();
	DECLARE_DYNCREATE(cTreeInfoView)

// Attributes
public:
	cGestureCreatorDoc* GetDocument() const;

	HTREEITEM m_currentMovement;
	HTREEITEM m_gestureTree;
	HTREEITEM m_vocabularyTree;
	HTREEITEM m_movementTree;

	enum eTreeInfoType
	{
		eTypeUnknown = 0,
		eTypeCurrentMoveRoot,
		eTypeGestureRoot,
		eTypeGesture,
		eTypeVocabularyRoot,
		eTypeVocabulary,
		eTypeMovementRoot,
		eTypeMovement,
	};

// Operations
public:
	void modifyGRTree();

// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~cTreeInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in ButtonView.cpp
inline cGestureCreatorDoc* cTreeInfoView::GetDocument() const
   { return reinterpret_cast<cGestureCreatorDoc*>(m_pDocument); }
#endif

