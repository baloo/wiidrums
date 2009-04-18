/**
 * \file TestMovementRecogniser.h
 * \brief Declaraction for testing cVocabulary
 * \date 01/03/08
 */

#ifndef _TESTMOVEMENTRECOGNITION_H_
#define _TESTMOVEMENTRECOGNITION_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cMovementRecogniser;	// forward declaration
}


/**
 * \classe cTestMovementRecogniser
 * \brief Test class for cMovementRecogniser
 * To be used with CPPUnit
 */
class cTestMovementRecogniser : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestMovementRecogniser);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( initialisation );
	CPPUNIT_TEST( manualRecognition );
	CPPUNIT_TEST( automaticRecognition );
	CPPUNIT_TEST_SUITE_END();

public:

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void initialisation();
	void manualRecognition();
	void automaticRecognition();

protected:
};

#endif // _TESTMOVEMENTRECOGNITION_H_
