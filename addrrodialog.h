#ifndef ADDRRODIALOG_H
#define ADDRRODIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QAbstractButton>

namespace Ui {
class addRroDialog;
}

class addRroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addRroDialog(int objID, int brID, QWidget *parent = 0);
    ~addRroDialog();
private slots:
    void on_comboBoxTerminal_activated(int index);

    void on_buttonBoxRro_clicked(QAbstractButton *button);


private:
    void newRro();
    void editRro();
    void createUI();
    void resetDlgData();
    void saveNewRro();
    void updateRro();
private:
    Ui::addRroDialog *ui;
    int objectID;
    int obj;
    int brendID;
    QSqlQueryModel *modTerminal;
    QSqlQueryModel *modRroType;
};

#endif // ADDRRODIALOG_H
