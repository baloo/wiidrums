/**
 * \file Clustering.cpp
 * \brief Implementation for the cClustering class
 * \date 09/02/08
 */

#include <vector>

#include "Movement.h"
#include "Clustering.h"


namespace AGR
{


/*! Constructor
 */	
cClustering::cClustering() : m_size( 0 ), m_buffer( NULL )
{
}



/*! Destructor
 */	
cClustering::~cClustering()
{
	clear();
}



/*! Clear the values
 */		
void cClustering::clear( )
{
	m_size = 0;
	m_buffer = NULL;
}



/*! Initialise the necessary values
 * \return Success
 */		
bool cClustering::initialise( const unsigned int size,	/**< Size of the buffer (Number of clusters) */
							  cVector3int* ptBuffer	)	/**< Pointer to the buffer to be manipulated */
{
	if( NULL == ptBuffer ||
		0 == size		 )
	{
		return false;
	}

	m_size = size;
	m_buffer = ptBuffer;

	return true;
}



/*! Create normalised vectors and their average vectors 
 * \return Success
 */	
bool cClustering::createNormalisedVectors( const cMovement* movementArray,			/**< Array containing the movement to be processed */
										   const unsigned int nbMovement,			/**< Number of movement in the array */
										   cMovement** ptNormalisedMovementArray,	/**< Array used to store the normalised movement */
										   cMovement* ptAverageMovement,			/**< Average movement */
										   cMovement* ptAllMovementsPoints,			/**< Store all the points of all the previous movements */
										   unsigned int *ptMovementNormalisedSize)	/**< Size used for normalised vectors, two time the average observation size (used if the pointer is not NULL) */
										
{
	// Check inputs
	if( NULL == movementArray				|| 
		0 == nbMovement						||
		NULL == ptNormalisedMovementArray	||
		NULL == ptAverageMovement			||
		NULL == ptAllMovementsPoints	)
	{
		return false;
	}

	// calculate average size, multiply by 2 for normalised size
	unsigned int nbPoints = 0;
	unsigned int normalisedMovementSize = 0;
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		nbPoints += movementArray[i].getSize();
	}
	normalisedMovementSize = (int)(nbPoints*1.5f) ;
	normalisedMovementSize /= nbMovement;
	if( NULL != ptMovementNormalisedSize ){
		*ptMovementNormalisedSize = normalisedMovementSize;
	}

	ptAllMovementsPoints->initialise( nbPoints + normalisedMovementSize*(nbMovement+1) );
	ptAverageMovement->initialise( normalisedMovementSize );

	// Normalise vectors
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		cMovement::changeVectorSize( movementArray[i], normalisedMovementSize, &(*ptNormalisedMovementArray)[i] );
	}

	// Create average vector of normalised vectors
	for( unsigned int i=0; i<normalisedMovementSize; i++ )
	{
		cVector3int* ptPoint = &( ptAverageMovement->modifyBuffer()[i] );
		*ptPoint = cVector3int(0, 0, 0);

		for( unsigned int j=0; j<nbMovement; j++ )
		{
			*ptPoint += (*ptNormalisedMovementArray)[j].getData( i );
		}
		*ptPoint /= nbMovement;
	}

	//----------------
	// fill the allObservations buffer
	cVector3int* bigBuffer = ptAllMovementsPoints->modifyBuffer();

	// the movements vectors points
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		for( unsigned int j=0; j < movementArray[i].getSize() ; j++ )
		{
			*bigBuffer = movementArray[i].getData( j );
			bigBuffer ++;
		}
	}
	
	// the normalised vectors points
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		for( unsigned int j=0; j<normalisedMovementSize; j++ )
		{
			*bigBuffer = (*ptNormalisedMovementArray)[i].getData( j );
			bigBuffer ++;
		}
	}

	// the average vector
	for( unsigned int j=0; j<normalisedMovementSize; j++ )
	{
		*bigBuffer = ptAverageMovement->getData( j );
		bigBuffer ++;
	}

	return true;
}


} // namespace AGR

