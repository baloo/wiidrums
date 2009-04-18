/**
 * \file WiiAccelerometer.cpp
 * \brief implementation for the accelerometer wii class
 * \date 15/11/07
 */


#include "stdafx.h"
#include "WiiAccelerometer.h"



/*! init the accelerometer
*/
bool cWiiAccelerometer::init( cWiimote* wiimote )
{
	if( !wiimote )
		return false;

	m_wiimote = wiimote;
	return true;
}


/*! Update the accelerometer
*/
bool cWiiAccelerometer::update()
{
	if( !m_wiimote )
		return false;

	m_rawAcceleration.x = m_wiimote->getRawXAcc();
	m_rawAcceleration.y = m_wiimote->getRawYAcc();
	m_rawAcceleration.z = m_wiimote->getRawZAcc();
	return true;
}


/*! Release the accelerometer
*/
bool cWiiAccelerometer::release()
{
	m_wiimote = NULL;
	return true;
}



