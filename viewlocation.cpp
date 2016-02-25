#include "viewlocation.h"
#include "iconhelper.h"
#include "myhelper.h"

ViewLocation::ViewLocation(QWidget *parent)
    : QWidget(parent),ui(new Ui::ViewLocation)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    m_location = this->geometry();
    max = false;
    mousePressed = false;

    //安装事件监听器,让标题栏识别鼠标双击
    ui->lab_Title->installEventFilter(this);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
    IconHelper::Instance()->SetIcon(ui->btnMenu_Min, QChar(0xf068), 10);
    IconHelper::Instance()->SetIcon(ui->lab_Ico, QChar(0xf015), 12);
    QUrl url("file:///D:\\qte\\frmMain\\Location.html");
    ui->webView->load((url));
    webFrame = ui->webView->page()->mainFrame();
}

bool ViewLocation::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        this->on_btnMenu_Max_clicked();
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void ViewLocation::mouseMoveEvent(QMouseEvent *e)
{
    if (mousePressed && (e->buttons() && Qt::LeftButton) && !max) {
        this->move(e->globalPos() - mousePoint);
        e->accept();
    }
}

void ViewLocation::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        mousePressed = true;
        mousePoint = e->globalPos() - this->pos();
        e->accept();
    }
}

void ViewLocation::mouseReleaseEvent(QMouseEvent *)
{
    mousePressed = false;
}

void ViewLocation::on_btnMenu_Close_clicked()
{
    this->close();
}

void ViewLocation::on_btnMenu_Max_clicked()
{
    if (max) {
        this->setGeometry(m_location);
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf096), 10);
        ui->btnMenu_Max->setToolTip("最大化");
    } else {
        m_location = this->geometry();
        this->setGeometry(qApp->desktop()->availableGeometry());
        IconHelper::Instance()->SetIcon(ui->btnMenu_Max, QChar(0xf079), 10);
        ui->btnMenu_Max->setToolTip("还原");
    }
    max = !max;
}

void ViewLocation::on_btnMenu_Min_clicked()
{
    this->showMinimized();
}

void ViewLocation::setLocation(double Longitude, double Latitude)
{
    longitude = Longitude;
    latitude = Latitude;
    ui->leLon->setText(QString("%1").arg(Longitude));
    ui->leLat->setText(QString("%1").arg(Latitude));
    QString setLongitude = QString("document.getElementById('longitude').value = '%1';").arg(longitude);
    QString setLatitude = QString("document.getElementById('latitude').value = '%1';").arg(latitude);
    webFrame->evaluateJavaScript(setLongitude);
    webFrame->evaluateJavaScript(setLatitude);
    webFrame->evaluateJavaScript("theLocation();");
}

void ViewLocation::location(bool b)
{
    QString setLongitude = QString("document.getElementById('longitude').value = '%1';").arg(longitude);
    QString setLatitude = QString("document.getElementById('latitude').value = '%1';").arg(latitude);
    webFrame->evaluateJavaScript(setLongitude);
    webFrame->evaluateJavaScript(setLatitude);
    webFrame->evaluateJavaScript("theLocation();");
}

