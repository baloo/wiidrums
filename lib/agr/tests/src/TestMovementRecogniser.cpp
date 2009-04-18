/**
 * \file TestMovementRecogniser.cpp
 * \brief Implementation for testing cMovementRecogniser
 * \date 03/03/08
 */

#include <cppunit/config/SourcePrefix.h>
#include <vector>
#include <time.h>

#include "Movement.h"
#include "MovementRecogniser.h"
using namespace AGR;

#include "TestMovementRecogniser.h"

CPPUNIT_TEST_SUITE_REGISTRATION( cTestMovementRecogniser );



void cTestMovementRecogniser::setUp()
{
}

void cTestMovementRecogniser::tearDown()
{
}



/*! unitialised cMovementRecogniser tests
 */
void cTestMovementRecogniser::uninitialised()
{
	cMovementRecogniser MovRecon;
	CPPUNIT_ASSERT( MovRecon.isReady() == false );
	CPPUNIT_ASSERT( NULL == MovRecon.m_devicesArray );
	CPPUNIT_ASSERT( 0 == MovRecon.m_maxMovementSize );
	CPPUNIT_ASSERT( 0 == MovRecon.m_nbDevices );

	cVector3int vec;
	cMovementRecogniser::eMovementState movState;
	movState = MovRecon.addAcceleration( 0, vec );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );

	movState = MovRecon.getState( 0 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );
	
	cMovement movement;
	bool bRes = MovRecon.getMovement( 0, &movement);
	CPPUNIT_ASSERT( false == bRes );

	bRes = MovRecon.manualRecognitionStart( 0 );
	CPPUNIT_ASSERT( false == bRes );
	
	bRes = MovRecon.manualRecognitionStop( 0, &movement );
	CPPUNIT_ASSERT( false == bRes );
}



/*! cMovementRecogniser initialisation tests
 */
void cTestMovementRecogniser::initialisation()
{	
	cMovementRecogniser MovRecon;
	cMovementRecogniser::eMovementState movState;
	cMovement movement;
	cVector3int vec( 128, 128, 153 ); // gravity 25 on Z

	MovRecon.initialise( 2, 50 );
	CPPUNIT_ASSERT( MovRecon.isReady() == true );
	CPPUNIT_ASSERT( NULL != MovRecon.m_devicesArray );
	CPPUNIT_ASSERT( 50 == MovRecon.m_maxMovementSize );
	CPPUNIT_ASSERT( 2 == MovRecon.m_nbDevices );
	
	movState = MovRecon.getState( 0 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );
	movState = MovRecon.getState( 1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );
	movState = MovRecon.getState( 2 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );
	
	movState = MovRecon.addAcceleration( 0, vec );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );
	movState = MovRecon.addAcceleration( 1, vec );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );
	movState = MovRecon.addAcceleration( 2, vec );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );
	
	
	bool bRes = MovRecon.manualRecognitionStop( 0, &movement );
	CPPUNIT_ASSERT( false == bRes );
	CPPUNIT_ASSERT( 0 == movement.getSize() );
	bRes = MovRecon.manualRecognitionStop( 2, &movement );
	CPPUNIT_ASSERT( false == bRes );

	bRes = MovRecon.manualRecognitionStart( 0 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 1 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 2 );
	CPPUNIT_ASSERT( false == bRes );
	
	// Vectors are empty
	bRes = MovRecon.manualRecognitionStop( 0, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 0 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );

	bRes = MovRecon.manualRecognitionStop( 1, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );

	bRes = MovRecon.manualRecognitionStop( 2, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 2 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );

	MovRecon.clear();
	CPPUNIT_ASSERT( MovRecon.isReady() == false );
	CPPUNIT_ASSERT( NULL == MovRecon.m_devicesArray );
	CPPUNIT_ASSERT( 0 == MovRecon.m_maxMovementSize );
	CPPUNIT_ASSERT( 0 == MovRecon.m_nbDevices );
}



/*! cMovementRecogniser manual recognition tests
 */
void cTestMovementRecogniser::manualRecognition()
{
	cMovementRecogniser MovRecon;
	cMovementRecogniser::eMovementState movState;
	cMovement movement;
	cVector3int vec1( 1, 2, 3 );
	cVector3int vec2( 10, 20, 30 );

	bool bRes = MovRecon.initialise( 2, 10 );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( MovRecon.isReady() == true );
	CPPUNIT_ASSERT( NULL != MovRecon.m_devicesArray );
	CPPUNIT_ASSERT( 10 == MovRecon.m_maxMovementSize );
	CPPUNIT_ASSERT( 2 == MovRecon.m_nbDevices );

	bRes = MovRecon.manualRecognitionStart( 0 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 1 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 2 );
	CPPUNIT_ASSERT( false == bRes );
	
	// Vectors are empty
	bRes = MovRecon.manualRecognitionStop( 0, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 0 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );

	bRes = MovRecon.manualRecognitionStop( 1, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );

	bRes = MovRecon.manualRecognitionStop( 2, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 2 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );

	// Restart manipulation
	bRes = MovRecon.manualRecognitionStart( 0 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 1 );
	CPPUNIT_ASSERT( true == bRes );
	bRes = MovRecon.manualRecognitionStart( 2 );
	CPPUNIT_ASSERT( false == bRes );

	// add a vector
	movState = MovRecon.addAcceleration( 0, vec1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualInProgress == movState );
	movState = MovRecon.addAcceleration( 1, vec2 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualInProgress == movState );
	movState = MovRecon.addAcceleration( 2, vec1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );

	// one result is available
	bRes = MovRecon.manualRecognitionStop( 0, &movement );
	CPPUNIT_ASSERT( true == bRes );
	movState = MovRecon.getState( 0 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualFinished == movState );
	CPPUNIT_ASSERT( movement.getSize() == 1 );
	CPPUNIT_ASSERT( movement.getData(0) == vec1 );

	bRes = MovRecon.manualRecognitionStop( 1, &movement );
	CPPUNIT_ASSERT( true == bRes );
	movState = MovRecon.getState( 1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualFinished == movState );
	CPPUNIT_ASSERT( movement.getData(0) == vec2 );

	bRes = MovRecon.manualRecognitionStop( 2, &movement );
	CPPUNIT_ASSERT( false == bRes );
	movState = MovRecon.getState( 2 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementERROR == movState );
	
	// Fill the vector buffer
	bRes = MovRecon.manualRecognitionStart( 0 );
	CPPUNIT_ASSERT( true == bRes );
	for( unsigned int i=0; i<9; i++ )
	{
		movState = MovRecon.addAcceleration( 0, vec1 );
		CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualInProgress == movState );
	}
	movState = MovRecon.addAcceleration( 0, vec1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualOutOfBuffer == movState );
	bRes = MovRecon.getMovement( 0, &movement );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( movement.getSize() == MovRecon.getMaxMovementSize() );
	movState = MovRecon.addAcceleration( 0, vec1 );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementManualInProgress == movState );
}



/*! cMovementRecogniser automatic recognition tests
 */
void cTestMovementRecogniser::automaticRecognition()
{
	cMovementRecogniser MovRecon;
	cMovementRecogniser::eMovementState movState;
	cMovement movement;
	static const unsigned int gravity = 25;
	
	bool bRes = MovRecon.initialise( 1, 10 );
	CPPUNIT_ASSERT( true == bRes );

	// Data around the center
	for( int i=0; i<20; i++ )
	{
		movState = MovRecon.addAcceleration( 0, cVector3int(128,128,128+gravity)  );
		CPPUNIT_ASSERT( cMovementRecogniser::eMovementNone == movState );
	}

	// Find a movement, size = 8 (the previous movement(start), the 6 big values, the last movement (end))
	// Start of the movement, the few 
	for( int i=0; i<6; i++ )
	{
		movState = MovRecon.addAcceleration( 0, cVector3int(i*30,200+i*10,200+i) );
		CPPUNIT_ASSERT( cMovementRecogniser::eMovementInProgress == movState );
	}
	
	// Last movement
	movState = MovRecon.addAcceleration( 0, cVector3int(128,128,128+gravity)  );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementInProgress == movState );
	
	// Should recognise the movement as finished now
	movState = MovRecon.addAcceleration( 0, cVector3int(128,128,128+gravity)  );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementFinished == movState );
	bRes = MovRecon.getMovement( 0, &movement );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( movement.getSize() == 8 );
	
	// Out Of Buffer (8 +the first)
	for( int i=0; i<8; i++ )
	{
		movState = MovRecon.addAcceleration( 0, cVector3int(i*30,200+i*10,200+i) );
		CPPUNIT_ASSERT( cMovementRecogniser::eMovementInProgress == movState );
	}
	// the 10th
	movState = MovRecon.addAcceleration( 0, cVector3int(128,128,128+gravity)  );
	CPPUNIT_ASSERT( cMovementRecogniser::eMovementOutOfBuffer == movState );
	bRes = MovRecon.getMovement( 0, &movement );
	CPPUNIT_ASSERT( true == bRes );
	CPPUNIT_ASSERT( movement.getSize() == MovRecon.getMaxMovementSize() );
}
