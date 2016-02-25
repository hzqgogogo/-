#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QtNetwork/QTcpServer>
#include "ui_mainwindow.h"
#include "show.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();

    void whenNewConnection();
    void whenDisconnected();

    void ReadData();
    void ReadError(QAbstractSocket::SocketError);
    void InputUserName();

signals:
    void newSocket(QTcpSocket *socket);

private:
    void initStyle();

    Ui::MainWindow *ui;
    QPoint mousePoint;
    bool mousePressed;
    bool max;
    QRect location;

    QTcpServer *server;
    QTabWidget *tabWidget;
    QMap<QTcpSocket *,  Show*> map;

    QTcpSocket *tcpClient;
    QPushButton *btnUserName;
    QLineEdit *leUserName;
    QGroupBox *boxInputUserName;
    QMap<QString, Show*> userMap;
    QString userTemp;
};


#endif
