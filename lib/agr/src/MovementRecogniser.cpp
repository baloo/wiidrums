/**
 * \file MovementRecogniser.cpp
 * \brief Implementation for the cMovementRecogniser class
 * \date 03/03/08
 */


#include <vector>

#include "Movement.h"
#include "MovementRecogniser.h"


namespace AGR
{


/*! Constructor
 */	
cMovementRecogniser::cMovementRecogniser():
		m_devicesArray(NULL), m_nbDevices(0), m_maxMovementSize(0)
{
}



/*! Destructor
 */	
cMovementRecogniser::~cMovementRecogniser()
{
	clear();
}



/*! Clear all the data
 */	
void cMovementRecogniser::clear()
{
	if( m_devicesArray )
	{
		for( unsigned int i=0; i<m_nbDevices; i++ )
		{
			delete [] m_devicesArray[ i ].m_array;
		}

		delete [] m_devicesArray;
		m_devicesArray = NULL;
	}

	m_nbDevices = 0;
	m_maxMovementSize = 0;
}



/*! Initialise the cMovementRecogniser object
 * \return Success
 */	
bool cMovementRecogniser::initialise( unsigned int nbDevices,			/**< Number of devices to be used */
									  unsigned int maxMovementSize )	/**< Maximum size of a movement */
{
	// check inputs
	if( 0 == nbDevices ||
		2 >= maxMovementSize )
	{
		return false;
	}

	m_nbDevices = nbDevices;
	m_maxMovementSize = maxMovementSize;

	if( m_devicesArray ) {
		delete [] m_devicesArray;
	}

	m_devicesArray = new sDeviceMovement[ nbDevices ];
	for( unsigned int i=0; i<nbDevices; i++ )
	{
		m_devicesArray[ i ].m_array = new cVector3int[ maxMovementSize ];
		m_devicesArray[ i ].m_currentState = eMovementNone;
		m_devicesArray[ i ].m_idStartMov   = 0;
		m_devicesArray[ i ].m_idEndMov     = 0;
		// Initialise the last acceleration to have a "past vector" to compare when comparing the first vector for the first time
		m_devicesArray[ i ].m_array[ maxMovementSize-1 ] = cVector3int(128, 128, 128+25); 
	}

	return true;
}



/*! Inform if the movement recogniser is ready to be used.
 * \return True if ready, false if not
 */	
bool cMovementRecogniser::isReady() const
{
	if( NULL == m_devicesArray	||
		0 == m_nbDevices		||
		0 == m_maxMovementSize	)
	{
		return false;
	}

	return true;
}



/*! Add a new acceleration data to a device.
 * \return The movement state
 */	
cMovementRecogniser::eMovementState cMovementRecogniser::addAcceleration( const tAGRDeviceId deviceId,		/**< Devices to be manipulated */
																		  const cVector3int &acceleration )	/**< New acceleration to be processed */
{
	// check initialisation
	if( isReady() == false ) {
		return eMovementERROR;
	}

	// check inputs
	if( deviceId >= m_nbDevices ){
		return eMovementERROR;
	}
	
	sDeviceMovement* ptDevice = &m_devicesArray[ deviceId ];

	if( ptDevice->m_currentState != eMovementManualInProgress	&&
		ptDevice->m_currentState != eMovementManualOutOfBuffer	) 
	{
		return process( deviceId, acceleration );
	}

	// manual recognition
	unsigned int nextPosMov = ptDevice->m_idEndMov + 1;
	if(	ptDevice->m_currentState == eMovementManualOutOfBuffer ) {
		nextPosMov = 0;
		ptDevice->m_currentState = eMovementManualInProgress;
	}

	ptDevice->m_array[ ptDevice->m_idEndMov ] = acceleration;
	if( nextPosMov == m_maxMovementSize )
	{
		ptDevice->m_currentState = eMovementManualOutOfBuffer;
		ptDevice->m_idEndMov = 0;
	}
	else
	{
		ptDevice->m_idEndMov = nextPosMov;
	}


	return ptDevice->m_currentState;
}



/*! Get the state of a device movement.
 * \return The device movement state
 */	
cMovementRecogniser::eMovementState cMovementRecogniser::getState( const tAGRDeviceId deviceId	/**< Devices to be manipulated */
																   ) const
{
	// check initialisation
	if( isReady() == false ) {
		return eMovementERROR;
	}

	// check inputs
	if( deviceId >= m_nbDevices ){
		return eMovementERROR;
	}

	return m_devicesArray[ deviceId ].m_currentState;
}



/*! Get the movement stored in a device movement.
 * \return Success
 */	
bool cMovementRecogniser::getMovement( const tAGRDeviceId deviceId, /**< Devices to be manipulated */
									   cMovement* ptMovement		/**< cMovement object where the movement will be stored */
									   ) const
{
	// check initialisation
	if( isReady() == false ) {
		return false;
	}

	// check inputs
	if( deviceId >= m_nbDevices	||
		NULL == ptMovement		)
	{
		return false;
	}

	sDeviceMovement* ptDevice = &m_devicesArray[ deviceId ];

	// Data is comprised between Start and End, Start included, End excluded
	// Two solution : 
	// Packed :   [   SXXXXE   ]
	// or
	// Unpacked : [XXE      SXX]
	// (S : Start, E: End, X: Data)

	const unsigned int sizeMov = ( ptDevice->m_idEndMov > ptDevice->m_idStartMov ) ? 
										ptDevice->m_idEndMov - ptDevice->m_idStartMov: 
										m_maxMovementSize - (ptDevice->m_idStartMov - ptDevice->m_idEndMov); 
	if( 0 == sizeMov ) {
		return false;
	}

	cVector3int* buffer = new cVector3int [ sizeMov ];
	if( ptDevice->m_idStartMov < ptDevice->m_idEndMov )
	{
		// Packed
		memcpy( buffer, &ptDevice->m_array[ptDevice->m_idStartMov] , sizeof(cVector3int)*sizeMov );
	}
	else
	{
		unsigned int sizePart1 = m_maxMovementSize-ptDevice->m_idStartMov;
		// Unpacked, part 1 
		memcpy( buffer, &ptDevice->m_array[ptDevice->m_idStartMov], sizeof(cVector3int)*sizePart1 );
		// Unpacked, part 2
		memcpy( buffer+sizePart1, ptDevice->m_array, sizeof(cVector3int)*ptDevice->m_idEndMov );
	}

	bool bRes = ptMovement->initialise( sizeMov, buffer );
	delete [] buffer;
	return bRes;
}



/*! Start the manual recognition for a device
 * \return Success
 * \note This could be used for manualy start a movement (eg. when pressing a button)
 * \note Anything that was been done earlier will be canceled by this action (like automatic or manual recognition)
 * \see manualRecognitionStop
 */	
bool cMovementRecogniser::manualRecognitionStart( const tAGRDeviceId deviceId )	/**< Devices to be manipulated */
{
	// check initialisation
	if( isReady() == false ) {
		return false;
	}

	// check inputs
	if( deviceId >= m_nbDevices ){
		return false;
	}
	
	sDeviceMovement* ptDevice = &m_devicesArray[ deviceId ];
	ptDevice->m_currentState = eMovementManualInProgress;
	ptDevice->m_idStartMov = 0;
	ptDevice->m_idEndMov = 0;

	return true;
}



/*! Stop the manual recognition for a device
 * \return Success
 * \note Only something that has started may be stopped...
 * \see manualRecognitionStart
 */	
bool cMovementRecogniser::manualRecognitionStop( const tAGRDeviceId deviceId,	/**< Devices to be manipulated */
												 cMovement* ptMovement )		/**< cMovement object where the movement will be stored */
{
	// check initialisation
	if( isReady() == false ) {
		return false;
	}

	// check inputs
	if( deviceId >= m_nbDevices	||
		NULL == ptMovement		)
	{
		return false;
	}

	sDeviceMovement* ptDevice = &m_devicesArray[ deviceId ];

	// Only something that has started may be stopped
	if( ptDevice->m_currentState != eMovementManualInProgress	&&
		ptDevice->m_currentState != eMovementManualOutOfBuffer	) 
	{
		return false;
	}

	if( ptDevice->m_idEndMov == ptDevice->m_idStartMov ) {	
		ptDevice->m_currentState = eMovementNone;
		return false;
	}

	ptDevice->m_currentState = eMovementManualFinished;
	return ptMovement->initialise( ptDevice->m_idEndMov, ptDevice->m_array );
}



/*! Process the movement recognition
 * \return The device movement state
 * \note This function has been done using empirical testing. Feel free to modify it or overload it
 */	
cMovementRecogniser::eMovementState cMovementRecogniser::process( const tAGRDeviceId deviceId,
																  const cVector3int &acceleration )
{
	// Feel free to change these values !
	static const unsigned int DIST_THRESHOLD			= 12*12;
	//static const unsigned int GRAVITY_THRESHOLD			= 25*25;
	//static const unsigned int GRAVITY_THRESHOLD_DELTA	= 5*5;
	static const unsigned int MINIMUM_MOVEMENT_SIZE		= 5;

	sDeviceMovement* ptDevice = &m_devicesArray[ deviceId ];

	// When needed, set movement start pointer for a new list
	if( eMovementFinished    == ptDevice->m_currentState 	||
		eMovementOutOfBuffer == ptDevice->m_currentState 	)
	{
		ptDevice->m_idStartMov = ptDevice->m_idEndMov;
	}

	// Get the previous vector
	const unsigned int prevIndex = (ptDevice->m_idEndMov != 0) ? ptDevice->m_idEndMov-1 : m_maxMovementSize-1 ;
	cVector3int prevVec = ptDevice->m_array[ prevIndex ];

	// Store the new one
	ptDevice->m_array[ ptDevice->m_idEndMov ] = acceleration;
	ptDevice->m_idEndMov = (ptDevice->m_idEndMov+1 < m_maxMovementSize) ? ptDevice->m_idEndMov+1 : 0 ;
//	const unsigned int nextIndex = (ptDevice->m_idEndMov+1 < m_maxMovementSize) ? ptDevice->m_idEndMov+1 : 0 ;

	// Square distance with the previous acceleration
	const cVector3int diffVector =  acceleration - prevVec;
	const unsigned int squareDist = diffVector.x*diffVector.x + 
									diffVector.y*diffVector.y +
									diffVector.z*diffVector.z ;

	// Square distance with the center (situated at 128,128,128 with the wiimote)
	//const cVector3int accZeroed = acceleration - cVector3int( 128, 128, 128 );
	//const int squareAcceration = accZeroed.x*accZeroed.x + accZeroed.y*accZeroed.y + accZeroed.z*accZeroed.z;

	//if( squareDist > DIST_THRESHOLD											||
	//	( squareAcceration > (GRAVITY_THRESHOLD+GRAVITY_THRESHOLD_DELTA) ||
	//	  squareAcceration < (GRAVITY_THRESHOLD-GRAVITY_THRESHOLD_DELTA) )	)
	if( squareDist > DIST_THRESHOLD	)
	{
		if( eMovementInProgress != ptDevice->m_currentState ) 
		{
			ptDevice->m_idStartMov = prevIndex;
			ptDevice->m_currentState = eMovementInProgress;
		}
		else if( ptDevice->m_idEndMov == ptDevice->m_idStartMov )
		{
			// Next turn will be out of buffer !
			ptDevice->m_currentState = eMovementOutOfBuffer;
		}
		else
		{
			ptDevice->m_currentState = eMovementInProgress;
		}
	}
	else
	{
	const unsigned int sizeMov = ( ptDevice->m_idEndMov > ptDevice->m_idStartMov ) ? 
										ptDevice->m_idEndMov - ptDevice->m_idStartMov : 
										m_maxMovementSize - (ptDevice->m_idStartMov - ptDevice->m_idEndMov) ;

		// The movement can be either finished, or no movement has been detected
		// also, if the movement is too small, it is discarded
		if( sizeMov >= MINIMUM_MOVEMENT_SIZE						&&
			( eMovementInProgress  == ptDevice->m_currentState ||
			  eMovementOutOfBuffer == ptDevice->m_currentState )	)
		{
			ptDevice->m_currentState = eMovementFinished;
			ptDevice->m_idEndMov = (ptDevice->m_idEndMov != 0) ? ptDevice->m_idEndMov-1 : m_maxMovementSize-1 ;
		}
		else
		{
			ptDevice->m_currentState = eMovementNone;
		}
	}

	return ptDevice->m_currentState;
}


} //namespace AGR

