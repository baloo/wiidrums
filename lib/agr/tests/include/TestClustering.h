/**
 * \file TestClustering.h
 * \brief Declaraction for testing cClustering and cKmeansClustering
 * \date 01/03/08
 */

#ifndef _TESTCLUSTERING_H_
#define _TESTCLUSTERING_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cClustring;		// forward declaration
class cKmeansClustring;	
class cMovement;	
class cVector3int;
}


/**
 * \classe cTestClustering
 * \brief Test class for cClustering and cKmeansClustering
 * To be used with CPPUnit
 */
class cTestClustering : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestClustering);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( intialisation );
	CPPUNIT_TEST( kmeansManipulation );
	CPPUNIT_TEST_SUITE_END();

public:

	cTestClustering();
	~cTestClustering();

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void intialisation();
	void kmeansManipulation();

protected:
	
	static const int m_nbClusters = 5;
	static const int m_movementSize = m_nbClusters;

	AGR::cMovement m_movement;
	AGR::cVector3int m_idealClusters[ m_nbClusters ];

	static const int m_nbMovement = 100;
	AGR::cMovement m_movementArray[ m_nbMovement ];

};

#endif // _TESTCLUSTERING_H_
