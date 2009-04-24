
#include "wiimotemanager.h"

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

    eventTimer = new QTimer(this);
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(timeOutHidenEvent()));
    timeInterval = 2000;
}

//Destructeur
WiimoteManager::~WiimoteManager(){

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
      emit wiimoteIRSensitivityChange(i, 3);
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
      wiiuse_set_ir(wiimotes[numWiimote], 1);
      return true;
    }

    return false;
}

//On déactive l'IR d'une wiimote
bool WiimoteManager::disableIRWiimote(int numWiimote){
    if(numWiimote <= MAX_WIIMOTES){
      wiiuse_set_ir(wiimotes[numWiimote], 0);
      return true;
    }

    return false;
}

//Changer la sensibilité des wiimotes
void WiimoteManager::changeIRSensitivity(int numWiimote, int sensitivity){
    wiiuse_set_ir_sensitivity(wiimotes[numWiimote], sensitivity);

    if(sensitivity > 5) sensitivity = 5;
    if(sensitivity <= 0) sensitivity = 0;
    //printf("WiimoteManager>> Sensibilité IR de la wiimote %d à %d\n", numWiimote, sensitivity);
    emit wiimoteIRSensitivityChange(numWiimote, sensitivity);
}


//On définit les LEDS en fonction de la position de la wiimote
void WiimoteManager::setLedsByDefault(){
    //La wiimote de la main gauche est identifiée par les deux LEDS de gauche
    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1 | WIIMOTE_LED_2);
    
    //La wiimote de la main droite est identifiée par les deux LEDS de droite
    wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_3 | WIIMOTE_LED_4);
}

//On éteint les LEDS de toute wiimote connectée
void WiimoteManager::noLedAnymore(){

    for(int i=0; i<MAX_WIIMOTES; i++){
      wiiuse_set_leds(wiimotes[i], WIIMOTE_LED_NONE);
    }
}

//Prise en charge des évènements de la wiimote
void WiimoteManager::handleWiimotesEvent(wiimote *wm){

  // Le bouton - désactive les accéléromètres
  if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)){
    wiiuse_motion_sensing(wm, 0);
  }

  // Le bouton + active les accéléromètres
  if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)){
    wiiuse_motion_sensing(wm, 1);
    wiiuse_set_orient_threshold(wm, 5);
  }

  // Si l'accéléromètre est activé on affiche 
  if (WIIUSE_USING_ACC(wm)) {
        printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
        printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
        printf("wiimote yaw   = %f\n", wm->orient.yaw);
  }

}

//Prise en charge de la déconnexion des Wiimotes
void WiimoteManager::handleDisconnect(wiimote* wm) {
    cout << "\n\n--- DISCONNECTED [wiimote id "<< wm->unid << "] ---\n";

    emit wiimoteDisconnected((wm->unid)-1);
    nbWiimoteConnected--;
}

//Exécution de la boucle d'évènements
void WiimoteManager::exec(){

    eventTimer->start(timeInterval);

    cout << "Main event loop" << endl;
    cout << nbWiimoteConnected << endl;

    //On active le motion tracking
    // Temporairement l'id est fixe
    //wiiuse_motion_sensing(wiimotes[0], 1);
    //wiiuse_motion_sensing(wiimotes[1], 1);
    
    while(nbWiimoteConnected != 0){
    //cout << "NbWiimotes : " << nbWiimoteConnected << endl;
      //Wiiuse_pool vérifie les évènements de chaque wiimote, si un évènement à eu lieu, alors la fonction est déclenchée
      if(wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
          cout << "Inside If" << endl;

          //Pour chaque wiimote
          for (int i=0; i < MAX_WIIMOTES; ++i) {
            cout << "Inside for : i = " << i << endl;
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

void WiimoteManager::readWiimoteAccel(QTimerEvent *){
    for(int i=0; i<MAX_WIIMOTES; i++){
      handleWiimotesEvent(wiimotes[i]);
    }
}

/*void WiimoteManager::updateWiimoteAccel(wiimote * wm){
    if(WIIUSE_USING_ACC(wm)) {
        //On a besoin d'un objet ffs
        
    }
}*/


