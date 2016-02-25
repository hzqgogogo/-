#ifndef SHOW_H
#define SHOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QLayout>
#include <QPushButton>
#include <QGroupBox>
#include "heartrate.h"
#include "speed.h"
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QWebView>
#include <QWebFrame>
#include "viewlocation.h"

static union FloatValue{
    char val[4];
    float f;
    int i;
} mf_t;

class Show : public QWidget
{
    Q_OBJECT
public:
    explicit Show(QWidget *parent = 0);
    void Accelerate(float data);
    void Heartbeat(int data);
    QPushButton *btnLocation;

    ViewLocation *webLocation;
    ~Show();

private slots:
    void startRead();
    void myTimer();

public slots:
    void startAddSocket(QTcpSocket *socket);
    void webLocationShow();
    void closeWebLocation();

signals:
    void newRate(int data);
    void newSpeed(float data);

private:
    void initShow();

    QTcpSocket *socket;
    HeartRate *heartRate;
    Speed *speed;
    QGroupBox *boxHeart;
    QGroupBox *boxSpeed;
    QGroupBox *boxInformation;
    QTimer m_timer;

    QLabel *labHeight;
    QLabel *labWeight;
    QLabel *labSteps;
    QLabel *labCalorie;
    QLabel *labHeartRateMinute;
    QLabel *labLocation;
    QLabel *labDistance;
    QLineEdit *leHeight;
    QLineEdit *leWeight;
    QLineEdit *leSteps;
    QLineEdit *leCalorie;
    QLineEdit *leHeartRateMinute;
    QLineEdit *leDistance;
};

#endif // SHOW_H
