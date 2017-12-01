#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QMessageBox>
#include <QString>
//#include "options.h"
#include "connectdialog.h"
#include <QSettings>

static bool createConnection()
{

    QSettings settings("HotlineDesktop.cfg", QSettings::IniFormat);
    QFile cfgfile("HotlineDesktop.cfg");
    if (!cfgfile.exists()){
        ConnectDialog *connDlg = new ConnectDialog();
        connDlg->exec();
        return false;
    }



    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    settings.beginGroup("Database");
    db.setHostName(settings.value("hostname").toString().trimmed());
    db.setDatabaseName(settings.value("name").toString().trimmed());
    db.setUserName(settings.value("user").toString().trimmed());
    db.setPassword(settings.value("pass").toString().trimmed());
    settings.endGroup();

    if(!db.open()) {
        qDebug() <<  "Не возможно подключиться к базе данных." << endl << "Причина:" << db.lastError().text();
        QMessageBox::critical(0, qApp->tr("Не могу открыть базу данных"),
                              QString("Не могу установить соединение с базой данных\nПричина: %1\n Проверьте настройки подключения.").arg(db.lastError().text()),
                              QMessageBox::Cancel);
        ConnectDialog *connDlg = new ConnectDialog();
        connDlg->exec();
        return false;
    }
    return true;

}


#endif // DATABASE_H
