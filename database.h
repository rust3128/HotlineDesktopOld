#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QMessageBox>
#include <QString>
#include "options.h"
#include "connectdialog.h"

static bool createConnection()
{

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("database/hostname").toString().trimmed());
    db.setDatabaseName(settings.value("database/name").toString().trimmed());
    db.setUserName(settings.value("database/user").toString().trimmed());
    db.setPassword(settings.value("database/pass").toString().trimmed());

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
