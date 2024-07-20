#include "MainWindow.h"
#include "DatabaseManager.h"
#include "MyApp.h"

#include <QtWidgets/QApplication>
#include "qstring.h"
#include "qdebug.h"
#include "qfile.h"

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

    // 加载样式表
    QFile file(":/qss/resource/qss/window.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(QString(file.readAll()));
    file.close();

    DatabaseManager::instance()->openDb(MyApp::strDataBasePath_ + "info.db");
    DatabaseManager::instance()->queryAll();
    MainWindow w;
    w.show();
    return a.exec();
}
