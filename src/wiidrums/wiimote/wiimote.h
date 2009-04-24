// vim: ts=4 sw=4 expandtab


#ifndef DEF_WIIMOTE
#define DEF_WIIMOTE

#include <wiiuse.h>


class Wiimote 
{
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

    void resync();
    int setFlags(int enable, int disable);
    int writeData(unsigned int addr, byte* data, byte len);
    float setSmoothAlpha(float alpha);

    void setIr(int status);
    void setIrVres(unsigned int x, unsigned int y);
    void setIrPosition(enum ir_position_t pos);
    void setAspectRatio(enum aspect_t aspect);
    void setIrSensitivity(int level);

  protected:
    wiimote_t * wm;
};





#endif
