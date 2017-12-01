#ifndef BRENDSDIALOG_H
#define BRENDSDIALOG_H

#include <QSqlQueryModel>

#include <QDialog>

namespace Ui {
class BrendsDialog;
}

class BrendsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrendsDialog(QWidget *parent = 0);
    ~BrendsDialog();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButtonClose_clicked();

    void on_listViewBrends_clicked(const QModelIndex &index);

    void on_pushButtonEdit_clicked();

    void on_pushButtonDel_clicked();

private:
    Ui::BrendsDialog *ui;
    QSqlQueryModel *modelBrends;
};

#endif // BRENDSDIALOG_H
