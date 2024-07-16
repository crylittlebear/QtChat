#pragma once

#include <QWidget>
#include "ui_login.h"
#include "rotatestackedwidget.h"
#include "CustomWidget.h"
#include "ClientSocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginClass; };
QT_END_NAMESPACE

class Login : public CustomWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = nullptr);
	~Login();

public slots:
	void on_btnLogin_clicked();

private:
	Ui::LoginClass *ui;
	bool dragging_ = false;
	bool scaling_ = false;
	QPoint dragStartPosition_;
	ClientSocket* tcpSocket_;
};	
