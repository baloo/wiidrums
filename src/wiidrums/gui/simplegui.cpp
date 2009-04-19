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

#include "SimpleGUI.h"

//Constructeur
SimpleGUI::SimpleGUI() : QWidget() {
	//On crée le gestionnaire de Wiimotes
	manager = new WiimoteManager;

	//Connexions avec le manager
	connect(manager, SIGNAL(alarmModeEnable(bool)), this, SLOT(switchInfoMode(bool)));
	connect(manager, SIGNAL(failedToConnect()), this, SLOT(failedToConnectMessage()));
	connect(manager, SIGNAL(wiimoteConnected(int, int)), this, SLOT(handleWiimoteConnected(int, int)));
	
	connect(manager, SIGNAL(alarmDeclenched(int, int)), this, SLOT(handleAlarmDeclenched(int, int)));


	//On crée les menus de la fenêtre
	this->creerMenus();

	//Construction de la zone de status
	statusLayout = new QHBoxLayout;
	globalStatus = new QLabel(QString::fromUtf8("<font color=\"green\" style=\"font-size:25px;\">Arrêt</font>"));
	statusLayout->addWidget(new QLabel("<font  style=\"font-size:25px; font-style:italic;\">Statut :</font>"));;
	statusLayout->addWidget(globalStatus);
	//statusLayout->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

	//Construction de la zone de gestion des wiimote
	QGroupBox *groupBox = new QGroupBox(tr("Gestion des Wiimotes :"));
	infoModeLabel = new QLabel(QString::fromUtf8("Aucune wiimote connectée..."));

	wiimoteLayout = new QVBoxLayout;
	groupBox->setLayout(wiimoteLayout);

	mainLayout = new QGridLayout;
	mainLayout->setMenuBar(menuBar);
	mainLayout->addLayout(statusLayout, 0, 0);
	mainLayout->addWidget(infoModeLabel, 1, 0);
	mainLayout->addWidget(groupBox,2, 0);
	mainLayout->setRowMinimumHeight(2, 300);
	mainLayout->setColumnMinimumWidth(0, 450);

	this->setLayout(mainLayout);
}

//Destructeur
SimpleGUI::~SimpleGUI(){
	delete manager;

	delete mainLayout;
	delete wiimoteLayout;
	delete statusLayout;

	delete infoModeLabel;
	delete globalStatus;

	delete menuBar;
	delete menuFichier;
	delete menuWiimote;
	delete menuAlarme;
	delete menuAide;

	delete aboutAction;
	delete quitAction;
	delete wiimoteConnexion;
	delete wiimoteDeconnexion;
	delete wiimoteAlarmMode;
}

void SimpleGUI::switchInfoMode(bool isAlarmEnabled){
	if(isAlarmEnabled){
	    infoModeLabel->setText(QString::fromUtf8("Alarme Activée"));
	    globalStatus->setText(QString::fromUtf8("<font color=\"green\" style=\"font-size:25px;\">OK</font>"));
	} else {
	    infoModeLabel->setText("Positionnement des Wiimotes");
	    globalStatus->setText(QString::fromUtf8("<font color=\"orange\" style=\"font-size:25px;\">Initialisation</font>"));
	}
}

//Gestion de l'affichage des wiimotes
void SimpleGUI::handleWiimoteConnected(int nbWiimotesConnected, int nbWiimotesFound){

    infoModeLabel->setText("Positionnement des Wiimotes");
	globalStatus->setText(QString::fromUtf8("<font color=\"orange\" style=\"font-size:25px;\">Initialisation</font>"));

    //On active les fonctions
    wiimoteConnexion->setEnabled(false);
    wiimoteDeconnexion->setEnabled(true);
    wiimoteAlarmMode->setEnabled(true);

    for(int i=1;i<=nbWiimotesConnected; i++){
 	wiimoteLayout->addWidget(new WiimoteView(manager, i));
    }
}

//Gestion de la déconnexion
void SimpleGUI::handleDisconnect(){
    manager->disconnectWiimotes();

    //On déactive le mode alarme
    wiimoteAlarmMode->setChecked(false);
    wiimoteAlarmMode->setEnabled(false);
    wiimoteDeconnexion->setEnabled(false);
    wiimoteConnexion->setEnabled(true);

    globalStatus->setText(QString::fromUtf8("<font color=\"green\" style=\"font-size:25px;\">Arrêt</font>"));

    infoModeLabel->setText(QString::fromUtf8("Aucune wiimote connectée..."));

    //On déactive l'accès aux wiimotes
    ;
}


//Gestion du déclenchement de l'alarme
void SimpleGUI::handleAlarmDeclenched(int numWiimote, int numDot){
    globalStatus->setText(QString::fromUtf8("<font color=\"red\" style=\"font-size:25px;\">Alarme déclenchée !</font>"));

    QString info = QString::fromUtf8("L'alarme à été déclenchée sur la Wiimote ");
    info.append(QString::number(numWiimote));
    info.append(", au point ");
    info.append(QString::number(numDot));

    infoModeLabel->setText(info);
}


void SimpleGUI::failedToConnectMessage(){
	infoModeLabel->setText(QString::fromUtf8("Aucune wiimote connectée..."));

	QMessageBox::critical(this, tr("Erreur de connexion"), QString::fromUtf8("Aucune wiimote n'a été trouvée. Veuillez réessayer ou relancez l'application puis retentez une connexion."));
}

void SimpleGUI::aboutDialog(){

	//On affiche une boite de dialogue aPropos
	QMessageBox::about(this, tr("A propos de WiiGuard v0.7 Beta..."),
             QString::fromUtf8("On a tous besoin un peu de securité chez soi... Pourquoi ne pas utiliser nos bonnes vieilles wiimotes lorsque l'on est absent\n\nWiiGuard sera le gardien en attendant votre retour...\n\n"
             "Auteurs :\n"
	     "    David BONNET\n"
	     "    Vincent GUYOMARD\n\n"
	     "Remerciements : WiiUse 0.12 et Qt 4.4.3"));
}

void SimpleGUI::lauchWiimoteConnexion(){
	QMessageBox::information(this, "Connexion des Wiimotes" ,QString::fromUtf8("Veuillez mettre les wiimotes en mode découverte (1+2) puis cliquez sur Ok"));

	infoModeLabel->setText(QString::fromUtf8("Connexion en cours..."));
	this->repaint();
	manager->connectWiimotes();

	QtConcurrent::run(manager,&WiimoteManager::exec);
}

void SimpleGUI::switchAlarmMode(bool isAlarmEnabled){
	if(isAlarmEnabled){
	    //Alors on lance le mode ALARME
	    manager->switchToAlarmMode();
	    manager->noLedAnymore();
	} else {
	    //On passe en mode INIT
	    manager->switchToInitMode();
	    manager->setLedsByDefault();
	}
}

//Méthode qui crée la barre de menus
void SimpleGUI::creerMenus(){

	//On crée les différents menus
	menuBar = new QMenuBar(0);
	menuFichier = menuBar->addMenu("Fichier");
	menuWiimote = menuBar->addMenu("Wiimote");
	menuAlarme = menuBar->addMenu("Alarme");
	menuAide = menuBar->addMenu("?");

	//On ajoute les actions associées
	quitAction = new QAction(tr("Quitter"), this);
	quitAction->setShortcut(tr("Alt+F4"));
	quitAction->setStatusTip(tr("Quitter l'application"));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(close() ));

	aboutAction = new QAction(tr("A Propos..."), this);
	aboutAction->setStatusTip(tr("A propos de l'application"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutDialog() ));

	wiimoteConnexion = new QAction(tr("Connexion des Wiimotes"), this);
	wiimoteConnexion->setShortcut(tr("Shift+C"));
	wiimoteConnexion->setStatusTip(tr("Lancer la connexion des wiimotes"));
	connect(wiimoteConnexion, SIGNAL(triggered()), this, SLOT(lauchWiimoteConnexion() ));

	wiimoteDeconnexion = new QAction(QString::fromUtf8("Déconnexion"), this);
	wiimoteDeconnexion->setShortcut(tr("Shift+D"));
	wiimoteDeconnexion->setStatusTip(QString::fromUtf8("Déconnecter les wiimotes"));
	wiimoteDeconnexion->setEnabled(false);
	connect(wiimoteDeconnexion, SIGNAL(triggered()), this, SLOT(handleDisconnect() ));

	wiimoteAlarmMode = new QAction(QString::fromUtf8("Démarrer ALARME"), this);
	wiimoteAlarmMode->setShortcut(tr("Shift+A"));
	wiimoteAlarmMode->setCheckable(true);
	wiimoteAlarmMode->setStatusTip(tr("Lancer l'alarme en cas d'intrusion"));
	wiimoteAlarmMode->setEnabled(false);
	connect(wiimoteAlarmMode, SIGNAL(toggled(bool)), this, SLOT(switchAlarmMode(bool) ));
	connect(manager, SIGNAL(alarmModeEnable(bool)), wiimoteAlarmMode, SLOT(setChecked(bool)));

	//On connecte le tout
	menuFichier->addAction(quitAction);
	menuWiimote->addAction(wiimoteConnexion);
	menuWiimote->addAction(wiimoteDeconnexion);
	menuAlarme->addAction(wiimoteAlarmMode);
	menuAide->addAction(aboutAction);
}

