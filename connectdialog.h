#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>




namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

private slots:
    void on_checkBoxEdit_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_toolButton_clicked();

signals:
    void closeMainWin(void);

private:
    Ui::ConnectDialog *ui;
    QSettings *settings;
    bool editing;

private:
    void setupUI();
    void writeSettings();

};

#endif // CONNECTDIALOG_H
