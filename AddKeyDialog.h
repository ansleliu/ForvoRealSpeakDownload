#ifndef ADDKEYDIALOG_H
#define ADDKEYDIALOG_H

#include <QDialog>

namespace Ui {
class AddKeyDialog;
}

class AddKeyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddKeyDialog(QWidget *parent = 0);
    ~AddKeyDialog();
    
private slots:
    void on_AcceptButton_clicked();

    void on_QuitButton_clicked();

public:
    QString getKeyValue();
    QString getKeyName();

private:
    Ui::AddKeyDialog *ui;

    QString keyValue;
    QString keyName;
};

#endif // ADDKEYDIALOG_H
