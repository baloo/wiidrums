
#include "simplegui.h"

//Constructeur
SimpleGUI::SimpleGUI() : QWidget() {
    //On crée le gestionnaire de Wiimotes
    manager = new WiimoteManager;
    genericAudioObject = new GenericAudioObject(Phonon::MusicCategory);
    //genericAudioObject->setCurrentSource( Phonon::MediaSource("../../../res/test.wav") );
    genericAudioObject->setCurrentSource( Phonon::MediaSource("../../../res/bruitblanc.wav") );
    //genericAudioObject->setCurrentSource( Phonon::MediaSource("../../../res/hiTom.wav") );
    
        //Connexions avec le manager
    connect(manager, SIGNAL(failedToConnect()), this, SLOT(failedToConnectMessage()));
    connect(manager, SIGNAL(wiimoteConnected(int, int)), this, SLOT(handleWiimoteConnected(int, int)));


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
    delete menuAide;

    delete aboutAction;
    delete quitAction;
    delete wiimoteConnexion;
    delete wiimoteDeconnexion;

    delete genericAudioObject;
}

//Gestion de l'affichage des wiimotes
void SimpleGUI::handleWiimoteConnected(int nbWiimotesConnected, int nbWiimotesFound){

    infoModeLabel->setText("Positionnement des Wiimotes");
    globalStatus->setText(QString::fromUtf8("<font color=\"orange\" style=\"font-size:25px;\">Initialisation</font>"));

    //On active les fonctions
    wiimoteConnexion->setEnabled(false);
    wiimoteDeconnexion->setEnabled(true);

    for(int i=1;i<=nbWiimotesConnected; i++){
      wiimoteLayout->addWidget(new WiimoteView(manager, i));
    }
}

//Gestion de la déconnexion
void SimpleGUI::handleDisconnect(){
    manager->disconnectWiimotes();

    wiimoteDeconnexion->setEnabled(false);
    wiimoteConnexion->setEnabled(true);

    globalStatus->setText(QString::fromUtf8("<font color=\"green\" style=\"font-size:25px;\">Arrêt</font>"));

    infoModeLabel->setText(QString::fromUtf8("Aucune wiimote connectée..."));

}

void SimpleGUI::failedToConnectMessage(){
    infoModeLabel->setText(QString::fromUtf8("Aucune wiimote connectée..."));

    QMessageBox::critical(this, tr("Erreur de connexion"), QString::fromUtf8("Aucune wiimote n'a été trouvée. Veuillez réessayer ou relancez l'application puis retentez une connexion."));
}

void SimpleGUI::aboutDialog(){

    //On affiche une boite de dialogue aPropos
    QMessageBox::about(this, tr("A propos de WiiDrums v0.0 Beta..."),
             QString::fromUtf8("Vous avez toujours rêvé d'une batterie, mais vous avez toujours eu peur de la réaction de vos voisins... Désormais avec WiiDrums utilisez vos Wiimotes pour jouer de la batterie sur votre bon vieux PC"
             "Auteurs :\n"
         "    Artur GAUTIER\n"
         "    David BONNET\n\n"
         "Remerciements : WiiGuard, WiiUse 0.12 et Qt 4.4.3"));
}

void SimpleGUI::launchWiimoteConnexion(){
    QMessageBox::information(this, "Connexion des Wiimotes" ,QString::fromUtf8("Veuillez mettre les wiimotes en mode découverte (1+2) puis cliquez sur Ok"));

    infoModeLabel->setText(QString::fromUtf8("Connexion en cours..."));
    this->repaint();
    manager->connectWiimotes();

    QtConcurrent::run(manager,&WiimoteManager::exec);
}

//Méthode qui crée la barre de menus
void SimpleGUI::creerMenus(){

    //On crée les différents menus
    menuBar = new QMenuBar(0);
    menuFichier = menuBar->addMenu("Fichier");
    menuWiimote = menuBar->addMenu("Wiimote");
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
    connect(wiimoteConnexion, SIGNAL(triggered()), this, SLOT(launchWiimoteConnexion() ));

    wiimoteDeconnexion = new QAction(QString::fromUtf8("Déconnexion"), this);
    wiimoteDeconnexion->setShortcut(tr("Shift+D"));
    wiimoteDeconnexion->setStatusTip(QString::fromUtf8("Déconnecter les wiimotes"));
    wiimoteDeconnexion->setEnabled(false);
    connect(wiimoteDeconnexion, SIGNAL(triggered()), this, SLOT(handleDisconnect() ));

    testSon = new QAction(QString::fromUtf8("Test son"), this);
    testSon->setShortcut(tr("Shift+S"));
    testSon->setStatusTip(QString::fromUtf8("Tester le son"));
    testSon->setEnabled(true);
    connect(testSon, SIGNAL(triggered()), genericAudioObject, SLOT(play() ));

    //On connecte le tout
    menuFichier->addAction(quitAction);
    menuWiimote->addAction(wiimoteConnexion);
    menuWiimote->addAction(wiimoteDeconnexion);
    menuAide->addAction(aboutAction);
    menuAide->addAction(testSon);
}

