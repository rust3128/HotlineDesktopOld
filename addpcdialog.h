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
    explicit AddPCDialog(int objID, int pc, QWidget *parent = 0);
    ~AddPCDialog();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);
    void slotGetNumberButton();
    void on_lineEditIP_textChanged();

private:
    void showPCType();
    void addNewPC();
    void existPc();
    void editPC();
private:
    Ui::AddPCDialog *ui;
    int rroTypeID;
    int objectID;
    int pcID;
};

#endif // ADDPCDIALOG_H
