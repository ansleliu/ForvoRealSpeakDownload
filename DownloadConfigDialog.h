#ifndef DOWNLOADCONFIGDIALOG_H
#define DOWNLOADCONFIGDIALOG_H

#include <QDialog>
#include "APIKeyValueMgr.h"
#include "WordListDB.h"

namespace Ui {
class DownloadConfigDialog;
}

class DownloadConfigDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DownloadConfigDialog(WordListDB *wordListdb = NULL,
                                  APIKeyValueMgr *apikeymgr = NULL,
                                  QWidget *parent = 0);
    ~DownloadConfigDialog();

public:
    void initAll();

    QString getKeyName();
    QString getKeyValue();
    int getDownLoadCount();
    int getStartID();
    int getEndID();

    bool getAutoMgr();
    
private slots:
    void on_AddKeyButton_clicked();

    void on_KeyComboBox_currentIndexChanged(const QString &keyname);

    void on_AcceptButton_clicked();

    void on_QuitButton_clicked();

    void on_AutoCheckBox_clicked(bool checked);

private:
    Ui::DownloadConfigDialog *ui;

    WordListDB *wordListDB;
    APIKeyValueMgr *apiKeyMgr;

    bool isAutoMgr;
    QString keyValue;
    QString keyName;
    int startID;
    int endID;
};

#endif // DOWNLOADCONFIGDIALOG_H
