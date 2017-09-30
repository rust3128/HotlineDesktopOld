#include "addpcdialog.h"
#include "ui_addpcdialog.h"
#include "qdynamicbutton.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>



AddPCDialog::AddPCDialog(int objID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPCDialog)
{
    ui->setupUi(this);
    objectID=objID;

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
        connect(button,SIGNAL(clicked()),this,SLOT(slotGetNumberButton()));

    }
    q.finish();
}

void AddPCDialog::on_buttonBox_clicked(QAbstractButton *button)
{


    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Save:
        addNewPC();
        this->close();
        break;
    case QDialogButtonBox::Reset:
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
    strSQL = QString("INSERT INTO computers (objectid, pctypeid, ip) VALUES (%1, %2, INET_ATON('%3'))")
            .arg(objectID)
            .arg(rroTypeID)
            .arg(ui->lineEditIP->text().trimmed());
//    qDebug() << strSQL;
    if(!q.exec(strSQL)) qDebug() << "Не удалось добавить компьютер" << q.lastError().text();

}


void AddPCDialog::slotGetNumberButton()
{
    QDynamicRadioButton *button = (QDynamicRadioButton*)sender();
    rroTypeID=button->gettypeID();
//    qDebug() << "PCType ID" << rroTypeID;
}
