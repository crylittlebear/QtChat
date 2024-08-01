#include "ChatUserList.h"

#include <qevent.h>
#include <qscrollbar.h>

ChatUserList::ChatUserList(QWidget *parent)
	: QListWidget(parent)
	, loadPending_(false)
{
	// 关闭QListWidget的水平拖动条和竖直拖动条
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	// 安装事件过滤器
	this->viewport()->installEventFilter(this);
}

ChatUserList::~ChatUserList()
{}

bool ChatUserList::eventFilter(QObject * watched, QEvent * event)
{
	if (watched == this->viewport()) {
		//if (event->type() == QEvent::Enter) {
		//	// 鼠标进入listWidget区域显示滚动条
		//	this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		//} else if (event->type() == QEvent::Leave) {
		//	// 鼠标离开listWidget区域隐藏滚动条
		//	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//}
	} 
	if (watched == this->viewport() && event->type() == QEvent::Wheel) {
		QWheelEvent* whellEvent = static_cast<QWheelEvent*>(event);
		// 获取鼠标在y方向上滚动的角度
		int numDegrees = whellEvent->angleDelta().y() / 8;
		int numStep = numDegrees / 15;
		// 设置滚动幅度
		verticalScrollBar()->setValue(verticalScrollBar()->value() - numStep);
		QScrollBar* vScrollBar = this->verticalScrollBar();
	}
	// 对于其他事件，交由基类处理
	return QListWidget::eventFilter(watched, event);
}
