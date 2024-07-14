#pragma once

#include <QLineEdit>

class IpLineEdit  : public QLineEdit
{
	Q_OBJECT

public:
	/*!
	* @brief 析构函数
	*/
	IpLineEdit(QWidget *parent = nullptr);

	/*!
	* @brief 构造函数
	*/
	~IpLineEdit();
	
	/*!
	* @brief 获取输入的IP地址
	*/
	QString text();

protected:
	/*!
	* @brief 事件过滤，处理按键事件
	*/
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
	int getIndex(QLineEdit* lineEdit);

private:
	QLineEdit* lineEdits_[4];
};
