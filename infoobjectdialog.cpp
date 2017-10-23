#include "infoobjectdialog.h"
#include "ui_infoobjectdialog.h"
#include "addpcdialog.h"
#include "options.h"
#include "addrrodialog.h"
#ifdef Q_OS_WIN
    #include "windows.h"
#endif
#include <QDebug>
#include <QSqlQuery>
#include <QtGlobal>
#include <QMessageBox>
#include <QTextCodec>





InfoObjectDialog::InfoObjectDialog(QSqlRecord record, QString nameBrend, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoObjectDialog)
{
    ui->setupUi(this);

    settings = new QSettings("HotlineDesktop.cfg", QSettings::IniFormat);

    azs.brendName=nameBrend;
    azs.objectID=record.value("objectid").toInt();
    azs.terminalID=record.value("terminalid").toInt();
    azs.name=record.value("adress").toString();
    azs.phone=record.value("phone").toString();
    azs.comments=record.value("comments").toString();
    this->setWindowTitle("Информация об АЗС "+QString::number(azs.terminalID)+" "+azs.brendName);
    ui->pingOutput->hide();
    ui->labelPingINFO->hide();
    ping = new QProcess(this);
    pingOFF=true;
    createUI();
//    qDebug() << record;
}

void InfoObjectDialog::closeEvent(QCloseEvent *event)
{
//    qDebug() << "Close Event" << endl << ping;
    if(ping) ping->kill();
}

InfoObjectDialog::~InfoObjectDialog()
{
    qDebug() << "InfoDialog Destruktor";
    delete ui;
    ping->kill();

}

void InfoObjectDialog::createUI()
{
    modelRro = new QSqlQueryModel();
    modelPC = new QSqlQueryModel();
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

    strSql = QString("SELECT pctype.typename, INET_NTOA(computers.ip), computers.pcid AS IP, computers.vncpass FROM pctype "
                     "INNER JOIN computers ON pctype.pctypeid = computers.pctypeid "
                     "WHERE computers.objectid = %1 "
                     "ORDER BY computers.pctypeid").arg(azs.objectID);
    modelPC->setQuery(strSql);
    modelPC->setHeaderData(0,Qt::Horizontal,"Назначение");
    modelPC->setHeaderData(1,Qt::Horizontal,"IP Адрес");
    modelPC->setHeaderData(3,Qt::Horizontal,"Пароль VNC");
    ui->tableViewPC->setModel(modelPC);
    ui->tableViewPC->verticalHeader()->hide();
    ui->tableViewPC->hideColumn(2);
    ui->tableViewPC->resizeColumnsToContents();
    ui->tableViewPC->verticalHeader()->setDefaultSectionSize(ui->tableViewRro->verticalHeader()->minimumSectionSize());
    ui->tableViewPC->selectRow(0);


    ui->labelColRro->setText("Кассовых аппаратов "+QString::number(modelRro->rowCount())+" шт. ПК "
                             +QString::number(modelPC->rowCount())+" шт.");


}



void InfoObjectDialog::on_toolButtonAdd_clicked()
{
    AddPCDialog *addPcDlg = new AddPCDialog(azs.objectID,-1);
    addPcDlg->exec();
    modelPC->setQuery(modelPC->query().lastQuery());
//    model.setQuery( model.query().lastQuery() );
}

void InfoObjectDialog::on_toolButtonEdit_clicked()
{
    QModelIndex idx = ui->tableViewPC->selectionModel()->currentIndex();
    AddPCDialog *addPcDlg = new AddPCDialog(azs.objectID, modelPC->data(modelPC->index(idx.row(),2)).toInt());
    addPcDlg->exec();
    modelPC->setQuery(modelPC->query().lastQuery());

}

void InfoObjectDialog::on_toolButton_clicked()
{
    QSqlQuery q;
    QString strSQL = QString("SELECT brendid FROM brends where name='%1'").arg(azs.brendName);
    q.exec(strSQL);
    q.next();
    addRroDialog *addRroDlg =  new addRroDialog(-1,q.value(0).toInt());
    addRroDlg->exec();
    modelRro->setQuery(modelRro->query().lastQuery());
}

void InfoObjectDialog::on_toolButtonRroEdit_clicked()
{
    QSqlQuery q;
    QModelIndex idx = ui->tableViewRro->selectionModel()->currentIndex();
    QString strSQL = QString("SELECT brendid FROM brends where name='%1'").arg(azs.brendName);
    q.exec(strSQL);
    q.next();
    addRroDialog *addRroDlg =  new addRroDialog(modelRro->data(modelRro->index(idx.row(),0)).toInt(),q.value(0).toInt());
    addRroDlg->exec();
    modelRro->setQuery(modelRro->query().lastQuery());
}

void InfoObjectDialog::on_tableViewPC_doubleClicked(const QModelIndex &idx)
{
    connectToObject(idx);
}

void InfoObjectDialog::on_toolButtonDell_clicked()
{
    QString strSQL;
    QSqlQuery q;
    QModelIndex idx = ui->tableViewPC->selectionModel()->currentIndex();
    int psID = modelPC->data(modelPC->index(idx.row(),2)).toInt();
    strSQL = QString("DELETE FROM `hotline`.`computers` WHERE `pcid`='%1';").arg(psID);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Удаление ПС"),
                          QString::fromUtf8("Вы действительно хотите удалить информацию об этом компьютере?"),
                          QMessageBox::Yes | QMessageBox::No );
    if (reply == QMessageBox::Yes)
        q.exec(strSQL);
    modelPC->setQuery(modelPC->query().lastQuery());
}

void InfoObjectDialog::connectToObject(const QModelIndex &idx)
{
    QStringList argum;
    QString ip = modelPC->data(modelPC->index(idx.row(),1)).toString();
//    QString pass = "-password=22222";
    QString pass = "-password="+modelPC->data(modelPC->index(idx.row(),3)).toString().trimmed();

#ifdef Q_OS_WIN
     argum << ip << pass;
#else
     argum << "-passwd" << "/home/rust/.vnc/passwd" << ip;
#endif

//    argum << "-passwd" << ip;
    settings->beginGroup("Common");
    QString command = settings->value("vncpromt").toString().trimmed();
    settings->endGroup();
    vncStart = new QProcess(this);
    if(command.length()==0) {
        QMessageBox::critical(0, qApp->tr("Не могу выполнить подключение по VNC"),
                              QString("Отсутсвует настройка по VNC подключению.\n"
                                      "Зайдтие в меню Настройка->Параметры."),
                              QMessageBox::Ok);
    }
    connect(vncStart, SIGNAL(finished(int)), this, SLOT(finVNC()));
    vncStart->setReadChannel(QProcess::StandardError);
    vncStart->start(command,argum);


}

void InfoObjectDialog::on_toolButtonVnc_clicked()
{
    QModelIndex idx = ui->tableViewPC->selectionModel()->currentIndex();
    connectToObject(idx);
}

void InfoObjectDialog::on_toolButtonDelRro_clicked()
{
    QSqlQuery q;
    QModelIndex idx = ui->tableViewRro->selectionModel()->currentIndex();
    int rroID = modelRro->data(modelRro->index(idx.row(),0)).toInt();
    QString zn = modelRro->data(modelRro->index(idx.row(),3)).toString();

    QString strSql = QString("DELETE FROM rro WHERE `rroid`='%1'").arg(rroID);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QString::fromUtf8("Удаление РРО"),
                          QString::fromUtf8("Вы действительно хотите удалить РРО с ЗН %1")
                                  .arg(zn),
                          QMessageBox::Yes | QMessageBox::No);

    switch (reply) {
    case QMessageBox::Yes:
        q.exec(strSql);
        break;
    case QMessageBox::No:
        return;
        break;
    default:
        break;
    }
    modelRro->setQuery(modelRro->query().lastQuery());
}

void InfoObjectDialog::finVNC()
{
    QByteArray *arr = new QByteArray;
    *arr = vncStart->readAllStandardError();
    qDebug() << "VNC Error" << arr->data();

}

void InfoObjectDialog::on_toolButtonPing_clicked()
{
    if(pingOFF) {
        QModelIndex idx = ui->tableViewRro->selectionModel()->currentIndex();
        QString ip = modelPC->data(modelPC->index(idx.row(),1)).toString();
        QString pingMess = QString("PING "+modelPC->data(modelPC->index(idx.row(),0)).toString()+" IP: "+ip);
        ui->labelPingINFO->setText(pingMess);
        pingOFF=false;
        ui->pingOutput->clear();
        ui->pingOutput->show();
        ui->labelPingINFO->show();
        ping->setProcessChannelMode(QProcess::MergedChannels);
        connect( ping, SIGNAL(readyReadStandardOutput ()), this, SLOT(print_ping()) );
        connect(ping,SIGNAL(readyReadStandardError()),this,SLOT(print_ping()));

#ifdef Q_OS_WIN
     ping->start("ping", QStringList() << "-t" <<ip);
#else
     ping->start("ping", QStringList() <<ip);
#endif
    } else {
//        qDebug() << "Ping ID" << ping->pid();
//        ping->terminate();

//        ping->kill(ping->pid(), SIGINT);
//        QProcess::execute(QString("kill -SIGINT %1").arg(ping->processId()));
//        QProcess::execute(QString("taskkill /PID %1 /F").arg(ping->processId()));

#ifdef Q_OS_WIN
     PROCESS_INFORMATION *pinfo = (PROCESS_INFORMATION  *)ping->pid();
     QProcess::execute(QString("taskkill /PID %1 /F").arg(pinfo->dwProcessId));
#else
     QProcess::execute(QString("kill -SIGINT %1").arg(ping->pid()));
#endif
        pingOFF=true;

//        ui->pingOutput->hide();
//        ui->pingOutput->clear();
//        ui->labelPingINFO->hide();

    }

}

void InfoObjectDialog::print_ping()
{
    QByteArray      output;
    output = ping->readAllStandardOutput ();
    QTextCodec *codec = QTextCodec::codecForName("cp-866");
    QString fio = codec->toUnicode(output.data());
//    fio.replace("\n","");
//    fio.split(QRegExp("\\s+"), QString::SkipEmptyParts);
//    fio.replace("\n","");
    fio.chop(2);
    if(fio.length()>0) {
        qDebug() << "ping output" << fio;
        ui->pingOutput->appendPlainText(fio); //вывод в гуи
    }
}

void InfoObjectDialog::on_pushButtonClose_clicked()
{
    this->close();
}

//quint64 InfoObjectDialog::getProcessID(const QProcess* proc)
//{

//    return proc->processId();
//#ifdef Q_OS_WIN
//    struct _PROCESS_INFORMATION* procinfo = proc->pid();
//    return procinfo->dwProcessId;
//#else // Linux
//    return proc->pid();
//#endif // Q_WS_WIN
//}
