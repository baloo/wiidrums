

#include "wiimoteview.h"

//Constructeur
WiimoteView::WiimoteView(WiimoteManager *wm, int numWm){
	alarmDelenched = false;

	this->numWiimote = numWm;
	this->manager = wm;

	QString name = QString("Wiimote ");
	name.append(QString::number(numWiimote));
	wiimoteName = new QLabel(name);

	QLabel *status = new QLabel("Statut :");

	wiimoteStatus = new QLabel("<font color=\"orange\">Initialisation</font>");

	//Gestion des points IR
	connect(manager, SIGNAL(wiimoteIRDotDetected(int, int, bool, int, int)), this, SLOT(handleIRDotDetected(int, int, bool, int, int)));

	//Gestion sensibilité IR
	connect(manager, SIGNAL(wiimoteIRSensitivityChange(int, int)), this, SLOT(handleIRSensitivityChange(int, int)));

	//Gestion déconnexion
	connect(manager, SIGNAL(wiimoteDisconnected(int)), this, SLOT(handleDisconnect(int)));

	//Panneau de status
	statusLayout = new QHBoxLayout;
	irStatus = new QLabel("<font color=\"red\">IR OFF</font>");

	irSensitivity = new QLabel("3");

	dot1 = new QLabel("<font color=\"gray\">1</font>");
	dot2 = new QLabel("<font color=\"gray\">2</font>");
	dot3 = new QLabel("<font color=\"gray\">3</font>");
	dot4 = new QLabel("<font color=\"gray\">4</font>");
	statusLayout->addWidget(irStatus);
	statusLayout->addWidget(new QLabel(QString::fromUtf8("Sensibilité IR :")));
	statusLayout->addWidget(irSensitivity);
	statusLayout->addWidget(new QLabel("Points IR :"));
	statusLayout->addWidget(dot1);
	statusLayout->addWidget(dot2);
	statusLayout->addWidget(dot3);
	statusLayout->addWidget(dot4);

	//Panneau de contôle
	controlLayout = new QHBoxLayout;

	IRButton = new QPushButton("Activer IR");
	irEnable = false;
	connect(IRButton, SIGNAL(released()), this, SLOT(handleIRActivation()));
	controlLayout->addWidget(IRButton);

	irSensitivitySlider = new QSlider(Qt::Horizontal, this);
	irSensitivitySlider->setRange(1, 5);
	controlLayout->addWidget(irSensitivitySlider);
	irSensitivitySlider->setEnabled(false);
	connect(irSensitivitySlider, SIGNAL(valueChanged(int)), this, SLOT(handleSliderIRSensitivity(int)));

	mainLayout = new QGridLayout;
	mainLayout->addWidget(wiimoteName, 0, 0);
	mainLayout->addWidget(status, 0, 1);
	mainLayout->addWidget(wiimoteStatus, 0, 2);
	mainLayout->addLayout(statusLayout, 1, 0, 1, 3);
	mainLayout->addLayout(controlLayout, 2, 0, 1, 3);

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

//Gestion de l'activation de l'IR des wiimotes
void WiimoteView::handleIRActivation(){
    if(!irEnable){
	manager->enableIRWiimote(numWiimote);
	IRButton->setText(QString::fromUtf8("Déactiver IR"));
	irStatus->setText("<font color=\"green\">IR ON</font>");
	irSensitivitySlider->setEnabled(true);
    } else {
	manager->disableIRWiimote(numWiimote);
	IRButton->setText("Activer IR");
	irStatus->setText("<font color=\"red\">IR OFF</font>");
	irSensitivitySlider->setEnabled(false);
    }

    irEnable = !irEnable;
}

//Gestion des points IR
void WiimoteView::handleIRDotDetected(int numWm, int numDot, bool visible, int x, int y){
	
	if((numWiimote == numWm) && !alarmDelenched){
	    //Alors nous sommes concernés
	    switch(numDot){
		case 1:
		    if(visible)
		      dot1->setText("<font color=\"green\">1</font>");
		    else
		      dot1->setText("<font color=\"gray\">1</font>");
		    break;
		case 2:
		    if(visible)
		      dot2->setText("<font color=\"green\">2</font>");
		    else
		      dot2->setText("<font color=\"gray\">2</font>");
		    break;
		case 3:
		    if(visible)
		      dot3->setText("<font color=\"green\">3</font>");
		    else
		      dot3->setText("<font color=\"gray\">3</font>");
		    break;
		case 4:
		    if(visible)
		      dot4->setText("<font color=\"green\">4</font>");
		    else
		      dot4->setText("<font color=\"gray\">4</font>");
		    break;
		default:
		    break;
	    }
	}
}

//Gestion du changement de sensibilité de la wiimote
void WiimoteView::handleIRSensitivityChange(int numWm, int sensitivity){

	if(numWiimote == numWm){
	    //Alors on est concernés
	    irSensitivity->setText(QString::number(sensitivity));
	    irSensitivitySlider->setSliderPosition(sensitivity);
	    //printf("WiimoteView>> Sensibilité IR de la wiimote %d à %d", numWm, sensitivity);
	}
}

//Gestion du slider d sensibilité
void WiimoteView::handleSliderIRSensitivity(int sensitivity){
	//On change la sensibilité sur le manager
	manager->changeIRSensitivity(numWiimote, sensitivity);
}


