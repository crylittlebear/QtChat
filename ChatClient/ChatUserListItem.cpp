#include "ChatUserListItem.h"

#include <qpixmap.h>

ChatUserListItem::ChatUserListItem(QWidget *parent)
	: ListItemBase(parent)
	, ui(new Ui::ChatUserListItemClass())
{
	ui->setupUi(this);
	setUserName("");
	setTime("");
	setLastMsg("");
	setUserHead(":/resource/head/default.jpg");
	setNonReadMsg(0);
	this->installEventFilter(this);
}

ChatUserListItem::~ChatUserListItem()
{
	delete ui;
}

void ChatUserListItem::setUserName(QString name) {
	ui->labelUserName->setText(name);
}

void ChatUserListItem::setLastMsg(QString msg) {
	ui->labelLastMsg->setText(msg);
}

void ChatUserListItem::setTime(QString time) {
	ui->labelTime->setText(time);
}

void ChatUserListItem::setUserHead(QString headPath) {
	ui->labelUserHead->setPixmap(QPixmap(headPath));
	ui->labelUserHead->setScaledContents(true);
}

void ChatUserListItem::setNonReadMsg(int size){
	if (size <= 0) return;
	ui->labelMsgNotRead->setFixedHeight(ui->labelMsgNotRead->width());
	QString numPath = QString(":/resource/number/number-%1.png").arg(size);
	ui->labelMsgNotRead->setPixmap(numPath);
	ui->labelMsgNotRead->setScaledContents(true);
	//ui->labelMsgNotRead->show();
}

void ChatUserListItem::enterEvent(QEvent* event) {
	ui->widgetAll->setStyleSheet("background-color: #68bfd7");
}

void ChatUserListItem::leaveEvent(QEvent* event) {
	ui->widgetAll->setStyleSheet("");
}

void ChatUserListItem::mousePressEvent(QMouseEvent* event) {
	emit sigItemClicked();
}
