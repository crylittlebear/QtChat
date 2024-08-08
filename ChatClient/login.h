#pragma once

#include <QWidget>
#include "ui_login.h"
#include "rotatestackedwidget.h"
#include "CustomWidget.h"
#include "ClientSocket.h"
#include "MainPannel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginClass; };
QT_END_NAMESPACE

class Login : public CustomWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = nullptr);
	~Login();


signals:
	void sigRotate();

public slots:
	void sltBtnLoginClicked();

	void sltTcpStatus(const quint8& status);

private:
	void initWidget();

private:
	Ui::LoginClass *ui;

	ClientSocket* tcpSocket_;

	bool connected_;

	MainPannel* mainWindow_;
};	
