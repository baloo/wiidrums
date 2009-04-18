/**
 * \file Movement.cpp
 * \brief Implementation for the movement class
 * \date 09/02/08
 */


#include <vector>

#include "Movement.h"


namespace AGR
{

//******************************************
// cVector3int
//------------------------------------------


/*! Default Constructor
 */	
cVector3int::cVector3int() :x(0),y(0),z(0)
{
}


/*! Copy constructor
 */	
cVector3int::cVector3int( const cVector3int& original) : x(original.x), y(original.y), z(original.z)
{
}


/*! Constructor with arguments
 */	
cVector3int::cVector3int( int _x, int _y, int _z) : x(_x), y(_y), z(_z)
{
}


/*! Overloaded operator +=
 */	
cVector3int& cVector3int::operator+=( const cVector3int &rhs ) 
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}


/*! Overloaded operator -=
 */	
cVector3int& cVector3int::operator-=( const cVector3int &rhs ) 
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}


/*! Overloaded operator *=
 */	
cVector3int& cVector3int::operator*=( const int &rhs ) 
{
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}


/*! Overloaded operator /=
 */	
cVector3int& cVector3int::operator/=( const int &rhs ) 
{
	x /= rhs;
	y /= rhs;
	z /= rhs;
	return *this;
}


/*! Overloaded operator +
 */	
const cVector3int cVector3int::operator+( const cVector3int &other
										  ) const 
{
	return cVector3int(*this) += other;
}


/*! Overloaded operator -
 */	
const cVector3int cVector3int::operator-( const cVector3int &other
										  ) const 
{
	return cVector3int(*this) -= other;
}


/*! Overloaded operator *
 */	
const cVector3int cVector3int::operator*( const int &value
										  ) const 
{
	return cVector3int(*this) *= value;
}


/*! Overloaded operator /
 */	
const cVector3int cVector3int::operator/( const int &value 
										  ) const 
{
	return cVector3int(*this) /= value;
}


/*! Overloaded operator ==
 */	
bool cVector3int::operator==(const cVector3int &rhs) const
{
	return ( x==rhs.x && y==rhs.y && z==rhs.z );
}


/*! Overloaded operator !=
 */	
bool cVector3int::operator!=(const cVector3int &rhs) const
{
	return !( (*this) == rhs );
}



/*! Copy the vector from another
 * \return Success
 */	
void cVector3int::copy( const cVector3int& original /**< original vector to be copied */
					   )
{
	x = original.x;
	y = original.y;
	z = original.z;
}



/*! Create a copy of the vector
 * \return Success
 */	
bool cVector3int::copyTo( cVector3int* destination /**< Destination vector */
					      ) const
{
	// input checks
	if( NULL == destination ) {
		return false;
	}

	destination->x = x;
	destination->y = y;
	destination->z = z;

	return true;
}



//******************************************
// cMovement
//------------------------------------------



/*! Constructor
 */	
cMovement::cMovement(): m_buffer(NULL), m_size(0)
{
}



/*! Copy constructor
 */	
cMovement::cMovement( const cMovement& original )
{
	m_size = original.m_size;
	if( m_size > 0 )
	{
		m_buffer = new cVector3int[ m_size ];
		memcpy( m_buffer, original.m_buffer, m_size*sizeof(cVector3int) );
	}
	else
	{
		m_buffer = NULL;
	}
}



/*! Destructor
 */	
cMovement::~cMovement()
{
	clear();
}



/*! Clear all the data
 */	
bool cMovement::copy( const cMovement& original )
{	
	m_size = original.m_size;
	if( m_size > 0 )
	{
		if( NULL != m_buffer ) {
			delete [] m_buffer;
		}

		m_buffer = new cVector3int[ m_size ];
		memcpy( m_buffer, original.m_buffer, m_size*sizeof(cVector3int) );
	}
	else
	{
		m_buffer = NULL;
	}
	return true;
}



/*! Clear all the data
 */	
void cMovement::clear()
{
	if( NULL != m_buffer ) {
		delete [] m_buffer;
	}
	m_buffer = NULL;
	m_size = 0;
}



/*! Initialisation using the STL vector container
 * \param inMove std::vector containing the list of cVector3int to be copied
 * \return Success
 */	
bool cMovement::initialise( const std::vector<cVector3int>& inMove )
{
	if( NULL != m_buffer ) {
		delete [] m_buffer;
	}

	if( inMove.empty() ){
		return false;
	}

	// create buffer
	m_size = (unsigned int)inMove.size();
	m_buffer = new cVector3int[ m_size ];
	cVector3int* iterBuffer = m_buffer;

	// copy data
	std::vector<cVector3int>::const_iterator iter;
	for( iter = inMove.begin(); iter != inMove.end(); iter++ )
	{
		iter->copyTo( iterBuffer );
		iterBuffer++;
	}

	return true;
}



/*! Initialisation using a buffer
 * \return Success
 */	
bool cMovement::initialise( const unsigned int size,			/**< Size of the movement */
							const cVector3int* vectorArray )	/**< Array containing the initial value, set NULL if not needed */
{
	// input checks
	if( 0 == size ) {
		return false; 
	}

	if( NULL != m_buffer ) {
		delete [] m_buffer;
	}

	// create buffer
	m_size = size;
	m_buffer = new cVector3int[ m_size ];
	if( vectorArray ) {
		memcpy( m_buffer, vectorArray, m_size*sizeof(cVector3int) );
	}

	return true;
}



/*! Change the size of a movement
 * \note OutputVector buffer will be created automatically if needed
 * \note The function have better results if input movements are positive (see note in the code)
 * \return Success
 */	
bool cMovement::changeVectorSize( const cMovement& inputMovement,	/**< Input movement used as model */
								  const unsigned int newSize,		/**< Size of the new vector */
								  cMovement* ptOutputMovement )		/**< Output vector resized */
{
	// input checks
	if( NULL == ptOutputMovement ||
		&inputMovement == ptOutputMovement) 
	{
		return false;
	}

	if( ptOutputMovement->getSize() != newSize )
	{
		// output buffer creation
		if( NULL != ptOutputMovement->m_buffer ) {
			delete [] ptOutputMovement->m_buffer;
		}
		ptOutputMovement->m_size = newSize;
		ptOutputMovement->m_buffer = new cVector3int[ newSize ];
	}


	// Compute normalised vector
	for( unsigned int i=0; i<newSize; i++ )
	{
		// Start                  End
		// S-------X-------X------E		Movement
		//       A         B
		// S----------X-----------E		Normalised
		//         C = value to find
		// A B, closed value to C
		//
		// X = value(A)*(distanceBC/distanceAB) + value(B)*(distanceAC/distanceAB)
		// distanceAB = 1/ (movement vector size -1)
		// distanceSA = distanceAB* A index (same for SB)
		// distanceSC = ( 1/ (normalised vector size -1) ) * C index
		// distanceAC = distanceSC - distanceSA
		// distanceBC = distanceSB - distanceSC

		int indexC = i;
		float distanceAB = ( 1.0f/((float)inputMovement.getSize()-1.0f) );
		float distanceSC = ( 1.0f/((float)ptOutputMovement->getSize()-1.0f) ) * indexC;
		
		int indexA=0, indexB=0;
		for( unsigned int j=0; j<inputMovement.getSize(); j++)
		{
			if(  j*distanceAB == distanceSC )
			{
				indexA = j; 
				indexB = j;
				break;
			}
			if( j*distanceAB > distanceSC )
			{
				indexA = j-1; 
				indexB = j;
				break;
			}
		}

		float distanceSA = distanceAB * indexA;
		float distanceSB = distanceAB * indexB;
		float distanceAC = distanceSC - distanceSA;
		float distanceBC = distanceSB - distanceSC;

		if( indexA == indexB )
		{
			ptOutputMovement->m_buffer[i] = inputMovement.m_buffer[indexA];
		}
		else
		{
			// 0.5f added to have a better float to int cast
			// NOTE : this works ONLY when the float value is greater than 0
			// In the case of the accelerometers values as used in AGR, this is not an issue. 
			ptOutputMovement->m_buffer[i].x = (int)( 0.5f + inputMovement.m_buffer[indexA].x*(distanceBC/distanceAB) + 
										  inputMovement.m_buffer[indexB].x*(distanceAC/distanceAB) );
			ptOutputMovement->m_buffer[i].y = (int)( 0.5f + inputMovement.m_buffer[indexA].y*(distanceBC/distanceAB) + 
										  inputMovement.m_buffer[indexB].y*(distanceAC/distanceAB) );
			ptOutputMovement->m_buffer[i].z = (int)( 0.5f + inputMovement.m_buffer[indexA].z*(distanceBC/distanceAB) + 
										  inputMovement.m_buffer[indexB].z*(distanceAC/distanceAB) );
		}
	}

	return true;
}


} // namespace AGR

