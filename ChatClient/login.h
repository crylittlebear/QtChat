#pragma once

#include <QWidget>
#include "ui_login.h"
#include "rotatestackedwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginClass; };
QT_END_NAMESPACE

class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = nullptr);
	~Login();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

private:
	bool isInDraggableArea(const QPoint& pos);
	bool isInScaleArea(const QPoint& pos);

private:
	Ui::LoginClass *ui;
	bool dragging_ = false;
	bool scaling_ = false;
	QPoint dragStartPosition_;
};	
