/**
 * \file WiiAccelerometer.h
 * \brief Declaration for the accelerometer wii class
 * \date 15/11/07
 */


#ifndef _WIIACCELEROMETER_H_
#define _WIIACCELEROMETER_H_

#include "Accelerometer.h"
#include "WiimoteManager.h"


/**
 * \class cWiiAccelerometre
 * \brief class for the wii accelerometers
 */	
class cWiiAccelerometer : public cAccelerometer
{
public:

	eDevice getType() { return InputWiimote; }

	bool init( cWiimote* wiimote );
	bool update();
	bool release();

protected:

	cWiimote* m_wiimote;

}; // class cWiiAccelerometre



#endif // _WIIACCELEROMETER_H_

