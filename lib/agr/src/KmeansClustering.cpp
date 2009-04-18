/**
 * \file KmeansClustering.cpp
 * \brief Implementation for the cKmeansClustering class
 * \date 09/02/08
 */


#include <vector>

#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"



namespace AGR
{


/*! Constructor
 */	
cKmeansClustering::cKmeansClustering() : cClustering()
{
}



/*! Destructor
 */	
cKmeansClustering::~cKmeansClustering()
{
	m_buffer = NULL;
}



/*! Create a copy of the object
 * \note An allocation is done for the created object, which need to be deleted by the used
 */
cClustering* cKmeansClustering::createCopy()
{
	cKmeansClustering* copy = new cKmeansClustering();
	return copy;
}



/*! Find the closer cluster of a point
 * \return -1 if failed. The closer cluster id if success
 */	
int cKmeansClustering::closerCluster( const cVector3int& point	/**< Point to be analysed */
									  ) const
{
	unsigned int closetClusterSquareDistance;

	// initialise the first one
	cVector3int cluster = m_buffer[0];
	int closestCluster = 0;
	cVector3int distance = cluster - point;
	closetClusterSquareDistance = distance.x*distance.x + distance.y*distance.y + distance.z*distance.z;
	for( unsigned int j=1; j<m_size; j++ )
	{
		cluster = m_buffer[j];
		
		cVector3int distance = cluster - point;
		int squareDist = distance.x*distance.x + distance.y*distance.y + distance.z*distance.z;
		if( squareDist < (int)closetClusterSquareDistance )
		{
			closetClusterSquareDistance = squareDist;
			closestCluster = j;
		}
	}
	return closestCluster;
}



/*! Clusterise the observation vector
 * \return -1 if failed. 0 if observation has not changed, 1 if observation has changed
 */	
int cKmeansClustering::clusterise( const cMovement& movement,	/**< Movement to be clusterised */
								   unsigned int* observation	/**< Output cluster observation sequence */
								   ) const
{
	if( NULL == observation ||
		NULL == m_buffer ) 
	{
		return -1;
	}

	int changed = 0;

	for( unsigned int i = 0; i<movement.getSize(); i++ )
	{
		const cVector3int point = movement.getData( i );

		// Assign points to the closed cluster
		unsigned int previousCluster = observation[i];
		observation[i] = closerCluster( point );
		if( previousCluster != observation[i] )
		{
			changed = 1;
		}
	}

	return changed;
}



/*! Perform the kmean algorithm
 * \return -1 if failed. The number of clustering iteration otherwith
 */	
int cKmeansClustering::createClusters( const cMovement* movementArray,	/**< Array of movement used for creating clusters */
									   const unsigned int nbMovement )	/**< Number of movement in the array */
{
	// check inputs
	if( NULL == movementArray ||
		0 == nbMovement )
	{ 
		return -1;
	}

	// check initialisation
	if( NULL == m_buffer || 
		m_size < 2 )
	{
		return -1;
	}

	// For more clarety
	const unsigned int& k = m_size;

	// Create data : normalised version of the observations and their average vector (AV)
	cMovement* normalisedVectors = new cMovement[ nbMovement ];
	cMovement averageVector;
	cMovement allPoints;
	createNormalisedVectors( movementArray, nbMovement, &normalisedVectors, &averageVector, &allPoints );

	delete [] normalisedVectors; // not needed


	//-------------
	// Initial clusters:
	// For each k clusters, assign them at equal distance on the AV
	{
		int step = averageVector.getSize() / k;
		for( unsigned int i=0; i< k; i++ )
		{
			unsigned int id = step*i;
			if( id >= averageVector.getSize() )	{
				id = averageVector.getSize()-1;
			}
			m_buffer[i] = averageVector.getData( id );
		}
	}

	//-------------
	// kmean loop

	bool finished = false;

	float* sumX = new float[ k ];
	float* sumY = new float[ k ];
	float* sumZ = new float[ k ];
	unsigned int* count = new unsigned int[ k ];
	
	// Store the assigned cluster for each points
	const unsigned int nbPoint = allPoints.getSize();
	unsigned int* assignedCluster = new unsigned int[ nbPoint ];

	// Set a maximum number of iteration for avoiding an infinit loop
	int iteration = 0;
	static const int maxIteration = 50;

	// Improvement gives more iteration for not stopping in a local minimum
	static const int improvementIterations = 10;
	int improvement = improvementIterations;

	while( !finished )
	{
		int changed = clusterise( allPoints, assignedCluster );

		if( 0 == changed  ) {
			improvement --;
			if( 0 == improvement ){
				// Means values should be stable earlier, this loop exit is "in case"
				finished = true;
			}
		}
		else if( changed > 0 ){
			improvement = improvementIterations;
		}
		else { //if( changed < 0 )
			return -1;
		}

		//-----------
		// Calculate mean value per cluster
		for( unsigned int i = 0; i<k; i++ )
		{
			sumX[ i ] = 0.0f;
			sumY[ i ] = 0.0f;
			sumZ[ i ] = 0.0f;
			count[ i ] = 0;
		}
		for( unsigned int i = 0; i<nbPoint; i++ )
		{
			const cVector3int point = allPoints.getData( i );
			unsigned int cluster = assignedCluster[ i ];
			sumX[ cluster ] += point.x;
			sumY[ cluster ] += point.y;
			sumZ[ cluster ] += point.z;
			count[ cluster ]++;
		}

		unsigned int maxInACluster = 0;
		unsigned int biggestClusterId = 0;
		for( unsigned int i = 0; i<k; i++ )
		{
			sumX[ i ] /= count[ i ];
			sumY[ i ] /= count[ i ];
			sumZ[ i ] /= count[ i ];

			if( count[ i ]> maxInACluster )
			{
				maxInACluster = count[i];
				biggestClusterId = i;
			}
		}

		// Check if a cluster is empty
		for( unsigned int i = 0; i<k; i++ )
		{
			if( count[ i ] == 0 )
			{
				finished = false;
				sumX[ i ] = sumX[ biggestClusterId ] + (-1)*i;		// Dirty workaround for being close but at
				sumY[ i ] = sumY[ biggestClusterId ] + (-1)*(i%3);	// the exact same place without all empty
				sumZ[ i ] = sumZ[ biggestClusterId ] + (-1)*(i%5);	// clusters being assigned the same place
			}
		}

		//----------
		// assign mean value to cluster

		bool meanChanged = false;
		cVector3int previousValue;
		for( unsigned int i = 0; i<k; i++ )
		{
			previousValue =  m_buffer[ i ] ;

			m_buffer[ i ].x = (int)( sumX[ i ] );
			m_buffer[ i ].y = (int)( sumY[ i ] );
			m_buffer[ i ].z = (int)( sumZ[ i ] );

			if( previousValue !=  m_buffer[ i ] ) {
				meanChanged = true;
			}

		}

		// If no changes, we may stop
		if( meanChanged == false ) {
			finished = true;
		}

		if( iteration >= maxIteration ) {
			finished = true;
		}
		iteration++;
	}

	delete [] sumX;
	delete [] sumY;
	delete [] sumZ;
	delete [] count;
	delete [] assignedCluster;
		
		
	return iteration;
}


} // namespace AGR

