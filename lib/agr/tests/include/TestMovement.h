/**
 * \file TestMovement.h
 * \brief Declaraction for testing cMovement
 * \date 28/02/08
 */

#ifndef _TESTMOVEMENT_H_
#define _TESTMOVEMENT_H_

#include <cppunit/extensions/HelperMacros.h>


namespace AGR {
class cVector3int;	// forward declaration
class cMovement;
}


/**
 * \classe cTestMovement
 * \brief Test class for cMovement
 * To be used with CPPUnit
 */
class cTestMovement : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestMovement);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( initialisation );
	CPPUNIT_TEST( manipulation );
	CPPUNIT_TEST_SUITE_END();

public:

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void initialisation();
	void manipulation();

protected:

};

#endif // _TESTMOVEMENT_H_
