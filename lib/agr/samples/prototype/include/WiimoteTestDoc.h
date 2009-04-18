// WiimoteTestDoc.h : interface of the CWiimoteTestDoc class
//


#pragma once

//#include "wiiuse.h"

//#define WIIMOTE_ID_1 1
//#define WIIMOTE_ID_2 2

#define USE_OLD_CODE	(0)

#include <list>
#include <vector>

#include "GestureRecogniser.h"
using namespace AGR;

class CWiimoteTestDoc : public CDocument
{
protected: // create from serialization only
	CWiimoteTestDoc();
	DECLARE_DYNCREATE(CWiimoteTestDoc)

// Attributes
public:
	
	cAccelerometer* m_accDeviceA;
	cAccelerometer* m_accDeviceB;
	cWiiAccelerometer m_accWiiA;
	cWiiAccelerometer m_accWiiB;

#define SIZE_ACC_ARRAY (400)
	int m_acc1_x_array[ SIZE_ACC_ARRAY ];
	int m_acc1_y_array[ SIZE_ACC_ARRAY ];
	int m_acc1_z_array[ SIZE_ACC_ARRAY ];
	int m_arrayLast1;
	int m_acc2_x_array[ SIZE_ACC_ARRAY ];
	int m_acc2_y_array[ SIZE_ACC_ARRAY ];
	int m_acc2_z_array[ SIZE_ACC_ARRAY ];
	int m_arrayLast2;

	void updateAccelerometer();

	int m_wiimote1SquareDist;

	std::vector<AGR::cVector3int> m_lastMovement;	
	std::vector<int>  m_lastMovementVocabulary;
	std::vector< std::vector<AGR::cVector3int> > m_movementVector;

	std::vector< float > m_movementVectorProb;
	std::vector< std::vector<int> > m_movementVocabulary;
	void calculateProba();
	void calculateVoca();

	std::vector<AGR::cVector3int>* m_selectedMovement;
	int	m_selectedId;

	cGestureRecogniser			m_gestureRecogniser;
	cMovement					m_GRcurrentMovement;
	std::vector< cMovement >	m_GRmovementVector;
	cMovement*					m_GRselectedMovement;


	//=============================================
	// Test HMM

	// comments CLEANED means that they have been added in a specific class

	void test();
	void preprocess();

	void kmean( const int k, std::vector<AGR::cVector3int>* clusterBuffer ); // CLEANED
	void initialiseHMM();// CLEANED

	int closerCluster( const AGR::cVector3int& point, const std::vector<AGR::cVector3int>* clusterBuffer, const int k );// CLEANED
	
	bool clusterise(	std::vector<int>* assignedClusters,
						const std::vector<AGR::cVector3int>* pointsBuffer, 
						const std::vector<AGR::cVector3int>* clusterBuffer );// CLEANED

	void normaliseVector(  std::vector<AGR::cVector3int>* normalised, const std::vector<AGR::cVector3int>* inputVector );	// CLEANED
	void computeObservationCluster( const std::vector<AGR::cVector3int>* observation, std::vector<int>* observationClusters ); // CLEANED

	// Used for processing
	unsigned int m_normalisedVectorSize;
	std::vector< std::vector<AGR::cVector3int> >	m_normalisedVectors;
	std::vector<AGR::cVector3int>					m_averageVector;
	std::vector<AGR::cVector3int>					m_pointsBuffer;

	// Vocabulary
	int m_vocabularySize;							// CLEANED
	std::vector<AGR::cVector3int>	m_clusterVocabulary;	// CLEANED
	
	// HMM info
	std::vector<AGR::cVector3int>	m_HMM_States;			// CLEANED
	int m_HMM_StateNumber;							// CLEANED
	std::vector< float >	m_HMM_PI;				// CLEANED
	std::vector< std::vector<float> >	m_HMM_A;	// CLEANED
	std::vector< std::vector<float> >	m_HMM_B;	// CLEANED

	// HMM Functions
	float forward( const std::vector<AGR::cVector3int>* observationCluster );
	void computeAlpha( std::vector< std::vector<float> >* alpha, const std::vector<int>* observationCluster );
	void computeBeta( std::vector< std::vector<float> >* beta, const std::vector<int>* observationCluster );
	void BaumWelchOptimisation();
	void BaumWelch( const std::vector< std::vector<int> >* observationCluster  );

	float computeGamma(	const int state, const int time, 
						const std::vector< std::vector<float> >* alpha, 
						const std::vector< std::vector<float> >* beta );	
	float computeXi( const int state1, const int state2, const int time, 
					 const std::vector< std::vector<float> >* alpha, 
					 const std::vector< std::vector<float> >* beta,
					 const std::vector<int>* observation );

	// Save
	void exportHMM();

	// depreciated
	void testOld();
	//=============================================


// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CWiimoteTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


