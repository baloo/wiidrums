#ifndef PLOT_H
#define PLOT_H


#include <QDialog>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>


class MyPlot : public QwtPlot
{
    Q_OBJECT

public:
    MyPlot(const QwtText &name, QWidget *parent=NULL);
};

#endif
