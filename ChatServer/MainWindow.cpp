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

    ui->lineEditUserName->setPixmap(":/resource/common/user.png");
    ui->lineEditPasswd->setPixmap(":/resource/common/padlock.png");

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
    //ui->tableViewUsers->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewUsers->resizeColumnsToContents();

    // 设置表格属性选择整行
    ui->tableViewUsers->setSelectionBehavior(QTableView::SelectRows);
    connect(ui->tableViewUsers, &QTableView::clicked, this, &MainWindow::sltTableClicked);

    // 设置文件备份的默认路径
    ui->lineEditBackup->setText(MyApp::strBackupPath_);

    // 初始化网络
    initNetwork();

    // 设置本机IP显示
    ui->labelHostAddr->setText(QString::fromLocal8Bit("本机IP: ") + myHelper::GetIP());
    
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
    sysTrayIcon_->setToolTip(QString::fromLocal8Bit("通信服务器系统图标"));

    QMenu* menu = new QMenu(this);
    menu->setStyleSheet("font-size: 8pt");
    menu->addAction(QString::fromLocal8Bit("显示主界面"));
    menu->addSeparator();
    menu->addAction(QString::fromLocal8Bit("退出"));
    sysTrayIcon_->setContextMenu(menu);

    // 设置QTextBrowser字体大小
    ui->textBrowser->setStyleSheet("font-size: 15pt");

    // 界面按钮相应的信号槽
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::sltBtnLoginClicked);
    connect(ui->btnWinClose, &QPushButton::clicked, this, &MainWindow::sltBtnWinCloseClicked);
    connect(ui->btnWinMin, &QPushButton::clicked, this, &MainWindow::sltBtnWinMinClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MainWindow::sltBtnExitClicked);
    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::sltBtnUserSearchClicked);
    connect(sysTrayIcon_, &QSystemTrayIcon::activated, this, &MainWindow::sltTrayIconClicked);
    connect(menu, &QMenu::triggered, this, &MainWindow::sltTrayIconMenuClicked);
    connect(ui->btnUserRefresh, &QPushButton::clicked, this, &MainWindow::sltBtnUserRefreshClicked);
    connect(ui->btnUserAdd, &QPushButton::clicked, this, &MainWindow::sltBtnUserInserClicked);
    connect(ui->btnUserDelete, &QPushButton::clicked, this, &MainWindow::sltBtnUserDeleteClicked);
    connect(ui->btnBackup, &QPushButton::clicked, this, &MainWindow::sltBtnBackupClicked);
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

void MainWindow::sltBtnWinCloseClicked() { 
    // 隐藏主程序窗口
    this->hide();
    // 在系统托盘显示此对象
    sysTrayIcon_->show();
}

void MainWindow::sltBtnWinMinClicked() { 
    this->showMinimized(); 
}

void MainWindow::sltBtnBackupClicked() {
    QString newFile = MyApp::strBackupPath_ + QString("info_%1.bak").arg(QDate::currentDate().toString("yyyy_MM_dd"));
    if (QFile::exists(newFile)) {
        QFile::remove(newFile);
    }
    bool bOk = QFile::copy(MyApp::strDataBasePath_ + "info.db", newFile);
    CustomMessageBox::information(this, bOk ? QString::fromLocal8Bit("备份数据成功") : QString::fromLocal8Bit("备份数据失败"));
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
        curUser_ = name;
        qDebug() << QString::fromLocal8Bit("用户: ").toUtf8().data() 
            << curUser_.toUtf8().data() 
            << QString::fromLocal8Bit("成功登陆了").toUtf8().data();
        ui->stackedWidgetMainPage->nextPage();
        break;
    case -1:
        CustomMessageBox::information(this, QString::fromLocal8Bit("用户不存在"), QString::fromLocal8Bit("提示"));
        break;
    case -2:
        CustomMessageBox::warning(this, QString::fromLocal8Bit("密码错误"), QString::fromLocal8Bit("警告"));
        break;
    case -3:
        CustomMessageBox::error(this, QString::fromLocal8Bit("重复登录"), QString::fromLocal8Bit("错误"));
        ui->stackedWidgetMainPage->nextPage();
        break;
    }
}

void MainWindow::sltChangePages(int index) {
    qDebug() << "page index: " << index;
    ui->stackWidgetWorkSpace->setCurrentIndex(index);
    if (index == 3) {
        sltBtnUserRefreshClicked();
    }
}

void MainWindow::sltBtnExitClicked() {
    ui->stackedWidgetMainPage->nextPage();
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
    if (action->text() == QString::fromLocal8Bit("显示主界面")) {
        if (!this->isVisible()) {
            this->show();
        }
    }
    else if (action->text() == QString::fromLocal8Bit("退出")) {
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
    itemModel_->setHorizontalHeaderLabels(QStringList() << "ID"
        << QString::fromLocal8Bit("用户名") << QString::fromLocal8Bit("密码") 
        << QString::fromLocal8Bit("状态") << QString::fromLocal8Bit("上次登录时间"));
    for (int i = 0; i < jsonArr.size(); ++i) {
        QJsonObject jsonObj = jsonArr.at(i).toObject();
        itemModel_->setData(itemModel_->index(i, 0), jsonObj.value("id").toInt());
        itemModel_->setData(itemModel_->index(i, 1), jsonObj.value("name").toString());
        itemModel_->setData(itemModel_->index(i, 2), jsonObj.value("passwd").toString());
        itemModel_->setData(itemModel_->index(i, 3), jsonObj.value("status").toInt() == Online ? 
            QString::fromLocal8Bit("在线") : QString::fromLocal8Bit("离线"));
        itemModel_->setData(itemModel_->index(i, 4), jsonObj.value("lasttime").toString());
    }
    for (int i = 0; i < jsonArr.size(); ++i) {
        for (int j = 0; j < 5; ++j) {
            itemModel_->item(i, j)->setTextAlignment(Qt::AlignCenter);
            ui->tableViewUsers->setRowHeight(i, 50);
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
#if 0
    QString name = ui->lineEditAddUserName->text();
    QString passwd = ui->lineEditAddPasswd->text();
    if (name.isEmpty()) {
        CustomMessageBox::warning(this, QString::fromLocal8Bit("用户名不能为空"));
        return;
    }
    int res = DatabaseManager::instance()->registerUser(name, passwd);
    if (res == -1) {
        CustomMessageBox::warning(this, QString::fromLocal8Bit("用户名已存在,添加用户失败"));
    }
    else {
        CustomMessageBox::warning(this, QString::fromLocal8Bit("添加用户成功"));
    }
#else
    CustomInputDialog* inputDlg = new CustomInputDialog(QString::fromLocal8Bit("添加用户"), this);
    int dlgCode = inputDlg->exec();
    if (dlgCode == QDialog::Rejected) {
        qDebug() << QString::fromLocal8Bit("点击了取消");
        return;
    }
    else {
        qDebug() << QString::fromLocal8Bit("点击了确定");
    }
    QStringList list = inputDlg->getStringList();
    if (list.isEmpty()) {
        qDebug() << QString::fromLocal8Bit("账号或密码为空").toUtf8().data();
        CustomMessageBox::warning(this, QString::fromLocal8Bit("用户名或密码不能为空,添加用户失败"));
        return;
    }
    qDebug() << QString::fromLocal8Bit("账号: ").toUtf8().data() << list[0].toUtf8().data()
        << QString::fromLocal8Bit(", 密码: ").toUtf8().data() << list[1].toUtf8().data();
    int registerResult = DatabaseManager::instance()->registerUser(list[0], list[1]);
    if (registerResult == -1) {
        CustomMessageBox::warning(this, QString::fromLocal8Bit("用户名已存在,添加失败"));
    }
    else {
        CustomMessageBox::information(this, QString::fromLocal8Bit("添加用户成功"), QString::fromLocal8Bit("信息"));
        sltBtnUserRefreshClicked();
    }
#endif
}

void MainWindow::sltBtnUserSearchClicked() {
    // 清空textBrowser的内容
    ui->textBrowser->clear();
    // 首先读取用户输入框的内容
    QString input = ui->lineEditSearch->text();
    if (input.isEmpty()) return;
    // 如果输入不为空
    QJsonArray jsonArr = DatabaseManager::instance()->getAllUsers();
    for (int i = 0; i < jsonArr.size(); ++i) {
        QJsonObject jsonObj = jsonArr.at(i).toObject();
        if (jsonObj.value("name").toString() == input) {
            QString res;
            res += QString::number(jsonObj.value("id").toInt());
            res += "    ";
            res += jsonObj.value("name").toString();
            res += "    ";
            res += QString::number(jsonObj.value("status").toInt());
            ui->textBrowser->append(res);
        }
    }
    ui->stackWidgetWorkSpace->setCurrentIndex(1);
}

void MainWindow::sltBtnUserDeleteClicked() {
    // 获取当前选中的索引列表
    QModelIndexList selectedIndexs = ui->tableViewUsers->selectionModel()->selectedRows();
    if (selectedIndexs.isEmpty()) {
        CustomMessageBox::warning(this, QString::fromLocal8Bit("请先选中用户列表中的某一行"));
        return;
    }
    // 获取选中的第一个索引
    QModelIndex index = selectedIndexs.first();
    int row = index.row();
    // 获取选中行的数据
    QModelIndex mIndex = itemModel_->index(row, 1);
    QString name = itemModel_->data(mIndex).toString();
    mIndex = itemModel_->index(row, 2);
    QString passwd = itemModel_->data(mIndex).toString();
    qDebug() << "name: " << name.toUtf8().data() << ", " << passwd.toUtf8().data();
    int dialogCode = CustomMessageBox::question(this, QString::fromLocal8Bit("确认删除？"));
    if (dialogCode == QDialog::Rejected) {
        qDebug() << QString::fromLocal8Bit("点击了取消");
        return;
    }
    int res = DatabaseManager::instance()->removeUser(name);
    if (res == -1) {
        CustomMessageBox::information(this, QString::fromLocal8Bit("删除用户失败"), QString::fromLocal8Bit("信息"));
    }
    else {
        CustomMessageBox::information(this, QString::fromLocal8Bit("删除用户成功"), QString::fromLocal8Bit("信息"));
        sltBtnUserRefreshClicked();
    }
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
    // 初始化消息服务器
    msgServer_ = new TcpMsgServer(this);
    bool bOk = msgServer_->startListen(60100);
    ui->textBrowser->setText(QString::fromLocal8Bit("消息服务器通知消息: "));
    ui->textBrowser->append(bOk ? QString::fromLocal8Bit("消息服务器监听成功, 端口: 60100") :
        QString::fromLocal8Bit("消息服务器监听失败"));

    // TODO 初始化文件服务器
    fileServer_ = new TcpFileServer(this);
    bOk = fileServer_->startListen(60101);
    ui->textBrowser->append(QString::fromLocal8Bit("文件服务器通知消息: "));
    ui->textBrowser->append(bOk ? QString::fromLocal8Bit("文件服务器监听成功, 端口60101") :
        QString::fromLocal8Bit("文件服务器监听失败"));

    connect(msgServer_, &TcpMsgServer::sigUserStatus, this, &MainWindow::sltShowUserStatus);
}

void MainWindow::setUserIdentity(const int& identity) {
    //ui->btnServerConfigPage->setVisible(identity == Manager);
}
