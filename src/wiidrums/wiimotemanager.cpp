/*
    WiiGuard - Home guardian with Wiimotes
    Copyright (C) 2009  David BONNET - Vincent GUYOMARD

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "WiimoteManager.h"
#include "include/wiiuse.h"

using std::cout; 
using namespace std;

//Constructeur
WiimoteManager::WiimoteManager(){
    /*
     * On initialise le tableau avec le nombre maximum de wiimotes utilisables
     * Même si une seule wiimote est connectée, les autres ne seront alors pas utilisées
     */
    wiimotes = wiiuse_init(MAX_WIIMOTES);
    nbWiimoteConnected = 0;

    //Au démarrage on est en mode d'initialisation
    initMode = true;

    //On crée un vecteur qui contient l'enregistrement des sources IR pour chaque Wiimote
    /* ANCIENNE VERSION
    QVector<bool> dotVisible(4);
    dotVisible.insert(0, false);
    dotVisible.insert(1, false);
    dotVisible.insert(2, false);
    dotVisible.insert(3, false);

    QVector<QVector<bool> > wiimotesIRDots(MAX_WIIMOTES, dotVisible);
    */
     //wiimotesIRDots = (int*)malloc(MAX_WIIMOTES*4*sizeof(int));

    eventTimer = new QTimer(this);
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(timeOutHidenEvent()));
    timeInterval = 250;
}

//Destructeur
WiimoteManager::~WiimoteManager(){
    //delete wiimotesIRDots;

    //On nettoie les structures des wiimotes
    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
}

//Slot qui gère les évènements cachés
void WiimoteManager::timeOutHidenEvent(){
    for(int i=0; i<MAX_WIIMOTES; i++){
	handleWiimotesEvent(wiimotes[i]);
    }
    
}

//Méthode de connexion des wiimotes
bool WiimoteManager::connectWiimotes(int timeout){
    int found, connected;

    //On lance la recherche des wiimotes pendant timeout secondes
    found = wiiuse_find(wiimotes, MAX_WIIMOTES, timeout);
    if (!found) {
	cout << "No wiimotes found.\n";
	emit failedToConnect();
	return false;
    }

    //Si on a trouvé des wiimotes alors on lance la connexion
    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
    if (connected){
	cout << "Connected to " << connected << " wiimotes (of "<< found << " found).\n";
	emit wiimoteConnected(connected, found);
    } else {
	cout << "Failed to connect to any wiimote.\n";
	emit failedToConnect();
	return false;
    }

    nbWiimoteConnected = connected;
    
    //On défini la sensibilité de l'IR à 3 par défaut
    for(int i=0; i<MAX_WIIMOTES; i++){
	wiiuse_set_ir_sensitivity(wiimotes[i], 3);
	emit wiimoteIRSensitivityChange(i+1, 3);
    }

    //On règle les LEDS des wiimotes par défaut
    this->setLedsByDefault();
    return true;
}

//On nettoye le tableau des wiimotes
void WiimoteManager::disconnectWiimotes(){
    for(int i=0; i<MAX_WIIMOTES; i++)
	wiiuse_disconnect(wiimotes[i]);

    nbWiimoteConnected = 0;
}

//On active l'IR d'une wiimote
bool WiimoteManager::enableIRWiimote(int numWiimote){

    if(numWiimote <= MAX_WIIMOTES){
	wiiuse_set_ir(wiimotes[numWiimote - 1], 1);
	return true;
    }

    return false;
}

//On déactive l'IR d'une wiimote
bool WiimoteManager::disableIRWiimote(int numWiimote){
    if(numWiimote <= MAX_WIIMOTES){
	wiiuse_set_ir(wiimotes[numWiimote - 1], 0);
	return true;
    }

    return false;
}

//Passer en mode Init
void WiimoteManager::switchToInitMode(){
    initMode = true;
    printf("### Init Mode ###\n");
    emit alarmModeEnable(false);
}

//Passer en mode Init
void WiimoteManager::switchToAlarmMode(){
    initMode = false;
    printf("### Alarm Mode ###\n");
    emit alarmModeEnable(true);
}

//Changer la sensibilité des wiimotes
void WiimoteManager::changeIRSensitivity(int numWiimote, int sensitivity){
    wiiuse_set_ir_sensitivity(wiimotes[numWiimote-1], sensitivity);

    if(sensitivity > 5) sensitivity = 5;
    if(sensitivity <= 0) sensitivity = 0;
    //printf("WiimoteManager>> Sensibilité IR de la wiimote %d à %d\n", numWiimote, sensitivity);
    emit wiimoteIRSensitivityChange(numWiimote, sensitivity);
}


//On définit les LEDS en fonction de la position de la wiimote
void WiimoteManager::setLedsByDefault(){
    //On ne gère pour le moment que 4 wiimotes
    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
    wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
    wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
    wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);
}

//On éteint les LEDS de toute wiimote connectée
void WiimoteManager::noLedAnymore(){

    for(int i=0; i<MAX_WIIMOTES; i++){
	wiiuse_set_leds(wiimotes[i], WIIMOTE_LED_NONE);
    }
}

//Prise en charge des évènements de la wiimote
void WiimoteManager::handleWiimotesEvent(wiimote *wm){

    if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP))
	switchToAlarmMode();

    if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))
	switchToInitMode();

    
    if (WIIUSE_USING_IR(wm)) {
	int i = 0;

	//QVector<bool> tmpIRDotStatus(4);

	//On balaye les 4 sources IR possibles
	for (; i < 4; ++i) {

	    if(initMode){
		//On enregistre la position des wiimote
		//tmpIRDotStatus.insert(i, wm->ir.dot[i].visible);
		wiimotesIRDots[wm->unid-1][i] = wm->ir.dot[i].visible;
	    } else {
		//On est en mode alarme, si une défférence est détectée on sonne l'alarme
		if(diffBetweenOriginalIRDotsAnd(wm, i)){
		    //Alors un faisceau à été franchi, on sonne l'alarme
		    emit alarmDeclenched(wm->unid, i+1);
		    printf("ALARME DECLENCHEE (Wiimote %d - Point %d)\n", wm->unid, i+1);
		}
	    }

	    //On vérifie si la source est visible
	    if (wm->ir.dot[i].visible)
		    printf("IR source %i: (%u, %u)\n", i, wm->ir.dot[i].x, wm->ir.dot[i].y);

	    emit wiimoteIRDotDetected(wm->unid, i+1, wm->ir.dot[i].visible, wm->ir.dot[i].x, wm->ir.dot[i].y);
	}

	/*
	if(initMode)
	    wiimotesIRDots.insert(wm->unid-1, tmpIRDotStatus);
	*/
    }

}

//Prise en charge de la déconnexion des Wiimotes
void WiimoteManager::handleDisconnect(wiimote* wm) {
    cout << "\n\n--- DISCONNECTED [wiimote id "<< wm->unid << "] ---\n";

    emit wiimoteDisconnected(wm->unid);
    nbWiimoteConnected--;
}

//On vérifie la différence entre les deux sources IR
bool WiimoteManager::diffBetweenOriginalIRDotsAnd(wiimote* wm, int numIRDot){
    
     //printf("Diff IRDot : %d != %d : Record dot\n", wm->ir.dot[numIRDot].visible, wiimotesIRDots.at(wm->unid-1).at(numIRDot));
     //return wm->ir.dot[numIRDot].visible != wiimotesIRDots.at(wm->unid-1).at(numIRDot);
     return wm->ir.dot[numIRDot].visible != wiimotesIRDots[wm->unid-1][numIRDot];
}

//Exécution de la boucle d'évènements
void WiimoteManager::exec(){

    //Ce booleen permet d'arrêter la boucle d'évènements
    //bool quit = false;

    eventTimer->start(timeInterval);

    while(nbWiimoteConnected != 0){
	//cout << "NbWiimotes : " << nbWiimoteConnected << "\n";
	//Wiiuse_pool vérifie les évènements de chaque wiimote, si un évènement à eu lieu, alors la fonction est déclenchée
	if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {

	    //Pour chaque wiimote
	    for (int i=0; i < MAX_WIIMOTES; ++i) {
		switch (wiimotes[i]->event) {
		    case WIIUSE_EVENT:
			this->handleWiimotesEvent(wiimotes[i]);
			break;
		    case WIIUSE_STATUS:
			//gérer le status
			break;
		    case WIIUSE_DISCONNECT:
		    case WIIUSE_UNEXPECTED_DISCONNECT:
			//Gérer la déconnexion
			handleDisconnect(wiimotes[i]);
			break;
		    default:
			break;
		}
	    }
	}

    }

    eventTimer->stop();
}



