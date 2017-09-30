#ifndef ADDPCDIALOG_H
#define ADDPCDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class AddPCDialog;
}

class AddPCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPCDialog(QWidget *parent = 0);
    ~AddPCDialog();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void slotGetNumberButton();
private:
    void showPCType();
private:
    Ui::AddPCDialog *ui;
    int rroTypeID;
};

#endif // ADDPCDIALOG_H
