#include "mainwindow.h"
#include "iconhelper.h"
#include "myhelper.h"
#include "frmmessagebox.h"
#include <QLayout>
#include <QPropertyAnimation>
#include <QMenu>
#include <QAction>
#include "parser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myHelper::FormInCenter(this);

#if 0
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 9999);
    connect(server, SIGNAL(newConnection()), this, SLOT(whenNewConnection()));
#endif
    btnUserName = new QPushButton(tr("确认"));
    leUserName = new QLineEdit;
    tabWidget = new QTabWidget;

    QHBoxLayout *tabLayout = new QHBoxLayout(ui->widget_main);
    tabLayout->addWidget(tabWidget);

    boxInputUserName = new QGroupBox(QString::fromUtf8("请输入用户名"), this);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(boxInputUserName->sizePolicy().hasHeightForWidth());
    boxInputUserName->setSizePolicy(sizePolicy);
    boxInputUserName->setMinimumSize(QSize(100, 100));
    QVBoxLayout *userLayout = new QVBoxLayout(boxInputUserName);
    userLayout->addWidget(leUserName);
    userLayout->addWidget(btnUserName);

    tabLayout->addWidget(boxInputUserName);
    connect(btnUserName, SIGNAL(clicked()), this, SLOT(InputUserName()));

    statusBar()->showMessage(tr("欢迎使用智能手环"), 2000);
    initStyle();

    tcpClient = new QTcpSocket(this);
    //    tcpClient->abort();//取消原有连接
    tcpClient->connectToHost("112.74.27.35", 10000);
//    tcpClient->connectToHost("192.168.1.101", 10000);

    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));
    connect(tcpClient,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}

void MainWindow::initStyle()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);

    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu, QChar(0xf0c9), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);

    //渐变动画显示窗体
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->setEasingCurve(QEasingCurve::OutCurve);
    animation->start();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void MainWindow::on_btnMenu_Close_clicked()
{
    int result = myHelper::ShowMessageBoxQuesion("确定要关闭吗？");
    if(result == 1)
    {
//        tcpClient->disconnectFromHost();
        qApp->exit();
    }
}

void MainWindow::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void MainWindow::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void MainWindow::whenNewConnection()
{
    Show *myShow = new Show(this);
    tabWidget->addTab(myShow, "Show");
    connect(this, SIGNAL(newSocket(QTcpSocket*)), myShow, SLOT(startAddSocket(QTcpSocket*)));
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), this, SLOT(whenDisconnected()));
    emit newSocket(socket);
    map.insert(socket, myShow);
}

void MainWindow::whenDisconnected()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket *>(sender());
    delete map.take(socket);
}

void MainWindow::ReadData()
{
    QByteArray buffer=tcpClient->readAll();

    char dataArray[1024]={0};
    memset(dataArray, 0, sizeof(dataArray));
    memcpy(dataArray, buffer.data(), 1024);
    switch(dataArray[0])
    {
    case 0x01:
    {
        qDebug() << "0x01";
        char sendData[1024];
        char a = 0x01;
        int length = 5;
        memcpy(sendData, &a, 1);
        memcpy(sendData+1, &length, 4);
        memcpy(sendData+1+4, "heart", 5);
        memcpy(sendData+1+4+5, &a, 1);
        tcpClient->write(sendData, 1+4+5+1);
    }
        break;
    case 0x00:
    {
        qDebug() << "失败";
        myHelper::ShowMessageBoxError("该用户不存在!");
    }
        break;
    case 0x05:
    {
        qDebug() << "成功";

        QList<QString> key = userMap.keys();
        bool flag = false;
        for(int i = 0; i < key.size(); i++)
        {
            if(key.at(i) == userTemp)
            {
                flag == true;
                break;
            }
        }
        if(flag == false)
        {
            Show *myShow = new Show(this);
            tabWidget->addTab(myShow, userTemp);
            userMap.insert(userTemp, myShow);

        }
    }
        break;
    case 0x06:
    {
        char getUserName[255]={0};
        int lengthUser = 0;
        memcpy(&lengthUser, dataArray+1, 4);
        memcpy(getUserName, dataArray+1+4, lengthUser);

        userMap.value(QString("%1").arg(getUserName))->closeWebLocation();
        delete userMap.take(getUserName);
        myHelper::ShowMessageBoxInfo(QString("用户%1").arg(getUserName));
    }
        break;
    case 0x10:
    {
        char json[1024]={0};
        bool ok;
        int lenghtJson;

        memcpy(&lenghtJson, dataArray+1, 4);
        memcpy(json, dataArray+1+4, lenghtJson);
        QString strJson = QString("%1").arg(json);
        QJson::Parser *parser = new QJson::Parser();
        QVariant result = parser->parse(strJson.toAscii(), &ok);
        QVariantMap mymap = result.toMap();
        userMap.value(mymap["User"].toString())->Accelerate(mymap["Accelerate"].toFloat());
    }
        break;
    case 0x11:
    {
        char json[1024]={0};
        bool ok;
        int lenghtJson;

        memcpy(&lenghtJson, dataArray+1, 4);
        memcpy(json, dataArray+1+4, lenghtJson);
        QString strJson = QString("%1").arg(json);
        QJson::Parser *parser = new QJson::Parser();
        QVariant result = parser->parse(strJson.toAscii(), &ok);
        QVariantMap mymap = result.toMap();
        userMap.value(mymap["User"].toString())->Heartbeat(mymap["Heartbeat"].toInt());
    }
        break;
    case 0x12:
    {
        char json[3072]={0};
        bool ok;
        int lenghtJson;
        memcpy(&lenghtJson, dataArray+1, 4);
        memcpy(json, dataArray+1+4, lenghtJson);
        QString strJson = QString("%1").arg(json);
        QJson::Parser *parser = new QJson::Parser();
        QVariant result = parser->parse(strJson.toAscii(), &ok);
        QVariantMap mymap = result.toMap();
        qDebug() << mymap["Lat"].toDouble();
        qDebug() << mymap["Lon"].toDouble();

        qDebug() << "----hzq";
        userMap.value(mymap["User"].toString())->webLocation->setLocation(mymap["Lon"].toDouble(),mymap["Lat"].toDouble());
    }
        break;
    case 0x13:
    {
        char json[1024]={0};
        bool ok;
        int lenghtJson;
        qDebug() << "-------";
        memcpy(&lenghtJson, dataArray+1, 4);
        memcpy(json, dataArray+1+4, lenghtJson);
        QString strJson = QString("%1").arg(json);
        QJson::Parser *parser = new QJson::Parser();
        QVariant result = parser->parse(strJson.toAscii(), &ok);
        QVariantMap mymap = result.toMap();
        qDebug() << strJson;
//        qDebug() << mymap["Step"].toInt();
//        qDebug() << mymap["Calorie"].toDouble();
//        qDebug() << mymap["Distance"].toFloat();
    }
        break;
    }
}

void MainWindow::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    qDebug() << tr("连接服务器失败,原因:%1") << tcpClient->errorString();
}

void MainWindow::InputUserName()
{
    if(tcpClient->state() == QAbstractSocket::ConnectedState)
    {
        char userName[255]={0};
        memcpy(userName, leUserName->text().toAscii().data(), 255);
        userTemp = leUserName->text();
        char sendData[1024];
        char a = 0x02;
        int length = strlen(userName);
        memcpy(sendData, &a, 1);
        memcpy(sendData+1, &length, 4);
        memcpy(sendData+1+4, userName, strlen(userName));
        tcpClient->write(sendData, 1+4+strlen(userName));
    }
}
