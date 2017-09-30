#ifndef ADDAZSDIALOG_H
#define ADDAZSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QAbstractButton>

namespace Ui {
class AddAzsDialog;
}

class AddAzsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAzsDialog(int objID, int brID, QWidget *parent = 0);
    ~AddAzsDialog();
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_comboBoxRegion_activated(int index);

private:
    void newAzs();
    void editAzs();
    void createUI();
    void saveNewAzs();
    void resetDlgData();
    void updateAzs();
private:
    Ui::AddAzsDialog *ui;
    QSqlQueryModel *modelRegions;
    int objectID;
    int brendID;
    int regionID;


};

#endif // ADDAZSDIALOG_H
