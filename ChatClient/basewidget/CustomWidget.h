#pragma once

#include <QWidget>

/*!
*  @brief 本程序使用的窗口类的基类,可以满足拖动和缩放需求
*/
class CustomWidget : public QWidget
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	CustomWidget(QWidget *parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomWidget();

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

private:
	/*!
	* @brief 判断鼠标是否处于拖动区域内
	*/
	bool isInDragingArea(const QPoint& point) const;

	/*!
	* @brief 判断鼠标是否处于缩放区域内
	*/
	bool isInScalingArea(const QPoint& point) const;

	/*!
	* @brief 判断是否处于窗口右侧缩放区域
	*/
	bool isInRightScalingArea(const QPoint& point) const;

	/*!
	* @brief 判断是否处于窗口下侧缩放区域
	*/
	bool isInBottomScalingArea(const QPoint& point) const;

	/*!
	* @brief 判断是否处于窗口右侧缩放区域
	*/
	bool isInBottomRightScalingArea(const QPoint& point) const;

private:
	int dragHeight_;
	bool isDraging_ = false;	/// 是否正在拖动中
	bool isScaling_ = false;	/// 是否正在缩放中
	QPoint dragStartPoint_;		/// 拖动起始鼠标位置窗口内坐标
};
