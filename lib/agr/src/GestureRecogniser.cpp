/**
* \file GestureRecogniser.cpp
* \brief Implementation for the gesture recogniser class
* \date 17/02/08
*/


#include <vector>
#include <fstream>

#include "GestureRecogniser.h"
#include "AGRVersion.h"


namespace AGR
{


/*! Constructor
 */	
cGestureRecogniser::cGestureRecogniser() : 
		m_vocabularyBuffer( NULL ),
		m_gestureBuffer( NULL ),
		m_vocabularyNameBuffer( NULL ),
		m_gestureNameBuffer( NULL ),
		m_vocabularySize( 0 ),
		m_gestureSize( 0 )
{
}



/*! Destructor
 */	
cGestureRecogniser::~cGestureRecogniser()
{
	clear();
}



/*! Clear all the data
 */	
void cGestureRecogniser::clear()
{
	if( m_vocabularyBuffer )
	{
		delete [] m_vocabularyBuffer;
		m_vocabularyBuffer = NULL;
	}	
	if( m_gestureBuffer )
	{
		delete [] m_gestureBuffer;
		m_gestureBuffer = NULL;
	}

	if( NULL != m_vocabularyNameBuffer )
	{
		for( unsigned int i=0; i<m_vocabularySize; i++ )
		{
			if( NULL != m_vocabularyNameBuffer[i] ){
				delete [] m_vocabularyNameBuffer[i];
				m_vocabularyNameBuffer[i] = NULL;
			}
		}
		delete [] m_vocabularyNameBuffer;
		m_vocabularyNameBuffer = NULL;
	}

	if( NULL != m_gestureNameBuffer )
	{
		for( unsigned int i=0; i<m_gestureSize; i++ )
		{
			if( NULL != m_gestureNameBuffer[i] ){
				delete [] m_gestureNameBuffer[i];
				m_gestureNameBuffer[i] = NULL;
			}
		}
		delete [] m_gestureNameBuffer;
		m_gestureNameBuffer = NULL;
	}

	m_vocabularySize = 0 ;
	m_gestureSize = 0 ;
	m_movementRecogniser.clear();
}



/*! Initialise the gesture recogniser object
 * \return Success
 */	
bool cGestureRecogniser::init( unsigned int nbVocabulary,	/**< Number of vacabulary to be used */
							   unsigned int nbGestures )	/**< Number of gestures to be used */
{
	if( 0 == nbVocabulary	||
		0 == nbGestures		)
	{
		return false;
	}

	clear();

	m_vocabularyBuffer = new cVocabulary [ nbVocabulary ];
	m_vocabularySize = nbVocabulary;

	m_gestureBuffer = new cGesture [ nbGestures ];
	m_gestureSize = nbGestures;

	m_vocabularyNameBuffer = new char* [nbVocabulary];
	memset( m_vocabularyNameBuffer, NULL, nbVocabulary*sizeof( char* ) );

	m_gestureNameBuffer = new char* [nbGestures];
	memset( m_gestureNameBuffer, NULL, nbGestures*sizeof( char* ) );


	return true;
}


/*! Inform if the vocabulary is ready to be used.
 * \return True if ready, false if not
 */	
bool cGestureRecogniser::isReady() const
{
	if( NULL == m_vocabularyBuffer	||
		NULL == m_gestureBuffer		)
	{
		return false;
	}
	return true;
}



/*! Load a cGestureRecogniser object from a file
 * \return Success
 */	
bool cGestureRecogniser::load( const char* fileName )	/**< Name of the file to read into*/
{
	if( !fileName ) {
		return false;
	}

	std::ifstream file;
	file.open( fileName, std::ios::in | std::ios::binary );
	
	if( file.is_open() == false ) {
		return false; 
	}

	// Check if binary with the header
	{	
		char buffer[3];

		std::streampos pos = file.tellg();
		file.read( buffer, 3 );
		if( 0 == memcmp( buffer, AGR_GESTURE_RECOGNISER_BIN_FILE_HEADER, 3 ) )
		{
			// binary
			file.seekg( pos );
			bool ret = loadBinary( file );
			file.close();
			return ret;			
		}
	}

	// The file is in ASCII format (or any other bad format)
	file.close();			// close the file in binary format...
	file.open( fileName );	// ... to reopen it in ascii format

	bool ret = loadASCII( file );
	file.close();
	return ret;
}



/*! Save a cGestureRecogniser object to a file
 * \return Success
 */	
bool cGestureRecogniser::save( const char* fileName,	/**< Name of the file to save into */
							   const bool binary		/**< Specify whether saving in binary of ASCII format */
							   ) const
{
	// Check input
	if( NULL == fileName ) {
		return false;
	}

	// Only save if ready
	if( isReady() == false ) {
		return false;
	}

	if( binary )
	{
		std::ofstream file;
		file.open( fileName, std::ios::out | std::ios::binary );
		bool ret = saveBinary( file );
		file.close();
		return ret;
	}
	else 
	{
		std::ofstream file;
		file.open( fileName );
		bool ret = saveASCII( file );
		file.close();
		return ret;
	}
}



/*! Resize the gesture and vocabulary buffers
 * \note Set the values to 0 to adjust the buffer to the numbers of object actually stored and ready
 * \return Success
 */	
bool cGestureRecogniser::resize( unsigned int newVocabularyBufferSize,	/**< Size of the new vocabulary buffer (0 for the actual number of vocabulary ready) */
								 unsigned int newGestureBufferSize )	/**< Size of the new gesture buffer (0 for the actual number of gesture ready) */
{
	if( newVocabularyBufferSize == m_vocabularySize &&
		newGestureBufferSize    == m_gestureSize	)
	{
		// Won't bother resizing to the same size !
		return true;
	}

	// Count existing buffer size
	unsigned int nbVocabulary = 0;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() == true ){
			nbVocabulary ++;
		}
	}
	if( 0 == newVocabularyBufferSize ) {
		newVocabularyBufferSize = nbVocabulary;
	}

	unsigned int nbGesture = 0;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() == true ){
			nbGesture ++;
		}
	}
	if( 0 == newGestureBufferSize ) {
		newGestureBufferSize = nbGesture;
	}

	if( newVocabularyBufferSize < nbVocabulary ||
		newGestureBufferSize < nbGesture )
	{
		// Not enough place to store existing objects
		return false;
	}


	// Create and copy vocabulary
	if( m_vocabularySize != newVocabularyBufferSize )
	{
		cVocabulary* oldVocabularyBuffer = m_vocabularyBuffer;
		char**		 oldVocabularyNameBuffer = m_vocabularyNameBuffer;

		m_vocabularyBuffer = new cVocabulary[ newVocabularyBufferSize ];
		m_vocabularyNameBuffer = new char* [ newVocabularyBufferSize ];
		memset( m_vocabularyNameBuffer, 0x00, sizeof(char*)*newVocabularyBufferSize );

		unsigned int idNewBuffer = 0;
		for( unsigned int i=0; i<m_vocabularySize; i++ )
		{
			if( oldVocabularyBuffer[i].isReady() )
			{
				if( idNewBuffer < newVocabularyBufferSize )
				{
					m_vocabularyBuffer[ idNewBuffer ].copy( oldVocabularyBuffer[i] );
					if( oldVocabularyNameBuffer[i] ) 
					{
						m_vocabularyNameBuffer[ idNewBuffer ] = new char [ strlen(oldVocabularyNameBuffer[i])+1 ];
						strcpy( m_vocabularyNameBuffer[ idNewBuffer ], oldVocabularyNameBuffer[i] );
					}

					// Change gestures using this vocabulary
					for( unsigned int j=0; j<m_gestureSize; j++ )
					{
						if( m_gestureBuffer[ j ].getVocabulary() == &oldVocabularyBuffer[i] ) 
						{
							m_gestureBuffer[ j ].assignVocabulary( &m_vocabularyBuffer[ idNewBuffer ] );
						}
					}

					idNewBuffer++;
				}
				else {
					return false;
				}
				
			}
		}

		// Free old memory alocations
		for( unsigned int i=0; i<m_vocabularySize; i++ )
		{
			if( oldVocabularyNameBuffer[i] ) {
				delete [] oldVocabularyNameBuffer[i];
			}
		}
		delete [] oldVocabularyNameBuffer;
		delete [] oldVocabularyBuffer;
		m_vocabularySize = newVocabularyBufferSize;
	}

	// Create and copy gesture
	if( m_gestureSize != newGestureBufferSize )
	{
		cGesture*	 oldGestureBuffer = m_gestureBuffer;
		char**		 oldGestureNameBuffer = m_gestureNameBuffer;

		m_gestureBuffer = new cGesture[ newGestureBufferSize ];
		m_gestureNameBuffer = new char* [ newGestureBufferSize ];
		memset( m_gestureNameBuffer, 0x00, sizeof(char*)*newGestureBufferSize );

		unsigned int idNewBuffer = 0;
		for( unsigned int i=0; i<m_gestureSize; i++ )
		{
			if( oldGestureBuffer[i].isReady() )
			{
				if( idNewBuffer < newGestureBufferSize )
				{
					m_gestureBuffer[idNewBuffer].copy( oldGestureBuffer[i] );
					if( oldGestureNameBuffer[i] )
					{
						m_gestureNameBuffer[ idNewBuffer ] = new char [ strlen(oldGestureNameBuffer[i])+1 ];
						strcpy( m_gestureNameBuffer[ idNewBuffer ], oldGestureNameBuffer[i] );
					}

					idNewBuffer++;
				}
				else {
					return false;
				}
			}
		}

		// Free old memory alocations
		for( unsigned int i=0; i<m_gestureSize; i++ )
		{
			if( oldGestureNameBuffer[i] ) {
				delete [] oldGestureNameBuffer[i];
			}
		}
		delete [] oldGestureNameBuffer;
		delete [] oldGestureBuffer;
		m_gestureSize = newGestureBufferSize;
	}

	return true;
}



/*! Return the number of gestures objects
 * \return the number of gestures objects
 */	
unsigned int cGestureRecogniser::getNumberGesture()
{
	return m_gestureSize;
}



/*! Return the number of vocabulary objects
 * \return the number of vocabulary objects
 */	
unsigned int cGestureRecogniser::getNumberVocabulary()
{
	return m_vocabularySize;
}



/*! Add a gesture into the gesture buffer.
 * \return -1 if the function failed, the id of the gesture otherwith.
 * \note If a vocabularyId is provided, the vocabulary object is then linked with the HMM.
 * \note If the vocabularyId is negative, the vocabulary associated the gesture need to be inside the vocabulary buffer. 
 * \note The input gesture is copied in the buffer.
 */	
tGestureId cGestureRecogniser::addGesture( const cGesture& gesture,		/**< Gesture to be added */
										   tVocabularyId vocabularyId,	/**< Id of the vocabulary associated with the gesture, see function's note */
										   const char* gestureName )	/**< Name of the gesture if wanted */
{
	// check initialisation
	if( isReady() == false	)
	{
		return -1;
	}

	// If the vocabulary is specified in the vocabulary
	const cVocabulary* ptVoc = NULL;
	if( 0 <= vocabularyId )
	{
		// link the vocabulary
		ptVoc = getVocabulary( vocabularyId );
		if( NULL == ptVoc ) {
			return -1;
		}
	}

	// check that the gesture is ready
	if( gesture.isReady() == false ){ 
		return -1;
	}

	// check that the gesture vocabulary is present in the vocabulary buffer
	// Only when no vocabularyId is provided
	if( 0 > vocabularyId )
	{
		ptVoc = gesture.getVocabulary();
		bool found = false;
		for( unsigned int i=0; i<m_vocabularySize; i++ )
		{
			if( ptVoc == &m_vocabularyBuffer[i] )
			{
				found = true;
				break;
			}
		}

		if( false == found ) {
			return -1; // vocabulary not present
		}
	}

    // find an not initialised gesture to for storage
	tGestureId emptyID = -1;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() == false )
		{
			emptyID = i;
			break;
		}
	}

	if( -1 == emptyID ) {
		return -1;	// no available storage space
	}

	if( m_gestureBuffer[ emptyID ].copy( gesture ) == false ) { 
		return -1; // copy failed
	}

	// If the Vocabulary id was specified and found, assign it now
	// ( Could not do it earlier, since it would involve modifying the input gesture )
	if( NULL != ptVoc ) {
		m_gestureBuffer[ emptyID ].assignVocabulary( ptVoc );
	}
	
	// Assign its name
	if( gestureName					&& 
		strlen( gestureName ) > 0	&&
		m_gestureNameBuffer			)
	{
		if( NULL != m_gestureNameBuffer[ emptyID ] )
			delete [] m_gestureNameBuffer[ emptyID ] ;
		m_gestureNameBuffer[ emptyID ] = new char[ strlen( gestureName )+1 ];
		strcpy( m_gestureNameBuffer[ emptyID ], gestureName );
	}

	return emptyID;
}



/*! Load a gesture from a file
 * \return -1 if the function failed, the id of the gesture otherwith.
 * \note The vocabularyId need to be provided, in order to be linked with the gesture. 
 */	
tGestureId cGestureRecogniser::loadGesture( const char* gestureFile,		/**< File where the gesture is read */
											tVocabularyId vocabularyId,		/**< Id of the vocabulary associated with the gesture */
											const char* gestureName )		/**< Name of the gesture if wanted */
{
	
	// check initialisation
	if( isReady() == false	)
	{
		return -1;
	}

	// Find the vocabulary
	const cVocabulary* ptVoc = NULL;
	ptVoc = getVocabulary( vocabularyId );
	if( NULL == ptVoc ) {
		return -1; // not found
	}

	// find an not initialised gesture to for storage
	tGestureId emptyID = -1;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() == false )
		{
			emptyID = i;
			break;
		}
	}

	if( -1 == emptyID ) {
		return -1;	// no available storage space
	}

	if( m_gestureBuffer[ emptyID ].load( gestureFile ) == false ){
		return -1;
	}

	if( m_gestureBuffer[ emptyID ].assignVocabulary( ptVoc )== false ){
		m_gestureBuffer[ emptyID ].clear();
		return -1;
	}
	
	// Assign its name
	if( gestureName					&& 
		strlen( gestureName ) > 0	&&
		m_gestureNameBuffer			)
	{
		if( NULL != m_gestureNameBuffer[ emptyID ] )
			delete [] m_gestureNameBuffer[ emptyID ] ;
		m_gestureNameBuffer[ emptyID ] = new char[ strlen( gestureName )+1 ];
		strcpy( m_gestureNameBuffer[ emptyID ], gestureName );
	}

	return emptyID;
}



/*! Save a gesture to a file
 * \note The gesture name is not saved
 * \return Success
 */	
bool cGestureRecogniser::saveGesture( const tGestureId gestureId,	/**< Id of the gesture to be saved */
									  const char* gestureFile,		/**< Name of the file to save in */
									  const bool binary				/**< Specify whether saving in binary of ASCII format */
									  ) const
{
	const cGesture* gesture = getGesture( gestureId );

	if( gesture == NULL ) { 
		return false;
	}

	return gesture->save( gestureFile, binary );
}



/*! Remove a gesture from the buffer
 * \return Success
 * \note The gesture is not deleted, but put in an not initialised state for being reused later
 */	
bool cGestureRecogniser::removeGesture( const tGestureId gestureId ) /**< Id of the gesture to be removed */
{
	if( gestureId < 0									||
		gestureId >= (int) m_gestureSize				||
		NULL == m_gestureBuffer							||
		m_gestureBuffer[ gestureId ].isReady() == false	)
	{
		return false;
	}

	m_gestureBuffer[ gestureId ].clear();
	return true;
}



/*! Get a gesture in the gesture buffer by its id. 
 * \return NULL if failed, the requested gesture otherwise 
 */	
cGesture* cGestureRecogniser::getGesture( const tGestureId gestureId  /**< Id of the gesture */
										  ) const
{
	if( gestureId < 0 ||
		gestureId >= (int) m_gestureSize ||
		NULL == m_gestureBuffer )
	{
		return NULL;
	}

	return &m_gestureBuffer[ gestureId ];
}



/*! Get a gesture name by its id. 
 * \return NULL if failed, the requested gesture name otherwise 
 */	
const char* cGestureRecogniser::getGestureName( const tGestureId gestureId	/**< Id of the gesture */
											    ) const
{
	if( gestureId < 0							||
		gestureId >= (int)m_gestureSize			||
		NULL == m_gestureNameBuffer				||
		NULL == m_gestureNameBuffer[gestureId]	)
	{
		return NULL;
	}

	return m_gestureNameBuffer[gestureId];
}



/*! Find a gesture id by its name. 
 * \return -1 if not found or failed, the requested gesture id otherwise 
 */	
tGestureId cGestureRecogniser::findGestureByName( const char* gestureName  /**< Name of the gesture */
												  ) const
{
	if( NULL == gestureName			||
		NULL == m_gestureNameBuffer	)
	{
		return -1;
	}

	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureNameBuffer[i]								&&
			0 == strcmp( gestureName, m_gestureNameBuffer[i])	)
		{
			return i;
		}
	}

	return -1;
}



/*! Add a vocabulary object to the buffer
 * \return -1 if the function failed, the id of the vocabulary otherwith.
 * \note The input vocabulary is copied in the buffer.
 */	
tVocabularyId cGestureRecogniser::addVocabulary( const cVocabulary& vocabulary,	/**< Vocabulary to be added */
												 const char* vocName )			/**< Name for the vocabulary if wanted */
{
	// check initialisation
	if( NULL == m_vocabularyBuffer )
	{
		return -1;
	}

	// check that the vocabulary is ready
	if( vocabulary.isReady() == false ){ 
		return -1;
	}

    // find an not initialised gesture to for storage
	tVocabularyId emptyID = -1;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() == false )
		{
			emptyID = i;
			break;
		}
	}

	if( -1 == emptyID ) {
		return -1;	// no available storage space
	}

	if( m_vocabularyBuffer[ emptyID ].copy( vocabulary ) == false ) 
	{ 
		return -1; // copy failed
	}
	
	// Assign its name
	if( vocName					&& 
		strlen( vocName ) > 0	&&
		m_vocabularyNameBuffer	)
	{
		if( NULL != m_vocabularyNameBuffer[ emptyID ] )
			delete [] m_vocabularyNameBuffer[ emptyID ] ;
		m_vocabularyNameBuffer[ emptyID ] = new char[ strlen( vocName )+1 ];
		strcpy( m_vocabularyNameBuffer[ emptyID ], vocName );
	}

	return emptyID;
}



/*! Load a vocabulary from a file
 * \return -1 if the function failed, the id of the gesture otherwith.
 */	
tVocabularyId cGestureRecogniser::loadVocabulary( const char* vocabularyFile,	/**< File where the vocabulary is read */
												  const char* vocName )			/**< Name for the vocabulary if wanted */
{
	// check initialisation
	if( NULL == m_vocabularyBuffer ) {
		return -1;
	}

    // find an not initialised gesture to for storage
	tVocabularyId emptyID = -1;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() == false )
		{
			emptyID = i;
			break;
		}
	}

	if( -1 == emptyID ) {
		return -1;	// no available storage space
	}

	// load file
	if( m_vocabularyBuffer[ emptyID ].load( vocabularyFile ) == false)
	{ 
		return -1; // loading failed
	}

	// check that the vocabulary is ready
	if( m_vocabularyBuffer[ emptyID ].isReady() == false )
	{ 
		m_vocabularyBuffer[ emptyID ].clear();
		return -1;
	}

	// Assign its name
	if( vocName					&& 
		strlen( vocName ) > 0	&&
		m_vocabularyNameBuffer	)
	{
		if( NULL != m_vocabularyNameBuffer[ emptyID ] )
			delete [] m_vocabularyNameBuffer[ emptyID ] ;
		m_vocabularyNameBuffer[ emptyID ] = new char[ strlen( vocName )+1 ];
		strcpy( m_vocabularyNameBuffer[ emptyID ], vocName );
	}

	return emptyID;
}



/*! Save a vocabulary to a file
 * \note The vocabulary name is not saved
 * \return Success
 */	
bool cGestureRecogniser::saveVocabulary( const tVocabularyId vocabularyId,	/**< Id of the vocabulary to be saved */
										 const char* vocabularyFile,		/**< Name of the file to save in */
										 const bool binary					/**< Specify whether saving in binary of ASCII format */
										 ) const
{
	const cVocabulary* vocabulary = getVocabulary( vocabularyId );

	if( vocabulary == NULL ) { 
		return false;
	}

	return vocabulary->save( vocabularyFile, binary );
}



/*! Remove a vocabulary from the buffer
 * \return Success
 * \note The vocabulary is not deleted, but put in an not initialised state for being reused later
 */	
bool cGestureRecogniser::removeVocabulary( const tVocabularyId vocabularyId ) /**< Id of the vocabulary to be removed */
{
	if( vocabularyId < 0										||
		vocabularyId >= (int)m_vocabularySize					||
		NULL == m_vocabularyBuffer								||
		NULL == m_gestureBuffer									||
		m_vocabularyBuffer[ vocabularyId ].isReady() == false	)
	{
		return false;
	}

	cVocabulary* vocabulary = &m_vocabularyBuffer[ vocabularyId ];
	
	//if( vocabulary->isReady() == false )
	//{
	//	// Vocabulary not initialised, therefore doesn't need to be removed
	//	return false;
	//}

	// Check if the vocabulary is not used by a gesture
	for( unsigned int i = 0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].getVocabulary() == vocabulary )
		{
			return false; // In use by a gesture, remove the gesture first
		}
	}

	vocabulary->clear();

	return true;
}



/*! Get a vocabulary in the vocabulary buffer by its id. 
 * \return NULL if failed, the requested vocabulary otherwise 
 */	
cVocabulary* cGestureRecogniser::getVocabulary( const tVocabularyId vocabularyId	/**< Id of the vocabulary */
												) const
{
	if( vocabularyId < 0 ||
		vocabularyId >= (int)m_vocabularySize ||
		NULL == m_vocabularyBuffer )
	{
		return NULL;
	}

	return &m_vocabularyBuffer[ vocabularyId ];
}



/*! Get a vocabulary name by its id. 
 * \return NULL if failed, the requested vocabulary name otherwise 
 */	
const char* cGestureRecogniser::getVocabularyName( const tVocabularyId vocabularyId	/**< Id of the vocabulary */
												   ) const
{
	if( vocabularyId < 0								||
		vocabularyId >= (int)m_vocabularySize			||
		NULL == m_vocabularyNameBuffer					||
		NULL == m_vocabularyNameBuffer[vocabularyId]	)
	{
		return NULL;
	}

	return m_vocabularyNameBuffer[vocabularyId];
}



/*! Find a gesture id by its name. 
 * \return -1 if not found or failed, the requested vocabulary id otherwise 
 */	
tVocabularyId cGestureRecogniser::findVocabularyByName( const char* vocName  /**< Name of the vocabulary */
														) const
{
	if( NULL == vocName					||
		NULL == m_vocabularyNameBuffer	)
	{
		return -1;
	}

	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_vocabularyNameBuffer[i] &&
			0 == strcmp( vocName, m_vocabularyNameBuffer[i]) )
		{
			return i;
		}
	}

	return -1;
}



/*! Get the id of the recognised gesture if any
 * \return -1 if failed, otherwise the id of the recognised gesture
 */	
tGestureId cGestureRecogniser::recognise( const cMovement& movement		/**< Movement to be analysed */
										  ) const
{
	if( movement.getSize() == 0 ) {
		return -1;
	}

	tGestureId closerId = -1;
	float bestProba = 0.0f;

	for( unsigned int i = 0; i < m_gestureSize; i++ )
	{
		const cGesture* gesture = getGesture( i );

		if( NULL == gesture ||
			gesture->isReady() == false )
		{
			continue; // skip it
		}

		float proba = gesture->forward( movement ) * gesture->getObservationSize(); 

		if( proba > bestProba && 
			isRecognised( *gesture, proba ) )
		{
			closerId = i;
			bestProba = proba;
		}
	}
	
	return closerId;
}



/*! Inform whether or not the movement is recognised according to the gesture
 * \return true if the gesture is recognised, false otherwise
 */	
bool cGestureRecogniser::recogniseGesture( const tGestureId gestureId,	/**< Gesture to be analysed */
										   const cMovement& movement	/**< Movement to be analysed */
										   ) const
{
	if( movement.getSize() == 0 ) {
		return false;
	}

	const cGesture* gesture = getGesture( gestureId );

	if( NULL == gesture ||
		gesture->isReady() == false )
	{
		return false;
	}
	float proba = gesture->forward( movement ) * gesture->getObservationSize(); 
	
	return isRecognised( *gesture, proba );
}



/*! Find the closer gesture to a movement
 * \note The probability used is weighed with the observation size
 * \return the id of the gesture with the highest probability
 */	
tGestureId cGestureRecogniser::closerGesture( const cMovement& movement	/**< Movement to be analysed */
											  ) const
{
	if( movement.getSize() == 0 ) {
		return -1;
	}

	tGestureId closestId = -1;
	float closerValue = 0.0f;

	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() )
		{
			float prob = m_gestureBuffer[i].forward( movement ) * m_gestureBuffer[i].getObservationSize();
			if( prob > closerValue )
			{
				closerValue = prob;
				closestId = i;
			}
		}
	}
	return closestId;
}


/*! Get the probability likelihood of the differents gestures of a movement
 * \note The result is weighed with the observation size
 */	
float cGestureRecogniser::gestureLikelihood( const tGestureId gestureId,	/**< Gesture to be analysed */
											 const cMovement& movement		/**< Movement to be analysed */
											 ) const
{	
	if( movement.getSize() == 0 ) {
		return 0.0f;
	}

	if(NULL == m_gestureBuffer )
	{
		return false;
	}

	const cGesture* gesture = getGesture( gestureId );

	if( NULL == gesture ||
		gesture->isReady() == false )
	{
			return 0.0f;
	}
	
	return gesture->forward( movement ) * gesture->getObservationSize();

}


/*! Get the probability likelihood of the differents gestures of a movement
 * \note The probability returned are weighed with their observation size
 */	
bool cGestureRecogniser::gesturesLikelihood( const cMovement& movement,	/**< Movement to be analysed */
											 float* probabilityVector	/**< Array for storing the probability likelihood, need to be the same size as the gesture buffer*/
											 ) const
{	
	if( NULL == probabilityVector	||
		NULL == m_gestureBuffer		)
	{
		return false;
	}

	for( unsigned int i = 0; i< m_gestureSize; i++ )
	{
		const cGesture* gesture = getGesture( i );

		if( NULL == gesture ||
			gesture->isReady() == false )
		{
			probabilityVector[ i ] = 0.0f;
		}
		else
		{
			probabilityVector[ i ] = gesture->forward( movement )* gesture->getObservationSize();
		}
	}

	return true;
}



/*! Load from an ASCII file
 * \return Success
 */
bool cGestureRecogniser::loadASCII( std::ifstream& file )
{
	if( file.is_open() == false ) {
		return false;
	}

	// version checking
	unsigned int version;
	char str[100];
	file >> str >> str >> str >> str >> version;
	if( version != AGR_GESTURE_RECOGNISER_ASCII_FILE_VERSION )
	{
		// TODO : backward compatibility
		return false;
	}
	file.getline( str, 100, '\n' );	// EOL

	clear();
	
	// Skip file info
	file.getline( str, 100, '\n' );

	// Nb Vocabulary
	file >> str >> str >> str >> str >> m_vocabularySize;
	file.getline( str, 100, '\n' );	// EOL
	m_vocabularyBuffer = new cVocabulary[ m_vocabularySize ];
	m_vocabularyNameBuffer = new char* [ m_vocabularySize ];
	memset( m_vocabularyNameBuffer, 0x00, m_vocabularySize*sizeof(char*) );
	
	// Skip file info
	file.getline( str, 100, '\n' ); // empty line
	file.getline( str, 100, '\n' ); // Vocabulary blabla

	// Read vocabulary
	for( unsigned int i=0; i< m_vocabularySize; i++ )
	{
		
		file.getline( str, 100, '\n' ); // ---------------

		// Vocabulary Name
		file.getline( str, 100, '\n' );
		if( 0 != strcmp( str, "NoName" )	&&
			0 != strlen(str)				&&
			NULL != m_vocabularyNameBuffer	)
		{
			int size = (int)(strlen(str)+1);
			m_vocabularyNameBuffer[i] = new char[ strlen(str)+1 ];
			strcpy( m_vocabularyNameBuffer[i], str );
			size++;
		}

		file.getline( str, 100, '\n' ); // - - - - - - - -
		m_vocabularyBuffer[ i ].loadASCII( file );
		file.getline( str, 100, '\n' ); // ---------------
		file.getline( str, 100, '\n' ); // empty line
	}

	// Nb gestures
	file >> str >> str >> str >> str >> m_gestureSize;
	file.getline( str, 100, '\n' );	// EOL
	m_gestureBuffer = new cGesture[ m_gestureSize ];
	m_gestureNameBuffer = new char* [ m_gestureSize ];
	memset( m_gestureNameBuffer, 0x00, m_gestureSize*sizeof(char*) );
	
	// Skip file info
	file.getline( str, 100, '\n' ); // empty line
	file.getline( str, 100, '\n' ); // gesture blabla
	
	// Read gestures
	for( unsigned int i=0; i< m_gestureSize; i++ )
	{		
		file.getline( str, 100, '\n' ); // ---------------

		// Vocabulary Name
		file.getline( str, 100, '\n' );
		if( 0 != strcmp( str, "NoName" ) &&
			0 != strlen( str )			 &&
			NULL != m_gestureNameBuffer	 )
		{
			m_gestureNameBuffer[i] = new char[ strlen(str)+1 ];
			strcpy( m_gestureNameBuffer[i], str );
		}

		file.getline( str, 100, '\n' ); // - - - - - - - -
		m_gestureBuffer[ i ].loadASCII( file );
		file.getline( str, 100, '\n' ); // - - - - - - - -

		// Vocabulary Id
		unsigned int vocId;
		file >> str >> str >> str >> vocId;
		m_gestureBuffer[ i ].assignVocabulary( getVocabulary( vocId ) );
		file.getline( str, 100, '\n' ); // empty line
		file.getline( str, 100, '\n' ); // ---------------
		file.getline( str, 100, '\n' ); // empty line
	}

	return true;
}



/*! Save from an ASCII file
 * \return Success
 */
bool cGestureRecogniser::saveASCII( std::ofstream& file ) const
{
	if( file.is_open() == false ) {
		return false;
	}

	// File informations
	file << "ASCII AGR File version " << AGR_GESTURE_RECOGNISER_ASCII_FILE_VERSION << std::endl;
	file << "Created with AGR v" << AGR_VERSION << "." << AGR_VERSION_MIN << std::endl;
	file << std::endl;

	// Number of vocabulary :  only those used
	unsigned int nbVocabulary = 0;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() )
		{
			nbVocabulary ++;
		}
	}
	file << "Nb Vocabulary object stored: " << nbVocabulary << std::endl << std::endl;

	file << "*** Vocabulary objects ***" << std::endl;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() )
		{
			file << "---------------" << std::endl;

			if( NULL != m_vocabularyNameBuffer		&&
				NULL != m_vocabularyNameBuffer[i]	)
				file << m_vocabularyNameBuffer[i] << std::endl;
			else
				file << "NoName" << std::endl;

			file << "- - - - - - - -" << std::endl;
			m_vocabularyBuffer[i].saveASCII( file );
			file << "---------------" << std::endl << std::endl;
		}
	}


	// Number of gestures :  only those used
	unsigned int nbGestures = 0;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() )
		{
			nbGestures ++;
		}
	}
	file << "Nb Gestures object stored: " << nbGestures << std::endl << std::endl;

	file << "*** Gestures objects ***" << std::endl;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() )
		{
			file << "---------------" << std::endl;

			if( NULL != m_gestureNameBuffer		&& 
				NULL != m_gestureNameBuffer[i]	)
				file << m_gestureNameBuffer[i] << std::endl;
			else
				file << "NoName" << std::endl;

			file << "- - - - - - - -" << std::endl;
			m_gestureBuffer[i].saveASCII( file );
			file << "- - - - - - - -" << std::endl;
			
			// Write the vocabulary Id associated with it
			unsigned int idVoc = 0;
			for( unsigned int j=0; j<m_vocabularySize; j++ )
			{
				if( m_vocabularyBuffer[j].isReady() )
				{
					if( m_gestureBuffer[i].m_ptVocabulary == &(m_vocabularyBuffer[j]) )	{
						break;
					}
					else{
						idVoc++;
					}
				}
			}	
			file << "Id Vocabulary associated: " << idVoc << std::endl;
			file << "---------------" << std::endl << std::endl;
		}
	}

	return true;
}



/*! Load from a binary file
 * \return Success
 * \todo Endianness
 */
bool cGestureRecogniser::loadBinary( std::ifstream& file )
{
	if( file.is_open() == false ) {
		return false;
	}

	{	// Binary header
		char buffer[3];
		if( !file.read( buffer, 3 ) ) {
			return false;
		}

		// Check header
		if( 0 != memcmp( buffer, AGR_GESTURE_RECOGNISER_BIN_FILE_HEADER, 3 ) ) {
			return false;
		}
	}

	{// Version
		unsigned int version;
		if( !file.read( (char*)(&version), sizeof(version) ) ) {
			return false;
		}

		// Check header
		if( version != AGR_GESTURE_RECOGNISER_BINARY_FILE_VERSION ) {
			return false;
		}
	}

	if( !file.read( (char*)(&m_vocabularySize), sizeof(m_vocabularySize) ) ){
		return false;
	}
	if( 0 == m_vocabularySize ){
		return false;
	}
	m_vocabularyBuffer = new cVocabulary[ m_vocabularySize ];
	m_vocabularyNameBuffer = new char* [ m_vocabularySize ];
	memset( m_vocabularyNameBuffer, 0x00, m_vocabularySize*sizeof(char*) );

	// Load the vocabulary
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		// load its name
		unsigned int nameSize;
		if( !file.read( (char*)(&nameSize), sizeof(int) ) ){
			return false;	
		}
		if( 0 < nameSize ) {
			m_vocabularyNameBuffer[i] = new char[ nameSize+1 ];
			if( !file.read( m_vocabularyNameBuffer[i], nameSize ) ){
				return false;	
			}
			m_vocabularyNameBuffer[i][nameSize] = '\0';
		}

		// load the object
		m_vocabularyBuffer[i].loadBinary( file );
	}
	

	if( !file.read( (char*)(&m_gestureSize), sizeof(m_gestureSize) ) ){
		return false;
	}
	if( 0 == m_gestureSize ){
		return false;
	}
	m_gestureBuffer = new cGesture[ m_gestureSize ];
	m_gestureNameBuffer = new char* [ m_gestureSize ];
	memset( m_gestureNameBuffer, 0x00, m_gestureSize*sizeof(char*) );

	// Load gesture
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		// load its name
		unsigned int nameSize;
		if( !file.read( (char*)(&nameSize), sizeof(int) ) ){
			return false;	
		}
		if( 0 < nameSize ) {
			m_gestureNameBuffer[i] = new char[ nameSize+1 ];
			if( !file.read( m_gestureNameBuffer[i], nameSize ) ){
				return false;	
			}
			m_gestureNameBuffer[i][nameSize] = '\0';
		}

		// load the object
		m_gestureBuffer[i].loadBinary( file );
			
		// Vocabulary Id associated with it
		unsigned int idVoc = 0;
		if( !file.read( (char*)(&idVoc), sizeof(idVoc) ) ){
			return false;
		}
		m_gestureBuffer[ i ].assignVocabulary( getVocabulary( idVoc ) );
			
	}
	return true;
}



/*! Save from a binary file
 * \return Success
 * \todo Endianness
 */
bool cGestureRecogniser::saveBinary( std::ofstream& file ) const
{
	if( file.is_open() == false ) {
		return false;
	}

	// Binary header
	file.write( AGR_GESTURE_RECOGNISER_BIN_FILE_HEADER, 3 );

	// Version
	file.write( (char*)(&AGR_GESTURE_RECOGNISER_BINARY_FILE_VERSION), sizeof(unsigned int) );

	// Number of vocabulary :  only those used
	unsigned int nbVocabulary = 0;
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() )
		{
			nbVocabulary ++;
		}
	}
	file.write( (char*)(&nbVocabulary), sizeof(nbVocabulary) );

	// Save the vocabulary
	for( unsigned int i=0; i<m_vocabularySize; i++ )
	{
		if( m_vocabularyBuffer[i].isReady() )
		{
			// save the name
			if( m_vocabularyNameBuffer		&&
				m_vocabularyNameBuffer[i]	)
			{
				unsigned int strSize = (unsigned int)strlen( m_vocabularyNameBuffer[i] );
				file.write( (char*)(&strSize), sizeof( unsigned int ) );
				file.write( m_vocabularyNameBuffer[i], strSize );
			}
			else
			{	
				unsigned int zero = 0;
				file.write( (char*)(&zero), sizeof( unsigned int ) );
			}
			m_vocabularyBuffer[i].saveBinary( file );
		}
	}
	
	// Number of gestures :  only those used
	unsigned int nbGestures = 0;
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() )
		{
			nbGestures ++;
		}
	}
	file.write( (char*)(&nbGestures), sizeof(nbGestures) );

	// save gesture
	for( unsigned int i=0; i<m_gestureSize; i++ )
	{
		if( m_gestureBuffer[i].isReady() )
		{
			
			// save the name
			if( m_gestureNameBuffer		&&
				m_gestureNameBuffer[i]	)
			{
				unsigned int strSize = (unsigned int)strlen( m_gestureNameBuffer[i] );
				file.write( (char*)(&strSize), sizeof( unsigned int ) );
				file.write( m_gestureNameBuffer[i], strSize );
			}
			else
			{	
				unsigned int zero = 0;
				file.write( (char*)(&zero), sizeof( unsigned int ) );
			}

			m_gestureBuffer[i].saveBinary( file );
			
			// Find the vocabulary Id associated with it
			unsigned int idVoc = 0;
			for( unsigned int j=0; j<m_vocabularySize; j++ )
			{
				if( m_vocabularyBuffer[j].isReady() )
				{
					if( m_gestureBuffer[i].m_ptVocabulary == &(m_vocabularyBuffer[j]) )	{
						break;
					}
					else{
						idVoc++;
					}
				}
			}	
			// associated vocabulary
			file.write( (char*)(&idVoc), sizeof(idVoc) );
		}
			
	}
	return true;
}



/*! Decide according to the probability whether or not the provided gesture is recognised
 * \return true if recognised, false otherwise
 * \note The probability has to be normalised, i.e. probability = gesture->forward( movement ) * gesture->getObservationSize()
 */
bool cGestureRecogniser::isRecognised( const cGesture& gesture, 	/**< Gesture to be analysed */
									   const float probability		/**< Gesture's probability (see note) */
									   ) const
{
	// The variance is increased since learning vectors are often too close to the mean value
	// I empiricaly found that unrecognised gesture are way under 0.0001 ( often around 10^(-7) or more )
	// Therefore, value below this threshold are also accepted
	if( probability >= 0.0001	||
		probability >= (gesture.getMeanProbabilityWeighted()-gesture.getVarianceProbabilityWeighted()*1.5f) )
	{
		return true;
	}
	return false;
}


}	// namespace AGR
