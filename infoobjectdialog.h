#ifndef INFOOBJECTDIALOG_H
#define INFOOBJECTDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSettings>
#include <QProcess>

struct azsInfo {
    int objectID;       // ID объекта
    QString brendName; // Бренд
    int terminalID;     //Номер терминала
    int regionID;       //Регион
    QString phone;
    QString name;
    QString comments;
};

namespace Ui {
class InfoObjectDialog;
}

class InfoObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoObjectDialog(QSqlRecord record, QString nameBrend,  QWidget *parent = 0);
    ~InfoObjectDialog();
private slots:
    void on_toolButtonAdd_clicked();

    void on_toolButtonEdit_clicked();

    void on_toolButton_clicked();

    void on_toolButtonRroEdit_clicked();

    void on_tableViewPC_doubleClicked(const QModelIndex &idx);

    void on_toolButtonDell_clicked();

    void on_toolButtonVnc_clicked();

    void on_toolButtonDelRro_clicked();
    void finVNC();

private:
    void createUI();
    void connectToObject(const QModelIndex &idx);   //Подключение по ПК по VNC
private:
    Ui::InfoObjectDialog *ui;
    azsInfo azs;
    QSqlQueryModel *modelRro;
    QSqlQueryModel *modelPC;
    QSettings *settings;
    QProcess *vncStart;
};

#endif // INFOOBJECTDIALOG_H
