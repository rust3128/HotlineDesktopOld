#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOptions_triggered();
    void on_actionBrends_triggered();
    void slotGetNumberButton();

    void on_actionAbout_triggered();

    void on_action_Qt_triggered();

private:
    void setToolBarBrends();
private:
    Ui::MainWindow *ui;
    QMdiArea *mdiArea;  // Область, в которую будут добавляться окна

};

#endif // MAINWINDOW_H
