#include "login.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"
#include "qlineedit.h"
#include "qfile.h"
#include "qdebug.h"
#include "qjsonobject.h"
#include "comapi/MyApp.h"
#include "comapi/unit.h"
#include "basewidget/CustomWidget.h"

Login::Login(QWidget *parent)
	: CustomWidget(parent)
	, ui(new Ui::LoginClass())
{
	ui->setupUi(this);
	// 使透明生效
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon(QIcon(":/resource/icon/chat.png"));

	connected_ = false;

	initWidget();

	tcpSocket_ = new ClientSocket;
	ui->labelWinTitle->setText(QString::fromLocal8Bit("正在连接服务器"));
	tcpSocket_->connectToServer(MyApp::strHostAddr_, MyApp::msgPort_);
	connect(tcpSocket_, &ClientSocket::signalStatus, this, &Login::sltTcpStatus);
}

Login::~Login()
{
	delete ui;
}

void Login::sltTcpStatus(const quint8& status) {
	switch(status) {
	case DisconnectedHost:
		connected_ = false;
		ui->labelWinTitle->setText(QString::fromLocal8Bit("服务器已断开"));
		break;
	case ConnectedHost:
		connected_ = true;
		ui->labelWinTitle->setText(QString::fromLocal8Bit("已连接服务器"));
		break;
	case LoginSuccess: {
		mainWindow_ = new MainPannel;
		mainWindow_->show();
		this->hide();
	}
		break;
	case LoginPasswdErr:
		CustomMessageBox::warning(this, QString::fromLocal8Bit("密码错误"));
		break;
	case UserNotFound:
		CustomMessageBox::warning(this, QString::fromLocal8Bit("用户未找到"));
		break;
	case LoginRepeat:
		CustomMessageBox::warning(this, QString::fromLocal8Bit("重复登录"));
		break;
	}
}

void Login::initWidget() {
	// 设置page1
	ui->labelWelcome->setPixmap(QPixmap(":/resource/background/welcome.png"));
	ui->labelWelcome->setScaledContents(true);
	ui->labelWinTitle->setText(QString::fromLocal8Bit("登录窗口"));
	ui->lineEditUser->setPlaceholderText(QString::fromLocal8Bit("请输入账号"));
	ui->lineEditPasswd->setPlaceholderText(QString::fromLocal8Bit("请输入密码"));
	ui->lineEditPasswd->setEchoMode(QLineEdit::Password);

	// 设置默认的账号和密码
	ui->lineEditUser->setText(QString::fromLocal8Bit("李立恒"));
	ui->lineEditPasswd->setText("root");

	ui->lineEditUser->setPic(QPixmap(":/resource/pixmap/user.png"));
	ui->lineEditPasswd->setPic(QPixmap(":/resource/pixmap/padlock.png"));

	// 设置page2
	ui->lineEditMsgServerPort->setAlignment(Qt::AlignCenter);
	ui->lineEditFileServerPort->setAlignment(Qt::AlignCenter);

	// 连接信号槽
	connect(ui->btnClose, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnClose2, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMin2, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMenu, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	connect(ui->btnCancel, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	//connect(ui->btnOk, &QPushButton::clicked, );
	connect(ui->btnLogin, &QPushButton::clicked, this, &Login::sltBtnLoginClicked);
}

void Login::sltBtnLoginClicked() {
	// 检查是否连接到服务器
	tcpSocket_->checkConnected();

	QString userName = ui->lineEditUser->text();
	QString passwd = ui->lineEditPasswd->text();

	if (userName.isEmpty() || passwd.isEmpty()) {
		CustomMessageBox::information(this, QString::fromLocal8Bit("用户名和密码均不能为空"));
		return;
	}

	QJsonObject json;
	json.insert("name", userName);
	json.insert("passwd", passwd);

	tcpSocket_->sltSendMessage(E_MSG_TYPE::Login, json);
}



