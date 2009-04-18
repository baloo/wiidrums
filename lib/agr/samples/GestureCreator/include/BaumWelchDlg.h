#pragma once
#include "afxwin.h"
#include "c:\program files\microsoft visual studio .net 2003\vc7\atlmfc\include\afxwin.h"


// cBaumWelchDlg dialog

class cBaumWelchDlg : public CDialog
{
	DECLARE_DYNAMIC(cBaumWelchDlg)

public:
	cBaumWelchDlg( cGestureCreatorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	virtual ~cBaumWelchDlg();

	cGestureCreatorDoc* m_pDoc;

// Dialog Data
	enum { IDD = IDD_DIALOGBAUMWELCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_movementListBox;
	CComboBox m_gestureComboBox;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CString m_gestureName;
};
