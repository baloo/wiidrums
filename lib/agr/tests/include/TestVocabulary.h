/**
 * \file TestVocabulary.h
 * \brief Declaraction for testing cVocabulary
 * \date 01/03/08
 */

#ifndef _TESTVOCABULARY_H_
#define _TESTVOCABULARY_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cVocabulary;	// forward declaration
class cMovement;
class cVector3int;
}


/**
 * \classe cTestVocabulary
 * \brief Test class for cVocabulary
 * To be used with CPPUnit
 */
class cTestVocabulary : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestVocabulary);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( initialisation );
	CPPUNIT_TEST( manipulation );
	CPPUNIT_TEST_SUITE_END();

public:

	cTestVocabulary();
	~cTestVocabulary();

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void initialisation();
	void manipulation();

protected:
	static const int m_nbClusters = 5;
	static const int m_movementSize = m_nbClusters;

	AGR::cMovement m_movement;
	AGR::cVector3int m_idealClusters[ m_nbClusters ];

	static const int m_nbMovement = 20;
	AGR::cMovement m_movementArray [ m_nbMovement ];
};

#endif // _TESTVOCABULARY_H_
