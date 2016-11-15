#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTimer>
#include <QDir>
#include <QFile>
#include "XMLParser.h"
#include "WordListDB.h"
#include "WordListProcess.h"
#include "HomeDialog.h"
#include "DownloadConfigDialog.h"
#include "APIKeyValueMgr.h"
#include "WordDownloadProcess.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //////////////////////////////////////////
    //开始部分
    void Home();

    //////////////////////////////////////////
    //单词下载列表转化
    void transformInfoShow(const QString &info);

    void on_action_Back_triggered();

    void on_action_Transform_triggered();

    void on_action_DownloadConfig_triggered();

    void on_action_StartDownload_triggered();

    void on_action_QT_triggered();

    void on_action_About_triggered();

    void on_action_Quit_triggered();

    void on_action_Listen_triggered();

private slots:
    void downloadControl();
    void haveNoThisWord();
    void LimitReached();
    void haveUnknownError();
    void NetWorkIsUnuse();

    void showInfo(const QString &info);
    void keyAutoMgr();

public:
    void downloadSound(const int &wid, const QString &keyvalue);
    void wordListTransform();

private:
    Ui::MainWindow *ui;
    /////////////////////////////////////////
    //单词下载列表的转化
    QString wordListDBPath;
    QString wordListTXTPath;

    QSqlTableModel *model;
    WordListDB *wordListDB;
    APIKeyValueMgr *apiValueMgr;

    bool isAutoMgr;
    QStringList keyList;
    int currKey;
    //////////////////////////////////////////
    //
    QDir m_Dir;

    QString downloadWord;
    QString fileName;
    QTime startTime;
    QTime finishTime;
    int TimeDiff;

    //////////////////////////////////////////
    //
    WordDownloadProcess *downloadPro;
    bool isDownloadConfiged;
    QString keyName;
    QString keyValue;

    int wid;
    int startID;
    int endID;
    int downloadCount;
};

#endif // MAINWINDOW_H
