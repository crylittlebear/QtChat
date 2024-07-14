#include <QtWidgets/QApplication>
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //ChatClient w;
    Login w;
    w.show();
    return a.exec();
}
