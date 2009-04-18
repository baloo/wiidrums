// TreeInfoView.cpp : implementation of the cTreeInfoView class
//

#include "stdafx.h"
#include "GestureCreator.h"

#include "GestureCreatorDoc.h"
#include "TreeInfoView.h"
#include "InfoView.h"
#include "..\include\treeinfoview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// cTreeInfoView

IMPLEMENT_DYNCREATE(cTreeInfoView, CTreeView)

BEGIN_MESSAGE_MAP(cTreeInfoView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnTvnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
END_MESSAGE_MAP()

// cTreeInfoView construction/destruction

cTreeInfoView::cTreeInfoView()
{
	// TODO: add construction code here

}

cTreeInfoView::~cTreeInfoView()
{
}

BOOL cTreeInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void cTreeInfoView::OnInitialUpdate()
{
	
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	treeCtrl.SetIndent( 5 );
	treeCtrl.ModifyStyle( 0, TVS_HASLINES|TVS_LINESATROOT|TVS_DISABLEDRAGDROP|TVS_NOTOOLTIPS|TVS_HASBUTTONS );

	m_currentMovement = treeCtrl.InsertItem( _T("Current movement") );
	treeCtrl.SetItemData( m_currentMovement, (DWORD)eTypeCurrentMoveRoot );
	treeCtrl.SetItemState( m_currentMovement,	TVIS_EXPANDED, TVIS_EXPANDED );

	m_gestureTree = treeCtrl.InsertItem(  _T("Gestures") );
	treeCtrl.SetItemData( m_gestureTree, (DWORD)eTypeGestureRoot );
	treeCtrl.SetItemState( m_gestureTree,		TVIS_EXPANDED, TVIS_EXPANDED );

	m_vocabularyTree = treeCtrl.InsertItem( _T("Vocabularies") );
	treeCtrl.SetItemData( m_vocabularyTree, (DWORD)eTypeVocabularyRoot );
	treeCtrl.SetItemState( m_vocabularyTree,	TVIS_EXPANDED, TVIS_EXPANDED );

	m_movementTree = treeCtrl.InsertItem( _T("Movements") );
	treeCtrl.SetItemData( m_movementTree, (DWORD)eTypeMovementRoot );
	treeCtrl.SetItemState( m_movementTree,		TVIS_EXPANDED, TVIS_EXPANDED );

	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	pDoc->m_ptTreeInfoView = this;

	CTreeView::OnInitialUpdate();
}


void cTreeInfoView::modifyGRTree()
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	CTreeCtrl& treeCtrl = GetTreeCtrl();

	// Clean existing gestures/vocabulary

	HTREEITEM child = treeCtrl.GetChildItem( m_gestureTree );
	while( child )
	{
		treeCtrl.DeleteItem( child );
		child = treeCtrl.GetChildItem( m_gestureTree );
	}

	child = treeCtrl.GetChildItem( m_vocabularyTree );
	while( child )
	{
		treeCtrl.DeleteItem( child );
		child = treeCtrl.GetChildItem( m_vocabularyTree );
	}	

	// Add gesture/vocabulary loaded
	for( unsigned int i = 0; i<pDoc->m_gestureRecogniser.getNumberGesture(); i++ )
	{
		const char* gestureName = pDoc->m_gestureRecogniser.getGestureName( i );
		if( gestureName )
		{
			child = treeCtrl.InsertItem( gestureName, m_gestureTree );
			treeCtrl.SetItemData( child, (DWORD)cTreeInfoView::eTypeGesture );
		}
	}

	// Add gesture/vocabulary loaded
	for( unsigned int i = 0; i<pDoc->m_gestureRecogniser.getNumberVocabulary(); i++ )
	{
		const char* vocName = pDoc->m_gestureRecogniser.getVocabularyName( i );
		if( vocName ) 
		{
			child = treeCtrl.InsertItem( vocName, m_vocabularyTree );
			treeCtrl.SetItemData( child, (DWORD)cTreeInfoView::eTypeVocabulary );
		}
	}
}


// cTreeInfoView diagnostics

#ifdef _DEBUG
void cTreeInfoView::AssertValid() const
{
	CTreeView::AssertValid();
}

void cTreeInfoView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

cGestureCreatorDoc* cTreeInfoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(cGestureCreatorDoc)));
	return (cGestureCreatorDoc*)m_pDocument;
}
#endif //_DEBUG


// cTreeInfoView message handlers

void cTreeInfoView::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void cTreeInfoView::OnTvnKeydown(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);

	if( VK_DELETE == pTVKeyDown->wVKey )
	{
		cGestureCreatorDoc* pDoc = GetDocument();
		if (!pDoc)
			return;
		CTreeCtrl& treeCtrl = GetTreeCtrl();
		HTREEITEM hitem = treeCtrl.GetSelectedItem();
		eTreeInfoType itemType = (eTreeInfoType)treeCtrl.GetItemData(hitem);

		if( eTypeMovement == itemType )
		{
			std::string name = treeCtrl.GetItemText(hitem).GetBuffer();
			std::map<std::string,cMovement>::iterator iter;
			iter = pDoc->m_movementList.find( name );	
			if( iter != pDoc->m_movementList.end() )
			{
				pDoc->m_movementList.erase( iter );
			}
			treeCtrl.DeleteItem( hitem );
		}
	}

	*pResult = 0;
}

void cTreeInfoView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CTreeCtrl& treeCtrl = GetTreeCtrl();
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	eTreeInfoType itemType = (eTreeInfoType)treeCtrl.GetItemData(hitem);
	pDoc->m_selectedMovement = &pDoc->m_currentMovement;
	switch( itemType )
	{
		case eTypeCurrentMoveRoot:
			{
				pDoc->m_selectedMovement = &pDoc->m_currentMovement;
			}
			break;

		case eTypeMovement:
			{
				std::string name = treeCtrl.GetItemText(hitem).GetBuffer();
				std::map<std::string,cMovement>::iterator iter;
				iter = pDoc->m_movementList.find( name );
				if( iter != pDoc->m_movementList.end() )
				{
					pDoc->m_selectedMovement = &pDoc->m_movementList[ name ];
				}
			}
			break;
			

		case eTypeVocabulary:
			{
				tVocabularyId vocId = pDoc->m_gestureRecogniser.findVocabularyByName( treeCtrl.GetItemText(hitem).GetBuffer() );
				if( vocId != -1 )
				{
					cVocabulary* ptVoc = pDoc->m_gestureRecogniser.getVocabulary( vocId );
					pDoc->m_vocabularyMovement.clear();
					pDoc->m_vocabularyMovement.initialise( ptVoc->getSize(), ptVoc->getBuffer() );
					pDoc->m_selectedMovement = &pDoc->m_vocabularyMovement;
				}
			}
			break;
	}

	pDoc->m_ptInfoView->InvalidateRect( NULL );

	*pResult = 0;
}
