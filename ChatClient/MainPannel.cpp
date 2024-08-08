#include "MainPannel.h"

#include "ChatUserListItem.h"

#include <qstringlist.h>
#include <qbuttongroup.h>
#include <qrandom.h>
#include <qdebug.h>
#include <qmenu.h>
#include <qsystemtrayicon.h>

MainPannel::MainPannel(QWidget *parent)
	: CustomWidget(parent)
	, ui(new Ui::MainPannelClass())
	, isMax_(false)
{
	ui->setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);

	setWindowIcon(QIcon(":/resource/icon/chat.png"));

	initPannel();

	initSysmenu();

	initSysTrayIcon();

	addUserList();

	btnGroup_ = new QButtonGroup(this);
	btnGroup_->addButton(ui->btnMsgPage, 0);
	btnGroup_->addButton(ui->btnFriendPage, 1);
	btnGroup_->addButton(ui->btnGroupPage, 2);
	btnGroup_->addButton(ui->btnCallPage, 3);

	strList_.append(":/resource/icon/chat-bubble-color.png");
	strList_.append(":/resource/icon/chat-bubble.png");
	strList_.append(":/resource/icon/user-profile-color.png");
	strList_.append(":/resource/icon/user-profile.png");
	strList_.append(":/resource/icon/friends-color.png");
	strList_.append(":/resource/icon/friends.png");
	strList_.append(":/resource/icon/phone-call-color.png");
	strList_.append(":/resource/icon/phone-call.png");

	btnGroup_->button(0)->setIcon(QIcon(strList_[0]));

	connect(btnGroup_, &QButtonGroup::idClicked, this, &MainPannel::sltPageButtonClicked);
}

MainPannel::~MainPannel()
{
	delete ui;
}

void MainPannel::sltSysMenuClicked(QAction* action) {
	qDebug() << "clicked!";
	if (action->text() == "我在线上") {

	} else if (action->text() == "离线") {

	} else if (action->text() == "显示主界面") {

	} else if (action->text() == QString::fromLocal8Bit("退出")) {
		this->close();
	}
}

void MainPannel::sltSystemTrayIconClicked(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::DoubleClick:
			if (!this->isVisible()) {
				this->show();
			}
			break;
		case QSystemTrayIcon::Trigger:
			break;
		default:
			break;
	}
}

void MainPannel::sltTrayIconMenuClicked(QAction* action) {
	if (action->text() == QString::fromLocal8Bit("我在线上")) {

	} else if (action->text() == QString::fromLocal8Bit("离线")) {

	} else if (action->text() == QString::fromLocal8Bit("显示主界面")) {

	} else if (action->text() == QString::fromLocal8Bit("退出")) {
		this->close();
	}
}

void MainPannel::initPannel() {
	// 左侧边栏
	ui->btnSidebarMessage->setEnterLeaveIcon(":/resource/icon/message-color.png",
		":/resource/icon/message.png");
	ui->btnSidebarCube->setEnterLeaveIcon(":/resource/icon/cube-color.png",
		":/resource/icon/cube.png");
	ui->btnSidebarPicture->setEnterLeaveIcon(":/resource/icon/picture-color.png",
		":/resource/icon/picture.png");
	ui->btnSidebarMusic->setEnterLeaveIcon(":/resource/icon/musical-note-color.png",
		":/resource/icon/musical-note.png");
	ui->btnSidebarMovie->setEnterLeaveIcon(":/resource/icon/video-camera-color.png",
		":/resource/icon/video-camera.png");
	ui->btnSidebarFolder->setEnterLeaveIcon(":/resource/icon/folder-color.png",
		":/resource/icon/folder.png");
	ui->btnSidebarPhone->setEnterLeaveIcon(":/resource/icon/mobile-color.png",
		":/resource/icon/mobile.png");
	ui->btnSidebarGame->setEnterLeaveIcon(":/resource/icon/game-color.png",
		":/resource/icon/game.png");
	ui->btnSidebarMenu->setEnterLeaveIcon(":/resource/icon/menu-color.png",
		":/resource/icon/menu.png");

	// 左侧用户页面切换按钮
	//ui->btnMsgPage->setEnterLeaveIcon(":/resource/icon/chat-bubble-color.png",
	//	":/resource/icon/chat-bubble.png");
	//ui->btnFriendPage->setEnterLeaveIcon(":/resource/icon/user-profile-color.png",
	//	":/resource/icon/user-profile.png");
	//ui->btnGroupPage->setEnterLeaveIcon(":/resource/icon/friends-color.png",
	//	":/resource/icon/friends.png");
	//ui->btnCallPage->setEnterLeaveIcon(":/resource/icon/phone-call-color.png",
	//	":/resource/icon/phone-call.png");
	ui->btnSearch->setEnterLeaveIcon(":/resource/icon/loupe-color.png",
		":/resource/icon/loupe.png");

	// 聊天窗口聊天按钮
	ui->btnChatEmoji->setEnterLeaveIcon(":/resource/icon/happiness-color.png",
		":/resource/icon/happiness.png");
	ui->btnChatBroom->setEnterLeaveIcon(":/resource/icon/broom-color.png",
		":/resource/icon/broom.png");
	ui->btnChatScissors->setEnterLeaveIcon(":/resource/icon/scissors-color.png",
		":/resource/icon/scissors.png");
	ui->btnChatImage->setEnterLeaveIcon(":/resource/icon/image-color.png",
		":/resource/icon/image-gray.png");
	ui->btnChatVoice->setEnterLeaveIcon(":/resource/icon/voice-color.png",
		":/resource/icon/voice-gray.png");
	ui->btnChatRecord->setEnterLeaveIcon(":/resource/icon/file-color.png",
		":/resource/icon/file.png");

	// 窗口放大缩小关闭按钮
	ui->btnMainPannelMin->setEnterLeaveIcon(":/resource/icon/minus-color.png",
		":/resource/icon/minus.png");
	ui->btnMainPannelMax->setEnterLeaveIcon(":/resource/icon/maximize-color.png",
		":/resource/icon/maximize.png");
	ui->btnMainPannelClose->setEnterLeaveIcon(":/resource/icon/close-red.png",
		":/resource/icon/close.png");

	connect(ui->btnMainPannelClose, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnMainPannelMin, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMainPannelMax, &QPushButton::clicked, [this]() {
		if (!isMax_) {
			this->showMaximized();
			isMax_ = true;
		} else {
			this->showNormal();
			isMax_ = false;
		}
	});
}

void MainPannel::initSysmenu() {
	// 设置子菜单
	QMenu* sysMenu = new QMenu(this);
	sysMenu->setStyleSheet("font-size: 12pt;");
	sysMenu->addAction(QIcon(":/resource/pixmap/setting (2).png"), QString::fromLocal8Bit("系统设置"));
	sysMenu->addAction(QIcon(":/resource/pixmap/email (2).png"), QString::fromLocal8Bit("消息管理器"));
	sysMenu->addAction(QIcon(":/resource/pixmap/folder (4).png"), QString::fromLocal8Bit("文件助手"));
	sysMenu->addAction(QIcon(":/resource/pixmap/reset-password.png"), QString::fromLocal8Bit("修改密码"));
	sysMenu->addAction(QIcon(":/resource/pixmap/admin.png"), QString::fromLocal8Bit("帮助"));
	sysMenu->addAction(QIcon(":/resource/pixmap/neural.png"), QString::fromLocal8Bit("连接服务器"));
	sysMenu->addAction(QIcon(":/resource/pixmap/sync.png"), QString::fromLocal8Bit("升级"));
	// 设置鼠标点击事件，向上弹出菜单项
	connect(ui->btnSidebarMenu, &QPushButton::clicked, [this, sysMenu]() {
		QRect buttonRect = ui->btnSidebarMenu->rect();
		QPoint globalPos = ui->btnSidebarMenu->mapToGlobal(buttonRect.bottomLeft());
		QPoint menuPos = QPoint(globalPos.x(), globalPos.y() - sysMenu->sizeHint().height());
		sysMenu->exec(menuPos);
	});
	connect(sysMenu, &QMenu::triggered, this, &MainPannel::sltSysMenuClicked);
}

void MainPannel::initSysTrayIcon() {
	trayIcon_ = new QSystemTrayIcon(this);
	trayIcon_->setIcon(QIcon(":/resource/icon/chat.png"));
	
	QMenu* menu = new QMenu(this);
	menu->setStyleSheet("font-size: 12pt;");
	menu->addAction(QIcon(":/resource/pixmap/online.png"), QString::fromLocal8Bit("我在线上"));
	menu->addAction(QIcon(":/resource/pixmap/offline.png"), QString::fromLocal8Bit("离线"));
	menu->addAction(QIcon(":/resource/pixmap/mainPannel.png"), QString::fromLocal8Bit("显示主界面"));
	menu->addAction(QIcon(":/resource/pixmap/exit.png"), QString::fromLocal8Bit("退出"));
	trayIcon_->setContextMenu(menu);
	trayIcon_->show();
	connect(trayIcon_, &QSystemTrayIcon::activated, this, &MainPannel::sltSystemTrayIconClicked);
	connect(menu, &QMenu::triggered, this, &MainPannel::sltTrayIconMenuClicked);
}

void MainPannel::addUserList() {
	QStringList nameList = {
		"Jinx",
		"Robin",
		"Vayne",
		"Lulu",
		"John"
	};
	QStringList msgList = {
		"How are you!",
		"Hello world",
		"see you!",
		"Thank you",
		"Good job",
	};
	QStringList headList = {
		":/resource/head/head1.jpg",
		":/resource/head/head2.jpg",
		":/resource/head/head3.jpg",
		":/resource/head/head4.jpg",
		":/resource/head/head5.jpg",
		":/resource/head/head6.jpg",
		":/resource/head/head7.jpg",
		":/resource/head/head8.jpg",
		":/resource/head/head9.jpg",
		":/resource/head/head10.jpg",
		":/resource/head/head11.jpg",
		":/resource/head/head12.jpg",
		":/resource/head/head13.jpg",
	};
	for (int i = 0; i < 20; ++i) {
		ChatUserListItem* listItem = new ChatUserListItem;	
		listItem->setUserName(nameList[i % 5]);
		listItem->setLastMsg(msgList[i % 5]);
		listItem->setUserHead(headList[i % 13]);
		listItem->setTime("7-30");
		listItem->setNonReadMsg(i % 10);
		QListWidgetItem* item = new QListWidgetItem;
		item->setSizeHint(QSize(280, 76));
		ui->listWidget->addItem(item);
		ui->listWidget->setItemWidget(item,listItem);
	}
}

void MainPannel::sltPageButtonClicked(int index) {
	int pageSize = ui->stackedWidget->count();
	for (int i = 0; i < pageSize; ++i) {
		if (index != i) {
			btnGroup_->button(i)->setIcon(QIcon(strList_[i * 2 + 1]));
		} else {
			btnGroup_->button(i)->setIcon(QIcon(strList_[i * 2]));
		}
	}
	static int preIndex = 0;
	if (index == preIndex) return;
	if (index > preIndex) {
		ui->stackedWidget->setLength(ui->stackedWidget->width(), RightToLeft);
	} else {
		ui->stackedWidget->setLength(ui->stackedWidget->width(), LeftToRight);
	}
	preIndex = index;
	ui->stackedWidget->start(index);
}