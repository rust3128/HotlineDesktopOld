#include "addazsdialog.h"
#include "ui_addazsdialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

AddAzsDialog::AddAzsDialog(int objID, int brID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddAzsDialog)
{
    ui->setupUi(this);
    objectID=objID;
    brendID=brID;
    createUI();

    if(objID==-1)
        newAzs();
    else
        editAzs();

}

void AddAzsDialog::createUI()
{
    QString strSQL;
    ///////список регионов
    modelRegions = new QSqlQueryModel();
    strSQL = QString("SELECT ownerid, name FROM regions WHERE brendid=%1").arg(brendID);
    modelRegions->setQuery(strSQL);
    ui->comboBoxRegion->setModel(modelRegions);
    ui->comboBoxRegion->setModelColumn(1);


    QRegExp terminalRegExp("[0-9]+");
    QRegExpValidator *terminalValidator = new QRegExpValidator(terminalRegExp,this);
    ui->lineEditTerminalID->setValidator(terminalValidator);
}


AddAzsDialog::~AddAzsDialog()
{
    delete ui;
}

void AddAzsDialog::newAzs()
{
    this->setWindowTitle("Добавление новой АЗС");
    ui->comboBoxRegion->setCurrentIndex(-1);

}

void AddAzsDialog::editAzs()
{
    QSqlQuery q;
    QModelIndex indexModel;
    QString strSQL = QString("SELECT regionid,terminalid,adress,phone,comments FROM objects WHERE objectid=%1")
            .arg(objectID);
    this->setWindowTitle("Редактирование данных по АЗС");
    q.exec(strSQL);
    q.next();
    ui->lineEditTerminalID->setText(q.value(1).toString());
    ui->lineEditAdress->setText(q.value(2).toString().trimmed());
    ui->lineEditPhone->setText(q.value(3).toString().trimmed());
    ui->plainTextEditComments->setPlainText(q.value(4).toString().trimmed());
    regionID=q.value(0).toInt();
    for(int i=0; i<modelRegions->rowCount();++i) {
        indexModel=modelRegions->index(i,0,QModelIndex());
        if(modelRegions->data(indexModel).toInt()==regionID){
            ui->comboBoxRegion->setCurrentIndex(i);
            break;
        }
    }

}



void AddAzsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        if(objectID==-1)
            saveNewAzs();
        else
            updateAzs();
        break;
    case QDialogButtonBox::Reset:
        resetDlgData();
        break;
    case QDialogButtonBox::Cancel:
        this->close();
        break;
    default:
        break;
    }
}

void AddAzsDialog::saveNewAzs()
{
    QSqlQuery q;
    QString strSQL;

    if(ui->comboBoxRegion->currentIndex()==-1) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбран регион!"));
        return;
    }
    if(ui->lineEditTerminalID->text().toInt()<=1000) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Неверный номер терминала!"));
        return;
    }

    strSQL = QString("INSERT INTO objects(brendid, regionid, terminalid, adress, phone, comments) VALUES(%1, %2, %3, '%4', '%5', '%6')")
            .arg(brendID)
            .arg(regionID)
            .arg(ui->lineEditTerminalID->text().trimmed())
            .arg(ui->lineEditAdress->text().trimmed())
            .arg(ui->lineEditPhone->text().trimmed())
            .arg(ui->plainTextEditComments->toPlainText());
    qDebug() << "SQL" << strSQL;
    if(!q.exec(strSQL))
        qDebug() << "Не могу добавить новый объект." << q.lastError().text();
    else
        QMessageBox::information(this,
                             QString::fromUtf8("Информация"),
                             QString::fromUtf8("Информация об новой АЗС успешно добалена!"));
    this->close();
}


void AddAzsDialog::resetDlgData()
{
    ui->comboBoxRegion->setCurrentIndex(-1);
    ui->lineEditTerminalID->clear();
    ui->lineEditAdress->clear();
    ui->lineEditPhone->clear();
    ui->plainTextEditComments->clear();
}

void AddAzsDialog::updateAzs()
{
    QSqlQuery q;
    QString strSQL;

    if(ui->comboBoxRegion->currentIndex()==-1) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбран регион!"));
        return;
    }
    if(ui->lineEditTerminalID->text().toInt()<=1000) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Неверный номер терминала!"));
        return;
    }

    strSQL = QString("UPDATE objects SET regionid='%1', terminalid='%2', adress='%3', phone='%4', comments='%5' WHERE objectid='%6'")
                             .arg(regionID)
                             .arg(ui->lineEditTerminalID->text().trimmed())
                             .arg(ui->lineEditAdress->text().trimmed())
                             .arg(ui->lineEditPhone->text().trimmed())
                             .arg(ui->plainTextEditComments->toPlainText())
                             .arg(objectID) ;
     qDebug() << "SQL" << strSQL;
    if(!q.exec(strSQL))
        qDebug() << "Не могу добавить новый объект." << q.lastError().text();
    else
        QMessageBox::information(this,
                             QString::fromUtf8("Информация"),
                             QString::fromUtf8("Информация об АЗС оспешно обновлена."));
    this->close();
}

void AddAzsDialog::on_comboBoxRegion_activated(int index)
{
    QModelIndex indexModel=modelRegions->index(index,0,QModelIndex());
    regionID=modelRegions->data(indexModel, Qt::DisplayRole).toInt();
    qDebug() << "Region ID" << regionID;
}
