/* 
 * vim: ts=4 sw=4 expandtab: 
 * */


#include "plot.h"

MyPlot::MyPlot( const QwtText& name, QWidget *parent )
    : QwtPlot( name, parent )
{
    // Show a title
    setTitle( "This is an Example" );

    QwtLegend * legend= new QwtLegend();

    // Show a legend at the bottom
    insertLegend(legend, QwtPlot::BottomLegend);


    // Show the axes
    setAxisTitle( xBottom, "x" );
    setAxisTitle( yLeft, "y" );

    // Insert two curves and get IDs for them

    QwtPlotCurve *curve1 = new QwtPlotCurve("y = sin(x)");
    QwtPlotCurve *curve2 = new QwtPlotCurve("y = sign(sin(x)");


    // Calculate the data, 500 points each
    const int points = 500;
    double x[ points ];
    double sn[ points ];
    double sg[ points ];

    for( int i=0; i<points; i++ )
    {
      x[i] = (3.0*3.14/double(points))*double(i);
    
      sn[i] = 2.0*sin( x[i] );
      sg[i] = (sn[i]>0)?1:((sn[i]<0)?-1:0);
    }

    // Copy the data to the plot
    curve1->setData( x, sn, points );
    curve2->setData( x, sg, points );

    // Attach
    curve1->attach( this );
    curve2->attach( this );

    // Set the style of the curves
    curve1->setPen( QPen( Qt::blue ) );
    curve2->setPen( QPen( Qt::green, 3 ) );

    // Show the plots
    replot();
}
