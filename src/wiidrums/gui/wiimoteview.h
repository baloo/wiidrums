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
 * Widget qui représente une Wiimote
 *
 * @author David Bonnet pour Polytech'WiiGuard Team
 * @date 21/03/2009 17:51
 * @since 24/03/2009 17:52
 * @version 1.0
 */


#ifndef DEF_WIIMOTEVIEW
#define DEF_WIIMOTEVIEW

#include <QtGui>
#include <QtCore>
#include <QAbstractSlider>
#include <wiimote/WiimoteManager.h>

class WiimoteView : public QWidget {

    Q_OBJECT

    public:
	/**
	 * Constructeur d'une vue de Wiimote
	 */
	WiimoteView(WiimoteManager *wm, int numWm);

	~WiimoteView();

    protected slots:

	/**
	 * Slot qui gère la déconnexion d'une wiimote
	 *
	 * @param numWm le numéro de la wiimote concernée
	 */
	void handleDisconnect(int numWm);

	/**
	 * Slot qui se charge de l'activation de l'IR
	 */
	void handleIRActivation();

	/**
	 * Méthode qui gère la wiimote lorsque le mode alarme est activé
	 */
	void handleAlarmModeEnabled(bool isAlarmModeEnabled);

	/**
	 * Slot qui gère le déclenchement de l'alarme sur la wiimote
	 */
	void handleAlarmDeclenched(int numWm, int numDot);

	/**
	 * Slot qui gère l'affichage des points IR détectés
	 */
	void handleIRDotDetected(int numWm, int numDot, bool visible, int x, int y);

	/**
	 * Slot qui gère le changement de sensibilité de la wiimote
	 */
	void handleIRSensitivityChange(int numWm, int sensitivity);

	/**
	 * Slot qui permet de lier le slider avec le manager
	 */
	void handleSliderIRSensitivity(int sensitivity);

    private:
	int numWiimote;
	WiimoteManager *manager;

	QLabel *wiimoteName;
	QLabel *wiimoteStatus;	//OK, Init ou Alarme

	QGridLayout *mainLayout;
	QHBoxLayout *statusLayout;
	QHBoxLayout *controlLayout;

	//Widgets de contrôle
	QPushButton *IRButton;
	bool irEnable;
	bool alarmDelenched;
	QSlider *irSensitivitySlider;

	//Widgets de status
	QLabel *irStatus;
	QLabel *dot1;
	QLabel *dot2;
	QLabel *dot3;
	QLabel *dot4;
	QLabel *irSensitivity;

};

#endif