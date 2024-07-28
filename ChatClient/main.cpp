#include <QtWidgets/QApplication>
#include "login.h"
#include "customLineEdit.h"
#include "qdebug.h"
#include "qfile.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置高分屏适配
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QFont font;
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setHintingPreference(QFont::PreferFullHinting);
    QApplication::setFont(font);

    // 通过样式表文件加载样式
	QFile file(":/qss/resource/qss/loadwindow.css");
	file.open(QIODevice::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();


    Login w;
    w.show();
    return a.exec();
}
