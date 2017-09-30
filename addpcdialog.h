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
    explicit AddPCDialog(int objID, QWidget *parent = 0);
    ~AddPCDialog();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void slotGetNumberButton();
private:
    void showPCType();
    void addNewPC();
private:
    Ui::AddPCDialog *ui;
    int rroTypeID;
    int objectID;
};

#endif // ADDPCDIALOG_H
