

#include "wiimoteview.h"

//Constructeur
WiimoteView::WiimoteView(WiimoteManager *wm, int numWm){

	this->numWiimote = numWm;
	this->manager = wm;
    this->accelEnable = false;

	QString name = QString("Wiimote ");
	name.append(QString::number(numWiimote));
	wiimoteName = new QLabel(name);

    //Graphique d'informations
    //graph = new WiiPlot(this);

	//Gestion déconnexion
	connect(manager, SIGNAL(wiimoteDisconnected(int)), this, SLOT(handleDisconnect(int)));

	//Panneau de contôle
	controlLayout = new QHBoxLayout;
    accelButton = new QPushButton(QString::fromUtf8("Activer Accéléromètre"), this);
    controlLayout->addWidget(accelButton);

    //Contrôle de l'accéléromètre
    connect(accelButton, SIGNAL(clicked()), this, SLOT(handleAccelActivation()));

	mainLayout = new QGridLayout;
	mainLayout->addWidget(wiimoteName, 0, 0);
    mainLayout->addLayout(controlLayout, 0, 1);
    //mainLayout->addWidget(graph, 0, 1);
	//mainLayout->addWidget(wiimoteStatus, 0, 2);

	this->setLayout(mainLayout);
}

//Destructeur
WiimoteView::~WiimoteView(){

}

//Gestion de la déconnexion des wiimotes
void WiimoteView::handleDisconnect(int numWm){
	if(numWiimote == numWm){
	    //Alors on est concernés
	    this->~WiimoteView();
	}
}

//Gestion de l'activation de l'accéléromètre
void WiimoteView::handleAccelActivation(){
    if(!accelEnable){
        qDebug() << QString::fromUtf8("WiimoteView>> Acitvation de l'accéléromètre pour la wiimote ") << numWiimote;
        manager->enableAccelWiimote(numWiimote);
        accelButton->setText(QString::fromUtf8("Déactiver Accéléromètre"));
    } else {
        qDebug() << QString::fromUtf8("WiimoteView>> Désactivation de l'accéléromètre pour la wiimote ") << numWiimote;
        manager->disableAccelWiimote(numWiimote);
        accelButton->setText(QString::fromUtf8("Activer Accéléromètre"));
    }

    accelEnable = !accelEnable;
}

