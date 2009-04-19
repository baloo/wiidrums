
/**
 * @file
 *
 * GUI simple pour WiiGuard. Cette GUI à pour ambition de simplifier
 * les actions pour l'utilisateur et de ne donner que les informations essentielles
 *
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 19/04/2009
 * @since 19/04/2009
 * @version 0.0
 */

#ifndef DEF_SIMPLEGUI
#define DEF_SIMPLEGUI

#include <QtGui>
#include <QtCore>
#include <QWidget>

#include "wiimotemanager.h"
#include "gui/wiimoteview.h"

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
        QMenu *menuAide;

        QAction *aboutAction;
        QAction *quitAction;
        QAction *wiimoteConnexion;
        QAction *wiimoteDeconnexion;

};

#endif

