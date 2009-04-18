/**
 * \file MovementRecogniser.h
 * \brief Declaration for the cMovementRecogniser class
 * \date 03/03/08
 */


#ifndef _MOVEMENTRECOGNISER_H_
#define _MOVEMENTRECOGNISER_H_


class cTestMovementRecogniser;	// forward declaration for unit testing class


namespace AGR
{

class cVector3int;	// defined in Movement.h
class cMovement;	// defined in Movement.h


typedef unsigned int tAGRDeviceId;	/**< Type used to manipulate AGR device Id */


/**
 * \class cMovementRecogniser
 * \brief Base class for movement recognition
 * \note If the recognition algorithm need to be changed, you can either modify process() or overload it in a child class.
 */
class cMovementRecogniser
{
friend class ::cTestMovementRecogniser;

public:

	/**
	* Enumeration of the differents movement recognition state a device can be
	* \note When receiving an OutOfBuffer state, the next iteration will overwrite the values.
	* Before that calling getMovement() could be used by the application for storing these values
	*/
	enum eMovementState
	{
		eMovementERROR = -1,			/**< An error occured */
		eMovementNone = 0,				/**< No movement recognised */
		eMovementInProgress,			/**< A movement is being recognised but not yet finished */
		eMovementManualInProgress,		/**< A movement is currently manualy recognised */
		eMovementFinished,				/**< A movement has been recognised */
		eMovementManualFinished,		/**< End of the manual movement recognition */
		eMovementOutOfBuffer,			/**< Movement out of buffer (size bigger than maxMovementSize). see eMovementState note */
		eMovementManualOutOfBuffer,		/**< Manual recognition movement out of buffer (size bigger than maxMovementSize). see eMovementState note */
	};

	cMovementRecogniser();
	virtual ~cMovementRecogniser();

	void clear();
	bool initialise( unsigned int nbDevices, 
					 unsigned int maxMovementSize = 50);
	bool isReady() const;

	eMovementState addAcceleration( const tAGRDeviceId deviceId, 
									const cVector3int &acceleration );

	eMovementState getState( const tAGRDeviceId deviceId ) const;
	bool getMovement( const tAGRDeviceId deviceId, 
					  cMovement* ptMovement ) const;

	bool manualRecognitionStart( const tAGRDeviceId deviceId );
	bool manualRecognitionStop( const tAGRDeviceId deviceId,
								 cMovement* ptMovement );
	

	//*******************
	// inlines

	/*! Number of devices accessor
	 * \return The number of devices 
	*/
	inline unsigned int getNbDevices() const 
	{
		return m_nbDevices;
	}	


	/*! Maximum size of a movement accessor
	 * \return The maximum size of a movement
	*/
	inline unsigned int getMaxMovementSize() const 
	{
		return m_maxMovementSize;
	}	

	// inlines
	//*******************

protected:

	/**
	* Protected member structure helping manipulating devices movement informations
	* /note The array contain the accelerations values, stored between m_idStartMov and m_idEndMov-1
	*/
	struct sDeviceMovement
	{
		cVector3int*	m_array;			/**< Array of accelerations */
		unsigned int	m_idStartMov;		/**< Used for manipulating the array : start of the movement */
		unsigned int	m_idEndMov;			/**< Used for manipulating the array : next position for the movement*/
		eMovementState	m_currentState;		/**< Current device movement state */
	};

	sDeviceMovement*	m_devicesArray;		/**< Store the devices informations */
	unsigned int		m_nbDevices;		/**< Number of devices */
	unsigned int		m_maxMovementSize;	/**< Maximum size of a movement (used for sDeviceMovement::m_array) */


	virtual eMovementState process( const tAGRDeviceId deviceId,
									const cVector3int &acceleration );
	
}; // class cMovementRecogniser



} // namespace AGR


#endif // _MOVEMENTRECOGNISER_H_

