/**
 * \file TestVector3int.h
 * \brief Declaraction for testing cVector3int
 * \date 28/02/08
 */

#ifndef _TESTVECTOR3INT_H_
#define _TESTVECTOR3INT_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cVector3int;	// forward declaration
}


/**
 * \classe cTestVector3int
 * \brief Test class for cVector3int
 * To be used with CPPUnit
 */
class cTestVector3int : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestVector3int);
	CPPUNIT_TEST( constructor );
	CPPUNIT_TEST( operators );
	CPPUNIT_TEST_SUITE_END();

public:

	virtual void setUp();
	virtual void tearDown();

	void constructor();
	void operators();

protected:

};

#endif // _TESTVECTOR3INT_H_
