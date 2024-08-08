#pragma once

#include <QWidget>
#include "basewidget/CustomWidget.h"
#include "ui_MainPannel.h"

#include <qstringlist.h>
#include <qsystemtrayicon.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainPannelClass; };
QT_END_NAMESPACE

class MainPannel : public CustomWidget
{
	Q_OBJECT

public:
	MainPannel(QWidget *parent = nullptr);
	~MainPannel();

	void addUserList();

public slots:
	void sltPageButtonClicked(int index);

	void sltSysMenuClicked(QAction* action);

	void sltSystemTrayIconClicked(QSystemTrayIcon::ActivationReason);

	void sltTrayIconMenuClicked(QAction* action);

private:
	void initPannel();

	void initSysmenu();

	void initSysTrayIcon();

	bool isMax_;
private:
	Ui::MainPannelClass *ui;

	QButtonGroup* btnGroup_;
	QStringList strList_;

	QSystemTrayIcon* trayIcon_;
};
