#ifndef VIEWLOCATION_H
#define VIEWLOCATION_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QtWebKit>
#include <QWebFrame>
#include "ui_location.h"

namespace Ui {
    class ViewLocation;
}

class ViewLocation : public QWidget
{
    Q_OBJECT
public:
    explicit ViewLocation(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();

public slots:
    void setLocation(double Longitude, double Latitude);
    void location(bool b);

private:
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect m_location;

    Ui::ViewLocation *ui;
    QWebFrame *webFrame;
    double longitude;
    double latitude;

//        QUrl url("file:///D:\\qte\\frmMain\\Location.html");

//        view = new QWebView(this);
//        view->load(url);

//        webFrame = view->page()->mainFrame();

//        QLayout *layout = new QVBoxLayout();
//        layout->addWidget(view);

//        QPushButton *btn = new QPushButton("定位", this);
//        connect(btn ,SIGNAL(clicked(bool)), this, SLOT(location(bool)));

//        layout->addWidget(btn);
//        this->setLayout(layout);
//    }

//public slots:
//    void setLocation(QString Longitude, QString Latitude)
//    {
//        longitude = Longitude;
//        latitude = Latitude;
//    }

//    void location(bool b)
//    {
//        qDebug()<<b;
//        QString setLongitude = QString("document.getElementById('longitude').value = '%1';").arg(longitude);
//        QString setLatitude = QString("document.getElementById('latitude').value = '%1';").arg(latitude);
//        webFrame->evaluateJavaScript(setLongitude);
//        webFrame->evaluateJavaScript(setLatitude);
//        webFrame->evaluateJavaScript("theLocation();");
//    }

//signals:

//private:
//    QWebView *view;
//    QWebFrame *webFrame;
//    QString longitude;
//    QString latitude;
};

#endif // VIEWLOCATION_H
