
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
#include <wiimotemanager.h>

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