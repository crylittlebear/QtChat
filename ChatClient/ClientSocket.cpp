#include "ClientSocket.h"

#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qjsonarray.h"

#include "comapi/unit.h"
#include "comapi/MyApp.h"

ClientSocket::ClientSocket(QObject *parent)
	: QObject(parent)
{
	id_ = -1;
	socket_ = new QTcpSocket(this);

	connectToServer(MyApp::strHostAddr_, MyApp::msgPort_);

	connect(socket_, &QTcpSocket::readyRead, this, &ClientSocket::sltReadyRead);
	connect(socket_, &QTcpSocket::connected, this, &ClientSocket::sltConnected);
	connect(socket_, &QTcpSocket::disconnected, this, &ClientSocket::sltDisconnected);
}

ClientSocket::~ClientSocket()
{
	sltSendOffline();
}

int ClientSocket::getUserId() const { return id_; }

void ClientSocket::setUserId(int id) { id_ = id; }

void ClientSocket::checkConnected() {
	if (socket_->state() != QTcpSocket::ConnectedState) {
		socket_->connectToHost(MyApp::strHostAddr_, MyApp::msgPort_);
	}
}

void ClientSocket::ClientSocket::closeConnected() {
	if (socket_->isOpen()) {
		socket_->abort();
	}
}

void ClientSocket::connectToServer(const QString& ip, const int& port) {
	// 如果套接字已经连接，则放弃连接
	if (socket_->isOpen()) {
		socket_->abort();
	}
	socket_->connectToHost(ip, port);
}

void ClientSocket::connectToServer(const QHostAddress& host, const int& port) {
	if (socket_->isOpen()) {
		socket_->abort();
	}
	socket_->connectToHost(host, port);
}

void ClientSocket::sltSendMessage(const quint8& type, const QJsonValue& dataVal) {
	// 连接服务器
	if (!socket_->isOpen()) {
		//socket_->connectToHost(QString("192.168.174.136"), 60100);
		socket_->connectToHost(MyApp::strHostAddr_, MyApp::msgPort_);
		socket_->waitForConnected(1000);
	}
	// 如果1s后依然没有连接上服务器，直接返回
	if (!socket_->isOpen()) {
		qDebug() << QString::fromLocal8Bit("连接服务器失败");
		return;
	} 
	// 构建Json对象
	QJsonObject json;
	json.insert("type", type);
	json.insert("from", id_);
	json.insert("data", dataVal);
	// 构建Json文档
	QJsonDocument document;
	document.setObject(json);
	qDebug() << "tcpSocket_->write: " << document.toJson(QJsonDocument::Compact);
	socket_->write(document.toJson(QJsonDocument::Compact));
}

void ClientSocket::sltSendOnline() {
	// TODO 上线的时候给当前好友上报状态
	//QJsonArray frinedAddr = 
	// 组织JsonArray
	//sltSendMessage()
}

void ClientSocket::sltSendOffline() {

}

void ClientSocket::parseLogin(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int code = dataObj.value("code").toInt();
		QString msg = dataObj.value("msg").toString();
		QString satrHead = dataObj.value("head").toString();
		// 登录成功
		if (code == 0 && msg == "ok") {
			id_ = dataObj.value("id").toInt();
			// 保存头像
			emit signalStatus(LoginSuccess);
		}	
		else if (code == -1) {
			// 用户未找到
			emit signalStatus(UserNotFound);
		}
		else if (code == -2) {
			// 密码错误
			emit signalStatus(LoginPasswdErr);
		} else if (code == -3) {
			// 重复登录
			emit signalStatus(LoginRepeat);
		}
	}
}

void ClientSocket::parseRegister(const QJsonValue& dataVal) {

}

void ClientSocket::sltDisconnected() {

}

void ClientSocket::sltConnected() {
	qDebug() << "connected to server!";
	emit signalStatus(ConnectedHost);
}

void ClientSocket::sltReadyRead() {
	// 从socket中读取数据
	QByteArray data = socket_->readAll();
	QJsonParseError jsonErr;
	// 转换为json文档
	QJsonDocument document = QJsonDocument::fromJson(data, &jsonErr);
	// 解析未发生错误
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError) {
		// json文档对象
		if (document.isObject()) {
			// 转化为对象
			QJsonObject jsonObj = document.object();
			QJsonValue dataVal = jsonObj.value("data");
			int type = jsonObj.value("type").toInt();
			int from = jsonObj.value("from").toInt();
			switch (type) {
			case LoginSuccess:
			case UserNotFound:
			case LoginPasswdErr:
			case LoginRepeat:
				parseLogin(dataVal);
				break;
			}
		}
	}
}