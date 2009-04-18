// NewGestureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GestureCreator.h"
#include "NewGestureDlg.h"
#include "GestureCreatorDoc.h"
#include "TreeInfoView.h"


// cNewGestureDlg dialog

IMPLEMENT_DYNAMIC(cNewGestureDlg, CDialog)
cNewGestureDlg::cNewGestureDlg( cGestureCreatorDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(cNewGestureDlg::IDD, pParent)
	, m_gestureName(_T(""))
	, m_vocabularyName(_T(""))
	, m_vocSize(6)
	, m_numberHiddenStates(10)
{
	m_pDoc = pDoc;
}

cNewGestureDlg::~cNewGestureDlg()
{
}


void cNewGestureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VOCABULARYSELECTIONCOMBO, m_vocabularyChoice);
	DDX_Control(pDX, IDC_MOVEMENTLIST, m_movementListBox);
	DDX_Text(pDX, IDC_EDIT1, m_gestureName);
	DDX_CBString(pDX, IDC_VOCABULARYSELECTIONCOMBO, m_vocabularyName);
	DDX_Text(pDX, IDC_EDITVOCABULARYSIZE, m_vocSize);
	DDX_Text(pDX, IDC_EDITNUMBERHIDDENSTATES, m_numberHiddenStates);
}


BEGIN_MESSAGE_MAP(cNewGestureDlg, CDialog)
	ON_CBN_EDITCHANGE(IDC_VOCABULARYSELECTIONCOMBO, OnCbnEditchangeVocabularyselectioncombo)
	ON_CBN_SELCHANGE(IDC_VOCABULARYSELECTIONCOMBO, OnCbnSelchangeVocabularyselectioncombo)
	ON_CBN_DROPDOWN(IDC_VOCABULARYSELECTIONCOMBO, OnCbnDropdownVocabularyselectioncombo)
END_MESSAGE_MAP()


// cNewGestureDlg message handlers

BOOL cNewGestureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_vocabularyChoice.ResetContent();
	m_vocabularyChoice.AddString( "Generate vocabulary" );
	for( unsigned int i = 0; i<m_pDoc->m_gestureRecogniser.getNumberVocabulary(); i++ )
	{
		const char* vocabularyName = m_pDoc->m_gestureRecogniser.getVocabularyName( i );
		if( NULL != vocabularyName ) {
			m_vocabularyChoice.AddString( vocabularyName );
		}
	}
	m_vocabularyName = "Generate vocabulary";

	std::map<std::string,cMovement>::iterator iter;
	for( iter =	m_pDoc->m_movementList.begin(); iter != m_pDoc->m_movementList.end(); iter++ )
	{
		m_movementListBox.AddString( iter->first.c_str() );
	}

	m_gestureName = "MyGesture";
	m_vocSize = 6;
	m_numberHiddenStates = 10;

	UpdateData( false );


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cNewGestureDlg::OnOK()
{
	UpdateData( true );

	//------------------------------
	// Setting checking

	if( "" == m_gestureName )
	{
		MessageBox( "No name has been provided for the gesture" );
		return;
	}


	bool generateVoc = false;
	if( m_vocabularyName == "Generate vocabulary" )
		generateVoc = true;
	
	if( true == generateVoc &&
		2 > m_vocSize		)
	{
		MessageBox( "Vocabulary size too small" );
		return;
	}

	if( 2 > m_numberHiddenStates )
	{
		MessageBox( "Number of hidden states too small" );
		return;
	}

	int nbMove = m_movementListBox.GetSelCount();
	if( 0 == nbMove )
	{
		MessageBox( "No movement selected" );
		return;
	}

	int* idSelected = new int [nbMove];
	m_movementListBox.GetSelItems( nbMove, idSelected );

	cMovement* movementsSelected = new cMovement[ nbMove ];
	CString moveName;
	for( int i=0; i<nbMove; i++ )
	{
		m_movementListBox.GetText( idSelected[i], moveName );
		movementsSelected[ i ].copy( m_pDoc->m_movementList[moveName.GetBuffer()] );
	}
	delete [] idSelected;

	//------------------------------
	// Vocabulary

	tVocabularyId vocId = -1;
	cVocabulary* ptVocabulary = NULL;
	if( generateVoc )
	{
		cVocabulary myVocabulary;
		cKmeansClustering clusterer;
		CString vocName;

		vocName.Format( "%sVoc", m_gestureName );
		myVocabulary.initialise( &clusterer, m_vocSize );
		myVocabulary.createClusters( movementsSelected, nbMove );
		vocId = m_pDoc->m_gestureRecogniser.addVocabulary( myVocabulary, vocName.GetBuffer() );
		if( -1 == vocId )
		{
			if( false == m_pDoc->m_gestureRecogniser.resize( m_pDoc->m_gestureRecogniser.getNumberVocabulary()+1, 0) ) 
			{
				MessageBox( "Error when resizing the gesture recogniser" );
				return;
			}
			vocId = m_pDoc->m_gestureRecogniser.addVocabulary( myVocabulary, vocName.GetBuffer() );
			if( -1 == vocId )
			{
				MessageBox( "Error when adding the vocabulary" );
				return;
			}
		}

		ptVocabulary = m_pDoc->m_gestureRecogniser.getVocabulary( vocId );

		HTREEITEM item = m_pDoc->m_ptTreeInfoView->GetTreeCtrl().InsertItem( vocName.GetBuffer(), m_pDoc->m_ptTreeInfoView->m_vocabularyTree );
		m_pDoc->m_ptTreeInfoView->GetTreeCtrl().SetItemData( item, (DWORD)cTreeInfoView::eTypeVocabulary );

	}
	else 
	{
		vocId =  m_pDoc->m_gestureRecogniser.findVocabularyByName( m_vocabularyName.GetBuffer() );
		if( -1 == vocId )
		{
			delete [] movementsSelected;
			MessageBox( "Vocabulary not found" );
			return;
		}
		ptVocabulary = m_pDoc->m_gestureRecogniser.getVocabulary( vocId );
	}

	//------------------------------
	// Gesture
	{
		cGesture myGesture;
		cKmeansClustering clusterer;
		myGesture.initialise( m_numberHiddenStates, ptVocabulary );
		myGesture.createHMM( &clusterer,  movementsSelected, nbMove );

		tGestureId gestId = m_pDoc->m_gestureRecogniser.addGesture( myGesture, vocId, m_gestureName.GetBuffer() );
		if( -1 == gestId )
		{
			if( false == m_pDoc->m_gestureRecogniser.resize( 0, m_pDoc->m_gestureRecogniser.getNumberGesture()+1) ) 
			{
				MessageBox( "Error when resizing the gesture recogniser" );
				return;
			}
			gestId = m_pDoc->m_gestureRecogniser.addGesture( myGesture, vocId, m_gestureName.GetBuffer() );
			if( -1 == vocId )
			{
				MessageBox( "Error when adding the gesture" );
				return;
			}
		}

		HTREEITEM item = m_pDoc->m_ptTreeInfoView->GetTreeCtrl().InsertItem( m_gestureName.GetBuffer(), m_pDoc->m_ptTreeInfoView->m_gestureTree );
		m_pDoc->m_ptTreeInfoView->GetTreeCtrl().SetItemData( item, (DWORD)cTreeInfoView::eTypeGesture );
	}


	delete [] movementsSelected;
	CDialog::OnOK();
}

void cNewGestureDlg::OnCbnEditchangeVocabularyselectioncombo()
{
	UpdateData( false );
}

void cNewGestureDlg::OnCbnSelchangeVocabularyselectioncombo()
{
	UpdateData( true );
}

void cNewGestureDlg::OnCbnDropdownVocabularyselectioncombo()
{
	UpdateData( true );
}
