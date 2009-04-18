// BaumWelchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GestureCreator.h"
#include "GestureCreatorDoc.h"
#include "BaumWelchDlg.h"


// cBaumWelchDlg dialog

IMPLEMENT_DYNAMIC(cBaumWelchDlg, CDialog)
cBaumWelchDlg::cBaumWelchDlg( cGestureCreatorDoc* pDoc,
							  CWnd* pParent /*=NULL*/)
	: CDialog(cBaumWelchDlg::IDD, pParent)
	, m_gestureName(_T(""))
{
	m_pDoc = pDoc;
}

cBaumWelchDlg::~cBaumWelchDlg()
{
}

void cBaumWelchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTMOVEMENTS, m_movementListBox);
	DDX_Control(pDX, IDC_COMBOGESTURE, m_gestureComboBox);
	DDX_CBString(pDX, IDC_COMBOGESTURE, m_gestureName);
}


BEGIN_MESSAGE_MAP(cBaumWelchDlg, CDialog)
END_MESSAGE_MAP()


// cBaumWelchDlg message handlers

BOOL cBaumWelchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_gestureComboBox.ResetContent();
	for( unsigned int i = 0; i<m_pDoc->m_gestureRecogniser.getNumberGesture(); i++ )
	{
		const char* gestureName = m_pDoc->m_gestureRecogniser.getGestureName( i );
		if( NULL != gestureName ) {
			m_gestureComboBox.AddString( gestureName );
		}
	}
	m_gestureComboBox.SetCurSel( 0 );

	std::map<std::string,cMovement>::iterator iter;
	for( iter =	m_pDoc->m_movementList.begin(); iter != m_pDoc->m_movementList.end(); iter++ )
	{
		m_movementListBox.AddString( iter->first.c_str() );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cBaumWelchDlg::OnOK()
{
	UpdateData( true );

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

	int iSel = m_gestureComboBox.GetCurSel();

	tGestureId idGest = m_pDoc->m_gestureRecogniser.findGestureByName( m_gestureName.GetBuffer() );
	if( idGest == -1 )
	{
		MessageBox( "Can't find specified gesture" );
		delete [] movementsSelected;
		return;
	}

	cGesture* pGesture = m_pDoc->m_gestureRecogniser.getGesture( idGest );
	if( pGesture ) {
		pGesture->BaumWelch( movementsSelected, nbMove );
	}
	delete [] movementsSelected;
	CDialog::OnOK();
}
