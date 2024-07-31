#pragma once

#include <QStackedWidget>
#include <qpropertyanimation.h>

#include "comapi/unit.h"

/*!
* @brief 具有滑动特效的StackedWidget类
*/
class SlideStackedWidget  : public QStackedWidget
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	explicit SlideStackedWidget(QWidget *parent);
	/*!
	* @brief 析构函数
	*/
	virtual ~SlideStackedWidget();
	/*!
	* @brief 获取类类型
	*/
	static QString getClassName();
	/*!
	* @brief 起始的StackedWidget页面
	*/
	void start(int index);
	/*!
	* @brief 设置上一个页面和当前页面
	*/
	void setIndex(int previos, int current);
	/*!
	* @brief 设置动画的起始和结束的距离以及动画的滑动方向
	*/
	void setLength(int length, SlideOrientationType type);
	/*!
	* @brief 设置动画的持续时间
	*/
	void setDuration(int duration);
	/*!
	* @brief 构造函数
	*/
	int getDuration() const;

protected:
	/*!
	* @brief 重写绘图事件
	*/
	virtual void paintEvent(QPaintEvent* event) override;
	/*!
	* @brief 获取前一个widget窗口
	*/
	void renderPreviousWidget(QPainter& painter, QTransform transform);
	/*!
	* @brief 获取当前widget窗口
	*/
	void rendCurrentWidget(QPainter& painter, QTransform transform);

private slots:
	/*!
	* @brief 值变化槽函数
	*/
	void sltValueChanged(const QVariant& value);
	/*!
	* @brief 动画结束槽函数
	*/
	void sltAnimationFinished();

private:
	bool isAnimating_;							// 动画是否正在进行
	float currentValue_;						// 当前的移动值
	int currIndex_;								// 当前页索引
	int preIndex_;								// 上一页索引
	SlideOrientationType slideType_;			// 窗口的滑动方向
	QPropertyAnimation* animation_;				// 动画类
};
