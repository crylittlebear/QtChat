/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#include "ServerSocket.h"
#include "MyApp.h"

#include "qdatastream.h"
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

void ServerSocket::sltSendMessage(const quint8& type, const QJsonValue& jsonVal) {
	if (!tcpSocket_->isOpen()) {
		return;
	}
	QJsonObject jsonObj;
	jsonObj.insert("type", type);
	jsonObj.insert("id", id_);
	jsonObj.insert("data", jsonVal);
	// 构建json文档
	QJsonDocument document;
	document.setObject(jsonObj);
	// 输出Debug信息
	qDebug() << "tcpSocket_->write()" << document.toJson(QJsonDocument::Compact);
	// 发送信息
	tcpSocket_->write(document.toJson(QJsonDocument::Compact));
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
			// 转发的群组id
			int groupId = jsonObj.value("to").toInt();
			QString strMsg = jsonObj.value("msg").toString();
			QString name = DatabaseManager::instance()->getUserName(groupId);
			// 查询该群组里面的在线好友
			QJsonArray jsonArr = DatabaseManager::instance()->getGroupUsers(groupId);
			if (jsonArr.size() < 2) { return; }
			for (int i = 0; i < jsonArr.size(); ++i) {
				QJsonObject json = jsonArr.at(i).toObject();
				int status = json.value("status").toInt();
				int userId = json.value("id").toInt();
				if (status == Online && userId != id_) {
					QJsonObject jsonMsg;
					jsonMsg.insert("group", groupId);
					jsonMsg.insert("id", id_);
					jsonMsg.insert("name", name);
					jsonMsg.insert("to", userId);
					jsonMsg.insert("msg", strMsg);
					jsonMsg.insert("head", DatabaseManager::instance()->getUserHead(id_));
					emit sigMsgToClient(type, userId, jsonMsg);
				}
			}
		}
	}
}

void ServerSocket::parseFaceMessages(const QByteArray& reply) {
	QJsonParseError jsonErr;
	QJsonDocument document = QJsonDocument::fromJson(reply, &jsonErr);
	if (!document.isNull() && jsonErr.error == QJsonParseError::NoError) {
		if (document.isObject()) {
			QJsonObject jsonObj = document.object();
			int type = jsonObj.value("type").toInt();
			QJsonValue dataVal = jsonObj.value("data");
			QJsonObject dataObj = dataVal.toObject();
			int id = dataObj.value("id").toInt();
			emit sigMsgToClient(type, id, dataObj);
		}
	}
}

void ServerSocket::sltConnected() {
	qDebug() << "connected()";
}

void ServerSocket::sltDisconnected() {
	qDebug() << "disconnected()";
	DatabaseManager::instance()->updateUserStatus(id_, Offline);
	emit sigDisconnected();
}

/*!
* =======================================================================
*                           文件套接字
* =======================================================================
*/

ServerFileSocket::ServerFileSocket(QObject* parent, QTcpSocket* socket) : QObject(parent) {
	loadSize_ = 50 * 1024;
	totalSendBytes_ = 0;
	totalRecvBytes_ = 0;
	bytesWritten_ = 0;
	bytesToWrite_ = 0;
	bytesReceived_ = 0;

	fileNameSize_ = 0;
	busy_ = false;

	userId_ = -1;
	msgToId_ = -1;

	// 本地文件存储
	fileToSend_ = new QFile(this);
	fileToRecv_ = new QFile(this);

	// 客户端
	if (socket == nullptr) {
		tcpSocket_ = new QTcpSocket(this);
	}
	tcpSocket_ = socket;

	// 信号槽
	connect(tcpSocket_, &QTcpSocket::readyRead, this, &ServerFileSocket::sltReadyRead);
	connect(tcpSocket_, &QTcpSocket::disconnected, this, &ServerFileSocket::sigDisconnected);
	connect(tcpSocket_, &QTcpSocket::bytesWritten, this, &ServerFileSocket::sltUpdateClientProcess);
}

ServerFileSocket::~ServerFileSocket() {

}

void ServerFileSocket::close() {
	tcpSocket_->abort();
}

bool ServerFileSocket::checkUserId(const qint32 id, const qint32& winId)
{
	return (id == userId_) && (winId == msgToId_);
}

void ServerFileSocket::fileTransferFinished() {
	bytesToWrite_ = 0;
	bytesWritten_ = 0;
	totalSendBytes_ = 0;
	totalRecvBytes_ = 0;
	bytesReceived_ = 0;
	fileNameSize_ = 0;
	busy_ = false;
}

void ServerFileSocket::startTransferFile(const QString& fileName) {
	if (busy_ || !tcpSocket_->isOpen()) {
		return;
	}
	fileToSend_ = new QFile((msgToId_ == -2 ? MyApp::strHeadPath_ : MyApp::strRecvPath_) + fileName);
	if (!fileToSend_->open(QFile::ReadOnly)) {
		qDebug() << "open file error!";
		return;
	}
	totalSendBytes_ = fileToSend_->size();
	QDataStream sendStream(&outBlock_, QIODevice::WriteOnly);
	sendStream.setVersion(QDataStream::Qt_5_15);
	QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/') - 1);
	sendStream << qint64(0) << qint64(0) << currentFileName;
	totalSendBytes_ += outBlock_.size();
	sendStream.device()->seek(0);
	sendStream << totalSendBytes_ << quint64(outBlock_.size() - sizeof(qint64) * 2);
	bytesToWrite_ = totalSendBytes_ - tcpSocket_->write(outBlock_);
	outBlock_.resize(0);
	busy_ = true;
	qDebug() << "Begin to send file " << fileName << userId_ << msgToId_;
}

void ServerFileSocket::initSocket()
{
}

void ServerFileSocket::sltReadyRead() {
	QDataStream in(tcpSocket_);
	in.setVersion(QDataStream::Qt_5_15);
	// 初始连接时的消息
	if (bytesReceived_ == 0 && userId_ == -1 && msgToId_ == -1 &&
		(tcpSocket_->bytesAvailable() == sizeof(quint32) * 2)) {
		in >> userId_ >> msgToId_;
		qDebug() << "File Server get Userid: " << userId_ << ", " << msgToId_;
		emit sigConnected();
		return;
	}
	// 如果接收到的数据小于等于20个字节，那么刚开始接收数据，保存头文件信息
	if (bytesReceived_ <= (sizeof(qint64) * 2)) {
		int len = sizeof(qint64) * 2;
		if (tcpSocket_->bytesAvailable() >= len && fileNameSize_ == 0) {
			in >> totalRecvBytes_ >> fileNameSize_;
			if (totalRecvBytes_ != 0) {
				bytesReceived_ += len;
			}
		}
		if (tcpSocket_->bytesAvailable() >= (qint64)fileNameSize_ &&
			fileNameSize_ != 0 && totalRecvBytes_ != 0) {
			in >> fileReadName_;
			bytesReceived_ += fileNameSize_;
			fileToRecv_->setFileName(((msgToId_ == -2) ? MyApp::strHeadPath_ : MyApp::strRecvPath_) + fileReadName_);
			if (!fileToRecv_->open(QFile::ReadOnly | QIODevice::Truncate)) {
				qDebug() << "open File error!";
				return;
			}
			qDebug() << "begin to recv file: " <<  fileReadName_;
		}
	}
	// 如果接受的数据小于总数据大小，那么写入文件
	if (bytesReceived_ < totalRecvBytes_) {
		bytesReceived_ += tcpSocket_->bytesAvailable();
		inBlock_ = tcpSocket_->readAll();
		if (fileToRecv_->isOpen()) {
			fileToRecv_->write(inBlock_);
		}
		inBlock_.resize(0);
	}
	// 接收数据完成
	if ((bytesReceived_ >= totalRecvBytes_) && (totalRecvBytes_ != 0)) {
		fileToRecv_->close();
		bytesReceived_ = 0;
		totalRecvBytes_ = 0;
		fileNameSize_ = 0;
		qDebug() << "recv file " << fileToRecv_->fileName() << " ok";
		fileTransferFinished();
	}
}

void ServerFileSocket::sltUpdateClientProcess(quint64 numBytes) {
	bytesWritten_ += numBytes;
	// 如果文件还没发送完，则继续发送
	if (bytesToWrite_ > 0) {
		outBlock_ = fileToSend_->read(qMin(loadSize_, bytesToWrite_));
		bytesToWrite_ -= outBlock_.size();
		outBlock_.resize(0);
	}
	else {
		// 没有任何数据需要发送
		if (fileToSend_->isOpen()) {
			fileToSend_->close();
		}
	}
	// 如果文件已经发送完毕
	if (bytesWritten_ >= totalSendBytes_) {
		if (fileToSend_->isOpen()) {
			fileToSend_->close();
		}
		bytesWritten_ = 0;
		bytesToWrite_ = 0;
		totalSendBytes_ = 0;
		qDebug() << fileToSend_->fileName() << "send finished";
		fileTransferFinished();
	}
}

void ServerFileSocket::sltDisplayError(QAbstractSocket::SocketError) {
	tcpSocket_->abort();
}