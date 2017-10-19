#include "mainwindow.h"
#include "options.h"
#include "database.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

//    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
//    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
//    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QTranslator *qt_translator = new QTranslator();
    if(qt_translator->load(":/resource/translate/qtbase_ru.qm"))
        a.installTranslator(qt_translator);

    if(!createConnection())
        return 1;

    MainWindow w;
    w.show();

    return a.exec();
}
