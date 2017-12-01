#include "connectdialog.h"
#include "ui_connectdialog.h"
#include "options.h"

#include <QMessageBox>
#include <QFileDialog>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    settings = new QSettings("HotlineDesktop.cfg", QSettings::IniFormat);
    editing=false;
    setupUI();


}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::setupUI()
{
    settings->beginGroup("Database");
    ui->lineEditHost->setText(settings->value("hostname").toString().trimmed());
    ui->lineEditDbPath->setText(settings->value("name").toString().trimmed());
    ui->lineEditUser->setText(settings->value("user").toString().trimmed());
    ui->lineEditPass->setText(settings->value("pass").toString().trimmed());
    settings->endGroup();
    settings->beginGroup("Common");
    ui->lineEditVnc->setText(settings->value("vncpromt").toString().trimmed());
    settings->endGroup();

    ui->lineEditPass->setEchoMode(QLineEdit::Password);

    ui->lineEditDbPath->setEnabled(false);
    ui->lineEditHost->setEnabled(false);
    ui->lineEditUser->setEnabled(false);
    ui->lineEditPass->setEnabled(false);
    ui->lineEditVnc->setEnabled(false);
    ui->toolButton->setEnabled(false);
}

void ConnectDialog::writeSettings()
{
    settings->beginGroup("Database");
    settings->setValue("hostname",ui->lineEditHost->text().trimmed());
    settings->setValue("name",ui->lineEditDbPath->text().trimmed());
    settings->setValue("user",ui->lineEditUser->text().trimmed());
    settings->setValue("pass",ui->lineEditPass->text().trimmed());
    settings->endGroup();
    settings->beginGroup("Common");
    settings->setValue("vncpromt",ui->lineEditVnc->text().trimmed());
    settings->endGroup();

    if (QMessageBox::Yes == QMessageBox::question(this, "Закрытие программы",
                          "Для применения настроек подключения необходимо закрыть программу.<br>"
                                                  "Сделать это сейчас?",
                          QMessageBox::Yes|QMessageBox::No))
            {
                emit closeMainWin();
            }
}

void ConnectDialog::on_checkBoxEdit_clicked()
{
    if(ui->checkBoxEdit->isChecked()) {
        ui->lineEditDbPath->setEnabled(true);
        ui->lineEditHost->setEnabled(true);
        ui->lineEditUser->setEnabled(true);
        ui->lineEditPass->setEnabled(true);
        ui->lineEditVnc->setEnabled(true);
        ui->toolButton->setEnabled(true);
        editing=true;

    } else {
        ui->lineEditDbPath->setEnabled(false);
        ui->lineEditHost->setEnabled(false);
        ui->lineEditUser->setEnabled(false);
        ui->lineEditPass->setEnabled(false);
        ui->lineEditVnc->setEnabled(false);
        ui->toolButton->setEnabled(false);
    }
}

void ConnectDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Ok:
        if(editing) writeSettings();
        this->accept();
        break;
    case QDialogButtonBox::Cancel:
        this->reject();
        break;
    default:
        break;
    }

}

void ConnectDialog::on_toolButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                QString::fromUtf8("Выберите запускаемый файл VNC клиента."),
                                QDir::currentPath(),
                                "Программы (*.exe);;Все файлы (*.*)");
    ui->lineEditVnc->setText(fileName);
}
