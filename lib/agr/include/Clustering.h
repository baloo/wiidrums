/**
 * \file Clustering.h
 * \brief Declaration for the clustering
 * \date 09/02/08
 */


#ifndef _CLUSTERING_H_
#define _CLUSTERING_H_


class cTestClustering;	// forward declaration for unit testing class

namespace AGR
{

class cVector3int;	// defined in Movement.h
class cMovement;	// defined in Movement.h


/**
 * \class cClustering
 * \brief cClustering class.
 */
class cClustering
{
friend class ::cTestClustering;
public:
	cClustering();
	virtual ~cClustering();

	virtual bool initialise( const unsigned int size, cVector3int* ptBuffer );
	virtual void clear();
	virtual cClustering* createCopy()=0;

	virtual int createClusters( const cMovement* movementArray,	const unsigned int nbMovement ) = 0;

	virtual int closerCluster( const cVector3int& point ) const =0;
	virtual int clusterise( const cMovement& movement, unsigned int* observation ) const =0;

	static bool createNormalisedVectors( const cMovement* movementArray,
										 const unsigned int nbMovement,
										 cMovement** ptNormalisedMovementArray,
										 cMovement* ptAverageMovement,
										 cMovement* ptAllMovementsPoints,
										 unsigned int *ptMovementNormalisedSize = NULL );

protected:

	unsigned int m_size;	/**< Number of clusters ( k ) */
	cVector3int* m_buffer;	/**< Pointer to the buffer containing the clusters */

}; // class cClustering


} // namespace AGR


#endif // _CLUSTERING_H_

