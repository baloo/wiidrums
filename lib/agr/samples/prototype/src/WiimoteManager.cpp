/**
 * \file WiimoteManager.cpp
 * \brief Implementation for the class managing the wiimotes, using wiisue
 * \date 15/11/07
 */

#include "stdafx.h"
#include "WiimoteManager.h"






unsigned char cWiimote::getRawXAcc()
{
	cWiimoteManager* wiiMng = cWiimoteManager::get();
	return wiiMng->m_wiiuse_wiimotes[ m_id-1 ]->accel.x;
}


unsigned char cWiimote::getRawYAcc()
{
	cWiimoteManager* wiiMng = cWiimoteManager::get();
	return wiiMng->m_wiiuse_wiimotes[ m_id-1 ]->accel.y;
}

unsigned char cWiimote::getRawZAcc()
{
	cWiimoteManager* wiiMng = cWiimoteManager::get();
	return wiiMng->m_wiiuse_wiimotes[ m_id-1 ]->accel.z;
}







//*****************************************
// cWiimoteManager
//*****************************************

cWiimoteManager* cWiimoteManager::s_instance = NULL;



/*! Get one of the wiimote
*/
cWiimote* cWiimoteManager::getWiimote( int id )
{ 
	if( id<1 || id>WIIMOTEMNG_MAX_DEVICE )
		return NULL;

	return &m_wiimote[ id-1 ];
}


/*! Return the singleton, create and initialise it if necessary
*/
cWiimoteManager* cWiimoteManager::get()
{
	if( s_instance )
		return s_instance;

	if( wiiuse_startup( "wiiuse.dll" ) == NULL )
		return NULL;

	s_instance = new cWiimoteManager();
	s_instance->init();

	return s_instance;
}



/*! init the singleton
*/
bool cWiimoteManager::init()
{
	memset( m_wiimote, 0x00, sizeof(cWiimote*)*WIIMOTEMNG_MAX_DEVICE);
	
	int wiiId[] = {1, 2, 3, 4 };
	m_wiimote[0].m_id = 1;
	m_wiimote[1].m_id = 2;
	m_wiimote[2].m_id = 3;
	m_wiimote[3].m_id = 4;

	// get the wiimote information from wiiuse
	m_wiiuse_wiimotes = wiiuse_init( 4, wiiId, _wiiEventCB, _wiiCtrlStatusCB, _wiiDisconectionCB );

	// find and connect the wiimotes
	wiiuse_find( m_wiiuse_wiimotes, 4, 5 );
	m_nbWiimoteConnected = wiiuse_connect( m_wiiuse_wiimotes, 4 );

	wiiuse_set_leds( m_wiiuse_wiimotes[0], WIIMOTE_LED_1 );
	wiiuse_set_leds( m_wiiuse_wiimotes[1], WIIMOTE_LED_2 );
	wiiuse_set_leds( m_wiiuse_wiimotes[2], WIIMOTE_LED_3 );
	wiiuse_set_leds( m_wiiuse_wiimotes[3], WIIMOTE_LED_4 );
	wiiuse_motion_sensing( m_wiiuse_wiimotes[0], 1 );
	wiiuse_motion_sensing( m_wiiuse_wiimotes[1], 1 );
	wiiuse_motion_sensing( m_wiiuse_wiimotes[2], 1 );
	wiiuse_motion_sensing( m_wiiuse_wiimotes[3], 1 );

	return true;
}



/*! Release the singleton
*/
bool cWiimoteManager::releaseManager()
{
	if( !s_instance )
		return false;

	wiiuse_disconnect( s_instance->m_wiiuse_wiimotes[0] );
	wiiuse_disconnect( s_instance->m_wiiuse_wiimotes[1] );
	wiiuse_disconnect( s_instance->m_wiiuse_wiimotes[2] );
	wiiuse_disconnect( s_instance->m_wiiuse_wiimotes[3] );

	wiiuse_shutdown();

	delete s_instance;
	s_instance = NULL;

	return true;
}

/*! Update the wiimotes
*/
bool cWiimoteManager::update()
{
	wiiuse_poll( m_wiiuse_wiimotes, 4 );
	return true;
}



/*! wiiuse event callback
*/
void cWiimoteManager::_wiiEventCB( struct wiimote_t* wm )
{
//	wiimote* wiimote = s_instance->m_wiiuse_wiimotes[ wm->unid-1 ];
//	wiimote = wm;
}



/*! wiiuse control status callback
*/
void cWiimoteManager::_wiiCtrlStatusCB(struct wiimote_t* wm, int attachment, int speaker, int ir, int led[4], float battery_level)
{
}



/*! wiiuse disconection callback
*/
void cWiimoteManager::_wiiDisconectionCB(struct wiimote_t* wm)
{
}
