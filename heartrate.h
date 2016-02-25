#ifndef HEARTRATE_H
#define HEARTRATE_H

#include <QWidget>
#include <QTcpSocket>
#include <QPainter>
#include <QLabel>

class QCustomPlot;
class HeartRate : public QWidget
{
    Q_OBJECT
public:
    explicit HeartRate(QWidget *parent = 0);

    ~HeartRate();

public slots:
    void dataInput(int testData);

private:
    void initHeartRate();

    QCustomPlot *qcustomplot;
    int index;
};

#endif // HEARTRATE_H
