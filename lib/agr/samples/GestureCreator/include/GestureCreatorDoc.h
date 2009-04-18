// GestureCreatorDoc.h : interface of the cGestureCreatorDoc class
//


#pragma once

#include <string>
#include <vector>
#include <map>

#include "Accelerometer.h"
#include "WiiAccelerometer.h"
#include "GestureRecogniser.h"

using namespace AGR;

class cTreeInfoView;	
class cMovement3DView;
class cButtonView;	
class cInfoView;		


class cGestureCreatorDoc : public CDocument
{
protected: // create from serialization only
	cGestureCreatorDoc();
	DECLARE_DYNCREATE(cGestureCreatorDoc)

// Attributes
public:

	cTreeInfoView*		m_ptTreeInfoView;
	cMovement3DView*	m_ptMovement3DView;
	cButtonView*		m_ptButtonView;
	cInfoView*			m_ptInfoView;

	cAccelerometer*		m_accDevice;
	cWiiAccelerometer	m_accWii;

	cGestureRecogniser	m_gestureRecogniser;
	CString				m_gestureFileName;
	
	cMovement			m_currentMovement;
	cMovement			m_vocabularyMovement;
	cMovement			*m_selectedMovement;

	std::map<std::string,cMovement>	m_movementList;
	unsigned int			m_newMovementId;

	static const		s_accelarationBufferSize = 60;
	cVector3int			m_accelerationBuffer[ s_accelarationBufferSize ];
	unsigned int		m_bufferAccelerationLast;

// Operations
public:
	void updateAccelerometer();

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~cGestureCreatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
};


