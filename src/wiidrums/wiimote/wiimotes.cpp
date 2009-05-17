// vim: ts=4 sw=4 expandtab:


#include "wiimotes.h"


Wiimotes::Wiimotes(int wiimotes)
{
  this->wiimotesList = new QVector<Wiimote *>(wiimotes, (Wiimote *) NULL);
  this->wiimotes = wiimotes;

  nbWiimoteConnected = 0;

  /*
     * On initialise le tableau avec le nombre maximum de wiimotes utilisables
     * Même si une seule wiimote est connectée, les autres ne seront alors pas utilisées
     */ 
  this->wms = wiiuse_init(this->wiimotes);
}

Wiimotes::~Wiimotes()
{
    delete(this->wiimotesList);

    //On nettoie les structures des wiimotes
    this->cleanup();
}

void Wiimotes::cleanup()
{
    wiiuse_cleanup(this->wms, this->wiimotes);
}

struct wiimote_t * Wiimotes::getById(int unid)
{
    return wiiuse_get_by_id(this->wms, this->wiimotes, unid);
}

void Wiimotes::setBluetoothStack(enum win_bt_stack_t type)
{
    wiiuse_set_bluetooth_stack(this->wms, this->wiimotes, type);
}

//Méthode de recherche des wiimotes
int Wiimotes::find(int max_wiimotes, int timeout)
{
    return wiiuse_find(this->wms, max_wiimotes, timeout);
}

//Connexion des wiimotes
int Wiimotes::connect()
{
    int myWiimotes;
    myWiimotes = wiiuse_connect(this->wms, this->wiimotes);

    this->wiimotesList->clear();

    for(int i=0; i < myWiimotes; i++)
    {
        this->wiimotesList->push_back(new Wiimote(this->wms[i]));
    }

    //On enregistre le nombre de wiimotes connectées
    nbWiimoteConnected = myWiimotes;

    //On définit la sensibilité de l'IR à 3 par défaut
    for(int i=0; i<this->wiimotes; i++){
      wiiuse_set_ir_sensitivity(this->wms[i], 3);
      emit wiimoteIRSensitivityChange(i, 3);
    }

    //On règle les LEDS des wiimotes par défaut
    this->setLedsByDefault();

    return myWiimotes;
}

//Déconnexion de toutes les wiimotes
void Wiimotes::disconnectWiimotes(){
    for(int i=0; i<this->wiimotes; i++)
        this->wiimotesList->value(i)->disconnect();

    nbWiimoteConnected = 0;
}

//On définit les LEDS en fonction de la position de la wiimote
void Wiimotes::setLedsByDefault(){
    //La wiimote de la main gauche est identifiée par les deux LEDS de gauche
    wiiuse_set_leds(this->wms[0], WIIMOTE_LED_1 | WIIMOTE_LED_2);
    
    //La wiimote de la main droite est identifiée par les deux LEDS de droite
    wiiuse_set_leds(this->wms[1], WIIMOTE_LED_3 | WIIMOTE_LED_4);
}

//On éteint les LEDS de toute wiimote connectée
void Wiimotes::noLedAnymore(){

    for(int i=0; i<this->wiimotes; i++){
      wiiuse_set_leds(this->wms[i], WIIMOTE_LED_NONE);
    }
}

//On active l'IR d'une wiimote
bool Wiimotes::enableIRWiimote(int numWiimote){

    if(numWiimote <= this->wiimotes){
      wiiuse_set_ir(this->wms[numWiimote], 1);
      return true;
    }

    return false;
}

//On déactive l'IR d'une wiimote
bool Wiimotes::disableIRWiimote(int numWiimote){
    if(numWiimote <= this->wiimotes){
      wiiuse_set_ir(this->wms[numWiimote], 0);
      return true;
    }

    return false;
}

//Changer la sensibilité des wiimotes
bool Wiimotes::changeIRSensitivity(int numWiimote, int sensitivity){
    wiiuse_set_ir_sensitivity(this->wms[numWiimote], sensitivity);

    if(sensitivity > 5) sensitivity = 5;
    if(sensitivity <= 0) sensitivity = 0;
    //printf("WiimoteManager>> Sensibilité IR de la wiimote %d à %d\n", numWiimote, sensitivity);
    return true;
}

//Déactiver l'accéléromètre
bool Wiimotes::disableAccelWiimote(int numWiimote){

    if(numWiimote <= this->wiimotes){
      wiiuse_motion_sensing(this->wms[numWiimote], 0);
      return true;
    }

    return false;
}

//Réactiver l'acéléromètre
bool Wiimotes::enableAccelWiimote(int numWiimote){

    if(numWiimote <= this->wiimotes){
      wiiuse_motion_sensing(this->wms[numWiimote], 1);
      wiiuse_set_orient_threshold(this->wms[numWiimote], 5);
      return true;
    }

    return false;
}

int Wiimotes::poll()
{
    return wiiuse_poll(this->wms, this->wiimotes);
}

//lance la boucle d'événements et emet un signal lorsqu'un évènement à lieu
void Wiimotes::exec(){

    qDebug() << "Main event loop" << endl;
    qDebug() << nbWiimoteConnected << endl;

    //On active le motion tracking
    // Temporairement l'id est fixe
    //wiiuse_motion_sensing(wiimotes[0], 1);
    //wiiuse_motion_sensing(wiimotes[1], 1);
    
    while(nbWiimoteConnected != 0){
    //cout << "NbWiimotes : " << nbWiimoteConnected << endl;
      //Wiiuse_pool vérifie les évènements de chaque wiimote, si un évènement à eu lieu, alors la fonction est déclenchée
      if(wiiuse_poll(this->wms, this->wiimotes)) {
          qDebug() << "Inside If" << endl;

          //Pour chaque wiimote
          for (int i=0; i < this->wiimotes; ++i) {
            qDebug() << "Inside for : i = " << i << endl;
            emit wiimoteEvent(i, this->wms[i]->event);
            
          }
      }
    }
}

QVector<Wiimote *> * Wiimotes::getWiimotes()
{
    return this->wiimotesList;
}

wiimote_t** Wiimotes::getWiiuseWiimotes()
{
    return this->wms;
}
