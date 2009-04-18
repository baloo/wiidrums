/**
 * \file TestGestureRecogniser.cpp
 * \brief Implementation for testing cGestureRecogniser
 * \date 02/03/08
 */

#include <cppunit/config/SourcePrefix.h>
#include <vector>
#include <time.h>

#include "GestureRecogniser.h"
using namespace AGR;

#include "TestGestureRecogniser.h"

CPPUNIT_TEST_SUITE_REGISTRATION( cTestGestureRecogniser );



cTestGestureRecogniser::cTestGestureRecogniser()
{
	cKmeansClustering clusterer;
	cVector3int randVect;
	cVector3int* buffer;

	// Vocabulary 1
	// values around {5,0,0}{20,10,6}{8,20,12}{0,10,15}{-8,0,6}, +-3
	buffer = new cVector3int[ m_v1_movementSize ];
	for( int i=0; i<m_v1_nbMovement; i++ )
	{
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 0 ] = cVector3int( 5,0,0 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 1 ] = cVector3int( 20,10,6 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 2 ] = cVector3int( 8,20,12 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 3 ] = cVector3int( 0,10,15 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 4 ] = cVector3int( -8,0,6 ) + randVect;

		m_v1_movementArray[i].initialise( m_v1_movementSize, buffer );
	}
	delete [] buffer;

	m_vocabulary1.initialise( &clusterer, m_v1_size );
	m_vocabulary1.createClusters( m_v1_movementArray, m_v1_nbMovement );


	// Vocabulary 2
	// values around {1,-2,3}{6,-10,15}{7,-2,25}{6,10,20}{12,15,6}{20,20,0}{26,5,3}, +-3
	buffer = new cVector3int[ m_v2_movementSize ];
	for( int i=0; i<m_v2_nbMovement; i++ )
	{
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 0 ] = cVector3int( 1,-2,3 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 1 ] = cVector3int( 6,-10,15 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 2 ] = cVector3int( 7,-2,25 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 3 ] = cVector3int( 6,10,20 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 4 ] = cVector3int( 12,15,6 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 5 ] = cVector3int( 20,20,0 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 6 ] = cVector3int( 26,5,3 ) + randVect;

		m_v2_movementArray[i].initialise( m_v2_movementSize, buffer );
	}
	delete [] buffer;

	m_vocabulary2.initialise( &clusterer, m_v2_size );
	m_vocabulary2.createClusters( m_v2_movementArray, m_v2_nbMovement );


	// Gesture 1
	// values around vocabulary 1
	buffer = new cVector3int[ m_v1_movementSize ];
	for( int i=0; i<m_g1_nbMovement; i++ )
	{
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 0 ] = cVector3int( 5,0,0 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 1 ] = cVector3int( 20,10,6 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 2 ] = cVector3int( 8,20,12 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 3 ] = cVector3int( 0,10,15 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 4 ] = cVector3int( -8,0,6 ) + randVect;

		m_g1_movementArray[i].initialise( m_v1_movementSize, buffer );
	}
	delete [] buffer;
	m_gesture1.initialise( m_g1_nbHidden, &m_vocabulary1 );
	m_gesture1.createHMM( &clusterer, m_g1_movementArray, m_g1_nbMovement );


	// Gesture 2
	// values around vocabulary 2
	buffer = new cVector3int[ m_v2_movementSize ];
	for( int i=0; i<m_g2_nbMovement; i++ )
	{
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 0 ] = cVector3int( 1,-2,3 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 1 ] = cVector3int( 6,-10,15 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 2 ] = cVector3int( 7,-2,25 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 3 ] = cVector3int( 6,10,20 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 4 ] = cVector3int( 12,15,6 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 5 ] = cVector3int( 20,20,0 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 6 ] = cVector3int( 26,5,3 ) + randVect;

		m_g2_movementArray[i].initialise( m_v2_movementSize, buffer );
	}
	delete [] buffer;
	m_gesture2.initialise( m_g2_nbHidden, &m_vocabulary2 );
	m_gesture2.createHMM( &clusterer, m_g2_movementArray, m_g2_nbMovement );

	
	// Gesture 2
	// values around vocabulary 2, but in disorder
	buffer = new cVector3int[ m_v2_movementSize ];
	for( int i=0; i<m_g2_nbMovement; i++ )
	{
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 6 ] = cVector3int( 1,-2,3 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 5 ] = cVector3int( 6,-10,15 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 4 ] = cVector3int( 7,-2,25 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 3 ] = cVector3int( 6,10,20 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 2 ] = cVector3int( 12,15,6 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 1 ] = cVector3int( 20,20,0 ) + randVect;
		randVect =  cVector3int( 3-(rand()%6), 3-(rand()%6), 3-(rand()%6) );
		buffer[ 0 ] = cVector3int( 26,5,3 ) + randVect;

		m_g3_movementArray[i].initialise( m_v2_movementSize, buffer );
	}
	delete [] buffer;
	m_gesture3.initialise( m_g3_nbHidden, &m_vocabulary2 );
	m_gesture3.createHMM( &clusterer, m_g3_movementArray, m_g3_nbMovement );

}


cTestGestureRecogniser::~cTestGestureRecogniser()
{
}


void cTestGestureRecogniser::setUp()
{
}

void cTestGestureRecogniser::tearDown()
{
}


/*! Uninitialised gesture recogniser tests
 */
void cTestGestureRecogniser::uninitialised()
{
	cGestureRecogniser GR;

	int iRes = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( iRes < 0 );

	iRes = GR.addGesture( m_gesture1 );
	CPPUNIT_ASSERT( iRes < 0 );

	iRes = GR.closerGesture( m_g1_movementArray[0] );
	CPPUNIT_ASSERT( iRes < 0 );
	
	const cGesture* ptGest = GR.getGesture( 0 );
	CPPUNIT_ASSERT( NULL == ptGest );

	const cVocabulary* ptVoc = GR.getVocabulary( 0 );
	CPPUNIT_ASSERT( NULL == ptVoc );

	unsigned int uiRes = GR.getNumberGesture();
	CPPUNIT_ASSERT( 0 == uiRes );

	uiRes = GR.getNumberVocabulary();
	CPPUNIT_ASSERT( 0 == uiRes );

	bool bRes = GR.save( "GR.txt" );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.init( 0, 0 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.init( 2, 0 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.init( 0, 3 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );
}



/*! Vocabulary manipulation tests
 */
void cTestGestureRecogniser::vocabularyManipulation()
{
	
	cGestureRecogniser GR;

	bool bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );

	const cVocabulary* ptVoc = GR.getVocabulary( 0 );
	CPPUNIT_ASSERT( NULL != ptVoc );

	ptVoc = GR.getVocabulary( 3 );
	CPPUNIT_ASSERT( NULL == ptVoc );

	int iRes = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( 0 == iRes );
	
	bRes = GR.removeVocabulary( 1 );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = GR.removeVocabulary( 3 );
	CPPUNIT_ASSERT( false == bRes );

	iRes = GR.addVocabulary( m_vocabulary2 );
	CPPUNIT_ASSERT( 1 == iRes );

	bRes = GR.removeVocabulary( 1 );
	CPPUNIT_ASSERT( true == bRes );
	ptVoc = GR.getVocabulary( 1 );
	CPPUNIT_ASSERT( ptVoc->isReady() == false );
	
	iRes = GR.addVocabulary( m_vocabulary2 );
	CPPUNIT_ASSERT( 1 == iRes );

	iRes = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( iRes < 0 );
	
	ptVoc = GR.getVocabulary( 0 );
	CPPUNIT_ASSERT( NULL != ptVoc );
	CPPUNIT_ASSERT( m_vocabulary1.getSize() == ptVoc->getSize() );
	for( unsigned int i=0; i< ptVoc->getSize(); i++ )
	{
		CPPUNIT_ASSERT( m_vocabulary1.getBuffer()[i].x == ptVoc->getBuffer()[i].x );
		CPPUNIT_ASSERT( m_vocabulary1.getBuffer()[i].y == ptVoc->getBuffer()[i].y );
		CPPUNIT_ASSERT( m_vocabulary1.getBuffer()[i].z == ptVoc->getBuffer()[i].z );
	}

	bRes = GR.saveVocabulary( 0, "GRVoc1.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.saveVocabulary( 1, "GRVoc2.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.saveVocabulary( 2, "GRVoc3.txt" );
	CPPUNIT_ASSERT( false == bRes );
	
	ptVoc = GR.getVocabulary( 3 );
	CPPUNIT_ASSERT( NULL == ptVoc );

	GR.clear();
	ptVoc = GR.getVocabulary( 0 );
	CPPUNIT_ASSERT( NULL == ptVoc );

	bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );

	iRes = GR.loadVocabulary( "GRVoc1.txt" );
	CPPUNIT_ASSERT( 0 == iRes );
	
	iRes = GR.loadVocabulary( "GRVoc2.txt" );
	CPPUNIT_ASSERT( 1 == iRes );
	
	iRes = GR.loadVocabulary( "GRVoc1.txt" );
	CPPUNIT_ASSERT( iRes < 0 );
}



/*! gesture manipulation tests
 */
void cTestGestureRecogniser::gestureManipulation()
{
	cGestureRecogniser GR;

	bool bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );

	const cGesture* ptGest = GR.getGesture( 0 );
	CPPUNIT_ASSERT( NULL != ptGest );

	ptGest = GR.getGesture( 3 );
	CPPUNIT_ASSERT( NULL == ptGest );

	int iRes = GR.addGesture( m_gesture1 );
	CPPUNIT_ASSERT_MESSAGE( "Vocabulary not already inside GR",
							iRes < 0 );
	
	tVocabularyId idVoc1 = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( 0 == idVoc1 );

	iRes =  GR.addGesture( m_gesture1, idVoc1 );
	CPPUNIT_ASSERT( 0 == iRes );
	
	tVocabularyId idVoc2 = GR.addVocabulary( m_vocabulary2 );
	CPPUNIT_ASSERT( 1 == idVoc2 );

	bRes = GR.removeGesture( 1 );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = GR.removeGesture( 3 );
	CPPUNIT_ASSERT( false == bRes );

	iRes =  GR.addGesture( m_gesture2, idVoc2 );
	CPPUNIT_ASSERT( 1 == iRes );

	bRes = GR.removeGesture( 1 );
	CPPUNIT_ASSERT( true == bRes );
	ptGest = GR.getGesture( 1 );
	CPPUNIT_ASSERT( ptGest->isReady() == false );
	
	iRes =  GR.addGesture( m_gesture2, idVoc2 );
	CPPUNIT_ASSERT( 1 == iRes );

	const cVocabulary* ptVoc = GR.getVocabulary( idVoc2 );
	CPPUNIT_ASSERT( NULL != ptVoc );
	m_gesture3.assignVocabulary( ptVoc );
	iRes =  GR.addGesture( m_gesture3 );
	CPPUNIT_ASSERT( 2 == iRes );
	
	iRes = GR.addGesture( m_gesture1, idVoc1 );
	CPPUNIT_ASSERT( iRes < 0 );

	ptGest = GR.getGesture( 0 );
	CPPUNIT_ASSERT( NULL != ptGest );
	CPPUNIT_ASSERT( ptGest->getVocabulary()->getBuffer()[0].x == m_gesture1.getVocabulary()->getBuffer()[0].x );

	bRes = GR.saveGesture( 0, "GRGesture1.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.saveGesture( 1, "GRGesture2.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.saveGesture( 2, "GRGesture3.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.saveGesture( 3, "GRGesture4.txt" );
	CPPUNIT_ASSERT( false == bRes );

	GR.clear();
	ptGest = GR.getGesture( 0 );
	CPPUNIT_ASSERT( NULL == ptGest );
	
	bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );

	idVoc1 = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( 0 == idVoc1 );

	iRes = GR.loadGesture( "GRGesture1.txt", idVoc1 );
	CPPUNIT_ASSERT( 0 == iRes );
	
	idVoc2 = GR.addVocabulary( m_vocabulary2 );
	CPPUNIT_ASSERT( 1 == idVoc2 );

	iRes = GR.loadGesture( "GRGesture2.txt", idVoc2 );
	CPPUNIT_ASSERT( 1 == iRes );

	iRes = GR.loadGesture( "GRGesture3.txt", idVoc2 );
	CPPUNIT_ASSERT( 2 == iRes );

	iRes = GR.loadGesture( "GRGesture1.txt", idVoc1 );
	CPPUNIT_ASSERT( iRes < 0);
}



/*! Gesture Recongiser manipulation tests
 */
void cTestGestureRecogniser::GRManipulation()
{
	cGestureRecogniser GR;

	CPPUNIT_ASSERT( false == GR.isReady() );

	bool bRes = GR.init( 2, 3 );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( true == GR.isReady() );
	CPPUNIT_ASSERT( 2 == GR.getNumberVocabulary() );
	CPPUNIT_ASSERT( 3 == GR.getNumberGesture() );

	// Initial creation
	tVocabularyId idVoc1 = GR.addVocabulary( m_vocabulary1 );
	CPPUNIT_ASSERT( 0 == idVoc1 );

	tVocabularyId idVoc2 = GR.addVocabulary( m_vocabulary2, "Voc2" );
	CPPUNIT_ASSERT( 1 == idVoc2 );

	tGestureId idGest1 =  GR.addGesture( m_gesture1, idVoc1 );
	CPPUNIT_ASSERT( 0 == idGest1 );

	tGestureId idGest2 =  GR.addGesture( m_gesture2, idVoc2, "GestureA" );
	CPPUNIT_ASSERT( 1 == idGest2 );

	tGestureId idGest3 =  GR.addGesture( m_gesture3, idVoc2, "With spaces" );
	CPPUNIT_ASSERT( 2 == idGest3 );
	

	// Save
	bRes = GR.save( "GRFile.txt" );
	CPPUNIT_ASSERT( true == bRes );
	
	bRes = GR.save( "GRFile.bin", true );
	CPPUNIT_ASSERT( true == bRes );

	cGestureRecogniser anotherGR;
	bRes = anotherGR.load( "GRFile.txt" );
	CPPUNIT_ASSERT( true == bRes );
	{
		const cVocabulary* ptVoc = anotherGR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = anotherGR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}

	anotherGR.clear();
	bRes = anotherGR.load( "GRFile.bin" );
	CPPUNIT_ASSERT( true == bRes );
	{
		const cVocabulary* ptVoc = anotherGR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = anotherGR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	} 
	anotherGR.clear();

	// Resize
	bRes = GR.resize( 1, 1 );	// Too small
	CPPUNIT_ASSERT( false == bRes );
	bRes = GR.resize( 0, 1 );
	CPPUNIT_ASSERT( false == bRes );
	bRes = GR.resize( 1, 0 );
	CPPUNIT_ASSERT( false == bRes );
	bRes = GR.resize( 10, 1 );
	CPPUNIT_ASSERT( false == bRes );
	bRes = GR.resize( 1, 10 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.resize( 0, 0 );	// Resize to fit the actual number of objects
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( 2 == GR.getNumberVocabulary() );
	CPPUNIT_ASSERT( 3 == GR.getNumberGesture() );
	{
		const cVocabulary* ptVoc = GR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = GR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}

	bRes = GR.resize( 5, 8 );	// Resize to fit the actual number of objects
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( 5 == GR.getNumberVocabulary() );
	CPPUNIT_ASSERT( 8 == GR.getNumberGesture() );
	{
		const cVocabulary* ptVoc = GR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = GR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}

	bRes = GR.resize( 0, 7 );	// Resize to fit the actual number of objects
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( 2 == GR.getNumberVocabulary() );
	CPPUNIT_ASSERT( 7 == GR.getNumberGesture() );
	{
		const cVocabulary* ptVoc = GR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = GR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}

	bRes = GR.resize( 5, 0 );	// Resize to fit the actual number of objects
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( 5 == GR.getNumberVocabulary() );
	CPPUNIT_ASSERT( 3 == GR.getNumberGesture() );
	{
		const cVocabulary* ptVoc = GR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = GR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}

	bRes = GR.resize( 0, 0 );	// Resize to fit the actual number of objects
	CPPUNIT_ASSERT( true == bRes );
	{
		const cVocabulary* ptVoc = GR.getVocabulary( 1 );
		CPPUNIT_ASSERT( NULL != ptVoc );
		CPPUNIT_ASSERT( ptVoc->isReady() == true );
		CPPUNIT_ASSERT( ptVoc->getBuffer()[0].x == m_vocabulary2.getBuffer()[0].x );

		const cGesture* ptGest = GR.getGesture( 1 );
		CPPUNIT_ASSERT( NULL != ptGest );
		CPPUNIT_ASSERT( ptGest->isReady() == true );
	}
}



/*! Basic gesture recognition tests
 */
void cTestGestureRecogniser::basicGestureRecognition()
{
	cGestureRecogniser GR;
	bool bRes = GR.load( "GRFile.txt" );
	CPPUNIT_ASSERT( true == bRes );

	tGestureId idRecognised = GR.recognise( m_g1_movementArray[0] );
	CPPUNIT_ASSERT( 0 <= idRecognised ); // no errors
	CPPUNIT_ASSERT_MESSAGE( "Gesture1 (id 0) has not been recognised",
							0 == idRecognised );

	idRecognised = GR.recognise( m_g2_movementArray[m_g2_nbMovement/2] );
	CPPUNIT_ASSERT( 0 <= idRecognised ); // no errors
	CPPUNIT_ASSERT_MESSAGE( "Gesture2 (id 1) has not been recognised",
							1 == idRecognised );

	bRes = GR.recogniseGesture( 0, m_g1_movementArray[m_g1_nbMovement-1] );
	CPPUNIT_ASSERT( true == bRes );

	bRes = GR.recogniseGesture( 1, m_g1_movementArray[0] );
	CPPUNIT_ASSERT( false == bRes );

	bRes = GR.recogniseGesture( 0, m_g2_movementArray[0] );
	CPPUNIT_ASSERT( false == bRes );

}
