/**
 * \file TestHMM.cpp
 * \brief Implementation for testing cHMM
 * \date 01/03/08
 */

#include <cppunit/config/SourcePrefix.h>
#include <vector>
#include <time.h>

#include "Vocabulary.h"
#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"
#include "HMM.h"
using namespace AGR;

#include "TestHMM.h"

CPPUNIT_TEST_SUITE_REGISTRATION( cTestHMM );


cTestHMM::cTestHMM()
{
	srand( (unsigned int)time(NULL) );

	m_idealClusters[ 0 ] = cVector3int(  0,  0,  0 );
	m_idealClusters[ 1 ] = cVector3int( 20,  0,  0 ); 
	m_idealClusters[ 2 ] = cVector3int(  0, 20,  0 ); 
	m_idealClusters[ 3 ] = cVector3int(  0,  0, 20 ); 
	m_idealClusters[ 4 ] = cVector3int( 20,  0, 20 );

	m_movement.initialise( m_nbClusters, m_idealClusters );

	cVector3int randVect;
	for( int i=0; i< m_nbMovement; i++ )
	{
		m_movementArray[ i ].initialise( m_nbClusters );
		for( int j=0; j<m_nbClusters; j++ )
		{
			randVect =  cVector3int( 5-(rand()%10), 5-(rand()%10), 5-(rand()%10) );
			m_movementArray[ i ].modifyBuffer()[ j ] = m_idealClusters[ j ] + randVect;
		}
	}	
}


cTestHMM::~cTestHMM()
{
}


void cTestHMM::setUp()
{
}

void cTestHMM::tearDown()
{
}


/*! Uninitialised Hmm tests
 */
void cTestHMM::uninitialised()
{
	cHMM myHMM;

	CPPUNIT_ASSERT( NULL == myHMM.m_A );
	CPPUNIT_ASSERT( NULL == myHMM.m_B );
	CPPUNIT_ASSERT( NULL == myHMM.m_PI );
	CPPUNIT_ASSERT( NULL == myHMM.m_ptVocabulary );
	CPPUNIT_ASSERT( 0 == myHMM.m_numberStates );
	CPPUNIT_ASSERT( myHMM.isReady() == false );

	bool bRes = myHMM.BaumWelch( m_movementArray, m_nbMovement, m_movementSize );
	CPPUNIT_ASSERT( false == bRes );

	cHMM anotherHMM;
	bRes = anotherHMM.copy( myHMM );
	CPPUNIT_ASSERT( false == bRes );

	cKmeansClustering clusterer;
	bRes = myHMM.createHMM( &clusterer, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	float fRes = myHMM.forward( m_movementArray[0] );
	CPPUNIT_ASSERT( fRes < 0 );

	bRes = myHMM.save( "myHMM.txt" );
	CPPUNIT_ASSERT( false == bRes );

	cVocabulary vocabulary;
	bRes = myHMM.assignVocabulary( &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

	bRes = myHMM.BaumWelch( m_movementArray, m_nbMovement, m_movementSize );
	CPPUNIT_ASSERT( false == bRes );

	bRes = anotherHMM.copy( myHMM );
	CPPUNIT_ASSERT( false == bRes );

	bRes = myHMM.createHMM( &clusterer, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	fRes = myHMM.forward( m_movementArray[0] );
	CPPUNIT_ASSERT( fRes < 0 );

	bRes = myHMM.save( "myHMM.txt" );
	CPPUNIT_ASSERT( false == bRes );

}


/*! Hmm initialisation tests
 */
void cTestHMM::initialisation()
{
	cHMM myHMM;
	cVocabulary vocabulary;
	cKmeansClustering clusterer;

	CPPUNIT_ASSERT(  myHMM.isReady() == false );

	bool bRes = myHMM.initialise( 0, NULL );
	CPPUNIT_ASSERT( false == bRes );
	CPPUNIT_ASSERT(  myHMM.isReady() == false );
	
	bRes = myHMM.initialise( 0, NULL );
	CPPUNIT_ASSERT( false == bRes );
	CPPUNIT_ASSERT(  myHMM.isReady() == false );

	bRes = myHMM.initialise( 0, &vocabulary );
	CPPUNIT_ASSERT( false == bRes );
	CPPUNIT_ASSERT(  myHMM.isReady() == false );

	bRes = myHMM.initialise( 5, &vocabulary );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( &vocabulary == myHMM.m_ptVocabulary );

	myHMM.clear();
	CPPUNIT_ASSERT( NULL == myHMM.m_ptVocabulary );

	bRes = myHMM.initialise( 5, &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

}



/*! Hmm creation tests
 */
void cTestHMM::hmmcreation()
{
	cHMM myHMM;
	cVocabulary vocabulary;
	cKmeansClustering clusterer;

	bool bRes = myHMM.initialise( 5, &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

	// Try with a non initialised vocabulary
	bRes = myHMM.createHMM( NULL, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = myHMM.createHMM( &clusterer, m_movementArray, 0 );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = myHMM.createHMM( &clusterer, NULL, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	bRes = myHMM.createHMM( &clusterer, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	// With an initialised vocabulary
	vocabulary.initialise( &clusterer, 5 );
	vocabulary.createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( vocabulary.isReady() == true );

	bRes = myHMM.createHMM( NULL, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = myHMM.createHMM( &clusterer, m_movementArray, 0 );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = myHMM.createHMM( &clusterer, NULL, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	bRes = myHMM.createHMM( &clusterer, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( true == bRes );

}



/*! Hmm manipulation tests
 */
void cTestHMM::manipulation()
{
	cHMM myHMM;
	cVocabulary vocabulary;
	cKmeansClustering clusterer;

	bool bRes = vocabulary.initialise( &clusterer, 5 );
	CPPUNIT_ASSERT( true == bRes );

	int iRes = vocabulary.createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( iRes >= 0 );

	bRes = myHMM.initialise( 5, &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

	bRes = myHMM.createHMM( &clusterer, m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( true == bRes );

	bRes = myHMM.save( "hmm.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = myHMM.save( "hmm.bin", true );
	CPPUNIT_ASSERT( true == bRes );

	float fRes = myHMM.forward( m_movement );
	fRes *= myHMM.getObservationSize();
	CPPUNIT_ASSERT_MESSAGE( "HMM forward result doesn't enter within the expected result tolerance" ,
							fRes > 0.001f );
	float forwardProb = fRes;


	float mean = myHMM.getMeanProbabilityWeighted();
	CPPUNIT_ASSERT( mean > 0.01f  );

	float variance = myHMM.getVarianceProbabilityWeighted();
	CPPUNIT_ASSERT( variance >= 0.0f  );
	float forwardProbWeighed = forwardProb * myHMM.getObservationSize();
	float worseAcceptable = mean - variance;
	CPPUNIT_ASSERT( forwardProbWeighed > worseAcceptable );



	cHMM otherHMM;
	bRes = otherHMM.copy( myHMM );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( true == otherHMM.isReady() );
	
	otherHMM.clear();
	bRes = otherHMM.load( "hmm.txt" );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT_MESSAGE( "Hmm loaded, but vocabulary not assigned", 
							false == otherHMM.isReady() );
	
	fRes = otherHMM.forward( m_movement );
	CPPUNIT_ASSERT( fRes < 0.0f );

	bRes = otherHMM.assignVocabulary( NULL );
	CPPUNIT_ASSERT( false == bRes );
	
	fRes = otherHMM.forward( m_movement );
	CPPUNIT_ASSERT( fRes < 0.0f );

	bRes = otherHMM.assignVocabulary( &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

	fRes = otherHMM.forward( m_movement );
	fRes *= otherHMM.getObservationSize();
	CPPUNIT_ASSERT( fRes > 0.001f );
	float difference = ( fRes > forwardProb) ? fRes - forwardProb : forwardProb- fRes ;
	CPPUNIT_ASSERT( difference < 0.0001f );

	
	otherHMM.clear();
	CPPUNIT_ASSERT( 0 == otherHMM.m_meanProbabilityWeighted );
	CPPUNIT_ASSERT( NULL == otherHMM.m_A );
	bRes = otherHMM.load( "hmm.bin" );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT_MESSAGE( "Hmm loaded, but vocabulary not assigned", 
							false == otherHMM.isReady() );
	
	fRes = otherHMM.forward( m_movement );
	CPPUNIT_ASSERT( fRes < 0.0f );

	bRes = otherHMM.assignVocabulary( NULL );
	CPPUNIT_ASSERT( false == bRes );
	
	fRes = otherHMM.forward( m_movement );
	CPPUNIT_ASSERT( fRes < 0.0f );

	bRes = otherHMM.assignVocabulary( &vocabulary );
	CPPUNIT_ASSERT( true == bRes );

	fRes = otherHMM.forward( m_movement );
	fRes *= otherHMM.getObservationSize();
	CPPUNIT_ASSERT( fRes > 0.001f );
	difference = ( fRes > forwardProb) ? fRes - forwardProb : forwardProb- fRes ;
	CPPUNIT_ASSERT( difference < 0.0001f );



	bRes = myHMM.BaumWelch( NULL, m_nbMovement );
	CPPUNIT_ASSERT( false == bRes );

	bRes = myHMM.BaumWelch( m_movementArray, 0 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = myHMM.BaumWelch( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( true == bRes );
	
	fRes = myHMM.forward( m_movement );
	fRes *= myHMM.getObservationSize();
	CPPUNIT_ASSERT_MESSAGE( "HMM forward result doesn't enter within the expected result tolerance" ,
							fRes > 0.001f );
	// Doesn't give always better results
	//CPPUNIT_ASSERT( fRes >= forwardProb );
	
}
