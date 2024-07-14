#include <QtWidgets/QApplication>
#include "login.h"
#include "customLineEdit.h"
#include "qdebug.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ChatClient w;
    Login w;
    //IpLineEdit w;
    w.show();
    return a.exec();
}
