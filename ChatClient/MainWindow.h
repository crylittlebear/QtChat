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
	* @brief ��д��갴���¼�
	*/
	virtual void mousePressEvent(QMouseEvent* event) override;

	/*!
	* @brief ��д����϶��¼�
	*/
	virtual void mouseMoveEvent(QMouseEvent* event) override;

	/*!
	* @brief ��д����ͷ��¼�
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

	/*!
	* @brief ����Ƿ��ڿ��϶�������
	*/
	bool isInDragingArea(const QPoint& point) const;

private:
	Ui::MainWindowClass *ui;
	bool draging_ = false;
	QPoint startPoint_;
};
