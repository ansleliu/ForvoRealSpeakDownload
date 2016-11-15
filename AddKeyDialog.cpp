#include "AddKeyDialog.h"
#include "ui_AddKeyDialog.h"

AddKeyDialog::AddKeyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddKeyDialog)
{
    ui->setupUi(this);
}

AddKeyDialog::~AddKeyDialog()
{
    delete ui;
}

void AddKeyDialog::on_AcceptButton_clicked()
{
    accept();
}

void AddKeyDialog::on_QuitButton_clicked()
{
    reject();
}

QString AddKeyDialog::getKeyValue()
{
    return keyValue = ui->KeyValueLineEdit->text();
}

QString AddKeyDialog::getKeyName()
{
    return keyName = ui->KeyNameLineEdit->text();
}
