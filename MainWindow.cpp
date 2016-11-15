#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTextCodec>
#include <QFileInfo>
#include <QMessageBox>
#include "AboutDialog.h"
#include "phonon"
/////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->MainTableView->resizeColumnsToContents();
    ui->TextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);

    ///////////////////////////////////////////
    //����Ĭ���ļ���
    m_Dir.mkpath("./Sound/");
    m_Dir.mkpath("./ForvoRealSpeak/WordListDB/");
    m_Dir.mkpath("./ForvoRealSpeak/WordListTXT/");
    m_Dir.mkpath("./ForvoRealSpeak/APIKeyValue/");

    ///////////////////////////////////////////
    //��ʼ������
    wordListDBPath = "";
    wordListTXTPath = "";
    isDownloadConfiged = false;
    isAutoMgr = false;
    currKey = 0;

    wordListDB = NULL;
    apiValueMgr = NULL;
    downloadPro = NULL;

    QTimer::singleShot(200, this, SLOT(Home()));
}

MainWindow::~MainWindow()
{
    if(wordListDB != NULL)
    {
        qDebug() << "Delete wordListDB";
        delete wordListDB;
        wordListDB = NULL;
    }
    if(apiValueMgr != NULL)
    {
        qDebug() << "Delete apiValueMgr";
        delete apiValueMgr;
        apiValueMgr = NULL;
    }
    if(downloadPro != NULL)
    {
        qDebug() << "Delete downloadPro";
        downloadPro->deleteLater();
        downloadPro = NULL;
    }
    delete ui;
}

void MainWindow::Home()
{
    /////////////////////////////////////////////////////////////////
    //APIKey���ݿ�
    QString appPath = QCoreApplication::applicationDirPath();
    //��pathName����ġ�/���ָ���ת��Ϊ�����ڵײ����ϵͳ�ķָ���������ת�����ַ�����
    QString apiValueDBPath = QDir::convertSeparators(
                appPath.append("./ForvoRealSpeak/APIKeyValue/ApiKeyValue.db"));


    if(apiValueMgr != NULL)
    {
        qDebug() << "Delete apiValueMgr";
        delete apiValueMgr;
        apiValueMgr = NULL;
    }

    if(downloadPro != NULL)
    {
        qDebug() << "Delete downloadPro";
        downloadPro->deleteLater();
        downloadPro = NULL;
    }

    if(wordListDB != NULL)
    {
        qDebug() << "Delete wordListDB";
        delete wordListDB;
        wordListDB = NULL;
    }
    /////////////////////////////////////////////////////////////////////////
    //ѡ�񴰿�
    HomeDialog *homeDialog;
    homeDialog = new HomeDialog(this);
    if(homeDialog->exec() == QDialog::Accepted)
    {
        model = NULL;

        if(!homeDialog->isNewFileCreate())
        {
            ///////////////////////////////////////////////////
            //�����ļ�
            wordListDBPath = homeDialog->getWordListDBPath();
            ui->action_Transform->setEnabled(false);

            apiValueMgr = new APIKeyValueMgr(apiValueDBPath,"APIKEY","QSQLITE");
            /////////////////////////////////////
            //ָ��õ��������б����ݿ�
            wordListDB = new WordListDB(wordListDBPath,"WORDLIST","QSQLITE");

            qDebug() << "��ǰ��������:" ;
            QStringList connection = QSqlDatabase::connectionNames();
            for(int i=0;i<connection.size();i++)
            {
                qDebug() << connection.at(i) ;
            }

            model = wordListDB->wordListDBModel(this,"WordList");
            ui->MainTableView->setModel(model);
            model->select();
        }
        else
        {
            ////////////////////////////////////////////////////////
            //���ļ���ת��

            /////////////////////////////////////
            //ָ��õ��������б����ݿ�
            wordListTXTPath = homeDialog->getWordListTXTPath();            
            QFileInfo fileInfo(wordListTXTPath);
            QString fileName = fileInfo.baseName() + ".db";
            wordListDBPath = "./ForvoRealSpeak/WordListDB/"+fileName;
            wordListDB = new WordListDB(wordListDBPath,"WORDLIST","QSQLITE");   

            if(wordListDB->getRowCount() > 0)
            {
                apiValueMgr = new APIKeyValueMgr(apiValueDBPath,"APIKEY","QSQLITE");
                /////////////////////////////////////////////////////
                //���ļ��Ѿ����ڣ���Ϊ�Ѵ����ļ���������ת��
                ui->action_StartDownload->setEnabled(true);
                ui->action_DownloadConfig->setEnabled(true);
                ui->action_Listen->setEnabled(true);
                ui->action_Transform->setEnabled(false);
            }
            else
            {
                /////////////////////////////////////////////
                //
                ui->action_Transform->setEnabled(true);
                ui->action_StartDownload->setEnabled(false);
                ui->action_DownloadConfig->setEnabled(false);
                ui->action_Listen->setEnabled(false);
            }

            model = wordListDB->wordListDBModel(this,"WordList");
            ui->MainTableView->setModel(model);
            model->select();

            qDebug() << "��ǰ��������:" ;
            QStringList connection = QSqlDatabase::connectionNames();
            for(int i=0;i<connection.size();i++)
            {
                qDebug() << connection.at(i) ;
            }
        }

        homeDialog->close();
        homeDialog->deleteLater();
        homeDialog = NULL;
    }
    else
    {
        homeDialog->close();
        homeDialog->deleteLater();
        homeDialog = NULL;
        on_action_Quit_triggered();
    }
}

void MainWindow::transformInfoShow(const QString &info)
{
    ui->TextBrowser->append(info);
    model->select();
    if(info.contains("transform ended:"))
    {
        ui->action_StartDownload->setEnabled(true);
        ui->action_DownloadConfig->setEnabled(true);
        ui->action_Listen->setEnabled(false);
        ui->action_Transform->setEnabled(false);

        ui->TextBrowser->setHidden(true);

        model->setSort(0, Qt::AscendingOrder); //��Wid���Լ���0����������
        model->select();
        ui->action_Back->setEnabled(true);
    }
}

void MainWindow::downloadSound(const int &wid,const QString &keyvalue)
{
    ////////////////////////////////////////////////////////////////////////
    //��Ƶ���ش���
    downloadPro->getWid(wid);
    downloadPro->getKeyValue(keyvalue);
    downloadPro->download();
}

void MainWindow::wordListTransform()
{
    ////////////////////////////////////////////////////
    //�����б�Ĵ���   
    QString txtFile = QString(wordListTXTPath);
    WordListProcess *wordListPro;
    wordListPro = new WordListProcess(txtFile,wordListDB,this);
    connect(wordListPro,SIGNAL(transformInfo(QString)),
            this,SLOT(transformInfoShow(QString)));
    connect(wordListPro,SIGNAL(transformEnded(QString)),
            this,SLOT(transformInfoShow(QString)));
    wordListPro->start();
    model->select();
}

void MainWindow::on_action_Back_triggered()
{
    //////////////////////////////////////
    //����Home
    Home();
}

void MainWindow::on_action_Transform_triggered()
{
    ui->TextBrowser->setHidden(false);
    ui->action_Transform->setEnabled(false);
    ui->action_Back->setEnabled(false);
    model->setSort(0, Qt::DescendingOrder);  //��Wid���Լ���0�н�������
    model->select();
    wordListTransform();
}

void MainWindow::on_action_DownloadConfig_triggered()
{
    DownloadConfigDialog *downloadConfig;
    downloadConfig = new DownloadConfigDialog(wordListDB,apiValueMgr,this);
    if(downloadConfig->exec() == QDialog::Accepted)
    {
        isAutoMgr = downloadConfig->getAutoMgr();
        if(isAutoMgr == false)
        {
            isDownloadConfiged = true;
            keyList.clear();
            currKey = 0;
            keyName = downloadConfig->getKeyName();
            keyValue = downloadConfig->getKeyValue();
            startID = downloadConfig->getStartID();
            endID = downloadConfig->getEndID();
            downloadCount = downloadConfig->getDownLoadCount();
        }
        else
        {
            isDownloadConfiged = true;

            keyList.clear();
            startID = downloadConfig->getStartID();
            keyList = apiValueMgr->getKeyName();
            endID = 500*keyList.size() + startID;

            currKey = 0;
            keyName = keyList.at(currKey);
            keyValue = apiValueMgr->getKeyValue(keyName);
        }
    }
    downloadConfig->close();
    downloadConfig->deleteLater();
    downloadConfig = NULL;
}

void MainWindow::on_action_StartDownload_triggered()
{
    ui->action_Back->setEnabled(false);
    if(isDownloadConfiged == false)
    {
        /////////////////////////////////////////////
        //��������
        on_action_DownloadConfig_triggered();
        if(isDownloadConfiged == false)
        {
            isDownloadConfiged = false;
            return;
        }
        isDownloadConfiged = false;
    }
    /////////////////////////////////////////////////
    //��ʼ����
    isDownloadConfiged = false;
    ui->action_DownloadConfig->setEnabled(false);
    ui->action_StartDownload->setEnabled(false);
    ui->action_Back->setEnabled(false);
    ////////////////////////////////////////////////////////////////////////
    //��Ƶ���ش���
    downloadPro = new WordDownloadProcess(wordListDB,apiValueMgr,this);

    connect(downloadPro,SIGNAL(downloadFinished()),
            this,SLOT(downloadControl()));
    connect(downloadPro,SIGNAL(notHaveThisWord()),
            this,SLOT(haveNoThisWord()));
    connect(downloadPro,SIGNAL(LimitPerDayReached()),
            this,SLOT(LimitReached()));
    connect(downloadPro,SIGNAL(UnknownError()),
            this,SLOT(haveUnknownError()));
    connect(downloadPro,SIGNAL(NetWorkUnuseable()),
            this,SLOT(NetWorkIsUnuse()));
    connect(downloadPro,SIGNAL(sendInfo(QString)),
            this,SLOT(showInfo(QString)));

    wid = startID-1;
    ui->TextBrowser->setHidden(false);
    ui->ProgressBar->setHidden(false);
    ui->ProgressBar->setMinimum(startID-1);
    ui->ProgressBar->setMaximum(endID);
    downloadControl();
}

void MainWindow::downloadControl()
{
    model->select();
    ui->TextBrowser->clear();
    qDebug() << QString("Word : %1 have been downloaded")
                .arg(wordListDB->getWord(wid));
    //////////////////////////////////////////
    //����
    wid++;
    ui->ProgressBar->setValue(wid);
    if(wid <= endID)
    {
        qDebug() << QString("Download word : %1")
                    .arg(wordListDB->getWord(wid));
        downloadSound(wid,keyValue);
    }
    else
    {
        ui->TextBrowser->append(QString("������ɣ��ô����� %1 ����")
                                .arg(wid-startID));
        qDebug() << QString("download finish, have download %1 word")
                    .arg(wid-startID);
        ui->TextBrowser->setHidden(true);
        ui->ProgressBar->setHidden(true);
        ui->action_DownloadConfig->setEnabled(true);
        ui->action_StartDownload->setEnabled(true);
        ui->action_Back->setEnabled(true);
    }
}

void MainWindow::haveNoThisWord()
{
    model->select();
    qDebug() << QString("Forvo have not the sound of word : %1")
                .arg(wordListDB->getWord(wid));
    ////////////////////////////////////////
    //����
    wid++;
    if(wid <= endID)
    {
        qDebug() << QString("Download word : %1")
                    .arg(wordListDB->getWord(wid));
        downloadSound(wid,keyValue);
    }
    else
    {
        ui->TextBrowser->append(QString("������ɣ��ô����� %1 ����")
                                .arg(wid-startID));
        qDebug() << QString("download finish, have download %1 word")
                    .arg(wid-startID);
        ui->TextBrowser->setHidden(true);
        ui->ProgressBar->setHidden(true);
        ui->action_DownloadConfig->setEnabled(true);
        ui->action_StartDownload->setEnabled(true);
        ui->action_Back->setEnabled(true);
    }
}

void MainWindow::LimitReached()
{
    qDebug() << QString("Download finish.\n Limit/day reached");
    if(isAutoMgr == false)
    {
        currKey++;
        if(currKey < keyList.size())
        {
            keyName = keyList.at(currKey);
            keyValue = apiValueMgr->getKeyValue(keyName);
            downloadControl();
        }
        else
        {
            ui->TextBrowser->setHidden(true);
            ui->ProgressBar->setHidden(true);
            ui->action_DownloadConfig->setEnabled(true);
            ui->action_StartDownload->setEnabled(true);
            ui->action_Back->setEnabled(true);
        }
    }
    else
    {
        ui->TextBrowser->setHidden(true);
        ui->ProgressBar->setHidden(true);
        ui->action_DownloadConfig->setEnabled(true);
        ui->action_StartDownload->setEnabled(true);
        ui->action_Back->setEnabled(true);
    }
}

void MainWindow::haveUnknownError()
{
    qDebug() << QString("Download finish.\n Have Unkown Error.");
    QMessageBox::warning(this, tr("δ֪����"), tr("��վ����δ֪����"));
    ui->TextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    ui->action_DownloadConfig->setEnabled(true);
    ui->action_StartDownload->setEnabled(true);
    ui->action_Back->setEnabled(true);
}

void MainWindow::NetWorkIsUnuse()
{
    QMessageBox::warning(this, tr("����Ӧ�����"), tr("������Ӧ����ȷ�����Ѿ����ӵ����磡"));
    ui->TextBrowser->setHidden(true);
    ui->ProgressBar->setHidden(true);
    ui->action_DownloadConfig->setEnabled(true);
    ui->action_StartDownload->setEnabled(true);
    ui->action_Back->setEnabled(true);
}

void MainWindow::showInfo(const QString &info)
{
    ui->TextBrowser->append(info);
}

void MainWindow::keyAutoMgr()
{

}

void MainWindow::on_action_Listen_triggered()
{
}

void MainWindow::on_action_QT_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_action_About_triggered()
{
    AboutDialog *about;
    about = new AboutDialog(this);
    if(about->exec() == QDialog::Rejected)
    {
        qDebug() << "aboutdialog rejected";
        about->close();
        about->deleteLater();
        about = NULL;
    }
}

void MainWindow::on_action_Quit_triggered()
{
    if (QMessageBox::Yes == QMessageBox::information(this,
                                                     "��ʾ��Ϣ",
                                                     "��ȷ��Ҫ�˳���?",
                                 QMessageBox::Yes | QMessageBox::No))
    {
        QApplication::closeAllWindows();
    }
    else
    {
        ui->action_DownloadConfig->setEnabled(false);
        ui->action_Listen->setEnabled(false);
        ui->action_StartDownload->setEnabled(false);
        ui->action_Transform->setEnabled(false);
    }
}

