#ifndef SPEED_H
#define SPEED_H

#include <QWidget>
#include <QPainter>
#include <QLabel>

class QCustomPlot;
class Speed : public QWidget
{
    Q_OBJECT
public:
    explicit Speed(QWidget *parent = 0);

    ~Speed();

public slots:
    void dataInput(float testData);

private:
    void initSpeed();

    QCustomPlot *qcustomplot;
    int index;
};

#endif // SPEED_H
