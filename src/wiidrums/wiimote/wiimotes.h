// vim: ts=4 sw=4 expandtab
/**
 * @file
 *
 * Classe qui sur-couche WiiUse sur la gestion de plusieurs wiimotes
 *
 * @author David Bonnet pour Polytech'WiiDrums Team
 * @date 19/04/2009 16:11
 * @since 14/05/2009 17:18
 * @version 0.0
 */

#ifndef DEF_WIIMOTES
#define DEF_WIIMOTES

#ifndef WIN32
    #include <unistd.h>
#endif

#include <QObject>
#include <QVector>
#include <QDebug>
#include <wiiuse.h>
#include "wiimote.h"


using namespace std;

class Wiimotes : public QObject
{
  Q_OBJECT

  public:
    Wiimotes(int wiimotes);
    ~Wiimotes();

    void cleanup();
    struct wiimote_t * getById(int unid);
    void setBluetoothStack(enum win_bt_stack_t type);

    /**
      Définir les LEDS des wiimotes par défaut
     */
    void setLedsByDefault();

    /**
      Extinction de toutes les LEDS des wiimotes
     */
    void noLedAnymore();

    /**
     Méthode de recherche des wiimotes

     @param max_wiimotes - Le nombre maximal de wiimotes à rechercher
     @param timeout - Le temps maximal en secondes
     */
    int find(int max_wiimotes, int timeout);

    /**
      Fonction de connexion des wiimotes
     */
    int connect();

    /**
      Méthode qui déconnecte toutes les wiimotes
     */
    void disconnectWiimotes();

    /**
      Fonction pour activer l'IR d'une wiimote
     */
    bool enableIRWiimote(int numWiimote);

    /**
      Fonction pour désactiver l'IR d'une Wiimote
     */
    bool disableIRWiimote(int numWiimote);

    /**
      Changer la sensibilité de l'IR pour les wiimotes
     */
    bool changeIRSensitivity(int numWiimote, int sensitivity);

    int poll();

    /**
      Boucle d'évènements sur les wiimotes
     */
    void exec();

    /**
      Getter du tableau de wiimotes
     */
    QVector<Wiimote *> * getWiimotes();

    /**
      Getter de la structure de wiimotes
     */
    wiimote_t** getWiiuseWiimotes();
  
  signals:
    
    /**
      Signal envoyé lors du changement de sensibilité de la wiimote
     */
    void wiimoteIRSensitivityChange(int numWiimote, int sensitivity);

    /**
     Signal envoyé lorsqu'un évènement se produit
     */
    void wiimoteEvent(int numWiimote, int event);

  protected:
    QVector<Wiimote *> * wiimotesList;
    int wiimotes;
    int nbWiimoteConnected;

    struct wiimote_t ** wms;

};





#endif

