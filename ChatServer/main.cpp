#include "MainWindow.h"
#include "DatabaseManager.h"
#include "MyApp.h"

#include "qapplication.h"
#include "qstring.h"
#include "qdebug.h"
#include "qfile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式表
    QFile file(":/qss/resource/qss/window.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(QString(file.readAll()));
    file.close();

    // 打开数据库
    DatabaseManager::instance()->openDb(MyApp::strDataBasePath_ + "info.db");

    MainWindow w;
    w.show();
    return a.exec();
}
