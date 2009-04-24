// vim: ts=4 sw=4 expandtab

#include "wiimote.h"


Wiimote::Wiimote(wiimote_t * mywm)
{
    this->wm = mywm;
    // We do nothing, everything should already be done
}

Wiimote::~Wiimote()
{

}

void Wiimote::setAccelThreshold(int threshold)
{
    wiiuse_set_accel_threshold(this->wm, threshold);
}

void Wiimote::setOrientThreshold(float threshold)
{
    wiiuse_set_orient_threshold(this->wm, threshold);
}

void Wiimote::disconnected()
{
    wiiuse_disconnected(this->wm);
}

void Wiimote::disconnect()
{
    wiiuse_disconnect(this->wm);
}

void Wiimote::rumble(int status)
{
    wiiuse_rumble(this->wm, status);
}

void Wiimote::toggleRumble(){
    wiiuse_toggle_rumble(this->wm);
}

void Wiimote::setLeds(int leds){
    wiiuse_set_leds(this->wm, leds);
}

void Wiimote::motionSensing(int status){
    wiiuse_motion_sensing(this->wm, status);
}

int Wiimote::readData(byte* buffer, unsigned int offset, unsigned short len)
{
    return wiiuse_read_data(this->wm, buffer, offset, len);
}

void Wiimote::status()
{
    wiiuse_status(this->wm);
}

void Wiimote::resync()
{
    wiiuse_resync(this->wm);
}

int Wiimote::setFlags(int enable, int disable)
{
    return wiiuse_set_flags(this->wm, enable, disable);
}

int Wiimote::writeData(unsigned int addr, byte* data, byte len)
{
    return wiiuse_write_data(this->wm, addr, data, len);
}

float Wiimote::setSmoothAlpha(float alpha)
{
    return wiiuse_set_smooth_alpha(this->wm, alpha);
}

void Wiimote::setIr(int status)
{
    wiiuse_set_ir(this->wm, status);
}

void Wiimote::setIrVres(unsigned int x, unsigned int y)
{
    wiiuse_set_ir_vres(this->wm, x, y);
}

void Wiimote::setIrPosition(enum ir_position_t pos)
{
    wiiuse_set_ir_position(this->wm, pos);
}

void Wiimote::setAspectRatio(enum aspect_t aspect)
{
    wiiuse_set_aspect_ratio(this->wm, aspect);
}

void Wiimote::setIrSensitivity(int level){
    wiiuse_set_ir_sensitivity(this->wm, level);
}


