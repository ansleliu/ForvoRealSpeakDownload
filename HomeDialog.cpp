#include "HomeDialog.h"
#include "ui_HomeDialog.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

HomeDialog::HomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeDialog)
{
    ui->setupUi(this);

    ui->OpenOldButton->setEnabled(false);
    ui->OKButton->setEnabled(false);

    wordListDBPath = "";
    wordListTXTPath = "";
}

HomeDialog::~HomeDialog()
{
    delete ui;
}

void HomeDialog::on_OpenOldButton_clicked()
{
    if(ui->OldRadioButton->isChecked())
    {
        QString file_name = QFileDialog::getOpenFileName(this,
                tr("�򿪵��������б����ݿ�"), "./ForvoRealSpeak/WordListDB/", tr("���ݿ��ļ�(*.db)"));
        if (file_name.isEmpty())
        {
            return;
        }
        else if(!file_name.contains(".db", Qt::CaseInsensitive))
        {
            QMessageBox::warning(this, tr("������"),
             tr("��ѵ��������б����ݿ�ŵ�WordListDBĿ¼�²�����򿪣�"));
            return;
        }
        else
        {
            QFileInfo fd(file_name);
            wordListDBPath = fd.filePath();
            ui->OldFileLineEdit->setText(wordListDBPath);
            ui->OKButton->setEnabled(true);
            ui->OKButton->setFocus();
        }
    }
    else if(ui->NewRadioButton->isChecked())
    {
        QString file_name = QFileDialog::getOpenFileName(this,
                tr("��Ҫת���ĵ��������б�TXT�ļ�"), "./ForvoRealSpeak/WordListTXT/", tr("�ı��ļ�(*.txt)"));
        if (file_name.isEmpty())
        {
            return;
        }
        else if(!file_name.contains(".txt", Qt::CaseInsensitive))
        {
            QMessageBox::warning(this, tr("������"),
             tr("���Ҫת���ĵ��������б�TXT�ļ��ŵ�WordListTXTĿ¼�²�����򿪣�"));
            return;
        }
        else
        {
            QFileInfo fd(file_name);
            wordListTXTPath = fd.filePath();
            ui->OldFileLineEdit->setText(wordListTXTPath);
            ui->OKButton->setEnabled(true);
            ui->OKButton->setFocus();
        }
    }
}

void HomeDialog::on_OldRadioButton_clicked()
{
    ui->OpenOldButton->setEnabled(true);
    ui->OpenOldButton->setFocus();
}

void HomeDialog::on_NewRadioButton_clicked()
{
    ui->OpenOldButton->setEnabled(true);
    ui->OpenOldButton->setFocus();
}

QString HomeDialog::getWordListDBPath()
{
    return wordListDBPath;
}

QString HomeDialog::getWordListTXTPath()
{
    return wordListTXTPath;
}

void HomeDialog::on_OKButton_clicked()
{
    accept();
}

void HomeDialog::on_QuitButton_clicked()
{
    close();
}

bool HomeDialog::isNewFileCreate()
{
    if(ui->OldRadioButton->isChecked())
    {
        return false;
    }
    else if(ui->NewRadioButton->isChecked())
    {
        return true;
    }
    return false;
}

