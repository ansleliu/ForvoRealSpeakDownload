#-------------------------------------------------
#
# Project created by QtCreator 2014-04-16T21:29:37
#
#-------------------------------------------------

QT       += core gui sql xml network phonon

TARGET = ForvoRealSpeak
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Download.cpp \
    DownloadControl.cpp \
    XMLParser.cpp \
    WordListProcess.cpp \
    MySQLite.cpp \
    WordListDB.cpp \
    HomeDialog.cpp \
    DownloadConfigDialog.cpp \
    APIKeyValueMgr.cpp \
    AddKeyDialog.cpp \
    WordDownloadProcess.cpp \
    AboutDialog.cpp

HEADERS  += MainWindow.h \
    Download.h \
    DownloadControl.h \
    XMLParser.h \
    WordListProcess.h \
    MySQLite.h \
    WordListDB.h \
    HomeDialog.h \
    DownloadConfigDialog.h \
    APIKeyValueMgr.h \
    AddKeyDialog.h \
    WordDownloadProcess.h \
    AboutDialog.h

FORMS    += MainWindow.ui \
    HomeDialog.ui \
    DownloadConfigDialog.ui \
    AddKeyDialog.ui \
    AboutDialog.ui

UI_DIR = ./ui
MOC_DIR = TEMP/moc
RCC_DIR = TEMP/rcc
OBJECTS_DIR = TEMP/obj

RESOURCES += \
    RC.qrc

RC_FILE = MyApp.rc

OTHER_FILES += \
    MyApp.rc
