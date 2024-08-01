#pragma once

#include <QWidget>
#include "ui_ChatUserListItem.h"

#include "ListItemBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatUserListItemClass; };
QT_END_NAMESPACE

/*!
* @brief 用户聊天列表中的列表项类
*/
class ChatUserListItem : public ListItemBase
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	ChatUserListItem(QWidget *parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~ChatUserListItem();

	/*!
	* @brief 设置用户名
	*/
	void setUserName(QString name);

	/*!
	* @brief 设置用户发来的最后一条消息
	*/
	void setLastMsg(QString msg);

	/*!
	* @brief 设置最后一条消息的时间
	*/
	void setTime(QString time);

	/*!
	* @brief 设置用户的头像
	*/
	void setUserHead(QString headPath);

	/*!
	* @brief 构造函数
	*/
	void setNonReadMsg(int size);

signals:
	/*!
	* @brief 用户消息条目点击信号
	*/
	void sigItemClicked();

protected:
	/*!
	* @brief 重写鼠标进入事件
	*/
	virtual void enterEvent(QEvent* event) override;

	/*!
	* @brief 重写鼠标离开事件
	*/
	virtual void leaveEvent(QEvent* event) override;

	/*!
	* @brief 重写鼠标按下事件
	*/
	virtual void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::ChatUserListItemClass *ui;
};
