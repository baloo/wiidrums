#pragma once
#include "afxwin.h"

class cGestureCreatorDoc;

// cNewGestureDlg dialog

class cNewGestureDlg : public CDialog
{
	DECLARE_DYNAMIC(cNewGestureDlg)

public:
	cNewGestureDlg( cGestureCreatorDoc* pDoc , CWnd* pParent = NULL);   // standard constructor
	virtual ~cNewGestureDlg();

	cGestureCreatorDoc* m_pDoc;

// Dialog Data
	enum { IDD = IDD_CREATEGESTUREDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_vocabularyChoice;
	CString m_vocabularyName;
	CListBox m_movementListBox;
	CString m_gestureName;
	int m_vocSize;
	int m_numberHiddenStates;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	afx_msg void OnCbnEditchangeVocabularyselectioncombo();
	afx_msg void OnCbnSelchangeVocabularyselectioncombo();
	afx_msg void OnCbnDropdownVocabularyselectioncombo();
};
