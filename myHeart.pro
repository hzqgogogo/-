

QT       += core gui network webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE += app

SOURCES += main.cpp \
          mainwindow.cpp \
          iconhelper.cpp \
          heartrate.cpp \
          qcustomplot.cpp \
          show.cpp \
          speed.cpp \
          frmmessagebox.cpp \
          viewlocation.cpp

HEADERS += myhelper.h \
          mainwindow.h \
          iconhelper.h \
          heartrate.h \
          qcustomplot.h \
          show.h \
          speed.h \
          frmmessagebox.h \
          viewlocation.h

FORMS += mainwindow.ui \
        frmmessagebox.ui \
        location.ui

win32:RC_FILE=app.rc

RESOURCES += rc.qrc

INCLUDEPATH += "d:\Qt\Qt4.8.4\lib\qjson\include\qjson"

LIBS += -L"d:\Qt\Qt4.8.4\lib\qjson\bin" -lqjson

