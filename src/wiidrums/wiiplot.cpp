
// vim: ts=4 sw=4 expandtab:

#include "wiiplot.h"

WiiPlot::WiiPlot(QWidget * parent):
    QwtPlot(parent),
    d_interval(0),
    d_timerId(-1)
{
    alignScales();

    //Init
    for( int i=PLOT_SIZE; i>0; i--)
    {
        cout << "Initing i=" << i << endl;

        d_t[i] = 0.5* i;
        d_x[i] = 0;
        d_y[i] = 0;
        d_z[i] = 0;
    }

    // Legend
    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    //Inset curves
    QwtPlotCurve * xPlot = QwtPlotCurve("X accel");
    QwtPlotCurve * yPlot = QwtPlotCurve("Y accel");
    QwtPlotCurve * zPlot = QwtPlotCurve("Z accel");
    xPlot->attach(this);
    yPlot->attach(this);
    zPlot->attach(this);

    //Set pen
    xPlot->setPen(QPen(Qt::blue));
    yPlot->setPen(QPen(Qt::green));
    zPlot->setPen(QPen(Qt::red));

    //Attach data sources to curves
    xPlot->setRawData(d_t, d_x, PLOT_SIZE);
    yPlot->setRawData(d_t, d_y, PLOT_SIZE);
    zPlot->setRawData(d_t, d_z, PLOT_SIZE);

    setAxisTitle(QwtPlot::xBottom, "Time/seconds");
    setAxisScale(QwtPlot::xBottom, 0, 100);
    
    setTimerInterval(0.0);
}

void WiiPlot::setTimerInterval(double ms)
{
    d_interval = qRound(ms);

    if( d_timerId >= 0)
    {
        killTimer(d_timerId);
        d_timerId = -1;
    }
    if(d_interval >= 0)
        d_timerId = startTimer(d_interval);
}

// Catch new values
void WiiPlot::timerEvent(QTimerEvent *, double x, double y, double z)
{
    for ( int i = PLOT_SIZE -1; i>0; i--){
        d_x[i] = d_x[i-1];
        d_y[i] = d_y[i-1];
        d_z[i] = d_z[i-1];
    }
    d_x[0] = x;
    d_y[0] = y;
    d_z[0] = z;

    // On reaffiche tout ca !
    replot();
}
