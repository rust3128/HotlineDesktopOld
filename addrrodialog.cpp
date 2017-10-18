#include "addrrodialog.h"
#include "ui_addrrodialog.h"
#include <QMessageBox>
#include <QDebug>

addRroDialog::addRroDialog(int objID, int brID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addRroDialog)
{
    ui->setupUi(this);
    objectID=objID;
    brendID=brID;
    createUI();

    if(objectID==-1)
        newRro();
    else
        editRro();
}

addRroDialog::~addRroDialog()
{
    delete ui;
}

void addRroDialog::createUI()
{
    modTerminal = new QSqlQueryModel();
    modRroType = new QSqlQueryModel();
    QString strSQL = QString("select objectid, terminalid FROM objects where brendid=%1")
            .arg(brendID);
    modTerminal->setQuery(strSQL);
    ui->comboBoxTerminal->setModel(modTerminal);
    ui->comboBoxTerminal->setModelColumn(1);


    strSQL = "select name from rrotype";
    modRroType->setQuery(strSQL);
    ui->comboBoxModel->setModel(modRroType);


}

void addRroDialog::newRro()
{
    this->setWindowTitle("Добавление нового РРО");
    ui->comboBoxTerminal->setCurrentIndex(-1);
     ui->comboBoxTerminal->setCurrentText("Выберите терминал");
    ui->comboBoxModel->setCurrentIndex(-1);
    ui->radioButtonUkr->setChecked(true);

}

void addRroDialog::editRro()
{
    QSqlQuery q;
    QString strSQL = QString("select * from rro where rroid=%1").arg(objectID);
    this->setWindowTitle("Правка данных по РРО");
    if(!q.exec(strSQL)) qDebug() << "Не могу получить данные по РРО" << q.lastError().text();
    q.next();
    ui->comboBoxTerminal->setCurrentText(q.value("terminalid").toString().trimmed());
    ui->comboBoxTerminal->setDisabled(true);
    ui->comboBoxModel->setCurrentText(q.value("rrotype").toString().trimmed());
    qDebug() << "RRO type" << q.value("rrotype").toString().trimmed();
    ui->spinBoxPos->setValue(q.value("posid").toInt());
    ui->lineEditZn->setText(q.value("zn").toString());
    ui->lineEditFn->setText(q.value("fn").toString());
    ui->lineEditIdDev->setText(q.value("iddev").toString());
    ui->dateTimeEditPers->setDateTime(q.value("datepers").toDateTime());
    if(q.value("ekvaer").toString().trimmed() == "ИКС")
        ui->radioButtonIks->setChecked(true);
    else
        ui->radioButtonUkr->setChecked(true);


}




void addRroDialog::resetDlgData()
{
    ui->comboBoxTerminal->setCurrentIndex(-1);
    ui->comboBoxTerminal->setCurrentText("Выберите терминал");
    ui->comboBoxModel->setCurrentIndex(-1);
    ui->radioButtonUkr->setChecked(true);
    ui->lineEditFn->clear();
    ui->lineEditIdDev->clear();
    ui->lineEditZn->clear();
    ui->dateTimeEditPers->setDate(QDate(2010,1,1));

}

void addRroDialog::saveNewRro()
{
    QSqlQuery q;
    QString strSql;
    QString ekvaerName;

    if(ui->comboBoxTerminal->currentIndex()==-1) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбран терминал!"));
        qDebug() << "NO TERMINAL";
        return;
    }
    if(ui->comboBoxModel->currentIndex()==-1) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не выбрана модель РРО!"));
        return;
    }
    if(ui->lineEditZn->text() == "" || ui->lineEditZn->text().length() != 10) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не верно указан заводской номер РРО!"));
        return;
    }

    if(ui->radioButtonIks->isChecked())
        ekvaerName="ИКС";
    else
        ekvaerName="УкрКард";

    strSql = QString("INSERT INTO `hotline`.`rro` "
                     "(`brendid`, `objectid`, `terminalid`, `posid`, `rrotype`, `zn`, `fn`, `ekvaer`, `iddev`, `datepers`) "
                     "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10')")
            .arg(brendID)
            .arg(obj)
            .arg(ui->comboBoxTerminal->currentText().trimmed())
            .arg(ui->spinBoxPos->value())
            .arg(ui->comboBoxModel->currentText().trimmed())
            .arg(ui->lineEditZn->text().trimmed())
            .arg(ui->lineEditFn->text().trimmed())
            .arg(ekvaerName)
            .arg(ui->lineEditIdDev->text().trimmed())
            .arg(ui->dateTimeEditPers->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
//    qDebug() << "Str SQL" << endl << strSql;
    if(!q.exec(strSql)) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не удалось добавить РРО!\nПричина:\n%1")
                              .arg(q.lastError().text()));
        return;
    } else {
        QMessageBox::information(this,
                             QString::fromUtf8("Информация"),
                             QString::fromUtf8("Новый РРО успешно добавлен!"));
        this->close();
    }


}

void addRroDialog::updateRro()
{
    QSqlQuery q;
    QString strEkvaer;

    if(ui->radioButtonIks->isChecked())
        strEkvaer=ui->radioButtonIks->text();
    else
        strEkvaer=ui->radioButtonUkr->text();

    QString strSQL = QString("UPDATE rro SET `posid`='%1', `rrotype`='%2', `zn`='%3', `fn`='%4', "
                     "`ekvaer`='%5', `iddev`='%6', `datepers`='%7' "
                     "WHERE `rroid`='%8';")
            .arg(ui->spinBoxPos->value())
            .arg(ui->comboBoxModel->currentText().trimmed())
            .arg(ui->lineEditZn->text().trimmed())
            .arg(ui->lineEditFn->text().trimmed())
            .arg(strEkvaer)
            .arg(ui->lineEditIdDev->text().trimmed())
            .arg(ui->dateTimeEditPers->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(objectID);
    if(!q.exec(strSQL)) {
        QMessageBox::critical(this,
                             QString::fromUtf8("Ошибка!"),
                             QString::fromUtf8("Не удалось обновить информацию по РРО!\nПричина:\n%1")
                              .arg(q.lastError().text()));
        return;
    } else {
        QMessageBox::information(this,
                             QString::fromUtf8("Информация"),
                             QString::fromUtf8("Данные по РРО успешно обновлены!"));
        this->close();
    }


}

void addRroDialog::on_comboBoxTerminal_activated(int index)
{
    QModelIndex indexModel=modTerminal->index(index,0,QModelIndex());
    obj=modTerminal->data(indexModel, Qt::DisplayRole).toInt();
//    qDebug() << "ObjectID" << objectID;
}

void addRroDialog::on_buttonBoxRro_clicked(QAbstractButton *button)
{
    switch (ui->buttonBoxRro->standardButton(button)) {
    case QDialogButtonBox::Save:
        if(objectID==-1)
            saveNewRro();
        else
            updateRro();
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
