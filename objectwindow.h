#ifndef OBJECTWINDOW_H
#define OBJECTWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

namespace Ui {
class ObjectWindow;
}

class ObjectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ObjectWindow(int brendID, QString brendName, QWidget *parent = 0);
    ~ObjectWindow();

private slots:
    void on_actionAdd_triggered();
    void on_actionEdit_triggered();
    void on_tableViewAzs_customContextMenuRequested(const QPoint &pos);
    void on_actionDel_triggered();
    void on_tableViewRro_customContextMenuRequested(const QPoint &pos);



    void on_tableViewAzs_doubleClicked(const QModelIndex &idx);

private:
    void createUI();
    void spanRro();
    void newAzsData();
    void editAzsData();
    void newRroData();
    void editRroData();
    void delAzs();
    void delRro();

private:
    Ui::ObjectWindow *ui;
    int brendid;            //Текущий бренд
    QString namebrend;      //Наименование текущего бренда
    QSqlTableModel *modelAzs;   //Модель АЗС
    QSqlTableModel *modelRro;   //Модель РРО
    QString filterBrend;      //Фильтр для выбора АЗС конкретного бренда

};

#endif // OBJECTWINDOW_H
