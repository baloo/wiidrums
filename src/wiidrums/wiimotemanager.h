/*
 * vim: ts=4 sw=4 expandtab:
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

/**
 * @file
 *
 * Gestionnaire de wiimotes pour l'application WiiGuard
 * Ce gestionnaire vérifie pour chaque wiimote l'état des points IR
 * et reporte les éventuels points qui disparaissent
 * 
 * La wiimote est matériellement limitée à 4 points IR détectés
 *
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 13/03/2009 14:19
 * @since 17/03/2009 13:50
 * @version 1.0
 */

#ifndef DEF_WIIMOTEMANAGER
#define DEF_WIIMOTEMANAGER

#define MAX_WIIMOTES 4  //4 wiimotes maximum

#include <QtCore>
#include <iostream>

#ifndef WIN32
    #include <unistd.h>
#endif

#include "include/wiiuse.h"

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

    private:

    /**
     * Méthode qui vérifie l'état des points IR avec la configuration actuelle
     *
     * @param wm Wiimote
     * @param numIRDot numéro du point IR
     */
    bool diffBetweenOriginalIRDotsAnd(wiimote* wm, int numIRDot);

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
     * Slot qui permet de switcher en mode d'initialisation
     */
    void switchToInitMode();

    /**
     * Slot qui permet de switcher en mode alarme
     */
    void switchToAlarmMode();

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
     * Signal envoyé pour chaque point IR d'une wiimote connectée
     */
    void wiimoteIRDotDetected(int numWiimote, int numDot, bool visible, int x, int y);

    /**
     * Signal envoyé lors du déclenchement de l'alarme
     */
    void alarmDeclenched(int numWiimote, int numDot);

    /**
     * Signal envoyé lors du passage d'un mode à l'autre
     */
    void alarmModeEnable(bool isAlarmEnabled);

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

