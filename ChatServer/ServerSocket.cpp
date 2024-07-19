#include "ServerSocket.h"

#include "qdebug.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "DatabaseManager.h"
#include "unit.h"


ServerSocket::ServerSocket(QTcpSocket* socket, QObject *parent)
	: QObject(parent) {
	id_ = -1;
	if (socket == nullptr) {
		tcpSocket_ = new QTcpSocket(this);
	}
	else {
		tcpSocket_ = socket;
	}
	connect(tcpSocket_, &QTcpSocket::readyRead, this, &ServerSocket::sltReadyRead);
	connect(tcpSocket_, &QTcpSocket::connected, this, &ServerSocket::sltConnected);
	connect(tcpSocket_, &QTcpSocket::disconnected, this, &ServerSocket::sltDisconnected);
}

ServerSocket::~ServerSocket() {

}

int ServerSocket::getUserId() const {
	return id_;
}

void ServerSocket::close() {
	tcpSocket_->abort();
}

void ServerSocket::sltReadyRead() {
	// 从套接字中读取待读数据
	QByteArray request = tcpSocket_->readAll();
	QJsonParseError jsonErr;
	// 将数据转化为JsonDocument格式
	QJsonDocument document = QJsonDocument::fromJson(request, &jsonErr);
	// 解析未发生错误
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError) {
		if (document.isObject()) {
			QJsonObject jsonObj = document.object();
			int type = jsonObj.value("type").toInt();
			QJsonValue jsonVal = jsonObj.value("data");
			switch (type) {
			case Register:
				parseRegister(jsonVal);
				break;
			case Login:
				parseLogin(jsonVal);
				break;
			case UserOnline:
				parseUserOnline(jsonVal);
				break;
			case Logout:
				parseLogout(jsonVal);
				emit sigDisconnected();
				tcpSocket_->abort();
				break;
			case UpdateHeadPic:
				parseUpdateUserHead(jsonVal);
				break;
			case AddFriend:
				parseAddFriend(jsonVal);
				break;
			case AddGroup:
				parseAddGroup(jsonVal);
				break;
			case CreateGroup:
				parseCreateGroup(jsonVal);
				break;
			case GetMyFriends:
				parseGetMyFriend(jsonVal);
				break;
			case GetMyGroups:
				parseGetMyGroups(jsonVal);
				break;
			case RefreshFriends:
				parseRefreshFriend(jsonVal);
				break;
			case RefreshGroup:
				parseRefreshGroups(jsonVal);
			case SendMsg:
			case SendPicture:
			case SendFile:
				parseFriendMessages(request);
				break;
			case SendGroupMsg:
				parseGroupMessages(request);
				break;
			case SendFace:
				parseFaceMessages(request);
				break;
			case SendFileOk:
				break;
			case GetFile:
				emit sigDownloadFile(jsonVal);
			default:
				break;
			}
		}
	}
}

void ServerSocket::sltSendMessage(const quint8& type, const QJsonValue& json) {

}

void ServerSocket::parseLogin(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject jsonObj = dataVal.toObject();
		QString name = jsonObj.value("name").toString();
		QString passwd = jsonObj.value("passwd").toString();
		QJsonObject responseObj = DatabaseManager::instance()->checkUserLogin(name, passwd);
		int code = responseObj.value("code").toInt();
		switch (code) {
		case 0:
			sltSendMessage(LoginSuccess, responseObj);
			break;
		case -1:
			sltSendMessage(UserNotFound, responseObj);
			break;
		case -2:
			sltSendMessage(LoginPasswdErr, responseObj);
			break;
		case -3:
			sltSendMessage(LoginRepeat, responseObj);
			break;
		default:
			break;
		}
	}
}

void ServerSocket::parseUserOnline(const QJsonValue& dataVal) {
	if (dataVal.isArray()) {
		QJsonArray jsonArr = dataVal.toArray();
		int size = jsonArr.size();
		for (int i = 0; i < size; ++i) {
			int id = jsonArr.at(i).toInt();
			int status = DatabaseManager::instance()->getUserLineStatus(id);
			// 如果当前好友在线
			if (status == Online) {
				QJsonObject jsonObj;
				jsonObj.insert("id", id);
				jsonObj.insert("text", "online");
				emit sigMsgToClient(UserOnline, id, jsonObj);
			}
		}
	}
}

void ServerSocket::parseLogout(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject jsonObj = dataVal.toObject();
		QJsonArray jsonArr = jsonObj.value("friends").toArray();
		int id = jsonObj.value("id").toInt();
		int size = jsonArr.size();
		DatabaseManager::instance()->updateUserStatus(id, Logout);
		for (int i = 0; i < size; ++i) {
			id = jsonArr.at(i).toInt();
			int status = DatabaseManager::instance()->getUserLineStatus(id);
			if (status == Online) {
				QJsonObject jsonObj;
				jsonObj.insert("id", id);
				jsonObj.insert("text", "offline");
				emit sigMsgToClient(UserOffline, id, jsonObj);
			}
		}
	}
}

void ServerSocket::parseUpdateUserHead(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int id = dataObj.value("id").toInt();
		QString strHead = dataObj.value("head").toString();
		qDebug() << "In function parseUpdateUserHead(), id = "
			<< id << ", strHead = " << strHead;
		DatabaseManager::instance()->updateUserHead(id, strHead);
		QJsonArray jsonArr = dataObj.value("friend").toArray();
		QJsonObject jsonObj;
		jsonObj.insert("id", id);
		jsonObj.insert("head", strHead);
		for (int i = 0; i < jsonArr.size(); ++i) {
			id = jsonArr.at(i).toInt();
			emit sigMsgToClient(UpdateHeadPic, id, jsonObj);
		}
	}
}

void ServerSocket::parseRegister(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject jsonObj = dataVal.toObject();
		QString name = jsonObj.value("name").toString();
		QString passwd = jsonObj.value("passwd").toString();
		int regId = DatabaseManager::instance()->registerUser(name, passwd);
		// 将注册完成的信息返回客户端
		QJsonObject responseObj;
		responseObj.insert("id", regId);
		responseObj.insert("name", name);
		responseObj.insert("passwd", passwd);
		// modified by heng
		if (regId == -1) {
			sltSendMessage(RegisterFailed, responseObj);
		}
		else {
			sltSendMessage(RegisterOk, responseObj);
		}
	}
}

void ServerSocket::parseAddFriend(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		QString strName = dataObj.value("name").toString();
		QJsonObject jsonFriend = DatabaseManager::instance()->addFriend(strName);
		int id = jsonFriend.value("id").toInt();
		QString strMsg = jsonFriend.value("msg").toString();
		// 将查询结果发送到客户端
		sltSendMessage(AddFriend, jsonFriend);
		if (id < 0) { return; }
		// 给对方发送添加请求
		QJsonObject jsonQuery = DatabaseManager::instance()->getUserInfo(id);
		QJsonObject jsonResponse;
		jsonResponse.insert("id", id);
		jsonResponse.insert("name", jsonQuery.value("name").toString());
		jsonResponse.insert("head", jsonQuery.value("head").toString());
		jsonResponse.insert("msg", strMsg.isEmpty() ? "附加消息: 你好!" : strMsg);
		emit sigMsgToClient(AddFriendResponse, id, jsonResponse);
	}
}

void ServerSocket::parseAddGroup(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int id = dataObj.value("id").toInt();
		QString strName = dataObj.value("name").toString();
		auto json = DatabaseManager::instance()->addGroup(id, strName);
		// 发送消息到客户端
		sltSendMessage(AddGroup, json);
	}
}

void ServerSocket::parseCreateGroup(const QJsonValue& dataVal) {
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int id = dataObj.value("id").toInt();
		QString strName = dataObj.value("id").toString();
		auto json = DatabaseManager::instance()->createGroup(id, strName);
		// 发送
		sltSendMessage(CreateGroup, json);
	}
}

void ServerSocket::parseGetMyFriend(const QJsonValue& dataVal) {
	QJsonArray responseArr;
	if (dataVal.isArray()) {
		QJsonArray dataArr = dataVal.toArray();
		int size = dataArr.size();
		for (int i = 0; i < size; ++i) {
			int id = dataArr.at(i).toInt();
			responseArr.append(DatabaseManager::instance()->getUserStatus(id));
		}
	}
	sltSendMessage(GetMyFriends, responseArr);
}

void ServerSocket::parseGetMyGroups(const QJsonValue& dataVal) {
	QJsonArray responseArr;
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int id = dataObj.value("id").toInt();
		responseArr = DatabaseManager::instance()->getGroupUsers(id);
	}
	sltSendMessage(GetMyGroups, responseArr);
}

void ServerSocket::parseRefreshFriend(const QJsonValue& dataVal) {
	QJsonArray jsonArr;
	if (dataVal.isArray()) {
		QJsonArray dataArr = dataVal.toArray();
		int size = dataArr.size();
		for (int i = 0; i < size; ++i) {
			int id = dataArr.at(i).toInt();
			jsonArr.append(DatabaseManager::instance()->getUserStatus(id));
		}
	}
	sltSendMessage(RefreshFriends, jsonArr);
}

void ServerSocket::parseRefreshGroups(const QJsonValue& dataVal) {
	QJsonArray jsonArr;
	if (dataVal.isObject()) {
		QJsonObject dataObj = dataVal.toObject();
		int id = dataObj.value("id").toInt();
		DatabaseManager::instance()->getGroupUsers(id);
	}
	sltSendMessage(RefreshGroup, jsonArr);
}

void ServerSocket::parseFriendMessages(const QByteArray& reply) {
	// 重新组装数据
	QJsonParseError jsonErr;
	// 转化为json文档
	QJsonDocument document = QJsonDocument::fromJson(reply, &jsonErr);
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError) {
		if (document.isObject()) {
			QJsonObject dataObj = document.object();
			int type = dataObj.value("type").toInt();
			QJsonValue dataVal = dataObj.value("data");
			QJsonObject jsonObj = dataVal.toObject();
			int id = jsonObj.value("id").toInt();
			sigMsgToClient(type, id, jsonObj);
		}
	}
}

void ServerSocket::parseGroupMessages(const QByteArray& reply) {

}

void ServerSocket::parseFaceMessages(const QByteArray& reply)
{
}

void ServerSocket::sltConnected() {
	qDebug() << "connected()";
}

void ServerSocket::sltDisconnected() {
	qDebug() << "disconnected()";
	DatabaseManager::instance()->updateUserStatus(id_, Offline);
	emit sigDisconnected();
}


