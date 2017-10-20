#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QTextStream>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    readHistory();
    ui->historyText->hide();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButtonHrono_clicked()
{
    ui->historyText->show();
}

void AboutDialog::readHistory()
{
    QFile file("history.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString str = stream.readAll();
        ui->historyText->setPlainText(str);

    }
}
