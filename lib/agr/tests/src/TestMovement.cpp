/**
 * \file TestMovement.cpp
 * \brief Implementation for testing cMovement
 * \date 28/02/08
 */

#include <cppunit/config/SourcePrefix.h>
#include "TestMovement.h"

#include "Movement.h"
using namespace AGR;

CPPUNIT_TEST_SUITE_REGISTRATION( cTestMovement );


void cTestMovement::setUp()
{
}


void cTestMovement::tearDown()
{
}


/*! Unitialised movement tests
 */
void cTestMovement::uninitialised()
{
	cMovement movement;

	CPPUNIT_ASSERT( movement.m_buffer == NULL );
	CPPUNIT_ASSERT( movement.m_size == 0 );
	CPPUNIT_ASSERT( movement.getBuffer() == NULL );
	CPPUNIT_ASSERT( movement.modifyBuffer() == NULL );
	CPPUNIT_ASSERT( movement.getSize() == 0 );

	cVector3int vec = movement.getData( 0 );
	CPPUNIT_ASSERT( vec.x == 0 && vec.y == 0 && vec.z == 0 );
}


/*! initialisation movement tests
 */
void cTestMovement::initialisation()
{
	cMovement movement;

	bool res = movement.initialise( 5, NULL );
	CPPUNIT_ASSERT( res == true && movement.m_size == 5 && movement.m_buffer != NULL );
	CPPUNIT_ASSERT( movement.getSize() == 5 && movement.getBuffer() != NULL );

	cVector3int vec = movement.getData( 0 );
	CPPUNIT_ASSERT( vec.x == 0 && vec.y == 0 && vec.z == 0 );

	res = movement.initialise( 8, NULL );
	CPPUNIT_ASSERT_MESSAGE( "Two initialise without explicitely clearing",
							res == true && 
							movement.m_buffer != NULL && 
							movement.m_size == 8 );

	movement.clear();
	CPPUNIT_ASSERT( movement.m_buffer == NULL && movement.m_size == 0 );

	cVector3int array[ 10 ];
	for( int i=0; i<10; i++ )
	{
		array[ i ] = cVector3int( i, -i, i*10 );
	}
	res = movement.initialise( 10, array );
	CPPUNIT_ASSERT( res == true && movement.m_size == 10 && movement.m_buffer != NULL );
	for( int i=0; i<10; i++ )
	{
		cVector3int vec = movement.getData( i );
		CPPUNIT_ASSERT(    i == vec.x &&
						  -i == vec.y &&
						10*i == vec.z );
	}

	movement.clear();
	CPPUNIT_ASSERT( movement.m_buffer == NULL && movement.m_size == 0 );
	
	std::vector<cVector3int> stdvec;
	res = movement.initialise( stdvec );
	CPPUNIT_ASSERT_MESSAGE( "input std::vector empty, initiliastion should fail",
							res == false && movement.m_buffer == NULL && movement.m_size == 0 );

	for( int i=0; i<10; i++ )
	{
		stdvec.push_back( cVector3int( i*10, i, -i ) );
	}

	res = movement.initialise( stdvec );
	CPPUNIT_ASSERT(	res == true && movement.m_buffer != NULL && movement.m_size == 10 );
	for( int i=0; i<10; i++ )
	{
		cVector3int vec = movement.getData( i );
		CPPUNIT_ASSERT( 10*i == vec.x &&
						   i == vec.y &&
						  -i == vec.z );
	}
}


/*! Manipualtion movement tests
 */
void cTestMovement::manipulation()
{
	cMovement movement;
	cVector3int array[ 10 ];
	for( int i=0; i<10; i++ )
	{
		array[ i ] = cVector3int( i, -i, i*10 );
	}
	bool res = movement.initialise( 10, array );
	CPPUNIT_ASSERT( res == true && movement.m_size == 10 && movement.m_buffer != NULL );


	cVector3int vec = movement.getData( 0 );
	CPPUNIT_ASSERT( vec.x == 0 && vec.y == 0 && vec.z == 0 );
	vec = movement.getData( 5 );
	CPPUNIT_ASSERT( vec.x == 5 && vec.y == -5 && vec.z == 50 );
	vec = movement.getData( 9 );
	CPPUNIT_ASSERT( vec.x == 9 && vec.y == -9 && vec.z == 90 );
	vec = movement.getData( 10 );
	CPPUNIT_ASSERT( vec.x == 0 && vec.y == 0 && vec.z == 0 );


	cMovement movementResized;
	res = cMovement::changeVectorSize( movement, 20, &movementResized ); 
	CPPUNIT_ASSERT( res == true );
	CPPUNIT_ASSERT( movementResized.m_size == 20 && movement.m_buffer != NULL );
	CPPUNIT_ASSERT( movement.m_size == 10 && movement.m_buffer != NULL );

	vec = movementResized.getData( 0 );
	CPPUNIT_ASSERT( vec.x == 0 && vec.y == 0 && vec.z == 0 );
	vec = movementResized.getData( 9 );
	CPPUNIT_ASSERT( vec.x == 4 && vec.y == -3 && vec.z == 43 );
	vec = movementResized.getData( 19 );

	// Note2 : Doesn't give the same result in Debug and Release for vec.y : expected -9, release -9, debug -8.
	// This is because of the float to int cast, 0.5 is added to improve results on positive numbers (may be improved in release)
	// See note in cMovement::changeVectorSize()
#if _DEBUG
	CPPUNIT_ASSERT( vec.x == 9 && vec.y == -8 && vec.z == 90 );
#else
	CPPUNIT_ASSERT( vec.x == 9 && vec.y == -9 && vec.z == 90 );
#endif
	
	res = cMovement::changeVectorSize( movement, 20, &movementResized ); 
	CPPUNIT_ASSERT( res == true );
	CPPUNIT_ASSERT( movementResized.m_size == 20 && movement.m_buffer != NULL );
	CPPUNIT_ASSERT( movement.m_size == 10 && movement.m_buffer != NULL );

	res = cMovement::changeVectorSize( movement, 20, &movement ); 
	CPPUNIT_ASSERT_MESSAGE( "change the size of the same vectors", res == false );
}
