/**
 * \file TestVocabulary.cpp
 * \brief Implementation for testing cVocabulary
 * \date 01/03/08
 */

#include <cppunit/config/SourcePrefix.h>
#include <vector>
#include <time.h>

#include "Vocabulary.h"
#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"
using namespace AGR;

#include "TestVocabulary.h"

CPPUNIT_TEST_SUITE_REGISTRATION( cTestVocabulary );


cTestVocabulary::cTestVocabulary()
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
		m_movementArray[ i ].initialise( 5 );
		for( int j=0; j<5; j++ )
		{
			randVect =  cVector3int( 5-(rand()%10), 5-(rand()%10), 5-(rand()%10) );
			m_movementArray[ i ].modifyBuffer()[ j ] = m_idealClusters[ rand()%m_movementSize ] + randVect;
		}
	}	
}


cTestVocabulary::~cTestVocabulary()
{
}


void cTestVocabulary::setUp()
{
}

void cTestVocabulary::tearDown()
{
}


/*! Uninitialised vocabulary tests
 */
void cTestVocabulary::uninitialised()
{
	cVocabulary voc;
	
	CPPUNIT_ASSERT( voc.getSize() == 0 );
	CPPUNIT_ASSERT( voc.getBuffer() == NULL );
	CPPUNIT_ASSERT( NULL == voc.m_buffer );
	CPPUNIT_ASSERT( NULL == voc.m_clusters );
	CPPUNIT_ASSERT( 0 == voc.m_size );
	CPPUNIT_ASSERT( voc.isReady() == false );


	cVocabulary voc2;
	bool bRes = voc2.copy( voc );
	CPPUNIT_ASSERT( false == bRes );

	bRes = voc.save( "vocabulary.txt" );
	CPPUNIT_ASSERT( false == bRes );

	cVector3int pt;
	int iRes = voc.closerCluster( pt );
	CPPUNIT_ASSERT( -1 == iRes );

	cMovement movArray[ 10 ];
	iRes = voc.createClusters( movArray, 10 );
	CPPUNIT_ASSERT( -1 == iRes );
	
	cMovement movement;
	unsigned int observation[10];
	iRes = voc.clusterise( movement, observation );
	CPPUNIT_ASSERT( -1 == iRes );

	cKmeansClustering cluster;
	bRes = voc.initialise( &cluster, 10 );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( NULL != voc.m_buffer );
	CPPUNIT_ASSERT( NULL != voc.m_clusters );
	CPPUNIT_ASSERT( 10 == voc.m_size );
	CPPUNIT_ASSERT( 10 == voc.getSize() );
	CPPUNIT_ASSERT( voc.isReady() == true );

	voc.clear();
	CPPUNIT_ASSERT( voc.getSize() == 0 );
	CPPUNIT_ASSERT( voc.getBuffer() == NULL );
	CPPUNIT_ASSERT( NULL == voc.m_buffer );
	CPPUNIT_ASSERT( NULL == voc.m_clusters );
	CPPUNIT_ASSERT( 0 == voc.m_size );
	CPPUNIT_ASSERT( voc.isReady() == false );

}


/*! Vocabulary initialisation tests
 */
void cTestVocabulary::initialisation()
{
	cVocabulary voc;
	cKmeansClustering cluster;

	bool bRes = voc.initialise( NULL, 10 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = voc.initialise( &cluster, 0 );
	CPPUNIT_ASSERT( false == bRes );

	bRes = voc.initialise( &cluster, 5 );
	CPPUNIT_ASSERT( true == bRes );

	bRes = voc.isReady();
	CPPUNIT_ASSERT( true == bRes );
}


/*! Vocabulary manipulation tests
 */
void cTestVocabulary::manipulation()
{
	cVocabulary voc;
	cKmeansClustering cluster;
	bool bRes = voc.initialise( &cluster, m_nbClusters );
	CPPUNIT_ASSERT( true == bRes );
	
	int iRes = voc.createClusters( NULL, m_nbMovement );
	CPPUNIT_ASSERT( iRes < 0 );
	
	iRes = voc.createClusters( m_movementArray, 0 );
	CPPUNIT_ASSERT( iRes < 0 );

	iRes = voc.createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( iRes > 0 );
	
	iRes = voc.createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( iRes > 0 );

	// Cluster manipulation is tested separately

	bRes = voc.save( "vocabulary.txt" );
	CPPUNIT_ASSERT( bRes == true );
	
	cVocabulary vocLoaded;
	bRes = vocLoaded.load( "vocabulary.txt" );
	CPPUNIT_ASSERT( bRes == true );
	CPPUNIT_ASSERT( vocLoaded.m_size == voc.m_size );
	for( int i=0; i<vocLoaded.m_size; i++ )
	{
		CPPUNIT_ASSERT( vocLoaded.m_buffer[i].x == voc.m_buffer[i].x );
		CPPUNIT_ASSERT( vocLoaded.m_buffer[i].y == voc.m_buffer[i].y );
		CPPUNIT_ASSERT( vocLoaded.m_buffer[i].z == voc.m_buffer[i].z );
	}

	
	bRes = voc.save( "vocabulary.bin", true );
	CPPUNIT_ASSERT( bRes == true );
	
	cVocabulary vocLoadedBin;
	bRes = vocLoadedBin.load( "vocabulary.bin" );
	CPPUNIT_ASSERT( bRes == true );
	CPPUNIT_ASSERT( vocLoadedBin.m_size == voc.m_size );
	for( int i=0; i<vocLoadedBin.m_size; i++ )
	{
		CPPUNIT_ASSERT( vocLoadedBin.m_buffer[i].x == voc.m_buffer[i].x );
		CPPUNIT_ASSERT( vocLoadedBin.m_buffer[i].y == voc.m_buffer[i].y );
		CPPUNIT_ASSERT( vocLoadedBin.m_buffer[i].z == voc.m_buffer[i].z );
	}
}
