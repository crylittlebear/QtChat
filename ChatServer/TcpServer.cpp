#include "TcpServer.h"
#include "DatabaseManager.h"
#include "unit.h"

#include "qdebug.h"

/*!
* ===========================================================
*						服务器管理类
* ===========================================================
*/

TcpServer::TcpServer(QObject *parent)
	: QObject(parent)
{
	tcpServer_ = new QTcpServer(this);
	connect(tcpServer_, &QTcpServer::newConnection, this, &TcpServer::sltNewConnection);
}


TcpServer::~TcpServer()
{
	if (tcpServer_->isListening()) {
		tcpServer_->close();
	}
}

bool TcpServer::startListen(int port) {
	if (tcpServer_->isListening()) {
		tcpServer_->close();
	}
	bool ok = tcpServer_->listen(QHostAddress::Any, port);
	return ok;
}

void TcpServer::closeListen() {
	if (tcpServer_->isListening()) {
		tcpServer_->close();
	}
}

/*!
* ===========================================================
*						 消息服务器
* ===========================================================
*/

TcpMsgServer::TcpMsgServer(QObject* parent)
	: TcpServer(parent)
{

}

TcpMsgServer::~TcpMsgServer()
{
	qDebug() << "Tcp Server close!";
	for (const auto& client : clients_) {
		clients_.removeOne(client);
		client->close();
	}
}

void TcpMsgServer::sltConnected() {
	ServerSocket* c_sock = (ServerSocket*)this->sender();
	if (c_sock == nullptr) { return; }
	connect(c_sock, &ServerSocket::sigMsgToClient, this, &TcpMsgServer::sltMsgToClient);
	connect(c_sock, &ServerSocket::sigDownloadFile, this, &TcpMsgServer::sigDownloadFile);
	emit sigUserStatus(QString("用户 [%1] 上线").arg(
		DatabaseManager::instance()->getUserName(c_sock->getUserId())));
	clients_.push_back(c_sock);
}

void TcpMsgServer::sltDisconnected() {
	ServerSocket* c_sock = (ServerSocket*)this->sender();
	if (c_sock == nullptr) { return; }
	for (int i = 0; i < clients_.size(); ++i) {
		if (clients_[i] == c_sock) {
			clients_.remove(i);
			emit sigUserStatus(QString("用户 [%1] 下线").arg(
				DatabaseManager::instance()->getUserName(c_sock->getUserId())));
		}
	}
	disconnect(c_sock, &ServerSocket::sigConnected, this, &TcpMsgServer::sltConnected);
	disconnect(c_sock, &ServerSocket::sigDisconnected, this, &TcpMsgServer::sltDisconnected);
	disconnect(c_sock, &ServerSocket::sigMsgToClient, this, &TcpMsgServer::sltMsgToClient);
	disconnect(c_sock, &ServerSocket::sigDownloadFile, this, &TcpMsgServer::sigDownloadFile);
}

void TcpMsgServer::sltMsgToClient(const quint8& type, const int& id, const QJsonValue& json) {
	// 查询要发过去的id
	for (int i = 0; i < clients_.size(); ++i) {
		if (id == clients_[i]->getUserId()) {
			clients_[i]->sltSendMessage(type, json);
			return;
		}
	}
}

void TcpMsgServer::sltNewConnection() {
	ServerSocket* c_sock = new ServerSocket(tcpServer_->nextPendingConnection(), this);
	connect(c_sock, &ServerSocket::sigConnected, this, &TcpMsgServer::sltConnected);
	connect(c_sock, &ServerSocket::sigDisconnected, this, &TcpMsgServer::sltDisconnected);
}

void TcpMsgServer::sltTransFileToClient(const int& userId, const QJsonValue& json) {
	// 查询要发送的文件id
	for (int i = 0; i < clients_.size(); ++i) {
		if (userId == clients_[i]->getUserId()) {
			clients_[i]->sltSendMessage(SendFile, json);
			return;
		}
	}
}

