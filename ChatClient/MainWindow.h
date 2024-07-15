#pragma once

#include <QWidget>
#include "ui_MainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	/*!
	* @brief 重写鼠标按下事件
	*/
	virtual void mousePressEvent(QMouseEvent* event) override;

	/*!
	* @brief 重写鼠标拖动事件
	*/
	virtual void mouseMoveEvent(QMouseEvent* event) override;

	/*!
	* @brief 重写鼠标释放事件
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

	/*!
	* @brief 鼠标是否处于可拖动区域内
	*/
	bool isInDragingArea(const QPoint& point) const;

private:
	Ui::MainWindowClass *ui;
	bool draging_ = false;
	QPoint startPoint_;
};
