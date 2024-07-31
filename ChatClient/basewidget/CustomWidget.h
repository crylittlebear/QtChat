#pragma once

#include <QWidget>
#include <qdialog.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

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
	QPoint mousePoint_;		/// 记录鼠标按下位置的相对位置
	bool mousePressed_;		/// 判断鼠标是否按下
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
	QWidget* widgetBody_;			/// 消息窗口的窗体部分
	QLabel* labelWinIcon_;			/// 消息窗口图标

private:
	QWidget* widgetWindowTitle_;	/// 消息窗口的标题部分
	QLabel* labelWinTitle_;			/// 消息窗口标题显示部分
	QPushButton* btnWinMin_;		/// 消息窗口最小化按钮
	QPushButton* btnWinClose_;		/// 消息窗口关闭按钮
};

/*!
* =================================================================================================
*									CustomMessageBox
* =================================================================================================
*/
class CustomMessageBox : public CustomBaseDialog {
	Q_OBJECT
public:
	typedef enum { 
		Information = 0x01, 
		Warning		= 0x02,
		Question	= 0x03,
		Error		= 0x04,
	} E_MSGBOX_TYPE;

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
	* @brief 开启窗口显示时间定时器
	*/
	void startTimer();

	/*!
	* @brief 显示信息提示窗口
	*/
	static int information(QWidget* parent, 
						   const QString& content, 
						   const QString& title = QString::fromLocal8Bit("信息"));

	/*!
	* @brief 显示问题提示窗口
	*/
	static int question(QWidget* parent, 
						const QString& content, 
						const QString& title = QString::fromLocal8Bit("询问"));

	/*!
	* @brief 显示警告提示窗口
	*/
	static int warning(QWidget* parent, 
					   const QString& content, 
					   const QString& title = QString::fromLocal8Bit("警告"));

	/*!
	* @brief 显示警告提示窗口
	*/
	static int error(QWidget* parent,
				       const QString& content,
		               const QString& title = QString::fromLocal8Bit("错误"));

public slots:
	/*!
	* @brief 定时器到期槽函数，消息窗口倒计时显示减一
	*/
	void sltTimerOut();

private:
	QLabel* labelIcon_;				/// 提示窗口图标
	QLabel* labelMsgContent_;		/// 提示窗口内容

	QPushButton* btnOk_;			/// 确认按钮
	QPushButton* btnCancel_;		/// 取消按钮

	QTimer* timer_;					/// 定时器
	int timerCount_;				/// 窗口倒计时秒数
};

/*!
* =================================================================================================
*									CustomInputDialog
* =================================================================================================
*/
/// 自定义输入对话框
class CustomInputDialog : public CustomBaseDialog {
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	explicit CustomInputDialog(QString title, QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~CustomInputDialog();

	/*!
	* @brief 获取输入文档
	*/
	QStringList getInputText();

	/*!
	* @brief 获取用户和密码信息
	*/
	QStringList getStringList() const;

	/*!
	* @brief 设置输入文档
	*/
	void setTitle(const QString& text);

	/*!
	* @brief 设置密码显示模式
	*/
	void setEchoMode(QLineEdit::EchoMode mode);

private:
	QLabel* labelText1_;			/// 窗口显示标签
	QLabel* labelText2_;			/// 窗口显示标签
	QLineEdit* lineEditInput1_;		/// 输入控件
	QLineEdit* lineEditInput2_;		/// 输入控件
	QPushButton* btnOk_;			/// 确定按钮
	QPushButton* btnCancel_;		/// 取消按钮

	QStringList list_;
};

/*!
* =================================================================================================
*									CustomLabelLineEdit
* =================================================================================================
*/
/// 自定义LineEdit控件，实现在lineEdit的前面部分显示图标
class LabelLineEdit : public QLineEdit {
public:
	/*!
	* @brief 构造函数
	*/
	LabelLineEdit(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~LabelLineEdit();

	/*!
	* @brief 为lineEdit中的label设置图片
	*/
	void setPixmap(const QString& pixmapPath);

private:
	QLabel* label_;				/// lineEdit中的图片显示label
};


class IconChangeButton : public QPushButton {
public:
	IconChangeButton(QWidget* parent = nullptr);

	void setEnterLeaveIcon(const QString& enter, const QString& leave);

protected:
	virtual void enterEvent(QEvent* event) override;

	virtual void leaveEvent(QEvent* event) override;

private:
	QString enter_;
	QString leave_;
};