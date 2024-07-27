#pragma once

#include <QObject>
#include "qtcpsocket.h"

class ClientSocket  : public QObject
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	explicit ClientSocket(QObject *parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~ClientSocket();

	/*!
	* @brief 获取和设置用户ID
	*/
	int getUserId() const;
	void setUserId(int id);

	/*!
	* @brief 检查是否已经连接
	*/
	void checkConnected();

	/*!
	* @brief 主动断开连接
	*/
	void closeConnected();

	/*!
	* @brief 连接到服务器
	*/
	void connectToServer(const QString& ip, const int& port);
	void connectToServer(const QHostAddress& host, const int& port);

signals:
	/*!
	* @brief 消息信号
	*/
	void signalMessage(const quint8& type, const QJsonValue& dataVal);

	/*!
	* @brief 状态信号
	*/
	void signalStatus(const quint8& state);

public slots:
	/*!
	* @brief socket消息发送封装 
	*/
	void sltSendMessage(const quint8& type, const QJsonValue& dataVal);

	/*!
	* @brief 发送上线通知
	*/
	void sltSendOnline();

	/*!
	* @brief 发送下线通知
	*/
	void sltSendOffline();

private:
	/*!
	* @brief 解析登录返回信息
	*/
	void parseLogin(const QJsonValue& dataVal);

	/*!
	* @brief 解析注册返回信息
	*/
	void parseRegister(const QJsonValue& dataVal);

private slots:
	/*!
	* @brief 与服务器断开连接
	*/
	void sltDisconnected();

	/*!
	* @brief 成功连接服务器
	*/
	void sltConnected();

	/*!
	*@brief 处理Tcp消息
	*/
	void sltReadyRead();

private:
	QTcpSocket* socket_;
	int id_;
};
