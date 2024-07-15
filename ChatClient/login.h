#pragma once

#include <QWidget>
#include "ui_login.h"
#include "rotatestackedwidget.h"
#include "CustomWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginClass; };
QT_END_NAMESPACE

class Login : public CustomWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = nullptr);
	~Login();

private:
	Ui::LoginClass *ui;
	bool dragging_ = false;
	bool scaling_ = false;
	QPoint dragStartPosition_;
};	
