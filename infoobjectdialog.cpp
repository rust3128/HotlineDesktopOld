#include "infoobjectdialog.h"
#include "ui_infoobjectdialog.h"
#include "addpcdialog.h"
#include <QDebug>

InfoObjectDialog::InfoObjectDialog(QSqlRecord record, QString nameBrend, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoObjectDialog)
{
    ui->setupUi(this);
    azs.brendName=nameBrend;
    azs.objectID=record.value("objectid").toInt();
    azs.terminalID=record.value("terminalid").toInt();
    azs.name=record.value("adress").toString();
    azs.phone=record.value("phone").toString();
    azs.comments=record.value("comments").toString();
    this->setWindowTitle("Информация об АЗС "+QString::number(azs.terminalID)+" "+azs.brendName);

    createUI();
//    qDebug() << record;
}

InfoObjectDialog::~InfoObjectDialog()
{
    delete ui;
}

void InfoObjectDialog::createUI()
{
    modelRro = new QSqlQueryModel();
    QString strSql = QString("SELECT rroid, posid, rrotype, zn, fn, ekvaer, iddev "
                             "FROM rro WHERE objectid=%1").arg(azs.objectID);

    ui->labelBrend->setText(azs.brendName);
    ui->labelAzs->setText("АЗС "+QString::number(azs.terminalID));
    ui->labelAdress->setText(azs.name);
    ui->labelPhone->setText("Телефоны: "+azs.phone);
    ui->labelComments->setText(azs.comments);

    modelRro->setQuery(strSql);
    modelRro->setHeaderData(1,Qt::Horizontal,"POS ID");
    modelRro->setHeaderData(2,Qt::Horizontal,"Тип Кассы");
    modelRro->setHeaderData(3,Qt::Horizontal,"ЗН");
    modelRro->setHeaderData(4,Qt::Horizontal,"ФН");
    modelRro->setHeaderData(5,Qt::Horizontal,"Экваер");
    modelRro->setHeaderData(6,Qt::Horizontal,"ID DEV");
    ui->tableViewRro->setModel(modelRro);
    ui->tableViewRro->verticalHeader()->hide();
    ui->tableViewRro->hideColumn(0);
    ui->tableViewRro->resizeColumnsToContents();
    ui->tableViewRro->verticalHeader()->setDefaultSectionSize(ui->tableViewRro->verticalHeader()->minimumSectionSize());
    ui->tableViewRro->selectRow(0);

    ui->labelColRro->setText("Кассовх аппаратов "+QString::number(modelRro->rowCount())+".");


}

void InfoObjectDialog::on_toolButtonAdd_clicked()
{
    AddPCDialog *addPcDlg = new AddPCDialog();
    addPcDlg->exec();
}
