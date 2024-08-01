#pragma once

#include <QWidget>
#include "basewidget/CustomWidget.h"
#include "ui_MainPannel.h"

#include <qstringlist.h>

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

private:
	void initPannel();
	bool isMax_;
private:
	Ui::MainPannelClass *ui;

	QButtonGroup* btnGroup_;
	QStringList strList_;
};
