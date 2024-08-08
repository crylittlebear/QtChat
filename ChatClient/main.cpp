#include <QtWidgets/QApplication>
#include "login.h"
#include "customLineEdit.h"
#include "qdebug.h"
#include "qfile.h"
#include "MainPannel.h"
#include "comapi/MyApp.h"

int main(int argc, char *argv[])
{
    //QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication a(argc, argv);

    // 设置高分屏适配
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

    // 创建数据文件夹
    MyApp::initApp("");

    Login w;
    //MainPannel w;
    w.show();
    return a.exec();
}
