#ifndef HOMEDIALOG_H
#define HOMEDIALOG_H

#include <QDialog>

namespace Ui {
class HomeDialog;
}

class HomeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit HomeDialog(QWidget *parent = 0);
    ~HomeDialog();
    
private slots:
    void on_OpenOldButton_clicked();

    void on_OldRadioButton_clicked();

    void on_NewRadioButton_clicked();

    void on_OKButton_clicked();

    void on_QuitButton_clicked();

public:
    bool isNewFileCreate();
    QString getWordListDBPath();
    QString getWordListTXTPath();

private:
    Ui::HomeDialog *ui;

    QString wordListDBPath;
    QString wordListTXTPath;
};

#endif // HOMEDIALOG_H
