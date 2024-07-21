/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#pragma once

#include "qwidget.h"
#include "qevent.h"
#include "qdialog.h"
#include "qlabel.h"
#include "qlineedit.h"

/*!
* =================================================================================================
*									CustomWidget
* =================================================================================================
*/

class CustomWidget : public QWidget
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	CustomWidget(QWidget* parent = nullptr);

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
	int dragHeight_;			/// 拖动窗口的高度
	bool isDraging_ = false;	/// 是否正在拖动中
	bool isScaling_ = false;	/// 是否正在缩放中
	QPoint dragStartPoint_;		/// 拖动起始鼠标位置窗口内坐标
};


/*!
* =================================================================================================
*									CustomDialog
* =================================================================================================
*/

class CustomDialog : public QDialog {
	Q_OBJECT
public:
	/*!
	* @brief 构造函数
	*/
	explicit CustomDialog(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomDialog();

protected:
	/*!
	* @brief 重写鼠标按下事件
	*/
	virtual void mousePressEvent(QMouseEvent* event) override;

	/*!
	* @brief 重写鼠标移动事件
	*/
	virtual void mouseMoveEvent(QMouseEvent* event) override;

	/*!
	* @brief 重写鼠标释放事件
	*/
	virtual void mouseReleaseEvent(QMouseEvent* event) override;

protected:
	QPoint mousePoint_;
	bool mousePressed_;
};

/*!
* =================================================================================================
*									CustomBaseDialog
* =================================================================================================
*/
class CustomBaseDialog : public CustomDialog {
	Q_OBJECT
public:
	/*!
	* @brief 构造函数
	*/
	explicit CustomBaseDialog(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomBaseDialog();

	/*!
	* @brief 设置窗口图标
	*/
	void setWinIcon(QPixmap pixmap);

	/*!
	* @brief 设置窗口标题
	*/
	void setWinTitle(const QString& text);

protected:
	QWidget* widgetBody_;

private:
	QWidget* widgetWindowTitle_;
	QLabel* labelWinIcon_;
	QLabel* labelWinTitle_;
	QPushButton* btnWinMin_;
	QPushButton* btnWinClose_;
};

/*!
* =================================================================================================
*									CustomMessageBox
* =================================================================================================
*/
class CustomMessageBox : public CustomBaseDialog {
	Q_OBJECT
public:
	typedef enum { Information = 0x01, Warning, Question, Error } E_MSGBOX_TYPE;

public:
	/*!
	* @brief 构造函数
	*/
	CustomMessageBox(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomMessageBox();

	/*!
	* @brief 显示消息
	*/
	void showMessage(const QString& content, 
					 const quint8& msgType = CustomMessageBox::Information, 
					 const QString& title = "");
	/*!
	* @brief 构造函数
	*/
	void startTimer();

	/*!
	* @brief 显示信息提示窗口
	*/
	static int information(QWidget* parent, const QString& content, const QString& title = "提示");

	/*!
	* @brief 显示信息提示窗口
	*/
	static int question(QWidget* parent, const QString& content, const QString& title = "询问");

	/*!
	* @brief 显示信息提示窗口
	*/
	static int warning(QWidget* parent, const QString& content, const QString& title = "告警");

public slots:
	void sltTimerOut();

private:
	QLabel* labelIcon_;			/// 提示窗口图标
	QLabel* labelMsgContent_;		/// 提示窗口内容

	QPushButton* btnOk_;		/// 确认按钮
	QPushButton* btnCancel_;	/// 取消按钮

	QTimer* timer_;				/// 定时器
	int timerCount_;			
};

/*!
* =================================================================================================
*									CustomInputDialog
* =================================================================================================
*/

class CustomInputDialog : public CustomBaseDialog {
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	explicit CustomInputDialog(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomInputDialog();

	/*!
	* @brief 获取输入文档
	*/
	static QString getInputText(QWidget* parent,
		const QString& text = "",
		const QString& title = "输入",
		QLineEdit::EchoMode mode = QLineEdit::Password);

	/*!
	* @brief 
	*/
	QString getText() const;

	/*!
	* @brief 设置输入文档
	*/
	void setInputText(const QString& text);

	/*!
	* @brief 设置密码显示模式
	*/
	void setEchoMode(QLineEdit::EchoMode mode);

private:
	QLabel* labelText_;
	QLineEdit* lineEditInput_;
	QPushButton* btnOk_;
	QPushButton* btnCancel_;
};
