
/**
 * @file
 *
 * Widget qui représente une Wiimote
 *
 * @author David Bonnet pour Polytech'WiiDrums Team
 * @date 30/04/2009 10:45
 * @since 16/05/2009 15:23
 * @version 1.0
 */


#ifndef DEF_WIIMOTEVIEW
#define DEF_WIIMOTEVIEW

#include <QtGui>
#include <QtCore>
#include <QAbstractSlider>
#include <wiimotemanager.h>

//#include <wiiplot.h>

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
      * Slot qui se charge de l'activation de l'accéléromètre
      */
      void handleAccelActivation();

      /*
      * Slot qui gère le changement de sensibilité de la wiimote
      *
      void handleIRSensitivityChange(int numWm, int sensitivity);

      /*
      * Slot qui permet de lier le slider avec le manager
      *
      void handleSliderIRSensitivity(int sensitivity);
      */

    private:
      int numWiimote;
      WiimoteManager *manager;

      QLabel *wiimoteName;
      //QLabel *wiimoteStatus;    //OK, Init ou Alarme

      QGridLayout *mainLayout;
      QHBoxLayout *statusLayout;
      QHBoxLayout *controlLayout;

      //Widgets d'informations
      //WiiPlot *graph;

      //Widgets de contrôle

      QPushButton *accelButton;
      bool accelEnable;
      /*
      bool alarmDelenched;
      QSlider *irSensitivitySlider;
      */

      //Widgets de status
      /*
      QLabel *irStatus;
      QLabel *dot1;
      QLabel *dot2;
      QLabel *dot3;
      QLabel *dot4;
      QLabel *irSensitivity;
      */

};

#endif
