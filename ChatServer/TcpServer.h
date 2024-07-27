#pragma once

#include <QObject>
#include "qtcpserver.h"

#include "ServerSocket.h"

/*!
* ===========================================================
*						服务器管理类
* ===========================================================
*/
class TcpServer  : public QObject
{
	Q_OBJECT

public:
	explicit TcpServer(QObject *parent = nullptr);
	~TcpServer();

	bool startListen(int port = 9999);
	void closeListen();

signals:
	void sigUserStatus(const QString& text);

protected slots:
	virtual void sltNewConnection() = 0;
	virtual void sltConnected() = 0;
	virtual void sltDisconnected() = 0;

protected:
	QTcpServer* tcpServer_;
};

/*!
* ===========================================================
*						 消息服务器
* ===========================================================
*/
class TcpMsgServer : public TcpServer {
	Q_OBJECT
public:
	explicit TcpMsgServer(QObject* parent = nullptr);
	~TcpMsgServer();

public slots:
	void sltTransFileToClient(const int& userId, const QJsonValue& json);

signals:
	void sigDownloadFile(const QJsonValue& json);

private slots:
	virtual void sltNewConnection() override;
	virtual void sltConnected() override;
	virtual void sltDisconnected() override;
	void sltMsgToClient(const quint8& type, const int& id, const QJsonValue& json);

private:
	// 客户端管理
	QVector<ServerSocket*> clients_;
};

/*!
* ===========================================================
*						 文件服务器
* ===========================================================
*/
class TcpFileServer : public TcpServer {
	Q_OBJECT
public:
	explicit TcpFileServer(QObject* parent = 0);
	~TcpFileServer();
signals:
	void signalRecvFinished(int id, const QJsonValue& json);

private slots:
	virtual void sltNewConnection() override;
	virtual void sltConnected() override;
	virtual void sltDisconnected() override;
	void sltClientDownloadFile(const QJsonValue& json);

private:
	// 客户端管理
	QVector < ServerFileSocket* > clients_;
};