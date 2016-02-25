#include <QApplication>
#include "mainwindow.h"
#include "myhelper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    myHelper::SetUTF8Code();
    myHelper::SetStyle("blue");

    MainWindow w;
    w.show();

    return app.exec();
}
