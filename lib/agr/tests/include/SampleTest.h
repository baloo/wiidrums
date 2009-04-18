/**
 * \file XXXXXTest.h
 * \brief Declaraction for testing XXXX
 * \date XX/XX/XX
 */

#ifndef _SAMPLETEST_H_
#define _SAMPLETEST_H_

#include <cppunit/extensions/HelperMacros.h>


/**
 * \classe cTypeTest
 * \brief Test class for the type declarations
 * To be used with CPPUnit
 */
class cSampleTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cSampleTest);
	CPPUNIT_TEST( test );
	CPPUNIT_TEST_SUITE_END();

public:
	cSampleTest();
	virtual ~cSampleTest();

	virtual void setUp();
	virtual void tearDown();

	void test();

protected:

};

#endif // _TYPETEST_H_
