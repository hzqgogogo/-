#include "show.h"

Show::Show(QWidget *parent)
    : QWidget(parent)
{
    initShow();

    connect(this, SIGNAL(newRate(int)), heartRate, SLOT(dataInput(int)));
    connect(this, SIGNAL(newSpeed(float)), speed, SLOT(dataInput(float)));
    connect(&m_timer, SIGNAL(timeout()),this, SLOT(myTimer()));
}

void Show::initShow()
{    
    webLocation = new ViewLocation;
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QLayout *leftLayout = new QVBoxLayout;
    QLayout *rightLayout = new QVBoxLayout;

    boxHeart = new QGroupBox(QString::fromUtf8("心率"), this);
    QLayout *layoutHeart = new QHBoxLayout(boxHeart);
    heartRate = new HeartRate(this);
    layoutHeart->addWidget(heartRate);

    boxSpeed = new QGroupBox(QString::fromUtf8("防摔倒"), this);
    QLayout *layoutSpeed = new QHBoxLayout(boxSpeed);
    speed = new Speed(this);
    layoutSpeed->addWidget(speed);

    leftLayout->addWidget(boxHeart);
    leftLayout->addWidget(boxSpeed);

    boxInformation = new QGroupBox(QString::fromUtf8("用户信息"), this);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(boxInformation->sizePolicy().hasHeightForWidth());
    boxInformation->setSizePolicy(sizePolicy);
    boxInformation->setMinimumSize(QSize(200, 0));
    QGridLayout *gridLayout = new QGridLayout(boxInformation);
    labHeight = new QLabel("身  高：");
    labWeight = new QLabel("体  重：");
    labSteps = new QLabel("步  数：");
    labCalorie = new QLabel("卡路里：");
    labHeartRateMinute = new QLabel("心率/分钟：");
    labLocation = new QLabel("地理位置：");
    labDistance = new QLabel("步行总长度：");
    btnLocation = new QPushButton("定位");
    leHeight = new QLineEdit("175cm");
    leWeight = new QLineEdit("60kg");
    leSteps = new QLineEdit;
    leCalorie = new QLineEdit;
    leHeartRateMinute = new QLineEdit;
    leDistance = new QLineEdit;
    leHeight->setReadOnly(true);
    leWeight->setReadOnly(true);
    leSteps->setReadOnly(true);
    leCalorie->setReadOnly(true);
    leHeartRateMinute->setReadOnly(true);
    leDistance->setReadOnly(true);
    gridLayout->addWidget(labLocation, 0, 0, 1, 1);
    gridLayout->addWidget(btnLocation, 0, 1, 1, 1);
    gridLayout->addWidget(labHeight, 1, 0, 1, 1);
    gridLayout->addWidget(leHeight, 1, 1, 1, 1);
    gridLayout->addWidget(labWeight, 2, 0, 1, 1);
    gridLayout->addWidget(leWeight, 2, 1, 1, 1);
    gridLayout->addWidget(labSteps, 3, 0, 1, 1);
    gridLayout->addWidget(leSteps, 3, 1, 1, 1);
    gridLayout->addWidget(labCalorie, 4, 0, 1, 1);
    gridLayout->addWidget(leCalorie, 4, 1, 1, 1);
    gridLayout->addWidget(labHeartRateMinute, 5, 0, 1, 1);
    gridLayout->addWidget(leHeartRateMinute, 5, 1, 1, 1);
    gridLayout->addWidget(labDistance, 6, 0, 1, 1);
    gridLayout->addWidget(leDistance, 6, 1, 1, 1);
    rightLayout->addWidget(boxInformation);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    connect(btnLocation, SIGNAL(clicked()),SLOT(webLocationShow()));
}

Show::~Show()
{

}

void Show::startRead()
{
    char dataArray[1024];
    memset(dataArray, 0, sizeof(dataArray));
    int data = 0;
    float data1 = 0.0;
    socket->read(dataArray,1024);
    if(dataArray[0] == 0x01)
    {
        data = atoi(&dataArray[1]);
        emit newRate(data);
    }
    if(dataArray[0] == 0x02)
    {
        //memcpy(&data1, &dataArray[1], 4);
        mf_t.val[0] = dataArray[4];
        mf_t.val[1] = dataArray[3];
        mf_t.val[2] = dataArray[2];
        mf_t.val[3] = dataArray[1];
        emit newSpeed(mf_t.f);
    }
    if(strcmp(dataArray, "12") == 0)
    {
        QPalette p = boxHeart->palette();
        p.setColor(QPalette::WindowText, Qt::red);
        boxHeart->setPalette(p);
        boxHeart->setTitle(QString::fromUtf8("心率低于60！！！"));
    }
    if(strcmp(dataArray, "32") == 0)
    {
        QPalette p = boxSpeed->palette();
        p.setColor(QPalette::WindowText, Qt::red);
        boxSpeed->setPalette(p);
        boxSpeed->setTitle(QString::fromUtf8("将要摔倒！！！"));
        m_timer.start(2000);
    }
    qDebug() << dataArray;

}

void Show::myTimer()
{
    QPalette p = boxSpeed->palette();
    p.setColor(QPalette::WindowText, Qt::black);
    boxSpeed->setPalette(p);
    boxSpeed->setTitle(QString::fromUtf8("防摔倒"));
    m_timer.stop();
}

void Show::startAddSocket(QTcpSocket *socket)
{
    this->socket = socket;
    connect(this->socket, SIGNAL(readyRead()), this, SLOT(startRead()));
}

void Show::Accelerate(float data)
{
    emit newSpeed(data);
}

void Show::Heartbeat(int data)
{
    emit newRate(data);
}

void Show::webLocationShow()
{
    webLocation->show();
}

void Show::closeWebLocation()
{
    webLocation->close();
}
