

#include "wiimoteview.h"

//Constructeur
WiimoteView::WiimoteView(WiimoteManager *wm, int numWm){

	this->numWiimote = numWm;
	this->manager = wm;

	QString name = QString("Wiimote ");
	name.append(QString::number(numWiimote));
	wiimoteName = new QLabel(name);

	//Gestion déconnexion
	connect(manager, SIGNAL(wiimoteDisconnected(int)), this, SLOT(handleDisconnect(int)));

	//Panneau de contôle
	controlLayout = new QHBoxLayout;

	mainLayout = new QGridLayout;
	mainLayout->addWidget(wiimoteName, 0, 0);
	//mainLayout->addWidget(status, 0, 1);
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


