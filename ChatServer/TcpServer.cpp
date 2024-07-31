#include "TcpServer.h"
#include "DatabaseManager.h"
#include "unit.h"

#include "qdebug.h"
#include "qjsonobject.h"

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
	: TcpServer(parent) {

}

TcpMsgServer::~TcpMsgServer() {
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
	emit sigUserStatus(QString::fromLocal8Bit("用户 [%1] 上线").arg(
		DatabaseManager::instance()->getUserName(c_sock->getUserId())));
	clients_.push_back(c_sock);
}

void TcpMsgServer::sltDisconnected() {
	ServerSocket* c_sock = (ServerSocket*)this->sender();
	if (c_sock == nullptr) { return; }
	for (int i = 0; i < clients_.size(); ++i) {
		if (clients_[i] == c_sock) {
			clients_.remove(i);
			emit sigUserStatus(QString::fromLocal8Bit("用户 [%1] 下线").arg(
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

/*!
* ===========================================================
*						 文件服务器
* ===========================================================
*/

TcpFileServer::TcpFileServer(QObject* parent) : TcpServer(parent) {

}

TcpFileServer::~TcpFileServer() {
	qDebug() << "File Tcp server close";
	foreach(ServerFileSocket* client, clients_) {
		clients_.removeOne(client);
		client->close();
	}
}

void TcpFileServer::sltConnected() {
	ServerFileSocket* sock = (ServerFileSocket*)this->sender();
	if (sock == nullptr) return;
	clients_.push_back(sock);
}

void TcpFileServer::sltDisconnected() {
	ServerFileSocket* sock = (ServerFileSocket*)this->sender();
	if (sock == nullptr) return;
	disconnect(sock, &ServerFileSocket::sigConnected, this, &TcpFileServer::sltConnected);
	disconnect(sock, &ServerFileSocket::sigDisconnected, this, &TcpFileServer::sltDisconnected);
}

void TcpFileServer::sltClientDownloadFile(const QJsonValue& json) {
	if (json.isObject()) {
		QJsonObject jsonObj = json.toObject();
		int userId = jsonObj.value("from").toInt();
		int toId = jsonObj.value("to").toInt();
		QString fileName = jsonObj.value("msg").toString();
		qDebug() << "get file: " << fileName << ", client size: " << clients_.size();
		for (int i = 0; i < clients_.size(); ++i) {
			if (clients_.at(i)->checkUserId(userId, toId)) {
				clients_.at(i)->startTransferFile(fileName);
				return;
			}
		}
	}
}

void TcpFileServer::sltNewConnection() {
	ServerFileSocket* sock = new ServerFileSocket(this, tcpServer_->nextPendingConnection());
	connect(sock, &ServerFileSocket::sigConnected, this, &TcpFileServer::sltConnected);
	connect(sock, &ServerFileSocket::sigDisconnected, this, &TcpFileServer::sltDisconnected);
}

