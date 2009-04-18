// GestureCreatorDoc.cpp : implementation of the cGestureCreatorDoc class
//

#include "stdafx.h"
#include "GestureCreator.h"

#include "GestureCreatorDoc.h"
#include "TreeInfoView.h"
#include "InfoView.h"
#include "..\include\gesturecreatordoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

cGestureCreatorDoc* g_Doc = NULL;

// cGestureCreatorDoc

IMPLEMENT_DYNCREATE(cGestureCreatorDoc, CDocument)

BEGIN_MESSAGE_MAP(cGestureCreatorDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
END_MESSAGE_MAP()


// cGestureCreatorDoc construction/destruction

cGestureCreatorDoc::cGestureCreatorDoc()
{
	cWiimoteManager::get()->update();
	cWiimote* wiimote = cWiimoteManager::get()->getWiimote( 1 );
	m_accWii.init( wiimote );
	m_accDevice = &m_accWii;

	m_ptTreeInfoView = NULL;
	m_ptMovement3DView = NULL;
	m_ptButtonView = NULL;
	m_ptInfoView = NULL; 

	g_Doc = this;
}

cGestureCreatorDoc::~cGestureCreatorDoc()
{
	cWiimoteManager::releaseManager();
	g_Doc = NULL;
}

BOOL cGestureCreatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_gestureRecogniser.clear();
	m_gestureRecogniser.init( 5, 5 );	// Initialise the GR with 5 vocabularies and gestures
										// But since this application is aimed to create the GR, we may increase this size later
	
	m_gestureRecogniser.getMovementRecogniser()->initialise( 1, 50 );
	m_selectedMovement = &m_currentMovement;
	
	memset( m_accelerationBuffer, 0x00, sizeof( cVector3int)*s_accelarationBufferSize );
	m_bufferAccelerationLast = 0;
	m_accelerationBuffer[ m_bufferAccelerationLast ] = cVector3int( 127, 127, 127 );

	if( !m_movementList.empty() )
		m_movementList.clear();
	m_newMovementId = 0;
	if( !m_movementList.empty() )
		m_movementList.clear();

	m_gestureFileName = "";
	if( m_ptTreeInfoView )
		m_ptTreeInfoView->modifyGRTree();

	return TRUE;
}




// cGestureCreatorDoc serialization

void cGestureCreatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// cGestureCreatorDoc diagnostics

#ifdef _DEBUG
void cGestureCreatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void cGestureCreatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void cGestureCreatorDoc::updateAccelerometer()
{
	cWiimoteManager::get()->update();
	m_accDevice->update();
	
	if( m_accDevice->m_rawAcceleration.x == 0 &&
		m_accDevice->m_rawAcceleration.y == 0 &&
		m_accDevice->m_rawAcceleration.z == 0 )
	{
		return;
	}

	cVector3int newVector( m_accDevice->m_rawAcceleration.x, m_accDevice->m_rawAcceleration.y, m_accDevice->m_rawAcceleration.z );
	
	if( ++(m_bufferAccelerationLast) == s_accelarationBufferSize )
		m_bufferAccelerationLast = 0;
	m_accelerationBuffer[ m_bufferAccelerationLast ] = newVector;


	cMovementRecogniser* ptMovRec = m_gestureRecogniser.getMovementRecogniser();
	cMovementRecogniser::eMovementState moveState;

	moveState = ptMovRec->addAcceleration( 0, newVector );	
	if( cMovementRecogniser::eMovementFinished == moveState )
	{
		ptMovRec->getMovement( 0, &m_currentMovement );

		// Check for necessary information update
		HTREEITEM treeItem = m_ptTreeInfoView->GetTreeCtrl().GetSelectedItem();
		cTreeInfoView::eTreeInfoType type = 
			(cTreeInfoView::eTreeInfoType)m_ptTreeInfoView->GetTreeCtrl().GetItemData( treeItem );
		if( cTreeInfoView::eTypeCurrentMoveRoot == type ){
			m_ptInfoView->InvalidateRect( NULL );
		}
	}
}

// cGestureCreatorDoc commands

void cGestureCreatorDoc::OnFileOpen()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "Gesture Files |*.bin;*.txt||", NULL, 0);
	
	if( fileDlg.DoModal ()==IDOK )
	{
		m_gestureRecogniser.clear();
		m_gestureRecogniser.getMovementRecogniser()->initialise( 1, 50 );
		bool bRes = m_gestureRecogniser.load( fileDlg.GetPathName().GetBuffer() );
		if( false == bRes )
		{
			MessageBox( NULL, "Gesture loading failed", "GestureCreator", MB_OK|MB_ICONERROR );
		}
		else {
			m_gestureFileName = fileDlg.GetPathName();
			SetTitle( fileDlg.GetFileTitle() );
		}
	}
	m_ptTreeInfoView->modifyGRTree();
	UpdateAllViews(NULL);
}

void cGestureCreatorDoc::OnFileSave()
{
	if( m_gestureFileName == "" )
	{
		OnFileSaveAs();
	}
	else 
	{
		bool bRes = m_gestureRecogniser.save( m_gestureFileName.GetBuffer() );
		if( false == bRes )
		{
			MessageBox( NULL, "Gesture saving failed", "GestureCreator", MB_OK|MB_ICONERROR );
		}
	}
}


afx_msg void cGestureCreatorDoc::OnFileSaveAs()
{
	CFileDialog fileDlg(FALSE, ".bin", NULL, OFN_HIDEREADONLY|OFN_NOCHANGEDIR, "Gesture Files (binary) |*.bin|Gesture Files (ascii) |*.txt||", NULL, 0);
	
	if( fileDlg.DoModal() == IDOK )
	{
		bool binary = ( fileDlg.GetFileExt() == "bin" );
		bool bRes = m_gestureRecogniser.save( fileDlg.GetPathName().GetBuffer(), binary );
		if( false == bRes )
		{
			MessageBox( NULL, "Gesture saving failed", "GestureCreator", MB_OK|MB_ICONERROR );
			return;
		}
		m_gestureFileName = fileDlg.GetPathName();
		SetTitle( fileDlg.GetFileTitle() );
	}
}
