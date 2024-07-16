#include "ClientSocket.h"

#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qjsonarray.h"

ClientSocket::ClientSocket(QObject *parent)
	: QObject(parent)
{
	id_ = -1;
	socket_ = new QTcpSocket(this);

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
		// TODO
		//socket_->connectToHost();
	}
}

void ClientSocket::ClientSocket::closeConnected() {
	if (socket_->isOpen()) {
		socket_->abort();
	}
}

void ClientSocket::connectToServer(const QString& ip, const int& port) {
	// ����׽����Ѿ����ӣ����������
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
	// ���ӷ�����
	if (!socket_->isOpen()) {
		//socket_->connectToHost(QString("127.0.0.1"))
		socket_->waitForConnected(1000);
	}
	// ���1s����Ȼû�������Ϸ�������ֱ�ӷ���
	if (!socket_->isOpen()) {
		return;
	}
	// ����Json����
	QJsonObject json;
	json.insert("type", type);
	json.insert("from", id_);
	json.insert("data", dataVal);
	// ����Json�ĵ�
	QJsonDocument document;
	document.setObject(json);
	qDebug() << "tcpSocket_->write: " << document.toJson(QJsonDocument::Compact);
	socket_->write(document.toJson(QJsonDocument::Compact));
}

void ClientSocket::sltSendOnline() {
	// TODO ���ߵ�ʱ�����ǰ�����ϱ�״̬
	//QJsonArray frinedAddr = 
	// ��֯JsonArray
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
		
		if (code == 0 && msg == "ok") {
			id_ = dataObj.value("id").toInt();
			// ����ͷ��

			//emit signalStatus(LoginSuccess);
		}
		else if (code == -1) {
			//emit signalStatus(LoginPasswdError);
		}
		else if (code == -2) {
			//emit signalStatus(LoginRepeat);
		}
	}
}

void ClientSocket::parseRegister(const QJsonValue& dataVal) {

}

void ClientSocket::sltDisconnected() {

}

void ClientSocket::sltConnected() {

}

void ClientSocket::sltReadyRead() {
	// ��socket�ж�ȡ����
	QByteArray data = socket_->readAll();
	QJsonParseError jsonErr;
	// ת��Ϊjson�ĵ�
	QJsonDocument document = QJsonDocument::fromJson(data, &jsonErr);
	// ����δ��������
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError) {
		// json�ĵ�����
		if (document.isObject()) {
			// ת��Ϊ����
			QJsonObject jsonObj = document.object();
			QJsonValue dataVal = jsonObj.value("data");
			int type = jsonObj.value("type").toInt();
			int from = jsonObj.value("from").toInt();
			if (type == 17) {
				QJsonObject dataObj = dataVal.toObject();
				int id = dataObj.value("id").toInt();
				if (id > 0) {
					qDebug() << "��¼�ɹ�";
					id_ = id;
				}
				else {
					qDebug() << "��¼ʧ��";
				}
			}
		}
	}
}