/**
 * @file
 *
 * Gestionnaire de wiimotes pour l'application wiidrums
 *
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 19/04/2009 16:11
 * @since 19/04/2009 13:50
 * @version 0.0
 */

#ifndef DEF_WIIMOTEMANAGER
#define DEF_WIIMOTEMANAGER

#define MAX_WIIMOTES 2  //2 wiimotes maximum

#include <QtCore>
#include <iostream>

#ifndef WIN32
    #include <unistd.h>
#endif

#include <wiiuse.h>

using namespace std;

class WiimoteManager : public QObject {

    Q_OBJECT

    public:
    /**
     * Constructeur du WiimoteManager
     */
    WiimoteManager();

    /**
     * Destructeur du WiimoteManager
     */
    ~WiimoteManager();

    /**
     * Méthode qui permet de connecter une ou plusieurs Wiimotes au WiimoteManager
     * /!\ ATTENTION /!\
     * Si des Wiimotes étaient déjà connectées elles devrons être resynchronisées
     *
     * @param timeout Le temps de recherche des wiimotes en secondes (5s par défaut)
     */
    bool connectWiimotes(int timeout = 5);

    /**
     * Méthode qui déconnecte l'ensemble des Wiimotes
     */
    void disconnectWiimotes();

    /**
     * Méthode qui allume les LEDS des wiimotes en fonction de leur position
     */
    void setLedsByDefault();

    /**
     * Méthode qui éteint les LEDS des wiimotes
     */
    void noLedAnymore();

    /**
     * Méthode qui prend en charge les évènements des Wiimotes
     *
     * @param wiimote La wiimote à prendre en charge
     */
    void handleWiimotesEvent(wiimote *wm);

    /**
     * Méthode qui prend en charge la déconnexion des wiimotes
     *
     * @param wiimote La wiimote à prendre en charge
     */
    void handleDisconnect(wiimote* wm);

    /**
     * Méthode qui lance la boucle d'évènements sur les wiimotes
     */
    void exec();

    signals:
    void timerEvent(QTimerEvent *e);

    protected slots:
    void readWiimoteAccel(QTimerEvent *e);

    private:

    public slots:
    /**
     * Slot qui permet d'activer l'IR d'une wiimote
     *
     * @param numWiimote - ATTENTION le numéro commence à 1
     */
    bool enableIRWiimote(int numWiimote = 1);

    /**
     * Slot qui permet d'activer l'IR d'une wiimote
     *
     * @param numWiimote - ATTENTION le numéro commence à 1
     */
    bool disableIRWiimote(int numWiimote = 1);

    /**
     * Slot qui permet de changer la sensibilité d'une wiimote
     *
     * @param sensitivity La sensibilité de l'IR de la wiimote
     */
    void changeIRSensitivity(int numWiimote, int sensitivity);

    /**
     * Slot qui gère les évènements masqués
     */
    void timeOutHidenEvent();

    signals:
    /**
     * Signal envoyé lors d'une connexion réussie avec les wiimotes
     */
    void wiimoteConnected(int nbWiimotesConnected, int nbWiimotesFound);

    /**
     * Signal envoyé lors de la déconnexion d'une wiimote
     *
     * @param numWiimote
     */
    void wiimoteDisconnected(int numWiimote);

    /**
     * Signal envoyé lors d'un échec de connexion avec les wiimotes
     */
    void failedToConnect();

    /**
     * Signal envoyé lors du changement de la sensibilité IR de la wiimote
     */
    void wiimoteIRSensitivityChange(int numWiimote, int sensitivity);

    private:
    //Tableau contenant les objets Wiimote
    wiimote **wiimotes;

    //Nombre de wiimotes connectées à l'instant T
    int nbWiimoteConnected;

    //Le mode init permet de placer les wiimotes
    bool initMode;

    /*Champs d'enregistrement des informations que la position des points IR des Wiimotes*/

    //Tableau contenant les structures de points IR selon le numéro de wiimote
    int wiimotesIRDots[MAX_WIIMOTES][4];
    //QVector<QVector<bool> > wiimotesIRDots;

    //Timer event
    QTimer *eventTimer;
    int timeInterval;
};

#endif

