/**
 * \file WiimoteManager.h
 * \brief Declaration for the class managing the wiimotes, using wiisue
 * \date 15/11/07
 */


#ifndef _WIIMOTEMANAGER_H_
#define _WIIMOTEMANAGER_H_

#include "wiiuse.h"


/**
 * \class cWiimote
 * \brief Class for the wiimote. 
 * \warning Only does what it is necessary for this project
 */	
class cWiimote
{
friend class cWiimoteManager;

public:
	wiimote* getWiiuseObject();

	unsigned char getRawXAcc();
	unsigned char getRawYAcc();
	unsigned char getRawZAcc();

protected:
	unsigned int m_id;
}; // class cWiimote




#define WIIMOTEMNG_MAX_DEVICE	(4)

/**
 * \class cWiimoteManager
 * \brief Class for the wiimote manager
 */	
class cWiimoteManager
{
friend class cWiimote;

public:
	static cWiimoteManager* get();
	static bool releaseManager();

	cWiimote* getWiimote( int id ); 
	bool update();

protected:
	cWiimote m_wiimote[ WIIMOTEMNG_MAX_DEVICE ];
	wiimote** m_wiiuse_wiimotes;
	int m_nbWiimoteConnected;
	
	static cWiimoteManager* s_instance;

	cWiimoteManager() {}
	bool init();

	
	static void _wiiEventCB(struct wiimote_t* wm); 
	static void _wiiCtrlStatusCB(struct wiimote_t* wm, int attachment, int speaker, int ir, int led[4], float battery_level);
	static void _wiiDisconectionCB(struct wiimote_t* wm);


}; // class cWiimoteManager


#endif // _WIIMOTEMANAGER_H_

