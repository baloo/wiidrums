/**
 * \file KmeansClustering.h
 * \brief Declaration for the kmean clustering
 * \date 09/02/08
 */


#ifndef _KMEANSCLUSTERING_H_
#define _KMEANSCLUSTERING_H_


namespace AGR
{

class cClustering;
class cMovement;
class cVector3int;

/**
 * \class cKmeansClustering
 * \brief cKmeansClustering class.
 */
class cKmeansClustering : public cClustering
{
public:
	cKmeansClustering();
	~cKmeansClustering();

	cClustering* createCopy();
	int createClusters( const cMovement* movementArray,	const unsigned int nbMovement );

	int closerCluster( const cVector3int& point ) const;
	int clusterise( const cMovement& movement, unsigned int* observation ) const;

protected:


}; // class cKmeansClustering


} // namespace AGR


#endif // _KMEANSCLUSTERING_H_

