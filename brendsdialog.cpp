#include "brendsdialog.h"
#include "ui_brendsdialog.h"

#include <QInputDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QModelIndex>
#include <QMessageBox>

BrendsDialog::BrendsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrendsDialog)
{
    ui->setupUi(this);
    modelBrends = new QSqlQueryModel();
    modelBrends->setQuery("SELECT name FROM brends WHERE isactive=1");

    ui->listViewBrends->setModel(modelBrends);
    ui->listViewBrends->setSelectionMode(QAbstractItemView::ExtendedSelection);

    ui->pushButtonEdit->hide();
    ui->pushButtonDel->hide();

}

BrendsDialog::~BrendsDialog()
{
    delete ui;
}

void BrendsDialog::on_pushButtonAdd_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString::fromUtf8("Добавление нового клиента"),
                                 QString::fromUtf8("Наименование клиента"),
                                 QLineEdit::Normal,
                                 "", &ok);
    if (ok && !text.isEmpty()){
        QSqlQuery q;
//        qDebug() << "Ввели " << text.trimmed();
        q.prepare("INSERT INTO brends SET name=:name");
        q.bindValue(":name",text.trimmed());
        if(!q.exec()) qDebug() << "Не удалось добавить клиента" << q.lastError().text();
        modelBrends->setQuery(modelBrends->query().lastQuery());
    }


}

void BrendsDialog::on_pushButtonClose_clicked()
{
    this->reject();
}

void BrendsDialog::on_listViewBrends_clicked(const QModelIndex &index)
{
    ui->pushButtonEdit->show();
    ui->pushButtonDel->show();

}

void BrendsDialog::on_pushButtonEdit_clicked()
{

    QModelIndex indexModel = ui->listViewBrends->currentIndex();
    QString currentBrebd = modelBrends->data(indexModel,Qt::DisplayRole).toString();
    bool ok;
    QString text = QInputDialog::getText(this,
                                 QString::fromUtf8("Правка"),
                                 QString::fromUtf8("Введите новое наименование:"),
                                 QLineEdit::Normal,
                                 currentBrebd, &ok);
    if (ok && !text.isEmpty()){
        QSqlQuery q;
        q.prepare("UPDATE brends SET name = :newName WHERE name=:oldName");
        q.bindValue(":newName",text.trimmed());
        q.bindValue(":oldName", currentBrebd);
        if(!q.exec()) qDebug() << "Не удалось обновить данные по клиенту" << q.lastError().text();
        modelBrends->setQuery(modelBrends->query().lastQuery());

    }
}

void BrendsDialog::on_pushButtonDel_clicked()
{
    QModelIndex indexModel = ui->listViewBrends->currentIndex();
    QString currentBrebd = modelBrends->data(indexModel,Qt::DisplayRole).toString();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Удаление клиента"),
                          QString::fromUtf8("Вы действительно хотите удалить клиента ")+currentBrebd,
                          QMessageBox::Yes | QMessageBox::No );

    if (reply == QMessageBox::Yes) {
        QSqlQuery q;
        q.prepare("UPDATE brends SET isactive=0 WHERE name=:oldName");
        q.bindValue(":oldName", currentBrebd);
        if(!q.exec()) qDebug() << "Не удалось деактивировать клиента" << q.lastError().text();
        modelBrends->setQuery(modelBrends->query().lastQuery());
    }
//    else if (reply == QMessageBox::No)
//        ui->listWidget->addItem("No");
//    else
//        ui->listWidget->addItem("Cancel");
}
