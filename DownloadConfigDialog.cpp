#include "DownloadConfigDialog.h"
#include "ui_DownloadConfigDialog.h"
#include "AddKeyDialog.h"

DownloadConfigDialog::DownloadConfigDialog(WordListDB *wordListdb,
                                           APIKeyValueMgr *apikeymgr,
                                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadConfigDialog)
{
    ui->setupUi(this);


    isAutoMgr =false;
    keyValue = "";
    keyName = "";

    wordListDB = wordListdb;
    apiKeyMgr = apikeymgr;
    initAll();
}

DownloadConfigDialog::~DownloadConfigDialog()
{
    delete ui;
    wordListDB = NULL;
    apiKeyMgr = NULL;
}

void DownloadConfigDialog::initAll()
{
    ui->AcceptButton->setEnabled(false);
    /////////////////////////////////////////////
    //
    QStringList keyNameList;
    keyNameList = apiKeyMgr->getKeyName();
    //遍历该表
    QStringListIterator Iterator(keyNameList);
    //向combobox中添加用户
    while(Iterator.hasNext())
    {
        ui->KeyComboBox->addItem(Iterator.next());
    }
}

QString DownloadConfigDialog::getKeyName()
{
    return keyName;
}

QString DownloadConfigDialog::getKeyValue()
{
    return keyValue;
}

int DownloadConfigDialog::getDownLoadCount()
{
    endID = ui->EndSpinBox->value();
    startID = ui->StartLineEdit->text().toInt();
    return (endID - startID + 1);
}

int DownloadConfigDialog::getStartID()
{
    startID = ui->StartLineEdit->text().toInt();
    return startID;
}

int DownloadConfigDialog::getEndID()
{
    endID = ui->EndSpinBox->value();
    return endID;
}

bool DownloadConfigDialog::getAutoMgr()
{
    return isAutoMgr;
}

void DownloadConfigDialog::on_AutoCheckBox_clicked(bool checked)
{
    if(checked)
    {
        ui->KeyComboBox->setEnabled(false);
        ui->EndSpinBox->setEnabled(false);
        ui->AcceptButton->setEnabled(true);
        isAutoMgr = true;
    }
    else
    {
        isAutoMgr = false;
        ui->KeyComboBox->setEnabled(true);
        ui->EndSpinBox->setEnabled(true);
    }
}

void DownloadConfigDialog::on_AddKeyButton_clicked()
{
    AddKeyDialog *addKey;
    addKey = new AddKeyDialog(this);
    if(addKey->exec() == QDialog::Accepted)
    {
        QString keyname = addKey->getKeyName();
        QString keyvalue = addKey->getKeyValue();

        apiKeyMgr->addNewRecord(keyvalue,keyname,false,0);

        ui->KeyComboBox->clear();
        QStringList keyNameList;
        keyNameList = apiKeyMgr->getKeyName();
        //遍历该表
        QStringListIterator Iterator(keyNameList);
        //向combobox中添加用户
        while(Iterator.hasNext())
        {
            ui->KeyComboBox->addItem(Iterator.next());
        }
    }
    addKey->close();
    delete addKey;
    addKey = NULL;
}

void DownloadConfigDialog::on_KeyComboBox_currentIndexChanged(const QString &keyname)
{
    ui->textBrowser->clear();
    /////////////////////////////////////////////////////
    //
    keyName = keyname;
    keyValue = apiKeyMgr->getKeyValue(keyName);
    ui->textBrowser->append("1.API名："+keyName);
    ui->textBrowser->append("2.API值："+keyValue);

    ui->AcceptButton->setEnabled(true);
    ///////////////////////////////////////////////////////////////////////////
    //
    QDate date = apiKeyMgr->getUseDate(keyname).toDate();
    ui->textBrowser->append("3.最后使用日期："+
                            apiKeyMgr->getUseDate(keyname).toString());

    QDate currdate = QDate::currentDate();
    ui->textBrowser->append("4.当前日期："+
                            currdate.toString("yyyy-MM-dd"));

    int count = 0;
    if(date.daysTo(currdate) > 0)
    {
        apiKeyMgr->updateUseDate(currdate.toString("yyyy-MM-dd"),keyName);
        apiKeyMgr->updateUseCount(keyValue,0);
        count = 0;
    }
    else
    {
        count = apiKeyMgr->getUseCount(keyValue);
    }

    ui->textBrowser->append("5.当天访问量(XML+Audio<=500)："+QString::number(count,'i',0));

    int haveDownloadCount = wordListDB->getDownloadCount();
    ui->StartLineEdit->setText(QString::number(haveDownloadCount+1,'i',0));

    ui->EndSpinBox->setMaximum(haveDownloadCount+(500 - count));
    ui->EndSpinBox->setMinimum(haveDownloadCount+1);
    ui->EndSpinBox->setValue(haveDownloadCount+(500 - count));

}

void DownloadConfigDialog::on_AcceptButton_clicked()
{
    accept();
}

void DownloadConfigDialog::on_QuitButton_clicked()
{
    reject();
}

