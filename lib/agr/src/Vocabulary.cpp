/**
 * \file Vocabulary.cpp
 * \brief Implementation for the cVocabulary class
 * \date 09/02/08
 */


#include <vector>
#include <fstream>

#include "Movement.h"
#include "Clustering.h"
#include "KmeansClustering.h"
#include "Vocabulary.h"

#include "AGRVersion.h"


namespace AGR
{


/*! Constructor
 */	
cVocabulary::cVocabulary() : m_clusters(NULL),m_buffer(NULL),m_size(0)
{
}



/*! Copy constructor
 */	
cVocabulary::cVocabulary( const cVocabulary& original )	/**< cVocabulary object to be copied */
{
	m_size = original.m_size;
	m_buffer = new cVector3int[ m_size ];
	memcpy( m_buffer, original.m_buffer, sizeof(cVector3int)*m_size );

	m_clusters = original.m_clusters->createCopy();
	m_clusters->initialise( m_size, m_buffer );
}



/*! Destructor
 */	
cVocabulary::~cVocabulary()
{ 
	clear();
}



/*! Clear all the data
 */	
void cVocabulary::clear()
{
	if( NULL != m_buffer ) {
		delete [] m_buffer;
		m_buffer = NULL;
	}

	if( NULL != m_clusters ) {
		delete m_clusters;
		m_clusters = NULL;
	}
	m_size = 0;
}


/*! Initialise the vocabulary
 * \return Success
 */	
bool cVocabulary::initialise( cClustering* ptClusterer,		/**< Cluster object to be used by m_clusters */
							  const unsigned int size,		/**< Size of the buffer, the number of clusters */
							  const cVector3int* buffer )	/**< Values for initialising the buffer (NULL if not needed) */
{
	// check input variables
	if( NULL == ptClusterer ||
		0 == size ) {
		return false; 
	}


	// create buffer
	if( NULL != m_buffer ) {
		delete [] m_buffer ;
	}
	m_size = size;
	m_buffer = new cVector3int[ m_size ];
	if( NULL != buffer )
	{
		memcpy( m_buffer, buffer, sizeof(cVector3int)*m_size );
	}

	m_clusters = ptClusterer->createCopy(); // a "new" allocation is done here
	m_clusters->initialise( m_size, m_buffer );

	return true;
}


/*! Copy a vocabulary object
 */	
bool cVocabulary::copy( const cVocabulary& original )	/**< cVocabulary object to be copied */
{
	// check that the original is ready
	if( original.isReady() == false ) {
		return false; 
	}

	// create buffer
	if( NULL != m_buffer ) {
		delete [] m_buffer ;
	}
	
	m_size = original.m_size;
	m_buffer = new cVector3int[ m_size ];
	memcpy( m_buffer, original.m_buffer, sizeof(cVector3int)*m_size );

	m_clusters = original.m_clusters->createCopy();
	m_clusters->initialise( m_size, m_buffer );

	return true;
}


/*! Load an vocabulary file.
 * \return Success
 */	
bool cVocabulary::load( const char* fileName ) /**< Name of the file to read into*/
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
		if( 0 == memcmp( buffer, AGR_VOCABULARY_BIN_FILE_HEADER, 3 ) )
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


/*! Save an vocabulary file.
 * \return Success
 */
bool cVocabulary::save( const char* fileName,	/**< Name of the file to save into */
						const bool binary		/**< Specify whether saving in binary of ASCII format */
						 ) const
{
	// Check input
	if( NULL == fileName ) {
		return false;
	}

	// Only save ready vocabulary
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



/*! Inform if the vocabulary is ready to be used.
 * \return True if ready, false if not
 * \note The function does not take into account if the clusters are created
 */	
bool cVocabulary::isReady() const
{
	if( NULL == m_clusters		||
		NULL == m_buffer		)
	{
		return false;
	}

	return true;
}



/*! Launch the cClustering createClusters() function 
 * \return -1 if failed. The number of clustering iteration otherwith
 */
int cVocabulary::createClusters( const cMovement* movementArray,	/**< Array of movement used for creating clusters */
								 const unsigned int nbMovement )	/**< Number of movement in the array */
{
	if( NULL == m_clusters ) {
		return -1; 
	}

	return m_clusters->createClusters( movementArray, nbMovement );
}



/*! Launch the cClustering clusterise() function 
 * \return -1 if failed. 0 if obsClusterArray has not changed, 1 if obsClusterArray has changed
 */
int cVocabulary::clusterise( const cMovement& movement,	/**< Movement to be clusterised */
							 unsigned int* observation	/**< Output cluster observation sequence */
							 ) const
{
	if( NULL == m_clusters ) {
		return -1; 
	}

	return m_clusters->clusterise( movement, observation );
}



/*! Launch the cClustering closerCluster() function 
 * \return -1 if failed. The closer cluster id if success
 */
int cVocabulary::closerCluster( const cVector3int& point	/**< Point to be analysed */
							    ) const
{
	if( NULL == m_clusters ) {
		return -1; 
	}

	return m_clusters->closerCluster( point );
}



/*! Load from an ASCII file
 * \note The vocabulary is not loaded there
 * \return Success
 */
bool cVocabulary::loadASCII( std::ifstream& file )
{
	if( file.is_open() == false ) {
		return false;
	}

	// version checking
	unsigned int version;
	char str[100];
	file >> str >> str >> str >> str >> version;
	if( version != AGR_VOCABULARY_ASCII_FILE_VERSION )
	{
		// TODO : backward compatibility
		return false;
	}
	file.getline( str, 100, '\n' );	// EOL

	// clean vocabulary values
	clear();

	// Skip file info
	file.getline( str, 100, '\n' );

	// Buffer size
	file >> str >> m_size;
	m_buffer = new cVector3int [ m_size ];
	file.getline( str, 100, '\n' ); // EOL

	// Clustering technique
	file.getline( str, 100, '\n' ); // Only cKmeansClustering for the moment
	m_clusters = new cKmeansClustering;
	m_clusters->initialise( m_size, m_buffer );

	// Buffer
	file.getline( str, 100, '\n' ); // Empty line
	file.getline( str, 100, '\n' ); // Buffer info
	for( unsigned int i=0; i<m_size; i++ )
	{
		file >> m_buffer[ i ].x >> m_buffer[ i ].y >> m_buffer[ i ].z ;
	}
	file.getline( str, 100, '\n' ); // Empty line

	return true;
}



/*! Save from an ASCII file
 * \return Success
 */
bool cVocabulary::saveASCII( std::ofstream& file ) const
{
	if( file.is_open() == false ) {
		return false;
	}

	// File informations
	file << "ASCII Vocabulary File version " << AGR_VOCABULARY_ASCII_FILE_VERSION << std::endl;
	file << "Created with AGR v" << AGR_VERSION << "." << AGR_VERSION_MIN << std::endl;
	file << std::endl;

	// Buffer size
	file << "Size: " << m_size << std::endl;

	// Clustering type
	file << "Clustering type Id : cKmeansClustering" << std::endl; // For the moment, only cKmeansClustering
	file << std::endl;

	file << "Buffer :" << std::endl;
	for( unsigned int i=0; i<m_size; i++ )
	{
		file << m_buffer[ i ].x << " " << m_buffer[ i ].y << " " << m_buffer[ i ].z << std::endl;
	}

	return true;
}



/*! Load from a binary file
 * \return Success
 * \todo Endianness
 */
bool cVocabulary::loadBinary( std::ifstream& file )
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
		if( 0 != memcmp( buffer, AGR_VOCABULARY_BIN_FILE_HEADER, 3 ) ) {
			return false;
		}
	}

	{// Version
		unsigned int version;
		if( !file.read( (char*)(&version), sizeof(version) ) ) {
			return false;
		}

		// Check header
		if( version != AGR_VOCABULARY_BINARY_FILE_VERSION ) {
			return false;
		}
	}
	
	// Buffer size
	if( !file.read( (char*)(&m_size), sizeof(m_size) ) ) {
		return false;
	}
	if( m_size == 0 ) {
		return 0;
	}
	m_buffer = new cVector3int [ m_size ];

	// No clustering information : kmeans is expected
	m_clusters = new cKmeansClustering;
	m_clusters->initialise( m_size, m_buffer );
	
	// Buffer
	for( unsigned int i=0; i<m_size; i++ )
	{
		if( !file.read( (char*)(&m_buffer[ i ].x), sizeof(int) ) ) {
			return false;
		}
		if( !file.read( (char*)(&m_buffer[ i ].y), sizeof(int) ) ) {
			return false;
		}
		if( !file.read( (char*)(&m_buffer[ i ].z), sizeof(int) ) ) {
			return false;
		}
	}

	return true;
}



/*! Save from a binary file
 * \return Success
 * \todo Endianness
 */
bool cVocabulary::saveBinary( std::ofstream& file ) const
{	
	if( file.is_open() == false ) {
		return false;
	}

	// Binary header
	file.write( AGR_VOCABULARY_BIN_FILE_HEADER, 3 );

	// Version
	file.write( (char*)(&AGR_VOCABULARY_BINARY_FILE_VERSION), sizeof(unsigned int) );
	
	// Buffer size
	file.write( (char*)(&m_size), sizeof(m_size) );

	// No clustering information : kmeans is expected
	
	// Buffer
	for( unsigned int i=0; i<m_size; i++ )
	{
		file.write( (char*)(&m_buffer[ i ].x), sizeof(int) );
		file.write( (char*)(&m_buffer[ i ].y), sizeof(int) );
		file.write( (char*)(&m_buffer[ i ].z), sizeof(int) );
	}

	return true;
}


} //namespace AGR

