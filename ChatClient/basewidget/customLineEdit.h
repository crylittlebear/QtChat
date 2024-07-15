#pragma once

#include <QLineEdit>
#include "qlabel.h"

class IpLineEdit  : public QLineEdit
{
	Q_OBJECT

public:
	/*!
	* @brief ��������
	*/
	IpLineEdit(QWidget *parent = nullptr);

	/*!
	* @brief ���캯��
	*/
	~IpLineEdit();
	
	/*!
	* @brief ��ȡ�����IP��ַ
	*/
	QString text();

protected:
	/*!
	* @brief �¼����ˣ��������¼�
	*/
	virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
	int getIndex(QLineEdit* lineEdit);

private:
	QLineEdit* lineEdits_[4];
};

/*
* ////////////////////////////////////////////////////////////////////////
*							IconLineEdit
* ////////////////////////////////////////////////////////////////////////
*/


class IconLineEdit : public QLineEdit {
	Q_OBJECT
public:
	/*!
	* @brief ���캯��
	*/
	IconLineEdit(QWidget* parent = nullptr);
	/*!
	* @brief ��������
	*/
	~IconLineEdit();

	void setPixmap(const QPixmap& pixmap);

private:
	QLabel* label_;
};