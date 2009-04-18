/**
 * \file GestureRecogniser.h
 * \brief Declaration for the Gesture Recogniser class
 * \date 10/02/08
 */


#ifndef _GESTURERECOGNISER_H_
#define _GESTURERECOGNISER_H_

#include <fstream>

#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"
#include "Vocabulary.h"
#include "HMM.h"

//*********************
// Optional movement recognition
#include "MovementRecogniser.h"



class cTestGestureRecogniser;	// forward declaration for unit testing class

namespace AGR
{

class cHMM;					// defined in HMM.h
class cVocabulary;			// defined in Vocabulary.h
class cMovement;			// defined in Movement.h
class cVector3int;			// defined in Movement.h
class cMovementRecogniser;	// defined in MovementRecogniser.h


typedef cHMM cGesture;		/**< For more clarety, cHMM is named as cGesture  */

typedef int tGestureId;		/**< Type used to manipulate gesture ID  */
typedef int tVocabularyId;	/**< Type used to manipulate vocabulary ID */


/**
 * \class cGestureRecogniser
 * \brief Gesture recogniser class
 *
 * This class is the main Accelerometer Gesture Recognition class. 
 */
class cGestureRecogniser
{
friend class ::cTestGestureRecogniser;
public:

	cGestureRecogniser();
	~cGestureRecogniser();

	//*********************
	// Gesture Recogniser manipulation
	void clear();
	bool init( unsigned int nbVocabulary,
			   unsigned int nbGestures );
	bool isReady() const;
	bool load( const char* fileName );
	bool save( const char* fileName,
			   const bool binary = false ) const;
	bool resize( unsigned int newVocabularyBufferSize,
				 unsigned int newGestureBufferSize );
	unsigned int getNumberGesture();
	unsigned int getNumberVocabulary();

	//*********************
	// Recognition
	tGestureId recognise( const cMovement& movement ) const;
	bool recogniseGesture( const tGestureId gestureId,
						   const cMovement& movement ) const;
	tGestureId closerGesture( const cMovement& movement ) const;
	float gestureLikelihood( const tGestureId gestureId,
						     const cMovement& movement ) const;
	bool gesturesLikelihood( const cMovement& movement,
							 float* probabilityVector ) const;


	//*********************
	// Gesture manipulation
	tGestureId addGesture( const cGesture& gesture,
						   tVocabularyId vocabularyId = -1,
						   const char* gestureName = NULL );
	tGestureId loadGesture( const char* gestureFile,
							tVocabularyId vocabularyId, 
						    const char* gestureName = NULL );
	bool saveGesture( const tGestureId gestureId, 
					  const char* gestureFile,
					  const bool binary = false ) const;
	bool removeGesture( const tGestureId gestureId );
	cGesture* getGesture( const tGestureId gestureId ) const;
	const char* getGestureName( const tGestureId gestureId ) const;
	tGestureId findGestureByName( const char* gestureName ) const;

	//*********************
	// Vocabulary manipulation
	tVocabularyId addVocabulary( const cVocabulary& vocabulary,
								 const char* vocName = NULL );
	tVocabularyId loadVocabulary( const char* vocabularyFile,
								  const char* vocName = NULL );
	bool saveVocabulary( const tVocabularyId vocabularyId,
						 const char* vocabularyFile,
						 const bool binary = false ) const;
	bool removeVocabulary( const tVocabularyId vocabularyId );
	cVocabulary* getVocabulary( const tVocabularyId vocabularyId ) const;
	const char* getVocabularyName( const tVocabularyId vocabularyId ) const;
	tVocabularyId findVocabularyByName( const char* vocName ) const;

	
	//*********************
	// Optional movement recognition

	/*! Movement recogniser size accessor
	 * \return A pointer to the movement recogniser object
	 */
	inline cMovementRecogniser* getMovementRecogniser()
	{
		return &m_movementRecogniser;
	}

protected:	
	
	cVocabulary* m_vocabularyBuffer;		/**< Vocabulary buffer */
	cGesture*	 m_gestureBuffer;			/**< Gesture buffer */
	char**		 m_vocabularyNameBuffer;	/**< Vocabulary names buffer */
	char**		 m_gestureNameBuffer;		/**< Gesture names buffer */
	unsigned int m_vocabularySize;			/**< Size of the vocabulary buffer */
	unsigned int m_gestureSize;				/**< Size of the gesture buffer */
	
	bool loadASCII( std::ifstream& file );
	bool saveASCII( std::ofstream& file ) const;
	bool loadBinary( std::ifstream& file );
	bool saveBinary( std::ofstream& file ) const;

	bool isRecognised( const cGesture& gesture, 
					   const float probability ) const;
	
	//*********************
	// Optional movement recognition
	cMovementRecogniser m_movementRecogniser;	/**< Movement Recogniser optional component */

}; // class cGestureRecogniser


} // namespace AGR


#endif // _GESTURERECOGNISER_H_

