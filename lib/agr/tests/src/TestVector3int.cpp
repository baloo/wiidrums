/**
 * \file TestVector3int.cpp
 * \brief Implementation for testing cVector3int
 * \date 28/02/08
 */

#include <cppunit/config/SourcePrefix.h>
#include "TestVector3int.h"

#include "Movement.h"
using namespace AGR;

CPPUNIT_TEST_SUITE_REGISTRATION( cTestVector3int );


void cTestVector3int::setUp()
{
}

void cTestVector3int::tearDown()
{
}


/*! Constructor tests
 */
void cTestVector3int::constructor()
{
	cVector3int vect1;
	CPPUNIT_ASSERT_MESSAGE( "empty constructor test",
		vect1.x == 0 && vect1.y == 0  && vect1.z == 0 );

	cVector3int vect2( 10, -20, 30 );
	CPPUNIT_ASSERT_MESSAGE( "argument constructor test",
		vect2.x == 10 && vect2.y == -20  && vect2.z == 30 );

	cVector3int vect3( vect2 );
	CPPUNIT_ASSERT_MESSAGE( "copy constructor test",
		vect3.x == vect2.x && vect3.y == vect2.y  && vect3.z == vect2.z );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int copy constructor, check that the input is not changed",
		vect2.x == 10 && vect2.y == -20  && vect2.z == 30 );

	cVector3int vect4( 0.5f, 0xBEEF, -100/3 );
	CPPUNIT_ASSERT_MESSAGE( "constructor test with strange values",
		vect4.x == 0 && vect4.y == 48879  && vect4.z == -33 );

	cVector3int* vect5 = new cVector3int( vect4 );
	CPPUNIT_ASSERT( vect5 );
	*vect5 *= 5;
	delete vect5;

}


/*! operators tests
 */
void cTestVector3int::operators()
{
	cVector3int vect1 = cVector3int( 10, 20, 30 );
	cVector3int vect2 = cVector3int( 40, 50, 60 );

	cVector3int vect3 = vect1 + vect2;
	CPPUNIT_ASSERT_MESSAGE( "cVector3int addition test",
		vect3.x == 50 && vect3.y == 70  && vect3.z == 90 );

	vect3 = vect1 - vect2; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int substraction test",
		vect3.x == -30 && vect3.y == -30  && vect3.z == -30 );

	vect3 = vect1 * 5; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int multiplication test",
		vect3.x == 50 && vect3.y == 100  && vect3.z == 150 );
	
	vect3 = vect1 / 10; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int division test",
		vect3.x == 1 && vect3.y == 2  && vect3.z == 3 );

	vect3 = cVector3int( 10, 20, 30 );
	vect3 += vect2;
	CPPUNIT_ASSERT_MESSAGE( "cVector3int addition test",
		vect3.x == 50 && vect3.y == 70  && vect3.z == 90 );

	vect3 = cVector3int( 10, 20, 30 );
	vect3 -= vect2; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int substraction test",
		vect3.x == -30 && vect3.y == -30  && vect3.z == -30 );

	vect3 = cVector3int( 10, 20, 30 );
	vect3 *= 5; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int multiplication test",
		vect3.x == 50 && vect3.y == 100  && vect3.z == 150 );
	
	vect3 = cVector3int( 10, 20, 30 );
	vect3 /= 10; 
	CPPUNIT_ASSERT_MESSAGE( "cVector3int division test",
		vect3.x == 1 && vect3.y == 2  && vect3.z == 3 );
	
	vect1 = cVector3int( 10, 20, 30 );
	vect2 = cVector3int( 10, 20, 30 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int equal test",
							vect1 == vect2 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int equal test",
							vect2 == vect1 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int different test",
							false == (vect1 != vect2) );

	vect3 = cVector3int( 11, 20, 30 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int different test",
							vect1 != vect3 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int different test",
							false == (vect1 == vect3) );
	vect3 = cVector3int( 10, 19, 30 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int different test",
							vect1 != vect3 );
	vect3 = cVector3int( 10, 20, 31 );
	CPPUNIT_ASSERT_MESSAGE( "cVector3int different test",
							vect1 != vect3 );

}
