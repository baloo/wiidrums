// vim : ts=4 sw=4 expandtab:

#ifndef WIIPLOT_H
#define WIIPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_curve.h>

const int PLOT_SIZE = 200; // 0 to 199


class WiiPlot : public QwtPlot
{
    // On passe la classe wiimotemanager en amie, de facon à ce qu'elle puisse
    // acceder à update()
    friend class WiimoteManager;


    Q_OBJECT

  public:
    WiiPlot(QWidget *parent=NULL);

  public slots:
    void setTimerInterval(double interval);

  protected:
    void update(double x, double y, double z);



    double d_x[PLOT_SIZE];
    double d_y[PLOT_SIZE];
    double d_z[PLOT_SIZE];
    double d_t[PLOT_SIZE];



    int d_interval; //timer in ms
    int d_timerId; 
}


#endif WIIPLOT_H

