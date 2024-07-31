#pragma once

#include <QWidget>
#include <qlistwidget>

class ChatUserList  : public QListWidget
{
	Q_OBJECT

public:
	ChatUserList(QWidget *parent = nullptr);
	~ChatUserList();

signals:
	void sigLoadingChatUser();

protected:
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
	bool loadPending_;
};
