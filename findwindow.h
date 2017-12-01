#ifndef FINDWINDOW_H
#define FINDWINDOW_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>

namespace Ui {
class FindWindow;
}

class FindWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FindWindow(QWidget *parent = 0);
    ~FindWindow();
private slots:
    void on_lineEditFind_textChanged(const QString &arg1);

private:
    void createUI();
    void setupModelRro();
private:
    Ui::FindWindow *ui;
    QSqlRelationalTableModel *modelRro;
    QString filter;

};

#endif // FINDWINDOW_H
