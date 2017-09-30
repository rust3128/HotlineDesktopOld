#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "brendsdialog.h"
#include "qdynamicbutton.h"
#include "objectwindow.h"
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mdiArea = new QMdiArea(this);  // инициализируем QMdiArea
    // Настраиваем скроллбары
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Устанавливаем Mdi Area в качестве центрального виджета
    setCentralWidget(mdiArea);

    setToolBarBrends();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOptions_triggered()
{
    ConnectDialog *connDlg = new ConnectDialog();
    connDlg->exec();
}

void MainWindow::on_actionBrends_triggered()
{
    BrendsDialog *brendsDlg = new BrendsDialog();
    brendsDlg->exec();
}

void MainWindow::slotGetNumberButton()
{
    QSqlQuery q;
    QDynamicButton *button = (QDynamicButton*)sender();
    int brendID=button->getButtonID();
    q.prepare("UPDATE brends SET rating = rating +1 WHERE brendid=:brendid");
    q.bindValue(":brendid", brendID);
    q.exec();
    q.finish();
    ObjectWindow *objWin = new ObjectWindow(brendID, button->text());
    objWin->setWindowTitle(button->text());
    mdiArea->addSubWindow(objWin);
    objWin->show();


}

void MainWindow::setToolBarBrends()
{
    QSqlQuery q("SELECT brendid, name, rating FROM brends WHERE isactive=1 ORDER BY rating DESC");
    q.exec();
    while(q.next()) {
        QDynamicButton *button = new QDynamicButton(q.value(0).toInt(),this);
        button->setText(q.value(1).toString());
        ui->toolBarBrends->addWidget(button);
        connect(button,SIGNAL(clicked()),this,SLOT(slotGetNumberButton()));

    }
    q.finish();
}
