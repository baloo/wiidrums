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

/**
 * @file
 *
 * GUI simple pour WiiGuard. Cette GUI à pour ambition de simplifier
 * les actions pour l'utilisateur et de ne donner que les informations essentielles
 *
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 20/03/2009 19:02
 * @since 24/03/2009 21:50
 * @version 1.0
 */

#ifndef DEF_SIMPLEGUI
#define DEF_SIMPLEGUI

#include <QtGui>
#include <QtCore>
#include <QWidget>

#include "wiimote/WiimoteManager.h"
#include "WiimoteView.h"

class SimpleGUI : public QWidget {

      Q_OBJECT

      public:
	  /**
	   * Constructeur de la GUI simple - se charge également d'afficher la fenêtre
	   */
	  SimpleGUI();

	  /**
	   * Destructeur de la GUI Simple
	   */
	  ~SimpleGUI();

      protected slots:
	  void aboutDialog();
	  void lauchWiimoteConnexion();
	  
	  /**
	   * Slot qui va gérer l'affichage des wiimotes sur la GUI
	   */
	  void handleWiimoteConnected(int nbWiimotesConnected, int nbWiimotesFound);
	  
	  /**
	   * Slot qui permet de déconnecter les wiimotes
	   */
	  void handleDisconnect();

	  /**
	   * Slot qui affiche un message en cas d'erreur de connexion
	   */
	  void failedToConnectMessage();

	  /**
	   * Slot qui gère l'activation de l'alarme
	   *
	   * @param isAlarmEnabled menu de l'alarme coché
	   */
	  void switchAlarmMode(bool isAlarmEnabled);

	  /**
	   * Slot qui gère l'affichage des infos en fonction de l'activation de l'alarme ou non
	   *
	   * @param isAlarmEnabled menu de l'alarme coché
	   */
	  void switchInfoMode(bool isAlarmEnabled);

	  /**
	   * Slot qui gère le déclenchement de l'alarme au niveau global
	   *
	   * @param numWiimote le numéro de la wiimote
	   * @param numDot le point qui à déclenché l'alarme
	   */
	  void handleAlarmDeclenched(int numWiimote, int numDot);

      private:
	  //Concernant la gestion des Wiimotes
	  WiimoteManager *manager;


	  /**
	   * Méthode qui crée les menus de la fenêtre principale
	   */
	  void creerMenus();

	  //Layout de la fenêtre
	  QGridLayout *mainLayout;
	  QVBoxLayout *wiimoteLayout;
	  QHBoxLayout *statusLayout;

	  //Label d'informations
	  QLabel *infoModeLabel;
	  QLabel *globalStatus;

	  //Pour les menus
	  QMenuBar *menuBar;

	  QMenu *menuFichier;
	  QMenu *menuWiimote;
	  QMenu *menuAlarme;
	  QMenu *menuAide;

	  QAction *aboutAction;
	  QAction *quitAction;
	  QAction *wiimoteConnexion;
	  QAction *wiimoteDeconnexion;
	  QAction *wiimoteAlarmMode;

};

#endif

