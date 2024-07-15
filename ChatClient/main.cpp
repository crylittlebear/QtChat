#include <QtWidgets/QApplication>
#include "login.h"
#include "customLineEdit.h"
#include "qdebug.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
