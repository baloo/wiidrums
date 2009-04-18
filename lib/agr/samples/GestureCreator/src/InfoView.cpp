// InfoView.cpp : implementation file
//

#include "stdafx.h"
#include "GestureCreator.h"

#include "GestureCreatorDoc.h"
#include "InfoView.h"
#include "TreeInfoView.h"


// cInfoView

IMPLEMENT_DYNCREATE(cInfoView, CView)

BEGIN_MESSAGE_MAP(cInfoView, CView)
END_MESSAGE_MAP()


cInfoView::cInfoView()
{
}

cInfoView::~cInfoView()
{
}


void cInfoView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;
	pDoc->m_ptInfoView = this;
}


// cInfoView diagnostics

#ifdef _DEBUG
void cInfoView::AssertValid() const
{
	CView::AssertValid();
}

void cInfoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

cGestureCreatorDoc* cInfoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(cGestureCreatorDoc)));
	return (cGestureCreatorDoc*)m_pDocument;
}
#endif //_DEBUG


void cInfoView::OnDraw(CDC* pDC)
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;
	
	HTREEITEM hTreeItem = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetSelectedItem();
	cTreeInfoView::eTreeInfoType treeItemType = 
		(cTreeInfoView::eTreeInfoType)pDoc->m_ptTreeInfoView->GetTreeCtrl().GetItemData( hTreeItem );
	
	switch( treeItemType )
	{
		case cTreeInfoView::eTypeMovement:
			drawMovementInfo( pDC );
			break;
		case cTreeInfoView::eTypeCurrentMoveRoot:
			drawMovementInfo( pDC, pDoc->m_currentMovement, "Current move" );
			break;
		case cTreeInfoView::eTypeVocabulary:
			drawVocabularyInfo( pDC );
			break;
		case cTreeInfoView::eTypeGesture:
			drawGestureInfo( pDC );
			break;
		default:
			drawGRInfo( pDC );
	}


	CView::OnDraw( pDC );
}


void cInfoView::drawMovementInfo( CDC* pDC )
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;

	HTREEITEM hTreeItem = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetSelectedItem();
	CString movName = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetItemText( hTreeItem );

	std::map<std::string,cMovement>::iterator iter;
	iter = pDoc->m_movementList.find( movName.GetBuffer() );
	if( pDoc->m_movementList.end() == iter ){
		return;
	}
	cMovement& movement = iter->second;

	drawMovementInfo( pDC, movement, movName );
}


void cInfoView::drawMovementInfo( CDC* pDC,
								  const cMovement& movement, 
								  CString movName )
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;

	// Movement name & size
	CString msg;
	msg.Format( "%s , size = %d", movName, movement.getSize() );
	CRect rect( 5, 10, 2000, 50 );
	pDC->DrawText( msg, &rect, 0 );

	int yPos = 40;

	// Movement probability
	for( unsigned int i = 0; i<pDoc->m_gestureRecogniser.getNumberGesture(); i++ )
	{
		const char* gestureName = pDoc->m_gestureRecogniser.getGestureName( i );
		const cGesture* gesture = pDoc->m_gestureRecogniser.getGesture( i );
		if( gesture				&&
			gesture->isReady()	)
		{
			CString name;
			if( gestureName ) {
				name.Format( "%s", gestureName );
			}
			else {
				name.Format( "Gesture %d ", i );
			}


			float probability =pDoc->m_gestureRecogniser.gestureLikelihood( i, movement );
			bool recognised = pDoc->m_gestureRecogniser.recogniseGesture( i, movement );
			if( recognised ) {
				msg.Format( "%s : Recognised (%f)", name, probability );
			}
			else {
				msg.Format( "%s : Not recognised (%f)", name, probability );
			}
			CRect rect( 5, yPos, 2000, yPos+20 );
			pDC->DrawText( msg, &rect, 0 );
			yPos+= 20;

			// Draw sequence
			msg.GetBufferSetLength( 100 );
			msg.Format( "Observation sequence : " );
			unsigned int* observations = new unsigned int[ gesture->getObservationSize() ];
			gesture->computeObservationSequence( movement, observations, gesture->getObservationSize() );
			for( unsigned int i=0; i<gesture->getObservationSize(); i++ )
			{
				msg.Format( "%s %d", msg, observations[i] );
			}
			rect = CRect( 5, yPos, 2000, yPos+20 );
			pDC->DrawText( msg, &rect, 0 );
			delete [] observations;
			yPos+= 20;
		}
	}
}


void cInfoView::drawGestureInfo( CDC* pDC )
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;

	HTREEITEM hTreeItem = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetSelectedItem();
	CString gestName = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetItemText( hTreeItem );
	tGestureId gestureId = pDoc->m_gestureRecogniser.findGestureByName( gestName.GetBuffer() );
	if( -1 == gestureId ) {
		return;
	}
	const cGesture* ptGesture = pDoc->m_gestureRecogniser.getGesture( gestureId );
	
	CString msg;
	msg.Format( "Gesture \"%s\"", gestName );
	CRect rect( 5, 10, 2000, 50 );
	pDC->DrawText( msg, &rect, 0 );
	
	// Find the vocabulary name
	for( unsigned int i = 0; i<pDoc->m_gestureRecogniser.getNumberVocabulary(); i++ )
	{
		cVocabulary* voc = pDoc->m_gestureRecogniser.getVocabulary( i );
		if( ptGesture->getVocabulary() == voc )
		{
			msg.Format( "Vocabulary: %s", pDoc->m_gestureRecogniser.getVocabularyName(i) );
			rect = CRect( 5, 30, 2000, 50 );
			pDC->DrawText( msg, &rect, 0 );
			break;
		}
	}
	
	msg.Format( "Size movement normalised %d", ptGesture->getObservationSize() );
	rect = CRect( 5, 50, 2000, 70 );
	pDC->DrawText( msg, &rect, 0 );
}


void cInfoView::drawVocabularyInfo( CDC* pDC )
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;

	HTREEITEM hTreeItem = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetSelectedItem();
	CString vocName = pDoc->m_ptTreeInfoView->GetTreeCtrl().GetItemText( hTreeItem );
	tVocabularyId vocId = pDoc->m_gestureRecogniser.findVocabularyByName( vocName.GetBuffer() );
	if( -1 == vocId ){
		return;
	}
	const cVocabulary* ptVoc = pDoc->m_gestureRecogniser.getVocabulary( vocId );
	
	CString msg;
	msg.Format( "Vocabulary \"%s\"", vocName );
	CRect rect( 5, 10, 2000, 30 );
	pDC->DrawText( msg, &rect, 0 );
	
	msg.Format( "Size %d", ptVoc->getSize() );
	rect = CRect( 5, 30, 2000, 50 );
	pDC->DrawText( msg, &rect, 0 );

}


void cInfoView::drawGRInfo( CDC* pDC )
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;

	CString msg;
	msg.Format( "Gesture recognition project \"%s\"", pDoc->GetTitle() );
	CRect rect( 5, 10, 2000, 30 );
	pDC->DrawText( msg, &rect, 0 );

}

