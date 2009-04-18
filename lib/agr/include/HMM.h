/**
 * \file HMM.h
 * \brief Declaration for the HMM class
 * \date 09/02/08
 */


#ifndef _HMM_H_
#define _HMM_H_


class cTestHMM;	// forward declaration for unit testing class


namespace AGR
{

class cVocabulary;	// defined in Vocabulary.h
class cVector3int;	// defined in Movement.h
class cMovement;	// defined in Movement.h


/**
 * \class cHMM
 * \brief cHMM class.
 */
class cHMM
{
friend class ::cTestHMM;
friend class cGestureRecogniser;

public:

	cHMM();
	cHMM( const cHMM& original );
	~cHMM();

	void clear();
	bool initialise( const unsigned int nbHiddenStates,
					 const cVocabulary* vocabulary );
	bool isReady() const;
	bool copy( const cHMM& original );
	
	bool load( const char* fileName );
	bool save( const char* fileName,
			   const bool binary = false ) const;
	bool assignVocabulary( const cVocabulary* vocabulary );

	bool createHMM( cClustering* clusterer,
				    const cMovement* movementArray,	
				    const unsigned int nbMovement );

	bool BaumWelch( const cMovement* movementArray,	
					const unsigned int nbMovement,
					unsigned int sizeObservation = 0 );

	bool BaumWelchClusterised(	unsigned int** observationArray,	
								const unsigned int nbObservation,
								const unsigned int sizeObservation );

	float forward( const cMovement& movement, 
				   unsigned int normaliseSize = 0 ) const;

	float forwardClusterised( const unsigned int* observation,
							  const unsigned int observationSize ) const;

	
	//******************************
	// inlines

	/*! Vocabulary size accessor
	 * \return The vocabulary object pointer
	 */
	inline const cVocabulary* getVocabulary() const
	{
		return m_ptVocabulary;		
	}

	/*! Get the expected observation size for this HMM
	 * \return The expected observation size
	 */
	inline unsigned int getObservationSize() const
	{
		return m_observationSize;
	}

	/*! Get the mean probability weighted
	 * \note This value is set when using createHMM or a BaumWelch function and is normalised 
	 * by the observation's size used in those function. Using default value (=m_observationSize) is recommanded
	 * \return The mean probability weighted
	 */
	inline float getMeanProbabilityWeighted() const
	{
		return m_meanProbabilityWeighted;
	}

	/*! Get the variance probability weighted
	 * \note This value is set when using createHMM or a BaumWelch function and is normalised 
	 * by the observation's size used in those function. Using default value (=m_observationSize) is recommanded
	 * \return The variance probability weighted
	 */
	inline float getVarianceProbabilityWeighted() const
	{
		return m_varianceProbabilityWeighted;
	}

	void computeObservationSequence( const cMovement& movement, 
									 unsigned int* observation,
									 const unsigned int normaliseSize = 0) const;

protected:	
	void computeAlpha( float** alpha, 
					   const unsigned int* observation, 
					   const unsigned int observationSize ) const;

	void computeBeta( float** beta,
					  const unsigned int* observation, 
					  const unsigned int observationSize ) const;

	float computeGamma( const unsigned int state, 
					    const unsigned int time, 
						float** alpha, 
						float** beta ) const;

	float computeXi( const unsigned int state1, 
					 const unsigned int state2,
					 const unsigned int time, 
					 float** alpha, 
					 float** beta,
					 const unsigned int* observation) const;

	bool loadASCII( std::ifstream& file );
	bool saveASCII( std::ofstream& file ) const;
	bool loadBinary( std::ifstream& file );
	bool saveBinary( std::ofstream& file ) const;

	bool computeMeanVariance(	unsigned int** observationArray,	
								const unsigned int nbObservation,
							    const unsigned int sizeObservation );


	unsigned int		m_numberStates;		/**< Number of hidden states */
	const cVocabulary*	m_ptVocabulary;		/**< HMM Vocabulary */
	unsigned int		m_vocabularySize;	/**< Size of the vocabulary, for checking compatibility */

	unsigned int	m_observationSize;				/**< Size of an observation vector */
    float			m_meanProbabilityWeighted;		/**< Mean probability of weighted observations probability */
    float			m_varianceProbabilityWeighted;	/**< Variance probability of weighted observations probability (see computeMeanVariance() note) */

	float*		m_PI;	/**< Initial probability distribution, size = m_numberStates */
	float**		m_A;	/**< Transition probability matrix, size = m_numberStates*m_numberStates */
	float**		m_B;	/**< Emission probability matrix, size = m_numberStates*m_ptVocabulary->size() */

}; // class cHMM


} // namespace AGR


#endif // _HMM_H_

