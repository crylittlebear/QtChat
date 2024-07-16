#include "MainWindow.h"

#include "qfile.h"

MainWindow::MainWindow(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(QPixmap(":/resource/background/desktop.png")));

    // 加载样式表
    QFile file(":/qss/resource/qss/window.css");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(QString(file.readAll()));
    file.close();

    connect(ui->btnUserSearch, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pageUserSearch);
    });
    connect(ui->btnUserInfo, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pageUserInfo);
    });
    connect(ui->btnUserConfig, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pageUserConfig);
    });
    connect(ui->btnServerConfig, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pageServerConfig);
    });
    connect(ui->btnPasswdMod, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pagePasswdMod);
    });
    connect(ui->btnDataBackup, &QPushButton::clicked, [this]() {
        ui->stackWidgetWorkSpace->setCurrentWidget(ui->pageDataBackup);
    });
    connect(ui->btnExit, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btnWinClose, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btnWinMin, &QPushButton::clicked, this, &QWidget::showMinimized);
}

MainWindow::~MainWindow()
{
    delete ui;
}
