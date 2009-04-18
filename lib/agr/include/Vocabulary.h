/**
 * \file Vocabulary.h
 * \brief Declaration for the vocabulary
 * \date 09/02/08
 */


#ifndef _VOCABULARY_H_
#define _VOCABULARY_H_


class cTestVocabulary;	// forward declaration for unit testing class


namespace AGR
{

class cVector3int;	// defined in Movement.h
class cMovement;	// defined in Movement.h
class cClustering;	// defined in Clustering.h


/**
 * \class cVocabulary
 * \brief cVocabulary class.
 */
class cVocabulary
{
friend class ::cTestVocabulary;
friend class cGestureRecogniser;

public:
	cVocabulary();
	cVocabulary( const cVocabulary& original );
	~cVocabulary();

	void clear();
	bool initialise( cClustering* ptClusterer, const unsigned int size, const cVector3int* buffer = NULL );
	bool isReady() const;
	bool copy( const cVocabulary& original );	
	bool load( const char* fileName );
	bool save( const char* fileName,
			   const bool binary = false ) const;

	int createClusters( const cMovement* movementArray, const unsigned int nbMovement );
	int clusterise( const cMovement& movement, unsigned int* observation ) const;
	int closerCluster( const cVector3int& point ) const;

	//*******************
	// inlines

	/*! Buffer size accessor
	 * \return The buffer size 
	*/
	inline unsigned int getSize() const 
	{
		return m_size;
	}	

	/*! Get the buffer for reading only
	 * \return The buffer (read only) 
	 */
	inline const cVector3int* getBuffer() const 
	{
		return m_buffer; 
	}	

	// inlines
	//*******************

protected:
	cClustering* m_clusters;	/**< Clustering object */
	cVector3int* m_buffer;		/**< Buffer containing the vocabulary data, used by m_clusters */
	unsigned int m_size;		/**< Size of the buffer */

	
	bool loadASCII( std::ifstream& file );
	bool saveASCII( std::ofstream& file ) const;
	bool loadBinary( std::ifstream& file );
	bool saveBinary( std::ofstream& file ) const;
	
}; // class cVocabulary



} // namespace AGR


#endif // _VOCABULARY_H_

