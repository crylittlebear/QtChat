/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#include "MainWindow.h"
#include "global.h"
#include "DatabaseManager.h"
#include "baseWidget/CustomWidget.h"

#include "qfile.h"
#include "qbuttongroup.h"
#include "qjsonobject.h"
#include "qmessagebox.h"
#include "qlineedit.h"
#include "qevent.h"

MainWindow::MainWindow(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(QPixmap(":/resource/background/desktop.png")));

    itemModel_ = new QStandardItemModel(this);
    ui->tableViewUsers->setModel(itemModel_);

    buttonGroup_ = new QButtonGroup(this);
    buttonGroup_->addButton(ui->btnUserSearchPage, 0);
    buttonGroup_->addButton(ui->btnUserInfoPage, 1);
    buttonGroup_->addButton(ui->btnServerConfigPage, 2);
    buttonGroup_->addButton(ui->btnUserConfigPage, 3);
    buttonGroup_->addButton(ui->btnPasswdModPage, 4);
    buttonGroup_->addButton(ui->btnDataBackupPage, 5);

    // 按钮切换页面
    using funcButtonGroupPtr = void (QButtonGroup::*)(int);
    funcButtonGroupPtr buttonGroupClickIntPtr = &QButtonGroup::idClicked;
    connect(buttonGroup_, buttonGroupClickIntPtr, this, &MainWindow::sltChangePages);
    
    // 设置stackWidget的初始页面
    ui->stackedWidgetMainPage->setCurrentIndex(0);
    ui->stackWidgetWorkSpace->setCurrentIndex(0);

    // 设置表格属性不可编辑
    ui->tableViewUsers->setEditTriggers(QTableView::NoEditTriggers);

    // 设置表格属性选择整行
    ui->tableViewUsers->setSelectionBehavior(QTableView::SelectRows);
    connect(ui->tableViewUsers, &QTableView::clicked, this, &MainWindow::sltTableClicked);

    // 设置文件备份的默认路径
    ui->lineEditBackup->setText(MyApp::strBackupPath_);

    // 初始化网络
    initNetwork();

    // 设置本机IP显示
    ui->labelHostAddr->setText(QString("本机IP: ") + myHelper::GetIP());
    
    // 设置当前事件
    ui->labelSystemTime->setText(
        QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss  ddd"));
    timerId_ = startTimer(1000);

    // 设置默认的账号和密码
    ui->lineEditUserName->setText("admin");
    ui->lineEditPasswd->setText("123456");
    ui->lineEditPasswd->setEchoMode(QLineEdit::Password);

    // 界面按钮相应的信号槽
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::sltBtnLoginClicked);
    connect(ui->btnLoginExit, &QPushButton::clicked, this, &MainWindow::sltBtnLoginExitClicked);
    connect(ui->btnWinClose, &QPushButton::clicked, this, &MainWindow::sltBtnWinCloseClicked);
    connect(ui->btnWinMin, &QPushButton::clicked, this, &MainWindow::sltBtnWinMinClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::sltBtnExitClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::sltBtnLoginExitClicked() { this->close(); }

void MainWindow::sltBtnWinCloseClicked() { this->close(); }

void MainWindow::sltBtnWinMinClicked() { this->showMinimized(); }

void MainWindow::sltBtnBackupClicked() {

}

void MainWindow::sltBtnDataUdoClicked() {

}

void MainWindow::sltBtnLoginClicked() {
    QString name = ui->lineEditUserName->text();
    QString passwd = ui->lineEditPasswd->text();

    QJsonObject jsonObj = DatabaseManager::instance()->checkUserLogin(name, passwd);
    int code = jsonObj.value("code").toInt();
    qDebug() << "code = " << code;
    switch (code) {
    case 0:
        ui->stackedWidgetMainPage->nextPage();
        break;
    case -1:
        CustomMessageBox::question(this, tr("查询用户失败"), tr("警告"));
        break;
    case -2:
        CustomMessageBox::warning(this, tr("密码错误"), tr("警告"));
        break;
    case -3:
        CustomMessageBox::warning(this, tr("重复登录"), tr("警告"));
        break;
    }
}

void MainWindow::sltChangePages(int index) {
    qDebug() << "page index: " << index;
    ui->stackWidgetWorkSpace->setCurrentIndex(index);
}

void MainWindow::sltBtnExitClicked() {
    this->close();
}

void MainWindow::sltTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {

}

void MainWindow::sltTrayIconMenuClicked(QAction* action) {

}

void MainWindow::sltShowUserStatus(const QString& text) {

}

void MainWindow::sltTableClicked(const QModelIndex& index) {

}

void MainWindow::sltBtnUserRefreshClicked() {

}

void MainWindow::sltBtnUserInserClicked() {

}

void MainWindow::closeEvent(QCloseEvent* event) {
    this->hide();
    event->ignore();
}

void MainWindow::timerEvent(QTimerEvent* event) {
    if (timerId_ == event->timerId()) {
        ui->labelSystemTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm:ss  ddd"));
    }
}

void MainWindow::initNetwork() {

}

void MainWindow::setUserIdentity(const int& identity) {

}
