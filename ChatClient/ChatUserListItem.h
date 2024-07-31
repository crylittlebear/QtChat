#pragma once

#include <QWidget>
#include "ui_ChatUserListItem.h"

#include "ListItemBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChatUserListItemClass; };
QT_END_NAMESPACE

class ChatUserListItem : public ListItemBase
{
	Q_OBJECT

public:
	ChatUserListItem(QWidget *parent = nullptr);
	~ChatUserListItem();

	void setUserName(QString name);
	void setLastMsg(QString msg);
	void setTime(QString time);
	void setUserHead(QString headPath);
	void setNonReadMsg(int size);

signals:
	void sigItemClicked();

protected:
	virtual void enterEvent(QEvent* event) override;
	virtual void leaveEvent(QEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;

private:
	Ui::ChatUserListItemClass *ui;
};
