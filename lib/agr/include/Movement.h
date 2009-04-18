/**
 * \file Movement.h
 * \brief Declaration for the movement class
 * \date 09/02/08
 */


#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

// Resolves problem with the memcpy not defined error
#include <string.h>

// std::vector only used in cMovement::initialise( const std::vector<cVector3int>& )
// May be remove if not needed

class cTestVector3int;	// forward declaration for unit testing class
class cTestMovement;	

namespace AGR
{



/**
 * \class cVector3int
 * \brief cVector3int class.
 */
class cVector3int
{
	friend class ::cTestVector3int;
public:
	cVector3int();
	cVector3int( const cVector3int& original );
	cVector3int( int x, int y, int z);

	// Overloaded operators
	cVector3int& operator+=(const cVector3int &rhs);
	cVector3int& operator-=(const cVector3int &rhs); 
	cVector3int& operator*=(const int &rhs);
	cVector3int& operator/=(const int &rhs);
	const cVector3int operator+(const cVector3int &other) const;
	const cVector3int operator-(const cVector3int &other) const;
	const cVector3int operator*(const int &value) const;
	const cVector3int operator/(const int &value) const;
	bool operator==(const cVector3int &rhs) const;
	bool operator!=(const cVector3int &rhs) const;
	

	// other vector feature
	void copy( const cVector3int& original ); 
	bool copyTo( cVector3int* destination ) const; 

	int x;	/**< X Vector Component */
	int y;	/**< Y Vector Component */
	int z;	/**< Z Vector Component */
protected:

}; // class cVector3int



/**
 * \class cMovement
 * \brief cMovement class.
 */
class cMovement
{
friend class ::cTestMovement;
public:
	cMovement();
	cMovement( const cMovement& original );
	~cMovement();

	void clear();
	bool initialise( const std::vector<cVector3int>& inMove );
	bool initialise( const unsigned int size, const cVector3int* vectorArray=NULL );
	bool copy( const cMovement& original ); 

	static bool changeVectorSize( const cMovement& inputMovement,
								  const unsigned int newSize, 
								  cMovement* ptOutputMovement );


	//******************************
	// inlines

	/*! Buffer size accessor
	 * \return The buffer size
	 */
	inline unsigned int getSize() const 
	{ 
		return m_size; 
	}				

	/*! Get a value from the buffer
	 * \return A buffer value (read only) 
	 */
	inline const cVector3int getData( const unsigned int id  /**< index of the data in the buffer */
									) const 	
	{
		if( m_buffer && id < m_size )
			return m_buffer[ id ]; 
		else
			return cVector3int( 0,0,0 );
	}

	/*! Get the buffer for reading only
	 * \return The buffer (read only) 
	 */
	inline const cVector3int* getBuffer() const 
	{
		return m_buffer;
	}	
	
	/*! Get The buffer for modifications
	 * \return The buffer
	 */
	inline cVector3int* modifyBuffer() 
	{ 
		return m_buffer; 
	}				

	// inlines
	//******************************

protected:

	cVector3int* m_buffer;	/**< Buffer containing the movement data */
	unsigned int m_size;	/**< Size of the buffer (number of cVector3int) */

}; // class cMovement


} // namespace AGR


#endif // _MOVEMENT_H_

