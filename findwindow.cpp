#include "findwindow.h"
#include "ui_findwindow.h"

FindWindow::FindWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    createUI();
    filter="ZN LIKE ('') OR FN LIKE ('')";
    setupModelRro();
}

FindWindow::~FindWindow()
{
    delete ui;
}

void FindWindow::createUI()
{
    ui->checkBoxFN->setChecked(true);
    ui->checkBoxZN->setChecked(true);
}

void FindWindow::setupModelRro()
{
    modelRro = new QSqlRelationalTableModel(this);
    modelRro->setTable("rro");
    modelRro->setHeaderData(1,Qt::Horizontal,"Владелец");
    modelRro->setHeaderData(3,Qt::Horizontal,"АЗС");
    modelRro->setHeaderData(4,Qt::Horizontal,"ПОС");
    modelRro->setHeaderData(5,Qt::Horizontal,"ЭККР");
    modelRro->setHeaderData(6,Qt::Horizontal,"ЗН");
    modelRro->setHeaderData(8,Qt::Horizontal,"ФН");
    modelRro->setHeaderData(10,Qt::Horizontal,"Экваер");
    modelRro->setHeaderData(12,Qt::Horizontal,"Персонализация");
    modelRro->setRelation(1, QSqlRelation("brends","brendid", "name" ));
    modelRro->setSort(0,Qt::AscendingOrder);
    modelRro->select();
    modelRro->setFilter(filter);

    ui->tableViewRezult->setModel(modelRro);
    ui->tableViewRezult->setItemDelegate(new QSqlRelationalDelegate(ui->tableViewRezult));
    ui->tableViewRezult->verticalHeader()->hide();
    ui->tableViewRezult->hideColumn(0);
    ui->tableViewRezult->hideColumn(2);
    ui->tableViewRezult->hideColumn(7);
    ui->tableViewRezult->hideColumn(9);
    ui->tableViewRezult->hideColumn(11);

    ui->tableViewRezult->resizeColumnsToContents();
    ui->tableViewRezult->verticalHeader()->setDefaultSectionSize(ui->tableViewRezult->verticalHeader()->minimumSectionSize());
}

void FindWindow::on_lineEditFind_textChanged(const QString &arg1)
{
    if(arg1.length()>=3){
        filter =QString("ZN LIKE ('%%1%') OR FN LIKE ('%%1%')").arg(arg1);
    } else {
        filter="ZN LIKE ('') OR FN LIKE ('')";
    }
    modelRro->select();
    modelRro->setFilter(filter);
    ui->tableViewRezult->resizeColumnsToContents();
}
