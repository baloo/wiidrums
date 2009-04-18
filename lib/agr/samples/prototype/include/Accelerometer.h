/**
 * \file cAccelerometer.h
 * \brief Declaration for the accelerometer class
 * \date 15/11/07
 */


#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_



/**
 *	\struct vec3u8_t
 *	\brief Unsigned x,y,z byte vector.
 */
typedef struct vec3u8_t {
	unsigned char x, y, z;
} vec3u8_t;


/**
 * \class cAccelerometer
 * \brief Interface for the accelerometers
 */	
class cAccelerometer
{
public:
	enum eDevice
	{
		InputNone = 0,
		InputWiimote,
		InputSixaxis,
		InputUnknown,
	};

	virtual ~cAccelerometer() {}
	virtual eDevice getType()=0;

	virtual bool update()=0;
	virtual bool release()=0;

	vec3u8_t m_rawAcceleration;
protected:
}; // class cAccelerometer



#endif // _ACCELEROMETER_H_

