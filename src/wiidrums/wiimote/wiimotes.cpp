// vim: ts=4 sw=4 expandtab:


#include "wiimotes.h"


Wiimotes::Wiimotes(int wiimotes)
{
    this->wiimotesList = new vector<Wiimote *>(wiimotes, (Wiimote *) NULL);
    this->wiimotes = wiimotes;
    this->wms = wiiuse_init(this->wiimotes);
}

Wiimotes::~Wiimotes()
{
    delete(this->wiimotesList);
}

void Wiimotes::cleanup()
{
    wiiuse_cleanup(this->wms, this->wiimotes);
}

struct wiimote_t * Wiimotes::getById(int unid)
{
    return wiiuse_get_by_id(this->wms, this->wiimotes, unid);
}

void Wiimotes::setBluetoothStack(enum win_bt_stack_t type)
{
    wiiuse_set_bluetooth_stack(this->wms, this->wiimotes, type);
}

void Wiimotes::setTimeout(byte normal_timeout, byte exp_timeout)
{
    wiiuse_set_timeout(this->wms, this->wiimotes, normal_timeout, exp_timeout);
}

int Wiimotes::find(int max_wiimotes, int timeout)
{
    return wiiuse_find(this->wms, max_wiimotes, timeout);
}

int Wiimotes::connect()
{
    int i;
    int myWiimotes;
    myWiimotes = wiiuse_connect(this->wms, this->wiimotes);

    this->wiimotesList->clear();

    for(i =0; i < myWiimotes; i++)
    {
        this->wiimotesList->push_back(new Wiimote(this->wms[i]));
    }

}

int Wiimotes::poll()
{
    return wiiuse_poll(this->wms, this->wiimotes);
}

vector<Wiimote *> * Wiimotes::getWiimotes()
{
    return this->wiimotesList;
}


