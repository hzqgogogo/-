#include "speed.h"
#include "qcustomplot.h"
#include <QLayout>

Speed::Speed(QWidget *parent)
    : QWidget(parent)
{
    index = 0;
    QHBoxLayout *layout = new QHBoxLayout(this);
    qcustomplot = new QCustomPlot(this);
    layout->addWidget(qcustomplot);
    initSpeed();
}

void Speed::initSpeed()
{
    if(qcustomplot != NULL)
    {
        qcustomplot->addGraph();
        qcustomplot->graph()->setBrush(QBrush(QColor(255, 200, 20, 70)));
        qcustomplot->xAxis->setRange(0, 300);
        qcustomplot->yAxis->setRange(0, 20);
        qcustomplot->xAxis->setLabel("x");
        qcustomplot->yAxis->setLabel("y");
        qcustomplot->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qcustomplot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    }
}

void Speed::dataInput(float testData)
{
    if(qcustomplot != NULL)
    {
        qcustomplot->graph(0)->addData(index, testData);
        qcustomplot->replot();
        index++;
    }
}

Speed::~Speed()
{

}
