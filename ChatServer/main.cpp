#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "DatabaseManager.h"
#include "qstring.h"
#include "qdebug.h"

//void testDatabase() {
//    DatabaseManager* manager = DatabaseManager::instance();
//    manager->openDb("employee");
//    manager->registerUser("heng", "123");
//    manager->queryAll();
//    qDebug() << manager->getUserName(1);
//    qDebug() << manager->getUserName(2);
//}

int main(int argc, char *argv[])
{
    //testDatabase();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
