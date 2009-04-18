/**
 * \file TestGestureRecogniser.h
 * \brief Declaraction for testing cGestureRecogniser
 * \date 02/03/08
 */

#ifndef _TESTGESTURERECOGNISER_H_
#define _TESTGESTURERECOGNISER_H_

#include <cppunit/extensions/HelperMacros.h>

namespace AGR {
class cGestureRecongiser;	// forward declaration
class cMovement;
class cVector3int;
class cHMM;
}


/**
 * \classe cTestGestureRecogniser
 * \brief Test class for cGestureRecongiser
 * To be used with CPPUnit
 */
class cTestGestureRecogniser : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE(cTestGestureRecogniser);
	CPPUNIT_TEST( uninitialised );
	CPPUNIT_TEST( vocabularyManipulation );
	CPPUNIT_TEST( gestureManipulation );
	CPPUNIT_TEST( GRManipulation );
	CPPUNIT_TEST( basicGestureRecognition );
	CPPUNIT_TEST_SUITE_END();

public:

	cTestGestureRecogniser();
	~cTestGestureRecogniser();

	virtual void setUp();
	virtual void tearDown();

	void uninitialised();
	void vocabularyManipulation();
	void gestureManipulation();
	void GRManipulation();
	void basicGestureRecognition();

protected:


	// Vocabulary 1 (used by gesture 1)
	AGR::cVocabulary m_vocabulary1;
	static const int m_v1_size= 5;
	static const int m_v1_movementSize = m_v1_size;
	static const int m_v1_nbMovement = 10;
	AGR::cMovement m_v1_movementArray [ m_v1_nbMovement ];

	// Vocabulary 2 (used by gesture 2 and 3)
	AGR::cVocabulary m_vocabulary2;
	static const int m_v2_size = 7;
	static const int m_v2_movementSize = m_v2_size;
	static const int m_v2_nbMovement = 20;
	AGR::cMovement m_v2_movementArray [ m_v2_nbMovement ];



	// Gesture 1 (use vocabulary 1)
	AGR::cGesture m_gesture1;
	static const int m_g1_nbHidden = 5;
	static const int m_g1_movementSize = m_g1_nbHidden;
	static const int m_g1_nbMovement = 15;
	AGR::cMovement m_g1_movementArray[ m_g1_nbMovement ];

	// Gesture 2 (use vocabulary 2)
	AGR::cGesture m_gesture2;
	static const int m_g2_nbHidden = 7;
	static const int m_g2_movementSize = m_g2_nbHidden;
	static const int m_g2_nbMovement = 10;
	AGR::cMovement m_g2_movementArray[ m_g2_nbMovement ];

	// Gesture 3 (use vocabulary 2)
	AGR::cGesture m_gesture3;
	static const int m_g3_nbHidden = 6;
	static const int m_g3_movementSize = m_g3_nbHidden;
	static const int m_g3_nbMovement = 10;
	AGR::cMovement m_g3_movementArray[ m_g3_nbMovement ];

};

#endif // _TESTGESTURERECOGNISER_H_
