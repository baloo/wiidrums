// WiimoteTestView.cpp : implementation of the CWiimoteTestView class
//

#include "stdafx.h"
#include "WiimoteTest.h"


#include <vector>

#include "WiiAccelerometer.h"

#include "WiimoteTestDoc.h"
#include "WiimoteTestView.h"
#include ".\wiimotetestview.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWiimoteTestView

IMPLEMENT_DYNCREATE(CWiimoteTestView, CScrollView)

BEGIN_MESSAGE_MAP(CWiimoteTestView, CScrollView)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CWiimoteTestView construction/destruction

CWiimoteTestView::CWiimoteTestView()
{
}

CWiimoteTestView::~CWiimoteTestView()
{
}

BOOL CWiimoteTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
 
	return CScrollView::PreCreateWindow(cs);
}


// CWiimoteTestView drawing

void CWiimoteTestView::OnDraw(CDC* pDC)
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

//	CScrollView::SetScrollSizes( 

	// TEST, hardcoded
	// TODO : proper menus

	// display curent value
	CString text;
	text.Format( "Wiimote 1 acc : x = %d, y = %d, z = %d, sqDist = %d", 
		pDoc->m_acc1_x_array[ pDoc->m_arrayLast1 ],
		pDoc->m_acc1_y_array[ pDoc->m_arrayLast1 ],
		pDoc->m_acc1_z_array[pDoc-> m_arrayLast1 ], 
		pDoc->m_wiimote1SquareDist
		);
	CRect rect = CRect( 10, 0, 1000, 100 );
	pDC->DrawText( text, &rect, 0 );

#if USE_OLD_CODE

	// View actual
	pDC->Rectangle( 10, 20, 350, 70 );
	rect = CRect( 15, 25, 1000, 100 );
	if( pDoc->m_movementVocabulary.empty() )
	{
		text.Format( "Current move, size = %d", pDoc->m_lastMovement.size()	);
		pDC->DrawText( text, &rect, 0 );
	}
	else
	{
		text.Format( "Current move, size = %d, prob %f", pDoc->m_lastMovement.size(), pDoc->forward( &(pDoc->m_lastMovement) ) );	
		pDC->DrawText( text, &rect, 0 );

		rect = CRect( 15, 45, 1000, 100 );
		text.Format( "Clustered : " );
		pDoc->m_lastMovementVocabulary.resize( pDoc->m_normalisedVectorSize );
		pDoc->computeObservationCluster( &(pDoc->m_lastMovement), &(pDoc->m_lastMovementVocabulary) );
		for( int i=0; i<pDoc->m_normalisedVectorSize; i++ )
		{
			text.Format( "%s %d", text, pDoc->m_lastMovementVocabulary[i] );
		}
		pDC->DrawText( text, &rect, 0 );
	}

	// Record actual
	pDC->Rectangle( 10, 69, 350, 100 );
	rect = CRect( 15, 72, 1000, 100 );
	text.Format( "Add current to the list" );
	pDC->DrawText( text, &rect, 0 );


	for( unsigned int i=0; i<pDoc->m_movementVector.size(); i++ )
	{
		pDC->Rectangle( 10, i*60+130, 350, i*60+180 );
		rect = CRect( 15, i*60+132, 1000, i*60+180 );
		if( pDoc->m_movementVectorProb.size() > 0 )
		{
			text.Format( "Move %d, length %d, prob %f ", i+1, pDoc->m_movementVector[i].size(), pDoc->m_movementVectorProb[i] );
		}
		else
		{
			text.Format( "Move %d, length %d", i+1, pDoc->m_movementVector[i].size() );
		}
		pDC->DrawText( text, &rect, 0 );

		
		if( !pDoc->m_movementVocabulary.empty() )
		{	
			rect = CRect( 15, i*60+150, 1000, i*60+180 );
			text.Format( "Clusterised : " );
			for( int j=0; j<pDoc->m_normalisedVectorSize; j++ )
			{
				text.Format( "%s %d", text, pDoc->m_movementVocabulary[ i ][ j ] );
			}			
			pDC->DrawText( text, &rect, 0 );
		}

	}

#else // New version using AGR

	// View actual
	pDC->Rectangle( 10, 20, 350, 70 );
	rect = CRect( 15, 25, 1000, 100 );
	if( pDoc->m_movementVocabulary.empty() )
	{
		text.Format( "Current move, size = %d", pDoc->m_GRcurrentMovement.getSize()	);
		pDC->DrawText( text, &rect, 0 );
	}
	else // not yet implemented
	{
		text.Format( "Current move, size = %d, prob %f", 
					 pDoc->m_GRcurrentMovement.getSize(), 
					 pDoc->m_gestureRecogniser.gestureLikelihood( 0, pDoc->m_GRcurrentMovement )
					 );

		pDC->DrawText( text, &rect, 0 );

		//rect = CRect( 15, 45, 1000, 100 );
		//text.Format( "Clustered : " );
		//pDoc->m_lastMovementVocabulary.resize( pDoc->m_normalisedVectorSize );
		//pDoc->computeObservationCluster( &(pDoc->m_lastMovement), &(pDoc->m_lastMovementVocabulary) );
		//for( int i=0; i<pDoc->m_normalisedVectorSize; i++ )
		//{
		//	text.Format( "%s %d", text, pDoc->m_lastMovementVocabulary[i] );
		//}
		//pDC->DrawText( text, &rect, 0 );
	}

	// Record actual
	pDC->Rectangle( 10, 69, 350, 100 );
	rect = CRect( 15, 72, 1000, 100 );
	text.Format( "Add current to the list" );
	pDC->DrawText( text, &rect, 0 );


	for( unsigned int i=0; i<pDoc->m_GRmovementVector.size(); i++ )
	{
		pDC->Rectangle( 10, i*60+130, 350, i*60+180 );
		rect = CRect( 15, i*60+132, 1000, i*60+180 );
		if( pDoc->m_movementVectorProb.size() > i )
		{
			text.Format( "Move %d, length %d, prob %f ", i+1, pDoc->m_GRmovementVector[i].getSize(), pDoc->m_movementVectorProb[i] );
		}
		else
		{
			text.Format( "Move %d, length %d", i+1, pDoc->m_GRmovementVector[i].getSize() );
		}
		pDC->DrawText( text, &rect, 0 );

		
		if( !pDoc->m_movementVocabulary.empty() )
		{	
			rect = CRect( 15, i*60+150, 1000, i*60+180 );
			text.Format( "Clusterised : " );
			for( int j=0; j<pDoc->m_movementVocabulary[ i ].size(); j++ )
			{
				text.Format( "%s %d", text, pDoc->m_movementVocabulary[ i ][ j ] );
			}			
			pDC->DrawText( text, &rect, 0 );
		}

	}

#endif

	//// Display graph
	//// wiimote 1, x
	//unsigned int startPoint = pDoc->m_arrayLast1+1;
	//if( startPoint == SIZE_ACC_ARRAY )
	//	startPoint = 0;
	//static const int posGraph1_x = 0;
	//static const int posGraph1_y = 50;
	//for( int x=0; x<SIZE_ACC_ARRAY; x++ )
	//{
	//	int i = (startPoint+x) % SIZE_ACC_ARRAY;
	//	if( i == startPoint )
	//		pDC->MoveTo( x + posGraph1_x, pDoc->m_acc1_x_array[i] + posGraph1_y );
	//	else
	//		pDC->LineTo( x + posGraph1_x, pDoc->m_acc1_x_array[i] + posGraph1_y );
	//}

	//// wiimote 1, y
	//static const int posGraph2_x = 0;
	//static const int posGraph2_y = 250;
	//for( int x=0; x<SIZE_ACC_ARRAY; x++ )
	//{
	//	int i = (startPoint+x) % SIZE_ACC_ARRAY;
	//	if( i == startPoint )
	//		pDC->MoveTo( x + posGraph2_x, pDoc->m_acc1_y_array[i] + posGraph2_y );
	//	else
	//		pDC->LineTo( x + posGraph2_x, pDoc->m_acc1_y_array[i] + posGraph2_y );
	//}

	//// wiimote 1, z
	//static const int posGraph3_x = 0;
	//static const int posGraph3_y = 450;
	//for( int x=0; x<SIZE_ACC_ARRAY; x++ )
	//{
	//	int i = (startPoint+x) % SIZE_ACC_ARRAY;
	//	if( i == startPoint )
	//		pDC->MoveTo( x + posGraph3_x, pDoc->m_acc1_z_array[i] + posGraph3_y );
	//	else
	//		pDC->LineTo( x + posGraph3_x, pDoc->m_acc1_z_array[i] + posGraph3_y );
	//}
}


void CWiimoteTestView::saveMoves()
{

	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	FILE* f = fopen( "movement.txt", "w" );

#if USE_OLD_CODE

	int nbMove = pDoc->m_movementVector.size();
	fprintf( f, "%d \n", nbMove );

	for( int i=0; i<nbMove; i++ )
	{
		fprintf( f, "%d \n", pDoc->m_movementVector[i].size() );
		std::vector<cVector3int>::iterator iter;
		for( iter = pDoc->m_movementVector[i].begin(); iter != pDoc->m_movementVector[i].end(); iter++ )
		{
			fprintf( f, "%d %d %d ", iter->x, iter->y, iter->z );
		}
		fprintf( f, "\n" );
	}

#else // use GR

	unsigned int nbMove = pDoc->m_GRmovementVector.size();
	fprintf( f, "%d \n", nbMove );

	for( unsigned int i=0; i<nbMove; i++ )
	{
		cMovement* ptMov = &pDoc->m_GRmovementVector[i];
		fprintf( f, "%d \n", ptMov->getSize() );
		for( unsigned int i=0; i< ptMov->getSize(); i++ )
		{
			cVector3int vec = ptMov->getData( i );
			fprintf( f, "%d %d %d ", vec.x, vec.y, vec.z );
		}
		fprintf( f, "\n" );
	}

#endif

	fclose( f );
	MessageBox( "Saved" );
}


void CWiimoteTestView::loadMoves()
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	FILE* f = fopen( "movement.txt", "r" );

#if USE_OLD_CODE

	int nbMove = 0;
	fscanf( f, "%d", &nbMove );
	pDoc->m_movementVector.resize( nbMove );

	for( int i=0; i<nbMove; i++ )
	{
		int nbPoint = 0;
		fscanf( f, "%d", &nbPoint );
		std::vector<cVector3int>::iterator iter;
		for( int j=0; j<nbPoint; j++ )
		{
			cVector3int vec;
			fscanf( f, "%d %d %d", &vec.x, &vec.y, &vec.z );
			pDoc->m_movementVector[i].push_back( vec );
		}
	}

#else // use GR

	int nbMove = 0;
	fscanf( f, "%d", &nbMove );
	pDoc->m_GRmovementVector.resize( nbMove );

	for( int i=0; i<nbMove; i++ )
	{
		unsigned int nbPoint = 0;
		fscanf( f, "%d", &nbPoint );
		pDoc->m_GRmovementVector[i].initialise( nbPoint );
		cVector3int* buffer = pDoc->m_GRmovementVector[i].modifyBuffer();
		for( unsigned int j=0; j<nbPoint; j++ )
		{
			fscanf( f, "%d %d %d", &buffer[j].x, &buffer[j].y, &buffer[j].z );
		}
	}

#endif

	fclose( f );
}


// CWiimoteTestView diagnostics

#ifdef _DEBUG
void CWiimoteTestView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWiimoteTestView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWiimoteTestDoc* CWiimoteTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWiimoteTestDoc)));
	return (CWiimoteTestDoc*)m_pDocument;
}
#endif //_DEBUG


// TODO : remove or clean
// used for tests
float time()
{
    static __int64 start = 0;
    static __int64 frequency = 0;

    if (start==0)
    {
        QueryPerformanceCounter((LARGE_INTEGER*)&start);
        QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
        return 0.0f;
    }

    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return (float) ((counter - start) / double(frequency));
}

// CWiimoteTestView message handlers

void CWiimoteTestView::OnTimer(UINT nIDEvent)
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CSize sizeTotal;
	sizeTotal.cx = 355;
	sizeTotal.cy = pDoc->m_movementVector.size()*60+190;
	SetScrollSizes(MM_TEXT, sizeTotal);

	InvalidateRect( NULL );
	SetTimer(1, 100, 0 );

	CScrollView::OnTimer(nIDEvent);
}

void CWiimoteTestView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);

	SetTimer( 1, 100, 0 );
}

void CWiimoteTestView::OnLButtonDown(UINT nFlags, CPoint inPoint)
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPoint point = inPoint;
	point.y += GetScrollPos( SB_VERT );

	
#if USE_OLD_CODE
	CRect rect;
	rect = CRect( 10, 20, 350, 70 );
	if( rect.PtInRect( point ) )
	{
		pDoc->m_selectedMovement = &(pDoc->m_lastMovement);
		pDoc->m_selectedId = -1;
	}
	
	rect = CRect( 10, 69, 350, 100 );
	if( rect.PtInRect( point ) )
	{
		pDoc->m_movementVector.push_back( pDoc->m_lastMovement );
		if( pDoc->m_selectedId >= 0 )
			pDoc->m_selectedMovement = &(pDoc->m_movementVector[pDoc->m_selectedId]);

	}

	// tuch one of the recorded movement ?
	for( unsigned int i=0; i<pDoc->m_movementVector.size(); i++ )
	{
		rect = CRect( 10, i*60+130, 350, i*60+180 );
		if( rect.PtInRect( point ) )
		{
			pDoc->m_selectedMovement = &(pDoc->m_movementVector[i]);
			pDoc->m_selectedId = i;
		}
	}

#else // using GR

	CRect rect;
	rect = CRect( 10, 20, 350, 70 );
	if( rect.PtInRect( point ) )
	{
		pDoc->m_GRselectedMovement = &(pDoc->m_GRcurrentMovement);
		pDoc->m_selectedId = -1;
	}
	
	rect = CRect( 10, 69, 350, 100 );
	if( rect.PtInRect( point ) )
	{
		pDoc->m_GRmovementVector.push_back( pDoc->m_GRcurrentMovement );
		if( pDoc->m_selectedId >= 0 )
			pDoc->m_GRselectedMovement = &(pDoc->m_GRmovementVector[pDoc->m_selectedId]);

	}

	// tuch one of the recorded movement ?
	for( unsigned int i=0; i<pDoc->m_GRmovementVector.size(); i++ )
	{
		rect = CRect( 10, i*60+130, 350, i*60+180 );
		if( rect.PtInRect( point ) )
		{
			pDoc->m_GRselectedMovement = &(pDoc->m_GRmovementVector[i]);
			pDoc->m_selectedId = i;
		}
	}
#endif


	CScrollView::OnLButtonDown(nFlags, point);
}





void CWiimoteTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CWiimoteTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	switch( nChar )
	{
		case 'T' :
#if USE_OLD_CODE
			pDoc->testOld();
			pDoc->calculateProba();
			pDoc->calculateVoca();
#else // GR code
			pDoc->test();
			pDoc->calculateProba();
			pDoc->calculateVoca();
#endif
			break;
			
		case 'E' :
#if USE_OLD_CODE
			pDoc->exportHMM();
#else // GR code
			pDoc->m_gestureRecogniser.save( "GR.txt" );
#endif
			break;

		
		case 'B' :
#if USE_OLD_CODE
			pDoc->BaumWelchOptimisation();
			pDoc->calculateProba();
#endif
			break;

		case 'W' :
			saveMoves();
			break;

		case 'X' :
			loadMoves();
			break;

	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
