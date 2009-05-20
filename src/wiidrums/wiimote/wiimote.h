// vim: ts=4 sw=4 expandtab


#ifndef DEF_WIIMOTE
#define DEF_WIIMOTE

#include <QObject>
#include <wiiuse.h>

/**
 Classe Wiimote qui implémente une wiimote avec les informations de Wiiuse mais également les informations sur l'accéléromètre.

 @author David Bonnet - Arthur Gautier pour Wiidrum Team
 @date 14/05/2009
 @since 14/05/2009 15:34
 @version 1.0
 */
class Wiimote : public QObject
{
    Q_OBJECT


  public:
    Wiimote(wiimote_t * wm);
    ~Wiimote();

    void setAccelThreshold(int threshold);
    void setOrientThreshold(float threshold);
    void disconnected();
    void disconnect();
    void rumble(int status);
    void toggleRumble();
    void setLeds(int leds);
    void motionSensing(int status);
    int readData(byte* buffer, unsigned int offset, unsigned short len);
    void status();

    int getUnid();

    void resync();
    int setFlags(int enable, int disable);
    int writeData(unsigned int addr, byte* data, byte len);
    float setSmoothAlpha(float alpha);

    void setIr(int status);
    void setIrVres(unsigned int x, unsigned int y);
    void setIrPosition(enum ir_position_t pos);
    void setAspectRatio(enum aspect_t aspect);
    void setIrSensitivity(int level);

    wiimote_t * getStruct();
    vec3b_t getAccel();
    orient_t getOrient();


  signals:
    void disconnectedSignal();

  protected:
    wiimote_t * wm;

};





#endif

