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
#include "qjsonarray.h"
#include "qmessagebox.h"
#include "qlineedit.h"
#include "qevent.h"
#include "qtableview.h"
#include "qmenu.h"
#include "qstringlist.h"

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

    // 新建QSystemTrayIcon对象
    sysTrayIcon_ = new QSystemTrayIcon(this);
    // 新建托盘要显示的icon
    QIcon icon = QIcon(":/resource/background/desktop.png");
    // 将icon设置到QSystemTrayIcon中
    sysTrayIcon_->setIcon(icon);
    // 当鼠标移动到托盘时要显示的内容
    sysTrayIcon_->setToolTip(QStringLiteral(" 通信服务器系统图标 "));

    QMenu* menu = new QMenu(this);
    menu->addAction(QString::fromLocal8Bit("MainWindow"));
    menu->addAction(QString::fromLocal8Bit("exit"));
    sysTrayIcon_->setContextMenu(menu);

    // 界面按钮相应的信号槽
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::sltBtnLoginClicked);
    connect(ui->btnLoginExit, &QPushButton::clicked, this, &MainWindow::sltBtnLoginExitClicked);
    connect(ui->btnWinClose, &QPushButton::clicked, this, &MainWindow::sltBtnWinCloseClicked);
    connect(ui->btnWinMin, &QPushButton::clicked, this, &MainWindow::sltBtnWinMinClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::sltBtnExitClicked);
    connect(sysTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::sltTrayIconClicked);
    connect(menu, &QMenu::triggered, this, &MainWindow::sltTrayIconMenuClicked);
    connect(ui->btnUserRefresh, &QPushButton::clicked, this, &MainWindow::sltBtnUserRefreshClicked);
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

void MainWindow::sltBtnWinCloseClicked() { 
    // 隐藏主程序窗口
    this->hide();
    // 在系统托盘显示此对象
    sysTrayIcon_->show();
}

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
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        if (!this->isVisible()) {
            this->show();
        }
        break;
    default:
        break;
    }
}

void MainWindow::sltTrayIconMenuClicked(QAction* action) {
    if (action->text() == "MainWindow") {
        if (!this->isVisible()) {
            this->show();
        }
    }
    else if (action->text() == "exit") {
        this->close();
    }
}

void MainWindow::sltShowUserStatus(const QString& text) {
    ui->textBrowser->append(text);
}

void MainWindow::sltTableClicked(const QModelIndex& index) {

}

void MainWindow::sltBtnUserRefreshClicked() {
    QJsonArray jsonArr = DatabaseManager::instance()->getAllUsers();
    itemModel_->clear();
    itemModel_->setColumnCount(5);
    itemModel_->setRowCount(jsonArr.size());
    itemModel_->setHorizontalHeaderLabels(QStringList() << "user"
        << "name" << "password" << "status" << "lastLoadTime");
    for (int i = 0; i < jsonArr.size(); ++i) {
        QJsonObject jsonObj = jsonArr.at(i).toObject();
        itemModel_->setData(itemModel_->index(i, 0), jsonObj.value("id").toInt());
        itemModel_->setData(itemModel_->index(i, 1), jsonObj.value("name").toString());
        itemModel_->setData(itemModel_->index(i, 2), jsonObj.value("passwd").toString());
        itemModel_->setData(itemModel_->index(i, 3), jsonObj.value("status").toInt());
        itemModel_->setData(itemModel_->index(i, 4), jsonObj.value("lasttime").toString());
    }
    for (int i = 0; i < jsonArr.size(); ++i) {
        for (int j = 0; j < 5; ++j) {
            itemModel_->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
    int tableViewerWidth = ui->tableViewUsers->width();
    ui->tableViewUsers->setColumnWidth(0, tableViewerWidth * 0.1);
    ui->tableViewUsers->setColumnWidth(1, tableViewerWidth * 0.2);
    ui->tableViewUsers->setColumnWidth(2, tableViewerWidth * 0.2);
    ui->tableViewUsers->setColumnWidth(3, tableViewerWidth * 0.1);
    ui->tableViewUsers->setColumnWidth(4, tableViewerWidth * 0.4);
}

void MainWindow::sltBtnUserInserClicked() {

}

void MainWindow::closeEvent(QCloseEvent* event) {
#if 0
    this->hide();
    event->ignore();
#endif
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
