// ButtonView.cpp : implementation file
//

#include "stdafx.h"
#include "GestureCreator.h"

#include "GestureCreatorDoc.h"
#include "ButtonView.h"
#include "TreeInfoView.h"
#include "NewGestureDlg.h"
#include "BaumWelchDlg.h"


// cButtonView

IMPLEMENT_DYNCREATE(cButtonView, CView)

BEGIN_MESSAGE_MAP(cButtonView, CView)
	ON_BN_CLICKED(ID_BUTTONADDCURRENTMOVE,	OnButtonAddCurrentMove )
	ON_BN_CLICKED(ID_BUTTONCREATEGESTURE,	OnButtonCreateGesture )
	ON_BN_CLICKED(ID_BUTTONCREATESPHERVOC,	OnButtonCreateSphericalVocabulary )
	ON_BN_CLICKED(ID_BUTTONBAUMWELCH,		OnButtonBaumWelch )
END_MESSAGE_MAP()


cButtonView::cButtonView()
{
	m_pButtonAddCurrentMove = NULL;
	m_pButtonCreateGesture = NULL;
	m_pButtonCreateSphericalVocabulary = NULL;
	m_pButtonBaumWelch = NULL;
}

cButtonView::~cButtonView()
{
	if( m_pButtonAddCurrentMove )
	{
		m_pButtonAddCurrentMove->DestroyWindow();
		delete m_pButtonAddCurrentMove;
		m_pButtonAddCurrentMove = NULL;
	}
	if( m_pButtonCreateGesture )
	{
		m_pButtonCreateGesture->DestroyWindow();
		delete m_pButtonCreateGesture;
		m_pButtonCreateGesture = NULL;
	}
	if( m_pButtonCreateSphericalVocabulary )
	{
		m_pButtonCreateSphericalVocabulary->DestroyWindow();
		delete m_pButtonCreateSphericalVocabulary;
		m_pButtonCreateSphericalVocabulary = NULL;
	}
	if( m_pButtonBaumWelch )
	{
		m_pButtonBaumWelch->DestroyWindow();
		delete m_pButtonBaumWelch;
		m_pButtonBaumWelch = NULL;
	}
}


void cButtonView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return ;
	pDoc->m_ptButtonView = this;

	m_pButtonAddCurrentMove = new CButton();
	m_pButtonAddCurrentMove->Create( "Add Current Movement", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(5, 170, 60, 20), this, ID_BUTTONADDCURRENTMOVE );
	m_pButtonAddCurrentMove->MoveWindow( 10, 10, 170, 20 );

	m_pButtonCreateGesture = new CButton();
	m_pButtonCreateGesture->Create( "Create Gesture", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(5, 170, 60, 20), this, ID_BUTTONCREATEGESTURE );
	m_pButtonCreateGesture->MoveWindow( 10, 35, 170, 20 );

	m_pButtonCreateSphericalVocabulary = new CButton();
	m_pButtonCreateSphericalVocabulary->Create( "Create spherical vocabulary", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(5, 170, 60, 20), this, ID_BUTTONCREATESPHERVOC );
	m_pButtonCreateSphericalVocabulary->MoveWindow( 10, 60, 210, 20 );

	m_pButtonBaumWelch = new CButton();
	m_pButtonBaumWelch->Create( "Baum-Welch re-esitmaton", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, CRect(5, 170, 60, 20), this, ID_BUTTONBAUMWELCH );
	m_pButtonBaumWelch->MoveWindow( 10, 85, 210, 20 );
}


// cButtonView diagnostics

#ifdef _DEBUG
void cButtonView::AssertValid() const
{
	CView::AssertValid();
}

void cButtonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

cGestureCreatorDoc* cButtonView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(cGestureCreatorDoc)));
	return (cGestureCreatorDoc*)m_pDocument;
}
#endif //_DEBUG


void cButtonView::OnDraw(CDC* pDC)
{
	CView::OnDraw( pDC );
}


afx_msg void cButtonView::OnButtonAddCurrentMove()
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	if( 0 != pDoc->m_currentMovement.getSize() )
	{
		pDoc->m_newMovementId++;
		CString str;
		str.Format( "Movement #%d", pDoc->m_newMovementId );

		pDoc->m_movementList[ str.GetBuffer() ].copy( pDoc->m_currentMovement );

		HTREEITEM item = pDoc->m_ptTreeInfoView->GetTreeCtrl().InsertItem( str, pDoc->m_ptTreeInfoView->m_movementTree );
		pDoc->m_ptTreeInfoView->GetTreeCtrl().SetItemData( item, (DWORD)cTreeInfoView::eTypeMovement );
	}
}


afx_msg void cButtonView::OnButtonCreateGesture()
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	if( pDoc->m_movementList.empty() )
		return;

	cNewGestureDlg myDlg( pDoc );
	myDlg.DoModal();
}


afx_msg void cButtonView::OnButtonBaumWelch()
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;
	if( pDoc->m_movementList.empty() )
		return;

	// We won't continue if no gesture can be used
	bool bSomeGesture = false;
	for( unsigned int i=0; i<pDoc->m_gestureRecogniser.getNumberGesture(); i++ )
	{
		if( pDoc->m_gestureRecogniser.getGesture(i)->isReady() ){
			bSomeGesture = true;
			break;
		}
	}
	if( !bSomeGesture )
		return;

	cBaumWelchDlg myDlg( pDoc );
	myDlg.DoModal();
}


afx_msg void cButtonView::OnButtonCreateSphericalVocabulary()
{
	cGestureCreatorDoc* pDoc = GetDocument();
	if (!pDoc)
		return;

	static const unsigned int bufferSize = 24;
	cVector3int vecBuffer[ bufferSize ];
	int radius = 60;

	static const float cosPi_4 =	 0.70710678f;	// cos(Pi/4)
	static const float sinPi_4 =	 cosPi_4;		// sin(Pi/4)
	static const float cos3Pi_4 =	-cosPi_4;		// cos(Pi*3/4)
	static const float sin3Pi_4 =	 cosPi_4;		// sin(Pi*3/4)
	static const float cos5Pi_4 =	-cosPi_4;		// cos(Pi*5/4)
	static const float sin5Pi_4 =	-cosPi_4;		// sin(Pi*5/4)
	static const float cos7Pi_4 =	 cosPi_4;		// cos(Pi*5/4)
	static const float sin7Pi_4 =	-cosPi_4;		// sin(Pi*5/4)

	// Center
	vecBuffer[  0 ] = cVector3int( 0, 0, 0 );

	// X/Y
	vecBuffer[  1 ] = cVector3int( radius, 0, 0 );
	vecBuffer[  2 ] = cVector3int( (int)(radius*cosPi_4), (int)(radius*sinPi_4), 0 );
	vecBuffer[  3 ] = cVector3int( 0, radius, 0 );
	vecBuffer[  4 ] = cVector3int( (int)(radius*cos3Pi_4), (int)(radius*sin3Pi_4), 0 );
	vecBuffer[  5 ] = cVector3int( -radius, 0, 0 );
	vecBuffer[  6 ] = cVector3int( (int)(radius*cos5Pi_4), (int)(radius*sin5Pi_4), 0 );
	vecBuffer[  7 ] = cVector3int( 0, -radius, 0 );
	vecBuffer[  8 ] = cVector3int( (int)(radius*cos7Pi_4), (int)(radius*sin7Pi_4), 0 );

	// X/Z
	vecBuffer[  9 ] = cVector3int( radius, 0, 0 );
	vecBuffer[ 10 ] = cVector3int( (int)(radius*cosPi_4), 0, (int)(radius*sinPi_4) );
	vecBuffer[ 11 ] = cVector3int( 0, 0, radius );
	vecBuffer[ 12 ] = cVector3int( (int)(radius*cos3Pi_4), 0, (int)(radius*sin3Pi_4) );
	vecBuffer[ 13 ] = cVector3int( -radius, 0, 0 );
	vecBuffer[ 14 ] = cVector3int( (int)(radius*cos5Pi_4), 0, (int)(radius*sin5Pi_4) );
	vecBuffer[ 15 ] = cVector3int( 0, 0, -radius );
	vecBuffer[ 16 ] = cVector3int( (int)(radius*cos7Pi_4), 0, (int)(radius*sin7Pi_4) );

	// Y/Z
	vecBuffer[ 17 ] = cVector3int( 0, radius, 0 );
	vecBuffer[ 18 ] = cVector3int( 0, (int)(radius*cosPi_4), (int)(radius*sinPi_4) );
	vecBuffer[ 19 ] = cVector3int( 0, 0, radius );
	vecBuffer[ 20 ] = cVector3int( 0, (int)(radius*cos3Pi_4), (int)(radius*sin3Pi_4) );
	vecBuffer[ 21 ] = cVector3int( 0, -radius, 0 );
	vecBuffer[ 22 ] = cVector3int( 0, (int)(radius*cos5Pi_4), (int)(radius*sin5Pi_4) );
	vecBuffer[ 23 ] = cVector3int( 0, 0, -radius );
	vecBuffer[ 23 ] = cVector3int( 0, (int)(radius*cos7Pi_4), (int)(radius*sin7Pi_4) );

	// Centerize the sphere
	cVector3int center( 127, 127, 127 );
	for( unsigned int i=0; i<bufferSize; i++ )
	{
		vecBuffer[ i ] += center;
	}

	cVocabulary myVoc;
	cKmeansClustering cluterer;
	myVoc.initialise( &cluterer, bufferSize, vecBuffer );
	static const char* sphericalVocName = "Spherical";

	tVocabularyId vocId = pDoc->m_gestureRecogniser.addVocabulary( myVoc, sphericalVocName );
	if( -1 == vocId )
	{
		if( false == pDoc->m_gestureRecogniser.resize( pDoc->m_gestureRecogniser.getNumberVocabulary()+1, 0) ) 
		{
			MessageBox( "Error when resizing the gesture recogniser" );
			return;
		}
		vocId = pDoc->m_gestureRecogniser.addVocabulary( myVoc, sphericalVocName );
		if( -1 == vocId )
		{
			MessageBox( "Error when adding the vocabulary" );
			return;
		}
	}
	HTREEITEM item = pDoc->m_ptTreeInfoView->GetTreeCtrl().InsertItem( sphericalVocName, pDoc->m_ptTreeInfoView->m_vocabularyTree );
	pDoc->m_ptTreeInfoView->GetTreeCtrl().SetItemData( item, (DWORD)cTreeInfoView::eTypeVocabulary );


}

// cButtonView message handlers

