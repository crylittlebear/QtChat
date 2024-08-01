#pragma once

#include <QWidget>
#include <qlistwidget>

/*!
* @brief 自定义用户对话列表
*/
class ChatUserList  : public QListWidget
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	ChatUserList(QWidget *parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~ChatUserList();

signals:
	void sigLoadingChatUser();

protected:
	/*!
	* @brief 重写事件过滤器
	*/
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
	bool loadPending_;
};
