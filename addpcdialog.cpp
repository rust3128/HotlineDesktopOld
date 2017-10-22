#include "addpcdialog.h"
#include "ui_addpcdialog.h"
#include "qdynamicbutton.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



AddPCDialog::AddPCDialog(int objID, int pc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPCDialog)
{
    ui->setupUi(this);
    objectID=objID;
    pcID=pc;
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    rroTypeID=-1;
    /* Создаем строку для регулярного выражения */
        QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
        /* Создаем регулярное выражение с применением строки, как
         * повторяющегося элемента
         */
        QRegExp ipRegex ("^" + ipRange
                         + "\\." + ipRange
                         + "\\." + ipRange
                         + "\\." + ipRange + "$");
        /* Создаем Валидатор регулярного выражения с применением
         * созданного регулярного выражения
         */
        QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
        /* Устанавливаем Валидатор на QLineEdit */
        ui->lineEditIP->setValidator(ipValidator);
        ui->lineEditVNCPass->setText("88888888");
       if(pcID >=0) {
           existPc();
       }

       showPCType();
}

AddPCDialog::~AddPCDialog()
{
    delete ui;
}

void AddPCDialog::showPCType()
{
    QSqlQuery q("SELECT pctypeid, typename FROM pctype");
    q.exec();
    while(q.next()) {
        QDynamicRadioButton *button = new QDynamicRadioButton(q.value(0).toInt(),this);
        button->setText(q.value(1).toString());
        ui->gridLayout->addWidget(button);
        if(rroTypeID==button->gettypeID()){
            button->setChecked(true);
        }
        connect(button,SIGNAL(clicked()),this,SLOT(slotGetNumberButton()));

    }
    q.finish();
}

void AddPCDialog::on_buttonBox_clicked(QAbstractButton *button)
{


    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        if(pcID<0)
            addNewPC();
        else
            editPC();
        this->close();
        break;
    case QDialogButtonBox::Cancel:
        this->close();
        break;
    default:
        break;
    }
}

void AddPCDialog::addNewPC()
{
    QString strSQL;
    QSqlQuery q;
    if(ui->lineEditIP->text().length()==0) {
        qDebug() << "Не верно указан IP Адрес.";
    }
    strSQL = QString("INSERT INTO computers (objectid, pctypeid, ip, vncpass) VALUES (%1, %2, INET_ATON('%3'), %4)")
            .arg(objectID)
            .arg(rroTypeID)
            .arg(ui->lineEditIP->text().trimmed())
            .arg(ui->lineEditVNCPass->text().trimmed());
//    qDebug() << strSQL;
    if(!q.exec(strSQL)) qDebug() << "Не удалось добавить компьютер" << q.lastError().text();

}

void AddPCDialog::existPc()
{
    QString strSQL = QString("SELECT pctypeid, INET_NTOA(ip), vncpass FROM computers WHERE pcid=%1")
            .arg(pcID);
    QSqlQuery q;
    if(!q.exec(strSQL)) qDebug() << "Не удалось получить информацию о РС" << q.lastError().text();
    q.next();
    rroTypeID=q.value(0).toInt();
    ui->lineEditIP->setText(q.value(1).toString());
    ui->lineEditVNCPass->setText(q.value(2).toString());
}

void AddPCDialog::editPC()
{
    QString strSQL;
    QSqlQuery q;
    strSQL = QString("UPDATE `hotline`.`computers` SET `pctypeid`='%1', `ip`=INET_ATON('%2'), `vncpass`='%3' WHERE `pcid`='%4'")
            .arg(rroTypeID)
            .arg(ui->lineEditIP->text().trimmed())
            .arg(ui->lineEditVNCPass->text().trimmed())
            .arg(pcID);
//    qDebug() << strSQL;
    if(!q.exec(strSQL)) qDebug() << "Не удалось обновить информацию о ПК" << q.lastError().text();

}


void AddPCDialog::slotGetNumberButton()
{
    QDynamicRadioButton *button = (QDynamicRadioButton*)sender();
    rroTypeID=button->gettypeID();
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
//    qDebug() << "PCType ID" << rroTypeID;
}





void AddPCDialog::on_lineEditIP_textChanged()
{
     ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
}
