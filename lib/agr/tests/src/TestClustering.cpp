/**
 * \file TestClustering.cpp
 * \brief Implementation for testing cMovement
 * \date 01/03/08
 */

#include <cppunit/config/SourcePrefix.h>
#include <vector>	
#include <time.h>

#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"
using namespace AGR;

#include "TestClustering.h"


CPPUNIT_TEST_SUITE_REGISTRATION( cTestClustering );


// The repartition will be arround 5 points :
// A ( 0, 0, 0 )
// B ( 20, 0, 0 )
// C ( 0, 20, 0 )
// D ( 0, 0, 20 )
// E ( 20, 20, 20 )
//
// The observation will be made arround them, with a value one of these points += 5


cTestClustering::cTestClustering()
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
			m_movementArray[ i ].modifyBuffer()[ j ] = m_idealClusters[ j ] + randVect;
		}
	}	
}


cTestClustering::~cTestClustering()
{
}


void cTestClustering::setUp()
{
}


void cTestClustering::tearDown()
{
}


/*! Uninitialised clustering tests
 */
void cTestClustering::uninitialised()
{
	cKmeansClustering kmeansObj;
	cClustering* ptClustering = &kmeansObj;

	unsigned int obs[ m_movementSize ];
	int ret = ptClustering->clusterise( m_movement, obs );
	CPPUNIT_ASSERT( -1 == ret );


	ret = ptClustering->createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( -1 == ret );

	cClustering* copy = ptClustering->createCopy();
	CPPUNIT_ASSERT( NULL != copy );
	CPPUNIT_ASSERT( copy->m_buffer == NULL );
	CPPUNIT_ASSERT( copy->m_size == 0 );
}


	
/*! initialisation clustering tests
 */
void cTestClustering::intialisation()
{
	cKmeansClustering kmeansObj;
	cClustering* ptClustering = &kmeansObj;

	bool res = ptClustering->initialise( 10, NULL );
	CPPUNIT_ASSERT( false == res );

	cVector3int buffer[ 10 ];
	res = ptClustering->initialise( 0, buffer );
	CPPUNIT_ASSERT( false == res );

	
	res = ptClustering->initialise( 10, buffer );
	CPPUNIT_ASSERT( true == res );
	CPPUNIT_ASSERT( NULL != ptClustering->m_buffer );
	CPPUNIT_ASSERT( 10 == ptClustering->m_size );

	ptClustering->clear();
	CPPUNIT_ASSERT( ptClustering->m_buffer == NULL );
	CPPUNIT_ASSERT( ptClustering->m_size == 0 );

	res = ptClustering->initialise( 10, buffer );
	CPPUNIT_ASSERT( true == res );
	CPPUNIT_ASSERT( NULL != ptClustering->m_buffer );
	CPPUNIT_ASSERT( 10 == ptClustering->m_size );
}
	


/*! kmeans manipulation tests
 */
void cTestClustering::kmeansManipulation()
{
	cKmeansClustering kmeansObj;
	cClustering* ptClustering = &kmeansObj;

	cVector3int buffer[ m_nbClusters ];
	bool bRes = ptClustering->initialise( m_nbClusters, buffer );
	CPPUNIT_ASSERT( true == bRes );

	int iRes = ptClustering->createClusters( m_movementArray, m_nbMovement );
	CPPUNIT_ASSERT( iRes >= 0 );

	// check that the clusters are aproximatly good ( += 5 )
	int clusterId[ m_nbClusters ];
	static const int delta = 5;
	for( int i=0; i<m_movementSize; i++ )
	{
		bool found = false;
		for( int j=0; j<m_movementSize; j++ )
		{
			if(	buffer[i].x <= m_idealClusters[ j ].x + delta && 
				buffer[i].x >= m_idealClusters[ j ].x - delta && 
				buffer[i].y <= m_idealClusters[ j ].y + delta && 
				buffer[i].y >= m_idealClusters[ j ].y - delta &&
				buffer[i].z <= m_idealClusters[ j ].z + delta && 
				buffer[i].z >= m_idealClusters[ j ].z - delta )
			{
				found = true;
				clusterId[ i ] = j;
				break;
			}

		}
		CPPUNIT_ASSERT_MESSAGE(  "Warning : Kmeans clustering result doesn't enter within the expected result tolerance",
								true == found );
	}

	// check that the 5 original clusters are well recognised:
	for( int i=0; i<m_nbClusters; i++ )
	{
		int cluster = ptClustering->closerCluster( m_idealClusters[ i ] );
		CPPUNIT_ASSERT( 0 <= cluster );
		CPPUNIT_ASSERT_MESSAGE( "Ideal cluster not assigned to its kmeans created cluster",
								clusterId[ cluster ] == i );
	}

	unsigned int observations[ m_movementSize ];
	iRes = ptClustering->clusterise( m_movement, observations );
	CPPUNIT_ASSERT( iRes >= 0 );

	for( int i=0; i<m_nbClusters; i++ )
	{
		unsigned int expectedCluster =  i ;
		unsigned int foundCluster = clusterId[ observations[ i ] ];
		CPPUNIT_ASSERT_MESSAGE( "Clustered observation not assigned to its expected cluster",
								expectedCluster == foundCluster );
	}



}
