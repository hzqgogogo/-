#include "heartrate.h"
#include "qcustomplot.h"
#include <QLayout>

HeartRate::HeartRate(QWidget *parent)
    : QWidget(parent)
{
    index = 0;
    QHBoxLayout *layout = new QHBoxLayout(this);
    qcustomplot = new QCustomPlot(this);
    layout->addWidget(qcustomplot);
    initHeartRate();
}

void HeartRate::initHeartRate()
{
    if(qcustomplot != NULL)
    {
        qcustomplot->addGraph();
        qcustomplot->graph()->setBrush(QBrush(QColor(255, 200, 20, 70)));
        qcustomplot->xAxis->setRange(0, 300);
        qcustomplot->yAxis->setRange(0, 1000);
        qcustomplot->xAxis->setLabel("x");
        qcustomplot->yAxis->setLabel("y");
        qcustomplot->setRangeZoom(Qt::Horizontal | Qt::Vertical);
        qcustomplot->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    }
}

void HeartRate::dataInput(int testData)
{
    if(qcustomplot != NULL)
    {
        qcustomplot->graph(0)->addData(index, testData);
        qcustomplot->replot();
        index++;
    }
}

HeartRate::~HeartRate()
{

}
