QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DEFINES += QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include

SOURCES += \
    src/sortdialog.cpp \
    src/gotodialog.cpp \
    src/finddialog.cpp \
    src/cell.cpp \
    src/excel.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    include/gotodialog.h \
    include/finddialog.h \
    include/cell.h \
    include/excel.h \
    include/mainwindow.h \
    include/sortdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
  resources.qrc

FORMS += \
  ui/sortdialog.ui \
  ui/gotodialog.ui \
  ui/finddialog.ui
