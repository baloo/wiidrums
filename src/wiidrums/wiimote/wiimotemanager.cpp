
#include "wiimotemanager.h"

using std::cout; 
using namespace std;

//Constructeur
WiimoteManager::WiimoteManager(){
    
    //On instancie notre gestionnaire de wiimotes
    wiimotes = new Wiimotes(MAX_WIIMOTES);
    connect(wiimotes, SIGNAL(wiimoteEvent(int, int)), this, SLOT(handleEventSignals(int,int)));


    //Au démarrage on est en mode d'initialisation
    initMode = true;

    eventTimer = new QTimer(this);
    connect(eventTimer, SIGNAL(timeout()), this, SLOT(readWiimoteAccel()));
    timeInterval = 2000;
}

//Destructeur
WiimoteManager::~WiimoteManager(){

    delete wiimotes;
}

//Méthode de connexion des wiimotes
bool WiimoteManager::connectWiimotes(int timeout){
    int found, connected;

    //On lance la recherche des wiimotes pendant timeout secondes
    found = wiimotes->find(MAX_WIIMOTES, timeout);
    if (!found) {
      cout << "No wiimotes found.\n";
      emit failedToConnect();
      return false;
    }

    //Si on a trouvé des wiimotes alors on lance la connexion
    connected = wiimotes->connect();
    if (connected){
      cout << "Connected to " << connected << " wiimotes (of "<< found << " found).\n";
      emit wiimoteConnected(connected, found);
    } else {
      cout << "Failed to connect to any wiimote.\n";
      emit failedToConnect();
      return false;
    }
    
    return true;
}

//On nettoye le tableau des wiimotes
void WiimoteManager::disconnectWiimotes(){
    //On lance la déconnexion sur l'ensemble de wiimotes
    wiimotes->disconnectWiimotes();

    eventTimer->stop();
}

//On active l'IR d'une wiimote
bool WiimoteManager::enableIRWiimote(int numWiimote){

    return wiimotes->enableIRWiimote(numWiimote);
}

//On déactive l'IR d'une wiimote
bool WiimoteManager::disableIRWiimote(int numWiimote){

    return wiimotes->disableIRWiimote(numWiimote);
}

//Changer la sensibilité des wiimotes
void WiimoteManager::changeIRSensitivity(int numWiimote, int sensitivity){
    if(wiimotes->changeIRSensitivity(numWiimote, sensitivity))
        emit wiimoteIRSensitivityChange(numWiimote, sensitivity);
}

//Déactiver l'accéléromètre
bool WiimoteManager::disableAccelWiimote(int numWiimote){

    return wiimotes->disableAccelWiimote(numWiimote);
}

//Réactiver l'acéléromètre
bool WiimoteManager::enableAccelWiimote(int numWiimote){

    return wiimotes->enableAccelWiimote(numWiimote);
}

//On définit les LEDS en fonction de la position de la wiimote
void WiimoteManager::setLedsByDefault(){
    wiimotes->setLedsByDefault();
}

//On éteint les LEDS de toute wiimote connectée
void WiimoteManager::noLedAnymore(){
    wiimotes->noLedAnymore();
}

//Prise en charge des évènements de la wiimote
void WiimoteManager::handleWiimotesEvent(Wiimote *wm){

  // Le bouton - désactive les accéléromètres
  if (IS_JUST_PRESSED(wm->getStruct(), WIIMOTE_BUTTON_MINUS)){
    wiiuse_motion_sensing(wm->getStruct(), 0);
  }

  // Le bouton + active les accéléromètres
  if (IS_JUST_PRESSED(wm->getStruct(), WIIMOTE_BUTTON_PLUS)){
    wiiuse_motion_sensing(wm->getStruct(), 1);
    wiiuse_set_orient_threshold(wm->getStruct(), 5);
  }

  // Si l'accéléromètre est activé on affiche 
  if (WIIUSE_USING_ACC(wm->getStruct())) {
        //printf("wiimote roll  = %f [%f]\n", wm->orient.roll, wm->orient.a_roll);
        //printf("wiimote pitch = %f [%f]\n", wm->orient.pitch, wm->orient.a_pitch);
        //printf("wiimote yaw   = %f\n", wm->orient.yaw);

        qDebug() << "wiimote x  = "<<  wm->getAccel().x;
        qDebug() << "wiimote y  = "<<  wm->getAccel().y;
        qDebug() << "wiimote z  = "<<  wm->getAccel().z;
        qDebug() << "wiimote roll  = "<<  wm->getOrient().roll <<" [" << wm->getOrient().a_roll << "]";
        qDebug() << "wiimote pitch = "<<  wm->getOrient().pitch <<" [" << wm->getOrient().a_pitch << "]";
        qDebug() << "wiimote yaw   = "<<  wm->getOrient().yaw << endl;       //yaw n'est pas implémenté sous wiuse. La valeur reste donc à 0

        //Si le pitch est entre 30 et 60 alors la wiimote est en position vers le bas
        //On lance alors l'émission du son
        if(wm->getOrient().pitch > 30 && wm->getOrient().pitch < 60){
            emit playSimpleSong();

            qDebug() << "WiimoteManager>> PlaySimpleSong";
        }
  }

}

//Prise en charge de la déconnexion des Wiimotes
void WiimoteManager::handleDisconnect(Wiimote * wm) {
    qDebug() << "\n\n--- DISCONNECTED [wiimote id "<< wm->getUnid() << "] ---\n";

    //emit wiimoteDisconnected((wm->unid)-1);
    wm->disconnected();
    nbWiimoteConnected--;
}

//Exécution de la boucle d'évènements
void WiimoteManager::exec(){

    eventTimer->start(timeInterval);

    //On lance la boucle d'évènements dans un thread séparé
    QtConcurrent::run(wiimotes,&Wiimotes::exec);
}

//Gestion des évènements envoyés par la boucle d'évènements
void WiimoteManager::handleEventSignals(int numWiimote, int event){
    try{
        switch (event) {
            case WIIUSE_EVENT:
              this->handleWiimotesEvent(wiimotes->getWiimotes()->at(numWiimote));
              break;
            case WIIUSE_STATUS:
              //gérer le status
              break;
            case WIIUSE_DISCONNECT:
            case WIIUSE_UNEXPECTED_DISCONNECT:
              //Gérer la déconnexion
              handleDisconnect(wiimotes->getWiimotes()->at(numWiimote));
              break;
            default:
              break;
        }
    }catch(...)
    {
            // On a essaye de recuperer une wiimote non initialisee

    }

}

void WiimoteManager::readWiimoteAccel(){
    for(int i=0; i<wiimotes->getWiimotes()->size(); i++){
        try{
            handleWiimotesEvent(wiimotes->getWiimotes()->at(i));
        }catch(...)
        {
            // On a essaye de recuperer une wiimote non initialisee
        }
    }
}

/*void WiimoteManager::updateWiimoteAccel(wiimote * wm){
    if(WIIUSE_USING_ACC(wm)) {
        //On a besoin d'un objet ffs
        
    }
}*/



