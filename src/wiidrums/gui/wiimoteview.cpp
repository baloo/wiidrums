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

#include "WiimoteView.h"

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
	connect(manager, SIGNAL(alarmModeEnable(bool)), this, SLOT(handleAlarmModeEnabled(bool)));

	//Gestion de l'alarme
	connect(manager, SIGNAL(alarmDeclenched(int, int)), this, SLOT(handleAlarmDeclenched(int, int)));
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

//On gère l'affichage pour l'alarme
void WiimoteView::handleAlarmModeEnabled(bool isAlarmModeEnabled){

	if(isAlarmModeEnabled){
	      wiimoteStatus->setText("<font color=\"green\">OK</font>");
	} else {
	      wiimoteStatus->setText("<font color=\"orange\">Initialisation</font>");
	      alarmDelenched = false;
	}
}

//Gestion de l'alarme
void WiimoteView::handleAlarmDeclenched(int numWm, int numDot){

	if(numWiimote == numWm){
	    //Alors nous sommes concernés par l'alarme
	    alarmDelenched = true;
	    wiimoteStatus->setText(QString::fromUtf8("<font color=\"red\">Alarme déclenchée</font>"));

	    switch(numDot){
		case 1:
		    dot1->setText("<font color=\"red\">1</font>");
		    break;
		case 2:
		    dot2->setText("<font color=\"red\">2</font>");
		    break;
		case 3:
		    dot3->setText("<font color=\"red\">3</font>");
		    break;
		case 4:
		    dot4->setText("<font color=\"red\">4</font>");
		    break;
		default:
		    break;
	    }

	}
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


