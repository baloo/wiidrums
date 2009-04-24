// vim: ts=4 sw=4 expandtab


#ifndef DEF_WIIMOTES
#define DEF_WIIMOTES

#include <wiiuse.h>
#include <vector>
#include "wiimote.h"


using namespace std;

class Wiimotes
{
  public:
    Wiimotes(int wiimotes);
    ~Wiimotes();

    void cleanup();
    struct wiimote_t * getById(int unid);
    void setBluetoothStack(enum win_bt_stack_t type);
    void setTimeout(byte normal_timeout, byte exp_timeout);

    int find(int max_wiimotes, int timeout);
    int connect();
    int poll();


    vector<Wiimote *> * getWiimotes();


  protected:
    vector<Wiimote *> * wiimotesList;
    int wiimotes;
    struct wiimote_t ** wms;

};





#endif

