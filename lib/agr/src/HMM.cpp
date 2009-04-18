/**
 * \file HMM.cpp
 * \brief Implementation for the cHMM class
 * \date 10/02/08
 */


#include <vector>
#include <fstream>
#include <cmath>

#include "Movement.h"
#include "Clustering.h"
#include "Vocabulary.h"
#include "HMM.h"

#include "AGRVersion.h"


namespace AGR
{


/*! Constructor
 */	
cHMM::cHMM() : 
	m_numberStates(0), m_ptVocabulary(NULL), m_vocabularySize( 0 ),
	m_observationSize( 0 ),	m_meanProbabilityWeighted( 0.0f ), 
	m_varianceProbabilityWeighted( 0.0f ), m_PI(NULL), m_A(NULL), m_B(NULL)
{
}


/*! Copy constructor
 */	
cHMM::cHMM( const cHMM& original )	/**< cHMM object to be copied */
{
	m_numberStates		= original.m_numberStates;
	m_ptVocabulary		= original.m_ptVocabulary;
	m_vocabularySize	= original.m_vocabularySize;
	m_observationSize	= original.m_observationSize;
	m_meanProbabilityWeighted		= original.m_meanProbabilityWeighted;
	m_varianceProbabilityWeighted	= original.m_varianceProbabilityWeighted;
	
	if( original.isReady() )
	{
		m_PI = new float [ m_numberStates ];
		memcpy( m_PI, original.m_PI, m_numberStates*sizeof(float) );

		m_A = new float* [ m_numberStates ];
		m_B = new float* [ m_numberStates ];
		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			m_A[i] = new float [ m_numberStates ];
			memcpy( m_A[i], original.m_A[i], m_numberStates*sizeof(float) );

			m_B[i] = new float [ m_ptVocabulary->getSize() ];
			memcpy( m_B[i], original.m_B[i], m_ptVocabulary->getSize()*sizeof(float) );
		}
	}
	else
	{
		m_PI = NULL;
		m_A = NULL;
		m_B = NULL;
	}
}



/*! Destructor
 */	
cHMM::~cHMM()
{ 
	clear();
}



/*! Clear all the data
 */	
void cHMM::clear()
{
	m_ptVocabulary = NULL;

	if( NULL != m_PI ){
		delete [] m_PI; 
		m_PI = NULL;
	}

	if( NULL != m_A )
	{
		for( unsigned int i = 0; i<m_numberStates; i++ )
		{
			if( NULL != m_A[i] ){
				delete [] m_A[i];
			}
		}
		delete [] m_A;
		m_A = NULL;
	}

	if( NULL != m_B )
	{
		for( unsigned int i = 0; i<m_numberStates; i++ )
		{
			if( NULL != m_B[i] ){
				delete [] m_B[i];
			}
		}
		delete [] m_B;
		m_B = NULL;
	} 
	m_numberStates = 0;
	m_vocabularySize = 0;
	m_observationSize = 0;
	m_meanProbabilityWeighted = 0.0f;
	m_varianceProbabilityWeighted = 0.0f;
}



/*! Copy a cHMM object
 * \return Success. May fails if the original object is not fully created.
 */	
bool cHMM::copy( const cHMM& original )	/**< cHMM object to be copied */
{
	if( original.isReady() == false )
	{
		return false;
	}

	clear();
	
	m_numberStates		= original.m_numberStates;
	m_ptVocabulary		= original.m_ptVocabulary;
	m_vocabularySize	= original.m_vocabularySize;
	m_observationSize	= original.m_observationSize;
	m_meanProbabilityWeighted		= original.m_meanProbabilityWeighted;
	m_varianceProbabilityWeighted	= original.m_varianceProbabilityWeighted;

	m_PI = new float [ m_numberStates ];
	memcpy( m_PI, original.m_PI, m_numberStates*sizeof(float) );

	m_A = new float* [ m_numberStates ];
	m_B = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		m_A[i] = new float [ m_numberStates ];
		memcpy( m_A[i], original.m_A[i], m_numberStates*sizeof(float) );

		m_B[i] = new float [ m_ptVocabulary->getSize() ];
		memcpy( m_B[i], original.m_B[i], m_ptVocabulary->getSize()*sizeof(float) );
	}

	return true;
}



/*! Load an HMM file.
 * \return Success
 * \note Does not load the vocabulary object, which need to be loaded separatly using cHMM::assignVocabulary()
 */	
bool cHMM::load( const char* fileName )	/**< Name of the file to read into */
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
		if( 0 == memcmp( buffer, AGR_HMM_BIN_FILE_HEADER, 3 ) )
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



/*! Save an HMM file.
 * \return Success
 * \note Does not save the vocabulary object
 */
bool cHMM::save( const char* fileName,	/**< Name of the file to save into */
			     const bool binary		/**< Specify whether saving in binary of ASCII format */
				 ) const
{
	// Check input
	if( NULL == fileName ) {
		return false;
	}

	// Only save ready HMM
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



/*! Assign a vocabulary to the HMM.
 * \return Success
 * \note This function should be used for assigning a vocabulary object when loading from files
 */
bool cHMM::assignVocabulary( const cVocabulary* vocabulary )
{
	if( NULL == vocabulary ) {
		return false;
	}

	if( vocabulary->getSize() != m_vocabularySize ){
		return false;
	}

	m_ptVocabulary = vocabulary;
	return true;
}



/*! Inform if the HMM is fully initialised and trained, ready to be used.
 * \return True if ready, false if not
 */	
bool cHMM::isReady() const
{
	if( NULL == m_A				||
		NULL == m_B				||
		NULL == m_PI			||
		NULL == m_ptVocabulary	||
		0 == m_numberStates )
	{
		return false;
	}

	return true;
}



/*! Initialise the data
 */	
bool cHMM::initialise( const unsigned int nbHiddenStates, 
					   const cVocabulary* vocabulary )
{
	if( NULL == vocabulary  ||
		0 == nbHiddenStates )
	{
		return false;
	}

	m_numberStates = nbHiddenStates;
	m_ptVocabulary = vocabulary;

	return true;
}



/*! Compute the movement observation likelihood probability
 * \note The observation will be clusterised into a cluster buffer of size normaliseSize
 * before being subject to the forward algorithm
 * \note If normaliseSize is set to 0, the function will use the HMM's observation size 
 * if this one is greater than 0, otherwise the movement size
 * \return Forward probability for the input movement
 */
float cHMM::forward( const cMovement& movement,	/**< Movement to be processed */
					 unsigned int normaliseSize	/**< Size of the observation which will be created, see function's note */
					 ) const
{
	// Check initialisation
	if( isReady() == false )
	{
		return -1.0f;
	}
	

	if( 0 == normaliseSize )
	{
		if( m_observationSize > 0 ) 
		{
			normaliseSize = m_observationSize;
		}
		else
		{
			normaliseSize = movement.getSize();
			if( 0 == normaliseSize ) {
				return false;
			}
		}
	}

	// Convert the observation sequence to a normalised and vocabulary clusterised one
	unsigned int* observationClusters = new unsigned int [normaliseSize] ;

	// Normalised and clusterised observation sequence creation
	computeObservationSequence( movement, observationClusters, normaliseSize );

	// Compute the probability
	float prob = forwardClusterised( observationClusters, normaliseSize );

	delete [] observationClusters;

	return prob;
}



/*! Compute the observation likelihood probability
 * \note If normaliseSize is set to 0, the function will use the HMM's observation size 
 * if this one is greater than 0, otherwise the movement size
 * \return Forward probability for the input movement
 */
float cHMM::forwardClusterised( const unsigned int* observation,	/**< Observation to be processed */
								const unsigned int observationSize	/**< Size of the observation */
								) const
{
	// Check initialisation
	if( isReady() == false )
	{
		return -1.0f;
	}

	// 1) Create the alpha buffer
	// 2) Compute the alpha buffer (forward algo)
	// 3) Compute the probability

	float** alpha; 

	// 1)
	alpha = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		alpha[i] = new float [ observationSize ];
	}


	// 2)
	computeAlpha( alpha, observation, observationSize );

	// 3)
	float prob = 0;
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		prob += alpha[ i ][ observationSize-1 ];
	}

	// cleaning
	for( unsigned int i=0; i<m_numberStates; i++ ){
		delete [] alpha[i];
	}
	delete [] alpha;

	return prob;
}



/*! BaumWelch maximum expectation algorithm
 *
 * The movement are clusterised in a sizeObservation clusters observation sequence
 * before being sent to the BaumWelchClusterised function.
 * \see BaumWelchClusterised
 * \return Success
 */
bool cHMM::BaumWelch( const cMovement* movementArray,		/**< Movements array to be processed */
					  const unsigned int nbMovement,		/**< Number of movement in the array */
					  unsigned int sizeObservation )		/**< Size of the observation sequence to be created, 0 to use the gesture observation size */
{
	// Check inputs
	if( NULL == movementArray	||
		0 == nbMovement			)
	{
		return false;
	} 

	// Check initialisation
	if( isReady() == false ) {
		return false;
	}

	if( 0 == sizeObservation )
	{
		if( m_observationSize == 0 ) {
			return false;
		}
		sizeObservation = m_observationSize;
	}

	cMovement movResized ;
	movResized.initialise( sizeObservation );
	unsigned int** obsClusters = new unsigned int* [ nbMovement ];
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		obsClusters[i] = new unsigned int [sizeObservation];
		cMovement::changeVectorSize( movementArray[i], sizeObservation, &movResized );
		m_ptVocabulary->clusterise( movResized, obsClusters[i] );
	}

	bool res = BaumWelchClusterised( obsClusters, nbMovement, sizeObservation );

	for( unsigned int i=0; i<nbMovement; i++ )
	{
		delete [] obsClusters[i];
	}
	delete [] obsClusters;

	return res;
}



/*! BaumWelch maximum expectation algorithm on a vocabulary clusterised observation sequence
 * \return success
 */
bool cHMM::BaumWelchClusterised( unsigned int** observationArray,		/**< Observations array to be processed */
								 const unsigned int nbObservation,		/**< Number of observations in the array */
								 const unsigned int sizeObservation )	/**< Size of the observation sequence */
{
	// Check inputs
	if( NULL == observationArray	||
		0 == nbObservation			||
		0 == sizeObservation		)
	{
		return false;
	}

	// Check initialisation
	if( isReady() == false )
	{
		return false;
	}

	// check coherance with the vocabulary object
	if(  m_ptVocabulary->getSize() != m_vocabularySize )
	{
		return false;
	}

	// Necessary variables
	float**	 alpha;		// Forward's alpha
	float**	 beta;		// Backward's beta
	float**	 new_PIs;	// new PI values, for each observation
	float*** new_As;	// new A values, for each observation
	float*** new_Bs;	// new B values, for each observation

	// Create these values

	alpha = new float* [ m_numberStates ];
	beta  = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		alpha[i] = new float [ sizeObservation ];
		beta[i]  = new float [ sizeObservation ];
	}

	new_PIs = new float* [ nbObservation ];
	new_As = new float** [ nbObservation ];
	new_Bs = new float** [ nbObservation ];

	for( unsigned int obs = 0; obs< nbObservation; obs++ )
	{
		new_PIs[obs] = new float [ m_numberStates ];

		new_As[obs] = new float* [ m_numberStates ];
		new_Bs[obs] = new float* [ m_numberStates ];
		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			new_As[obs][i] = new float [ m_numberStates ];
			new_Bs[obs][i] = new float [ m_ptVocabulary->getSize() ];
		}
	}


	// convergence loop - for the moment, only done once
	bool finished = false;
	while( !finished )
	{
		
		//*************************
		// Compute the new values

		for( unsigned int obs = 0; obs< nbObservation; obs++ )
		{
			computeAlpha( alpha, observationArray[ obs ], sizeObservation );
			computeBeta ( beta,  observationArray[ obs ], sizeObservation );

			// Pi
			for( unsigned int i=0; i<m_numberStates; i++) 
			{
				new_PIs[obs][ i ] = computeGamma( i, 0, alpha, beta );
			}

			// A
			for( unsigned int i=0; i<m_numberStates; i++) 
			{
				for( unsigned int j=0; j<m_numberStates; j++) 
				{
					float numerator = 0.0f;
					float denomerator = 0.0f;

					for( unsigned int t=0; t<sizeObservation-2; t++ )
					{
						numerator += computeXi( i, j, t, alpha, beta, observationArray[obs] );

						for( unsigned int h=0; h<m_numberStates; h++ )
						{
							denomerator += computeXi( i, h, t, alpha, beta, observationArray[obs] );
						}
					}

					if( denomerator == 0 ){
						new_As[obs][i][j] = 0;
					}
					else {
						new_As[obs][i][j] = numerator/denomerator;
					}

				} // for j
			} // for i

			// B
			for( unsigned int i=0; i<m_numberStates; i++)
			{
				for( unsigned int k=0; k<m_ptVocabulary->getSize(); k++) 
				{
					float num = 0.0f;
					float denom = 0.0f;

					for( unsigned int t=0; t<sizeObservation; t++ )
					{
						float interDenom = computeGamma( i, t, alpha, beta );
						if( observationArray[obs][t] == k ){
							num += interDenom;
						}
						denom += interDenom;
					}
					if( denom == 0 ){
						new_Bs[obs][i][k] = 0;
					}
					else{
						new_Bs[obs][i][k] = num / denom;
					}

				} // for j
			} // for i
			
		} // for each observation

		//*************************
		// copy the new values in the HMM buffers

		// zero values

		memset( m_PI, 0x00, sizeof(float)*m_numberStates );
		for( unsigned int i=0; i<m_numberStates; i++ ) 
		{
			memset( m_A[i], 0x00, sizeof(float)*m_numberStates );
			memset( m_B[i], 0x00, sizeof(float)*m_ptVocabulary->getSize() );
		}

		// add values
		for( unsigned int obs = 0; obs<nbObservation; obs++ )
		{
			for( unsigned int i=0; i<m_numberStates; i++) 
			{
				m_PI[ i ] += new_PIs[obs][ i ];
				
				for( unsigned int j=0; j<m_numberStates; j++ )
				{
					m_A[i][j] += new_As[obs][i][j];
				}

				for( unsigned int j=0; j<m_ptVocabulary->getSize(); j++ )
				{
					m_B[i][j] += new_Bs[obs][i][j];
				}
			}
		}
	
		// computing the average
		for( unsigned int i=0; i<m_numberStates; i++)
		{

			//---- PI ----
        	m_PI[ i ] /= nbObservation;

			// TEST : in order to increase results
        	m_PI[ i ] *= 1.01f;
			if( m_PI[i] < 0.01f )
				m_PI[i] = 0.01f;
			if( m_PI[i] > 1 )
				m_PI[i] = 1;
			
			//---- A ----
			for( unsigned int j=0; j<m_numberStates; j++ )
			{
				m_A[i][j] /= nbObservation;
				
				// TEST : in order to increase results
				m_A[i][j] *= 1.01f;
				if( m_A[i][j] < 0.001f )
					m_A[i][j] = 0.001f;
				if( m_A[i][j] > 1 )
					m_A[i][j] = 1;
			}

			//---- B ----
			for( unsigned int j=0; j<m_ptVocabulary->getSize(); j++ )
			{
				m_B[i][j] /= nbObservation;
				
				// TEST : in order to increase results
				m_B[i][j] *= 1.01f;
				if( m_B[i][j] < 0.001f )
					m_B[i][j] = 0.001f;
				if( m_B[i][j] > 1 )
					m_B[i][j] = 1;
			}
		} // average computing

		finished = true; // Only once for the moment

	} // while( !finished )

	//*****************
	// cleaning the memory used for the Baum Welch algorithm
	
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		delete [] alpha[i];
		delete [] beta[i];
	}
	delete [] alpha;
	delete [] beta;


	for( unsigned int obs = 0; obs< nbObservation; obs++ )
	{
		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			delete [] new_As[obs][i];
			delete [] new_Bs[obs][i];
		}
		delete [] new_As[obs];
		delete [] new_Bs[obs];
		delete [] new_PIs[obs];
	}
	delete [] new_PIs;
	delete [] new_As;
	delete [] new_Bs;

	
	//*****************
	// Compute the new mean and variance

	computeMeanVariance( observationArray, nbObservation, sizeObservation );

	return true;
}



/*! Create the cluster and initialise the values from statistics
 * 
 * The HMM is created (PI, A and B buffers are created and initialised).
 * Initials values are using statistics from a set of movement clusterised two times,
 * firstly in a new set of cluster representing the hidden states, secondly using
 * the vocabulary representing the observations sequences. Using these values, statistics
 * are used in order to initialise PI, A and B. The clusterised representation of the
 * Hidden models are only temporary for this function, the vocabulary is after the only
 * clustering method used.
 * \return Success
 */
bool cHMM::createHMM( cClustering* clusterer,			/**< Cluster method used to create HMM clusters */
					  const cMovement* movementArray,	/**< Array of movement used for initialising the HMM values */
					  const unsigned int nbMovement )	/**< Number of movement in the array */
{
	// Check inputs
	if( NULL == clusterer		||
		NULL == movementArray	||
		0 == nbMovement			)
	{
		return false;
	}

	// Check initialisation
	if( NULL == m_ptVocabulary ||
		0 == m_numberStates )
	{
		return false;
	}

	if( m_ptVocabulary->isReady() == false ) {
		return false;
	}
	m_vocabularySize = m_ptVocabulary->getSize();

	// Create the cluster in a temporary buffer
	cVector3int* statesClusters = new cVector3int[ m_numberStates ];
	clusterer->initialise( m_numberStates, statesClusters );
	clusterer->createClusters( movementArray, nbMovement );

	// Create data : normalised version of the observations and their average vector (AV)
	cMovement* normalisedMovArray = new cMovement[ nbMovement ];
	cMovement averageMovement;
	cMovement allPoints;
	bool res = cClustering::createNormalisedVectors( movementArray, nbMovement, &normalisedMovArray,
													 &averageMovement, &allPoints, &m_observationSize );
	if( false == res )
	{
		delete [] normalisedMovArray;
		delete [] statesClusters;
		return false;
	}

	//*******************
	// PI

	// Pi creation
	m_PI = new float [ m_numberStates ];
	memset( m_PI, 0x00, m_numberStates*sizeof(float) );

	// Pi initial value
	{
		int clusterId;
		cVector3int point;

		// the observation vectors points
		for( unsigned int i=0; i<nbMovement; i++ )
		{
			point = movementArray[i].getData(0);
			clusterId = clusterer->closerCluster( point );
			m_PI[ clusterId ]++;
		}

		// the average vector
		point = averageMovement.getData(0);
		clusterId = clusterer->closerCluster( point );
		m_PI[ clusterId ]++;

		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			m_PI[ i ] /= (nbMovement+1);
		}
	}

	
	//*******************
	// A : Transition probability matrix

	m_A = new float* [ m_numberStates ];

	for( unsigned int i=0; i<m_numberStates; i++ ) 
	{
		m_A[i] = new float [ m_numberStates ];
		for( unsigned int j=0; j<m_numberStates; j++ )
		{
			float count = 0;
			float total_count = 0;
			unsigned int clusterIdT0;
			unsigned int clusterIdT1;

			for( unsigned int v=0; v<nbMovement; v++ ) // go throught each normalised vector
			{
				for( unsigned int p=0 ; p < normalisedMovArray[v].getSize()-1 ; p++ ) // go throught each point
				{
					clusterIdT0 = clusterer->closerCluster( normalisedMovArray[v].getData(p) );
					clusterIdT1 = clusterer->closerCluster( normalisedMovArray[v].getData(p+1) );
					if( clusterIdT0 == i  )
					{
						total_count++;
						if( clusterIdT1 == j ){
							count++;
						}
					}

				} // for p (vector points)

				clusterIdT0 = clusterer->closerCluster( normalisedMovArray[v].getData(normalisedMovArray[v].getSize()-1) );
				if( clusterIdT0 == i  )
				{
					total_count++;
				}

				if( total_count == 0 )
					m_A[i][j] = 0;
				else
					m_A[i][j] = count/total_count;

				if( m_A[i][j] == 0 )
					m_A[i][j] = 0.001f;	// TEST, for not having zeros probabilites values

			}	// for v (normalised vector)

		}	// for j
	}	//  for i

	//*******************
	// B : Emmision probability

	// necessary buffers
	unsigned int nbPoint = allPoints.getSize();
	unsigned int* assignedClusterHmm = new unsigned int [ nbPoint ];	// vocabulary points are assigned to
	unsigned int* assignedClusterVoc = new unsigned int [ nbPoint ];	// Hmm state points are assigned to
	clusterer->clusterise( allPoints, assignedClusterHmm );
	m_ptVocabulary->clusterise( allPoints, assignedClusterVoc );
	unsigned int vocabularySize = m_ptVocabulary->getSize();

	m_B = new float* [ m_numberStates ];

	for( unsigned int i=0; i<m_numberStates; i++ ) 
	{
		m_B[i] = new float [ vocabularySize ];

		for( unsigned int j=0; j<vocabularySize; j++ )
		{
			m_B[ i ][ j ] = 0;
		}
		float total_count = 0;


		// for each point
			// if the point is assigned to the current HMM state
				// find the vocabulary assigned and B[i][ assigned ]++ it
				// total_count ++;
		for( unsigned int p=0; p<nbPoint; p++ )
		{
			if( assignedClusterHmm[p] == i )
			{
				m_B[ i ][ assignedClusterVoc[p] ] ++;
				total_count ++;
			}
		}

		// for each vocabulary v
			// B[i][ j ] /= total_count
		for( unsigned int v=0; v<vocabularySize; v++ )
		{
			m_B[ i ][ v ] /= total_count;

			if( m_B[ i ][ v ] == 0 )
				m_B[ i ][ v ] = 0.001f; // TEST, for not having zeros probabilites values
		}

	}	//  for i

	// cleaning m_B processing data
	delete [] assignedClusterHmm;
	delete [] assignedClusterVoc;

	
	//*******************
	// Mean and variance

	cMovement movResized ;
	movResized.initialise( m_observationSize );
	unsigned int** obsClusters = new unsigned int* [ nbMovement ];
	for( unsigned int i=0; i<nbMovement; i++ )
	{
		obsClusters[i] = new unsigned int [ m_observationSize ];
		cMovement::changeVectorSize( movementArray[i], m_observationSize, &movResized );
		m_ptVocabulary->clusterise( movResized, obsClusters[i] );
	}
	computeMeanVariance( obsClusters, nbMovement, m_observationSize );


	//*******************
	// Cleaning

	for( unsigned int i=0; i<nbMovement; i++ ) {		
		delete [] obsClusters[i];
	}
	delete [] obsClusters;

	delete [] normalisedMovArray;
	delete [] statesClusters;
	clusterer->clear();

	return true;
}



/*! Compute the observation sequence
 */
void cHMM::computeObservationSequence( const cMovement& movement,		/**< Movement to be vocabulary clusterised */
									   unsigned int* observation,		/**< Store the observation sequence generated */
									   const unsigned int normaliseSize	/**< Observation size, set 0 if using the movement size */
									   ) const
{
	cMovement movResized;
	const cMovement* ptMovement = &movement;

	if( 0 != normaliseSize &&
		movement.getSize() != normaliseSize )
	{
		movResized.initialise( movement.getSize() );
		cMovement::changeVectorSize( movement, normaliseSize, &movResized );
		ptMovement = &movResized;
	}
	
	for( unsigned int i=0; i<normaliseSize; i++ )
	{
		observation[i] = m_ptVocabulary->closerCluster( ptMovement->getData(i) );
	}
}


//****************
// Protected


/*! Compute the alpha component using the forward algorithm
 */
void cHMM::computeAlpha( float** alpha,						/**< Output alpha buffer */
						 const unsigned int* observation,	/**< Observation to be processed */
						 const unsigned int observationSize /**< Observation size */
						 ) const
{
	// For time 0
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		alpha[i][0] = m_PI[i] * m_B[i][ observation[0] ] ;
	}

	for( unsigned int t=1; t<observationSize; t++ )
	{
		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			float sum = 0;
			for( unsigned int j=0; j<m_numberStates; j++ )
			{
				sum += alpha[j][t-1] * m_A[j][i];
			}
			float value = sum * m_B[i][ observation[t] ];
			alpha[i][t] = value;
		}
	}
}



/*! Compute the beta component using the backward algorithm
 */
void cHMM::computeBeta( float** beta,						/**< Output beta buffer */
						const unsigned int* observation,	/**< Observation to be processed */
						const unsigned int observationSize	/**< Observation size */
						) const
{
	// For time 0
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		beta[ i ][ observationSize-1 ] = 1.0f;
	}

	for( int t=observationSize-2; t>=0; t-- )
	{
		for( unsigned int i=0; i<m_numberStates; i++ )
		{
			float sum = 0;
			for( unsigned int j=0; j<m_numberStates; j++ )
			{
				sum += m_A[i][j] * m_B[j][ observation[t+1] ] * beta[j][t+1];
			}
			beta[i][t] = sum;
		}
	}
}



/*! Compute the expected state occupancy count γ (Gamma)
 * \return The expected state occupancy count γ (Gamma)
 */
float cHMM::computeGamma( const unsigned int state,		/**< State */
						  const unsigned int time,		/**< Time */
						  float** alpha,				/**< Alpha, from the forward algorithm */
						  float** beta					/**< Beta, from the forward algorithm */
						  ) const
{
	float numerator = alpha[state][time] * beta[state][time];
	float denominator = 0.0f;

	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		denominator += alpha[i][time] * beta[i][time];
	}

	if( denominator == 0 )
		return 0;

	return numerator / denominator;
}



/*! Compute the expected state transition count ξ( Xi )
 * \return The expected state transition count ξ
 */
float cHMM::computeXi( const unsigned int state1,		/**< State 1 */
					   const unsigned int state2,		/**< State 2 */
					   const unsigned int time,			/**< Time */
					   float** alpha,					/**< Alpha, from the forward algorithm */
					   float** beta,					/**< Beta, from the forward algorithm */
					   const unsigned int* observation	/**< Observation sequence */
					   ) const
{
	float numerator =	alpha[ state1 ][ time ] * 
						m_A  [ state1 ][ state2 ] * 
						m_B  [ state2 ][ observation[time+1] ] *
						beta [ state2 ][ time+1 ];

	float denominator = 0.0f;
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		denominator += alpha[i][time] * beta[i][time];
	}

	if( denominator == 0 )
		return 0;

	return numerator / denominator;
}



/*! Compute the mean and the variance
 * \note The probability used in this function is weighted by the observation size
 * \note The variance is not actually the probability-weighted average of the squared 
 * deviations from the mean, after that the value is square rooted => The variance here is
 * the square root probability-weighted average of the squared deviations from the mean.
 * \return Success
 */
bool cHMM::computeMeanVariance(	unsigned int** observationArray,		/**< Observations array to be processed */
								const unsigned int nbObservation,		/**< Number of observations in the array */
							    const unsigned int sizeObservation )	/**< Size of the observation sequence */
{
	if( NULL == observationArray	||
		0    == nbObservation		)
	{
		return false;
	}

	float* probArry = new float [ nbObservation ];

	m_meanProbabilityWeighted = 0.0f;
	for( unsigned int i = 0; i< nbObservation; i++ )
	{
		probArry[ i ] = sizeObservation * forwardClusterised(  observationArray[ i ], sizeObservation );
		m_meanProbabilityWeighted += probArry[ i ];
	}
	m_meanProbabilityWeighted /= nbObservation;

	m_varianceProbabilityWeighted = 0.0f;
	for( unsigned int i = 0; i< nbObservation; i++ )
	{
		probArry[ i ] -= m_meanProbabilityWeighted;		//   prob(i) - mean
		probArry[ i ] *= probArry[ i ];					// ( prob(i) - mean )^2
		m_varianceProbabilityWeighted += probArry[ i ];
	}
	m_varianceProbabilityWeighted /= nbObservation;
	m_varianceProbabilityWeighted = std::sqrt( m_varianceProbabilityWeighted );

	// The variance will be increased by 50% when doing tests
	// Therefore, we need to make sure that this won't return a negative probability
	while( m_varianceProbabilityWeighted * 1.5f >= m_meanProbabilityWeighted )
	{
		m_varianceProbabilityWeighted *= 0.95f;
	}

	// clean
	delete [] probArry;

	return true;
}



/*! Load from an ASCII file
 * \note The vocabulary is not loaded there
 * \return Success
 */
bool cHMM::loadASCII( std::ifstream& file )
{
	if( file.is_open() == false ) {
		return false;
	}

	// version checking
	unsigned int version;
	char str[100];
	file >> str >> str >> str >> str >> version;
	if( version != AGR_HMM_ASCII_FILE_VERSION )
	{
		// TODO : backward compatibility
		return false;
	}
	file.getline( str, 100, '\n' );	// EOL

	// The previous vocabulary object pointed is saved and reassigned,
	// in case the user assign the vocabulary object before loading the file
	const cVocabulary* prevVocabulary = m_ptVocabulary;

	// clean HMM values
	clear();
	m_ptVocabulary = prevVocabulary;

	// Skip file info
	file.getline( str, 100, '\n' );

	// Nb States
	file >> str;
	file >> m_numberStates;
	file.getline( str, 100, '\n' );	// EOL

	// Observation size
	file >> str;
	file >> m_observationSize;
	file.getline( str, 100, '\n' );	// EOL

	// Mean
	file >> str;
	file >> m_meanProbabilityWeighted;
	file.getline( str, 100, '\n' );	// EOL

	// Variance
	file >> str;
	file >> m_varianceProbabilityWeighted;
	file.getline( str, 100, '\n' );	// EOL

	// Vocabulary size
	file >> str >> str >> str >> str >> str >> str >> m_vocabularySize;
	file.getline( str, 100, '\n' );	// EOL
	if( 0 == m_vocabularySize ) {
		return false;
	}

	// Skip file info
	file.getline( str, 100, '\n' );	// empty line
	file.getline( str, 100, '\n' );	// PI info

	// PI
	m_PI = new float [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		file >> m_PI[ i ];
	} 

	// Skip file info
	file.getline( str, 100, '\n' );	// EOL
	file.getline( str, 100, '\n' );	// empty line
	file.getline( str, 100, '\n' );	// A info

	// A
	m_A = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		m_A[i] = new float [ m_numberStates ];
		for( unsigned int j=0; j<m_numberStates; j++ )
		{
			file >> m_A[ i ][ j ] ;
		}
	} 
	
	// Skip file info
	file.getline( str, 100, '\n' );	// EOL
	file.getline( str, 100, '\n' );	// empty line
	file.getline( str, 100, '\n' );	// B info

	// B
	m_B = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		m_B[i] = new float [ m_vocabularySize ];
		for( unsigned int j=0; j<m_vocabularySize; j++ )
		{
			file >> m_B[ i ][ j ] ;
		}
	} 
	file.getline( str, 100, '\n' ); // Empty line

	return true;
}



/*! Save from an ASCII file
 * \return Success
 */
bool cHMM::saveASCII( std::ofstream& file ) const
{
	if( file.is_open() == false ) {
		return false;
	}

	// File informations
	file << "ASCII HMM File version " << AGR_HMM_ASCII_FILE_VERSION << std::endl;
	file << "Created with AGR v" << AGR_VERSION << "." << AGR_VERSION_MIN << std::endl;
	file << std::endl;
	
	// HMM informations
	file << "nbState " << m_numberStates << std::endl;
	file << "observationSize " << m_observationSize << std::endl;
	file << "Mean " << m_meanProbabilityWeighted << std::endl;
	file << "Variance " << m_varianceProbabilityWeighted << std::endl;
	file << std::endl;

	// Vocabulary information
	file << "Size of the vocabulary used : " << m_ptVocabulary->getSize() << std::endl;
	file << std::endl;

	// PI
	file << "PI : Initial probability distribution, size = " << m_numberStates << std::endl;
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		file << m_PI[ i ] << " ";
	} 
	file << std::endl;
	file << std::endl;

	// A
	file << "A : transition probability matrix, size = " << m_numberStates << " * " << m_numberStates << std::endl;
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_numberStates; j++ )
		{
			file << m_A[ i ][ j ] << " ";
		}
		file << std::endl;
	} 
	file << std::endl;

	// B
	file << "B : Emission probability matrix, size = " << m_numberStates << " * " <<  m_ptVocabulary->getSize() << std::endl;
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_ptVocabulary->getSize(); j++ )
		{
			file << m_B[ i ][ j ] << " ";
		}
		file << std::endl;
	} 

	return true;
}



/*! Load from a binary file
 * \return Success
 * \todo Endianness
 */
bool cHMM::loadBinary( std::ifstream& file )
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
		if( 0 != memcmp( buffer, AGR_HMM_BIN_FILE_HEADER, 3 ) ) {
			return false;
		}
	}

	{// Version
		unsigned int version;
		if( !file.read( (char*)(&version), sizeof(version) ) ) {
			return false;
		}

		// Check header
		if( version != AGR_HMM_BINARY_FILE_VERSION ) {
			return false;
		}
	}
	
	//--------------------------	
	// clean current objects values
	// The previous vocabulary object pointed is saved and reassigned,
	// in case the user assigned the vocabulary object before loading the file
	const cVocabulary* prevVocabulary = m_ptVocabulary;
	clear();
	m_ptVocabulary = prevVocabulary;

	//--------------------------
	// Object informations 

	if(	!file.read( (char*)(&m_numberStates),			     sizeof(m_numberStates) )				||	// Nb states
		!file.read( (char*)(&m_observationSize),			 sizeof(m_observationSize) )			||	// Observation size
		!file.read( (char*)(&m_meanProbabilityWeighted),	 sizeof(m_meanProbabilityWeighted) )	||	// Mean probability
		!file.read( (char*)(&m_varianceProbabilityWeighted), sizeof(m_varianceProbabilityWeighted))	||	// Variance probability
		!file.read( (char*)(&m_vocabularySize),				 sizeof(m_vocabularySize) )				)	// Vocabulary size
	{
		return false;
	}

	if( m_numberStates == 0		||
		m_vocabularySize == 0	)
	{
		return false;
	}

	m_PI = new float  [ m_numberStates ];
	m_A  = new float* [ m_numberStates ];
	m_B  = new float* [ m_numberStates ];
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		m_A[i] = new float [ m_numberStates ];
		m_B[i] = new float [ m_vocabularySize ];
	}

	//--------------------------
	// HMM informations 

	//--------------------------
	// PI
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		if( !file.read( (char*)(&m_PI[ i ]), sizeof(float) ) ){
			return false;
		}
	}

	//--------------------------
	// A
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_numberStates; j++ )
		{
			if( !file.read( (char*)(&m_A[ i ][ j ]), sizeof(float) ) ){
				return false;
			}
		}
	}

	//--------------------------
	// B
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_vocabularySize; j++ )
		{
			if( !file.read( (char*)(&m_B[ i ][ j ]), sizeof(float) ) ){
				return false;
			}
		}
	}

	return true;
}



/*! Save from a binary file
 * \return Success
 * \todo Endianness
 */
bool cHMM::saveBinary( std::ofstream& file ) const
{
	// Binary header
	file.write( AGR_HMM_BIN_FILE_HEADER, 3 );

	// Version
	file.write( (char*)(&AGR_HMM_BINARY_FILE_VERSION), sizeof(unsigned int) );

	//--------------------------
	// Object informations 

	// Nb states
	file.write( (char*)(&m_numberStates), sizeof(m_numberStates) );
	// Observation size
	file.write( (char*)(&m_observationSize), sizeof(m_observationSize) );
	// Mean probability
	file.write( (char*)(&m_meanProbabilityWeighted), sizeof(m_meanProbabilityWeighted) );
	// Variance probability
	file.write( (char*)(&m_varianceProbabilityWeighted), sizeof(m_varianceProbabilityWeighted) );
	// Vocabulary size
	file.write( (char*)(&m_vocabularySize), sizeof(m_vocabularySize) );

	//--------------------------
	// HMM informations 

	//--------------------------
	// PI
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		file.write( (char*)(&m_PI[ i ]), sizeof(float) );
	}

	//--------------------------
	// A
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_numberStates; j++ )
		{
			file.write( (char*)(&m_A[ i ][ j ]), sizeof(float) );
		}
	}

	//--------------------------
	// B
	for( unsigned int i=0; i<m_numberStates; i++ )
	{
		for( unsigned int j=0; j<m_vocabularySize; j++ )
		{
			file.write( (char*)(&m_B[ i ][ j ]), sizeof(float) );
		}
	}

	return true;
}



} // namespace AGR

