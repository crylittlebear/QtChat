#pragma once

#include <QObject>
#include "qtcpsocket.h"

class ClientSocket  : public QObject
{
	Q_OBJECT

public:
	/*!
	* @brief ���캯��
	*/
	explicit ClientSocket(QObject *parent = nullptr);

	/*!
	* @brief ��������
	*/
	~ClientSocket();

	/*!
	* @brief ��ȡ�������û�ID
	*/
	int getUserId() const;
	void setUserId(int id);

	/*!
	* @brief ����Ƿ��Ѿ�����
	*/
	void checkConnected();

	/*!
	* @brief �����Ͽ�����
	*/
	void closeConnected();

	/*!
	* @brief ���ӵ�������
	*/
	void connectToServer(const QString& ip, const int& port);
	void connectToServer(const QHostAddress& host, const int& port);

signals:
	/*!
	* @brief ��Ϣ�ź�
	*/
	void signalMessage(const quint8& type, const QJsonValue& dataVal);

	/*!
	* @brief ״̬�ź�
	*/
	void signalStatus(const quint8& state);

public slots:
	/*!
	* @brief socket��Ϣ���ͷ�װ 
	*/
	void sltSendMessage(const quint8& type, const QJsonValue& dataVal);

	/*!
	* @brief ��������֪ͨ
	*/
	void sltSendOnline();

	/*!
	* @brief ��������֪ͨ
	*/
	void sltSendOffline();

private:
	/*!
	* @brief ������¼������Ϣ
	*/
	void parseLogin(const QJsonValue& dataVal);

	/*!
	* @brief ����ע�᷵����Ϣ
	*/
	void parseRegister(const QJsonValue& dataVal);

private slots:
	/*!
	* @brief ��������Ͽ�����
	*/
	void sltDisconnected();

	/*!
	* @brief �ɹ����ӷ�����
	*/
	void sltConnected();

	/*!
	*@brief ����Tcp��Ϣ
	*/
	void sltReadyRead();

private:
	QTcpSocket* socket_;
	int id_;
};
