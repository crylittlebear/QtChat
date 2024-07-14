#pragma once

#include <QLineEdit>

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
