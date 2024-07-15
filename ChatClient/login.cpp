#include "login.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"
#include "qlineedit.h"
#include "qfile.h"
#include "qdebug.h"
#include "MainWindow.h"

Login::Login(QWidget *parent)
	: CustomWidget(parent)
	, ui(new Ui::LoginClass())
{
	ui->setupUi(this);
	// 去掉QT自带的标题栏
	setWindowFlags(Qt::FramelessWindowHint);
	// 使透明生效
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon(QIcon(":/resource/ico/chatting.png"));
	// 设置page1
	ui->btnClose->setIcon(QIcon(":/resource/common/ic_close_white.png"));
	ui->btnMenu->setIcon(QIcon(":/resource/common/ic_login_cfg.png"));
	ui->btnMin->setIcon(QIcon(":/resource/common/ic_min_white.png"));
	ui->labelWinIcon->setPixmap(QPixmap(":/resource/ico/chatting.png"));
	ui->labelWinIcon->setScaledContents(true);
	ui->labelWelcome->setPixmap(QPixmap(":/resource/background/welcome.png"));
	ui->labelUserHead->setPixmap(QPixmap(":/resource/head/hacker.png"));
	ui->labelUserHead->setScaledContents(true);
	ui->labelWelcome->setScaledContents(true);
	ui->labelWinTitle->setText(QString::fromLocal8Bit("登录窗口"));
	ui->lineEditUser->setPlaceholderText(QString::fromLocal8Bit("请输入账号"));
	ui->lineEditPasswd->setPlaceholderText(QString::fromLocal8Bit("请输入密码"));
	ui->lineEditPasswd->setEchoMode(QLineEdit::Password);

	// 设置page2
	ui->btnClose2->setIcon(QIcon(":/resource/common/ic_close_white.png"));
	ui->btnMin2->setIcon(QIcon(":/resource/common/ic_min_white.png"));
	ui->lineEditMsgServerPort->setAlignment(Qt::AlignCenter);
	ui->lineEditFileServerPort->setAlignment(Qt::AlignCenter);

	// 通过样式表文件加载样式
	QFile file(":/qss/resource/qss/loadwindow.css");
	file.open(QIODevice::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	// 连接信号槽
	connect(ui->btnClose, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnClose2, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMin2, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMenu, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	connect(ui->btnCancel, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	connect(ui->btnOk, &QPushButton::clicked, [this]() {
		qDebug() << ui->lineEditServerAddress->text();
	});
	connect(ui->btnLogin, &QPushButton::clicked, [this]() {
		MainWindow* win = new MainWindow;
		win->show();
		this->hide();
	});
}

Login::~Login()
{
	delete ui;
}



