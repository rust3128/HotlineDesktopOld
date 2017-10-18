#include "objectwindow.h"
#include "ui_objectwindow.h"
#include "addazsdialog.h"
#include "addrrodialog.h"
#include "infoobjectdialog.h"
#include <QDebug>
#include <QMessageBox>


ObjectWindow::ObjectWindow(int brendID, QString brendName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectWindow)
{
    ui->setupUi(this);
    brendid=brendID;
    namebrend=brendName;
    createUI();
    connect(ui->tableViewRro, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(on_tableViewRro_customContextMenuRequested(QPoint)));
}

ObjectWindow::~ObjectWindow()
{
    delete ui;
}

void ObjectWindow::createUI()
{
    modelAzs = new QSqlTableModel();
    modelRro = new QSqlTableModel();
    createUIRegions();
    filterBrend=QString("brendid=%1").arg(brendid);
    ui->labelBrends->setText(namebrend);
    ///// Инициализация вкладки АЗС
    modelAzs->setTable("objects");
    modelAzs->select();
    modelAzs->setFilter(filterBrend);
    modelAzs->setHeaderData(3,Qt::Horizontal,"Терминал");
    modelAzs->setHeaderData(4,Qt::Horizontal,"Расположение");
    modelAzs->setHeaderData(5,Qt::Horizontal,"Телефоны");
    modelAzs->setHeaderData(6,Qt::Horizontal,"Примечание");

    ui->tableViewAzs->setModel(modelAzs);
    ui->tableViewAzs->verticalHeader()->hide();
    ui->tableViewAzs->hideColumn(0);
    ui->tableViewAzs->hideColumn(1);
    ui->tableViewAzs->hideColumn(2);
    ui->tableViewAzs->hideColumn(7);
    ui->tableViewAzs->resizeColumnsToContents();
    ui->tableViewAzs->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewAzs->verticalHeader()->setDefaultSectionSize(ui->tableViewAzs->verticalHeader()->minimumSectionSize());
//    ui->tableViewAzs->setCurrentIndex(modelAzs->index(0,0));
    ui->tableViewAzs->selectRow(0);
    ///Инициализация вкладки РРО
    modelRro->setTable("rro");
    modelRro->setSort(3,Qt::AscendingOrder);
    modelRro->select();
    modelRro->setFilter(filterBrend);


    modelRro->setHeaderData(3,Qt::Horizontal,"Терминал");
    modelRro->setHeaderData(4,Qt::Horizontal,"POS ID");
    modelRro->setHeaderData(5,Qt::Horizontal,"Тип Кассы");
    modelRro->setHeaderData(6,Qt::Horizontal,"ЗН");
    modelRro->setHeaderData(7,Qt::Horizontal,"Изготовлена");
    modelRro->setHeaderData(8,Qt::Horizontal,"ФН");
    modelRro->setHeaderData(9,Qt::Horizontal,"Ввод в\nэксплуатацию");
    modelRro->setHeaderData(10,Qt::Horizontal,"Экваер");
    modelRro->setHeaderData(11,Qt::Horizontal,"ID DEV");
    modelRro->setHeaderData(12,Qt::Horizontal,"Персонализирован");



    ui->tableViewRro->setModel(modelRro);
    ui->tableViewRro->verticalHeader()->hide();
    ui->tableViewRro->hideColumn(0);
    ui->tableViewRro->hideColumn(1);
    ui->tableViewRro->hideColumn(2);
    ui->tableViewRro->hideColumn(7);
    ui->tableViewRro->hideColumn(9);
    ui->tableViewRro->resizeColumnsToContents();

    ui->tableViewRro->verticalHeader()->setDefaultSectionSize(ui->tableViewRro->verticalHeader()->minimumSectionSize());
    ui->tableViewRro->selectRow(0);
//    spanRro();

}

void ObjectWindow::createUIRegions()
{
    QString strSQL;
    modelRegions = new QSqlQueryModel();
    strSQL = QString("SELECT ownerid, name FROM regions WHERE brendid=%1").arg(brendid);
    modelRegions->setQuery(strSQL);
    ui->comboBoxRegion->setModel(modelRegions);
    ui->comboBoxRegion->setModelColumn(1);
    ui->comboBoxRegion->setCurrentIndex(-1);
    ui->checkBoxShowAll->setChecked(true);
    regionID=-1;
}

void ObjectWindow::spanRro()
{
    int currTerm = modelRro->data(modelRro->index(0,3)).toInt();
    int nextTerm;
    int rowSpanBeg=0;
    int rowSpanCol=1;
    for(int i=1; i<=modelRro->rowCount();i++) {
        nextTerm = modelRro->data(modelRro->index(i,3)).toInt();
        if(currTerm==nextTerm ) {
            rowSpanCol++;
        }
        else {
           ui->tableViewRro->setSpan(rowSpanBeg,3,rowSpanCol,1);
           rowSpanCol=1;
           currTerm=nextTerm;
           rowSpanBeg=i;
        }
    }
}

void ObjectWindow::on_actionAdd_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        newAzsData();
        break;
    case 1:
        newRroData();
        break;
    default:
        break;
    }

}

void ObjectWindow::newAzsData()
{
    AddAzsDialog *addAzsDlg = new AddAzsDialog(-1,brendid);
    addAzsDlg->exec();
    modelAzs->select();
    modelAzs->setFilter(filterBrend);
}

void ObjectWindow::newRroData()
{
    addRroDialog *addRroDlg =  new addRroDialog(-1,brendid);
    addRroDlg->exec();
    modelRro->select();
    modelRro->setFilter(filterBrend);
}


void ObjectWindow::on_actionEdit_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        editAzsData();
        break;
    case 1:
        editRroData();
        break;
    default:
        break;
    }

}

void ObjectWindow::editAzsData()
{

    QModelIndex idx = ui->tableViewAzs->selectionModel()->currentIndex();
    int objID = modelAzs->data(modelAzs->index(idx.row(),0)).toInt();
//    qDebug() << "objectID" << objID << "Вкладка" << ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(objID==0){
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбран терминал для редактирования!"));
        return;
    }
    AddAzsDialog *addAzsDlg = new AddAzsDialog(objID,brendid,this);
    addAzsDlg->exec();
    modelAzs->select();
    modelAzs->setFilter(filterBrend);
}

void ObjectWindow::editRroData()
{
    QModelIndex idx = ui->tableViewRro->selectionModel()->currentIndex();
    int objID = modelRro->data(modelRro->index(idx.row(),0)).toInt();
    if(objID==0){
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбран РРО для редактирования!"));
        return;
    }
    addRroDialog *editRroDlg = new addRroDialog(objID,brendid);
    editRroDlg->exec();
    modelRro->select();
    modelRro->setFilter(filterBrend);
}



void ObjectWindow::on_tableViewAzs_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionAdd);
    menu->addAction(ui->actionEdit);
    menu->addSeparator();
    menu->addAction(ui->actionDel);
    menu->popup(ui->tableViewAzs->viewport()->mapToGlobal(pos));
}

void ObjectWindow::on_actionDel_triggered()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        delAzs();
        break;
    case 1:
        delRro();
        break;
    default:
        break;
    }
}

void ObjectWindow::delAzs()
{
    QSqlQuery q;
    QModelIndex idx = ui->tableViewAzs->selectionModel()->currentIndex();
    int objID = modelAzs->data(modelAzs->index(idx.row(),0)).toInt();
    int termID = modelAzs->data(modelAzs->index(idx.row(),3)).toInt();
    QString adress = modelAzs->data(modelAzs->index(idx.row(),4)).toString();
    QString strSql = QString("DELETE FROM objects WHERE `objectid`='%1'").arg(objID);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Удаление АЗС"),
                          QString::fromUtf8("Вы действительно хотите удалить информацию по АЗС %1\n%2")
                                  .arg(termID)
                                  .arg(adress),
                          QMessageBox::Yes | QMessageBox::No);

    switch (reply) {
    case QMessageBox::Yes:
        q.exec(strSql);
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        break;
    }
    modelAzs->select();
    modelAzs->setFilter(filterBrend);
}

void ObjectWindow::delRro()
{
    QSqlQuery q;
    QModelIndex idx = ui->tableViewRro->selectionModel()->currentIndex();
    int rroID = modelRro->data(modelRro->index(idx.row(),0)).toInt();
    QString zn = modelRro->data(modelRro->index(idx.row(),6)).toString();

    QString strSql = QString("DELETE FROM rro WHERE `rroid`='%1'").arg(rroID);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Удаление РРО"),
                          QString::fromUtf8("Вы действительно хотите удалить РРО с ЗН %1")
                                  .arg(zn),
                          QMessageBox::Yes | QMessageBox::No);

    switch (reply) {
    case QMessageBox::Yes:
        q.exec(strSql);
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        break;
    }
    modelRro->select();
    modelRro->setFilter(filterBrend);
}

void ObjectWindow::on_tableViewRro_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->actionAdd);
    menu->addAction(ui->actionEdit);
    menu->addSeparator();
    menu->addAction(ui->actionDel);
    menu->popup(ui->tableViewRro->viewport()->mapToGlobal(pos));
}



void ObjectWindow::on_tableViewAzs_doubleClicked(const QModelIndex &idx)
{
//    int objID = modelRro->data(modelRro->index(idx.row(),0)).toInt();
//    modelAzs->record(idx.row());
    InfoObjectDialog *infObjDlg = new InfoObjectDialog(modelAzs->record(idx.row()),namebrend);
    infObjDlg->exec();
}

void ObjectWindow::on_comboBoxRegion_activated(int idx)
{
    QModelIndex indexModel=modelRegions->index(idx,0,QModelIndex());
    regionID=modelRegions->data(indexModel, Qt::DisplayRole).toInt();
//    qDebug() << "Region id " << regionID;
    modelAzs->setFilter(filterBrend+QString(" and regionid=%1").arg(regionID));
    ui->checkBoxShowAll->setChecked(false);
}

void ObjectWindow::on_checkBoxShowAll_clicked()
{
    if(ui->checkBoxShowAll->isChecked()){
        ui->comboBoxRegion->setCurrentIndex(-1);
        modelAzs->setFilter(filterBrend);
    }
}
