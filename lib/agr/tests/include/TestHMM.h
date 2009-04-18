/**
 * \file TestHMM.h
 * \brief Declaraction for testing cHMM
 * \date 01/03/08
 */

#ifndef _TESTHMM_H_
#define _TESTHMM_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cHMM;	// forward declaration
class cVocabulary;
class cMovement;
class cVector3int;
}


/**
 * \classe cTestHMM
 * \brief Test class for cHMM
 * To be used with CPPUnit
 */
class cTestHMM : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestHMM);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( initialisation );
	CPPUNIT_TEST( hmmcreation );
	CPPUNIT_TEST( manipulation );
	CPPUNIT_TEST_SUITE_END();

public:

	cTestHMM();
	~cTestHMM();

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void initialisation();
	void hmmcreation();
	void manipulation();

protected:

	static const int m_nbClusters = 5;
	static const int m_movementSize = m_nbClusters;

	AGR::cMovement m_movement;
	AGR::cVector3int m_idealClusters[ m_nbClusters ];

	static const int m_nbMovement = 15;
	AGR::cMovement m_movementArray[ m_nbMovement ];
};

#endif // _TESTHMM_H_
