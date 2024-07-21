/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#include "DatabaseManager.h"
#include "unit.h"

#include "qdatetime.h"
#include "qjsonarray.h"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "qdebug.h"
#include "qsqlquery.h"

#define DATA_TIME_FORMAT QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")

DatabaseManager::DatabaseManager(QObject *parent)
	: QObject(parent)
{

}

DatabaseManager::~DatabaseManager()
{
	if (database_.isOpen()) {
		database_.close();
	}
}

bool DatabaseManager::openDb(const QString& databaseName)
{
	database_ = QSqlDatabase::addDatabase("QSQLITE");
	database_.setDatabaseName(databaseName);
	if (!database_.open()) {
		qDebug() << "open database failed";
		return false;
	}
	// 添加数据库表
	QSqlQuery query;
	query.exec("create table userinfo("
		"id primary key,"
		"name varchar(20),"
		"passwd varchar(20),"
		"head varchar(20),"
		"status int,"
		"groupId int,"
		"lasttime datetime);");
	query.exec("create table groupinfo("
		"id int primary key," 
		"groupId int," 
		"name varchar(20)," 
		"head varchar(20), "
		"userId int,"
		"identity int);");
	query.exec("create table userhead("
		"id primary key,"
		"name varchar(20),"
		"data varchar(20);");
	query.exec("insert into userinfo values("
		"1, 'admin', '123456', '2.bmp', 0, 1, '');");

	changeAllUserStatus();
	queryAll();
	return true;
}

void DatabaseManager::closeDb()
{
	database_.close();
}

void DatabaseManager::updateUserStatus(const int& id, const quint8& status)
{
	QString sql = "update userinfo set status=";
	sql += QString::number(status);
	sql += QString(", lasttime='");
	sql += DATA_TIME_FORMAT;
	sql += QString("' where id=");
	sql += QString::number(id);

	QSqlQuery query(sql);
	query.exec();
}

void DatabaseManager::updateUserHead(const int& id, const QString& strHead)
{
	QString sql = "update userhead set head='";
	sql += strHead;
	sql += "' where id=";
	sql += QString::number(id);
	QSqlQuery query(sql);
	bool res = query.exec();
	qDebug() << "update head: " << res << "," << id;
}

void DatabaseManager::testHeadPic(const int& id, const QString& name, const QString& strHead)
{
	QSqlQuery query;
	query.prepare("insert into userhead values(?, ?, ?);");
	query.bindValue(0, id);
	query.bindValue(1, name);
	query.bindValue(2, strHead);

	bool res = query.exec();
	qDebug() << "testHeadPic() ok";
	queryAll();
}

QJsonArray DatabaseManager::getAllUsers()
{
	QSqlQuery query("select * from userinfo order by id");
	QJsonArray jsonarr;
	while (query.next()) {
		QJsonObject jsonObj;
		jsonObj.insert("id", query.value("id").toInt());
		jsonObj.insert("name", query.value("name").toString());
		jsonObj.insert("passwd", query.value("passwd").toString());
		jsonObj.insert("head", query.value("head").toString());
		jsonObj.insert("status", query.value("status").toInt());
		jsonObj.insert("groupId", query.value("groupId").toInt());
		jsonObj.insert("lasttime", query.value("lasttime").toString());
		jsonarr.append(jsonObj);
	}
	return jsonarr;
}

QJsonObject DatabaseManager::getUserStatus(const int& id) const
{
	QJsonObject jsonObj;
	QString sql = QString("select [name], [status], [head] from userinfo"
		" where id = %1").arg(id);
	QSqlQuery query(sql);

	int status = 0;
	QString name = "";
	QString head = "0.bmp";
	if (query.next()) {
		name = query.value(0).toString();
		status = query.value(1).toInt();
		head = query.value(2).toString();
	}
	jsonObj.insert("id", id);
	jsonObj.insert("name", name);
	jsonObj.insert("status", status);
	jsonObj.insert("haed", head);
	return jsonObj;
}

int DatabaseManager::getUserLineStatus(const int& id) const
{
	QString sql = QString("select [status] from userinfo where id = %1;").arg(id);
	QSqlQuery query(sql);
	if (query.next()) {
		return query.value(0).toInt();
	}
	return -1;
}

QJsonObject DatabaseManager::checkUserLogin(const QString& name, const QString& passwd)
{
#if 1
	/*!
	* 这里还是有一些逻辑上的漏洞需要完善，比如账号不存在和密码错误，这里在执行select的
	* 时候是账号和密码一起搜索的，严谨一点应该先查找账号是否存在
	*/
	QString sql = QString("select [id], [passwd], [head], [status] from userinfo where name = '%1';").arg(name);
	QSqlQuery query(sql);
	QJsonObject responseObj;
	int id = -1;
	// 对应账号未找到
	int code = -1;
	QString head = "0.bmp";
	// 如果数据库中存在该账号相关的信息
	if (query.next()) {
		id = query.value("id").toInt();
		head = query.value("head").toString();
		// 如果找到该用户但是密码匹配失败
		if (query.value("passwd").toString() != passwd) {
			code = -2;
		}
		else if (query.value("status").toInt() == Online) {
			// 找到该用户且密码匹配成功
			code = -3;
		}
		else {
			code = 0;
			updateUserStatus(id, Online);
		}
	}
	responseObj.insert("id", id);
	responseObj.insert("msg", code < 0 ? "error" : "ok");
	responseObj.insert("head", head);
	responseObj.insert("code", code);
	return responseObj;
#else 
	QString sql = QString("select [id], [head], [status] from userinfo where "
		"name = '%1' and passwd = '%2';").arg(name).arg(passwd);
	QJsonObject jsonObj;
	int id = -1;
	int code = -1;
	QString strHead = "0.bmp";
	QSqlQuery query(sql);
	if (query.next()) {
		id = query.value(0).toInt();
		int status = query.value("status").toInt();
		if (status == Online) {
			id = -2;
			code = -2;
		}
		else {
			updateUserStatus(id, Online);
			code = 0;
		}
		strHead = query.value("head").toString();
	}
	jsonObj.insert("id", id);
	jsonObj.insert("msg", id < 0 ? "error" : "ok");
	jsonObj.insert("head", strHead);
	jsonObj.insert("code", code);
	return jsonObj;
#endif
}

int DatabaseManager::registerUser(const QString& name, const QString& passwd)
{
	QString sql = QString("select [id] from userinfo where name = '%1';").arg(name);
	QSqlQuery query(sql);
	if (query.next()) {
		return -1;
	}
	query = QSqlQuery("select [id] from userinfo order by id;");
	int id = 1;
	if (query.next()) {
		id = query.value(0).toInt();
	}
	// 创建新用户
	query.prepare("insert into userinfo (id, name, passwd, head, status, groupId, lasttime)"
		" values(?, ?, ?, ?, ?, ?, ?);");
	query.bindValue(0, id + 1);
	query.bindValue(1, name);
	query.bindValue(2, passwd);
	query.bindValue(3, "0.bmp");
	query.bindValue(4, 0);
	query.bindValue(5, 0);
	query.bindValue(6, DATA_TIME_FORMAT);
	// 执行sql语句
	query.exec();
	return id + 1;
}

QJsonObject DatabaseManager::addFriend(const QString& name)
{
	QJsonObject jsonObj;
	QString sql = QString("select [id], [status], [head] from userinfo where name = %1;").arg(name);
	
	int id = -1;
	int status = -1;
	QString head = "0.bmp";
	QSqlQuery query(sql);
	if (query.next()) {
		id = query.value(0).toInt();
		status = query.value(1).toInt();
		head = query.value(2).toString();
	}
	jsonObj.insert("id", id);
	jsonObj.insert("name", name);
	jsonObj.insert("status", status);
	jsonObj.insert("head", head);
	return jsonObj;
}

QJsonObject DatabaseManager::addGroup(const int& userId, const QString& name)
{
	// 先查询是否有该群组
	QString strQuery = "SELECT [groupId] FROM GROUPINFO ";
	strQuery.append("WHERE name='");
	strQuery.append(name);
	strQuery.append("';");

	int nGroupId = -1;
	int nCode = -1;
	QString strHead = "5.bmp";

	QSqlQuery query(strQuery);

	// 查询到有该用户组
	if (query.next()) {
		// 查询到有该群组，再判断该用户是否已经加入该群组
		nGroupId = query.value(0).toInt();
		strQuery = QString("SELECT [userId] FROM GROUPINFO WHERE groupId=");
		strQuery.append(QString::number(nGroupId));
		strQuery.append("");

		query = QSqlQuery(strQuery);
		// 查询到已经添加到该群组
		if (query.next()) {
			nCode = -2;
		}
		else
		{
			// 查询数据库
			query = QSqlQuery("SELECT [id] FROM GROUPINFO ORDER BY id DESC;");
			int nIndex = 0;
			// 查询最高ID
			if (query.next()) {
				nIndex = query.value(0).toInt();
			}

			nIndex += 1;

			// 根据新ID重新创建用户
			query.prepare("INSERT INTO GROUPINFO (id, groupId, name, userId, identity) "
				"VALUES (?, ?, ?, ?, ?);");
			query.bindValue(0, nIndex);
			query.bindValue(1, nGroupId);
			query.bindValue(2, name);
			query.bindValue(3, userId);
			query.bindValue(4, 3);
			// 执行插入
			query.exec();
		}
	}

	// 构建 Json 对象
	QJsonObject json;
	json.insert("id", nGroupId);
	json.insert("name", name);
	json.insert("head", strHead);
	json.insert("code", nCode);

	queryAll();

	return json;
}

QJsonObject DatabaseManager::createGroup(const int& userId, const QString& name)
{
	//查询群组是否存在
	QString strQuery = "SELECT [id],[groupId],[head] FROM GROUPINFO ";
	strQuery.append("WHERE name='");
	strQuery.append(name);
	strQuery.append("' AND userId=");
	strQuery.append(QString::number(userId));

	int nIndex = -1;
	int nGroupId = -1;
	QString strHead = "1.bmp";

	QSqlQuery query(strQuery);
	// 查询用户是否在群组中
	if (query.next()) {
		nIndex = query.value("id").toInt();
		nGroupId = query.value("groupId").toInt();
		strHead = query.value("head").toString();
	}
	else {
		//GROUPINFO
		//1.
		//2.
		// 查询数据库
		query = QSqlQuery("SELECT [id] FROM GROUPINFO ORDER BY id DESC;");
		nIndex = 0;
		// 查询最高ID
		if (query.next()) {
			nIndex = query.value("id").toInt();
		}

		// 再查询该ID下面的群组
		nGroupId = 0;
		strQuery = QString("SELECT [groupId] FROM GROUPINFO WHERE userId=");
		strQuery.append(QString::number(userId));
		strQuery.append(" ORDER BY groupId DESC");
		query = QSqlQuery(strQuery);
		// 查询最高ID
		if (query.next()) {
			nGroupId = query.value("groupId").toInt();
		}

		nIndex += 1;
		nGroupId += 1;

		// 根据新ID重新创建用户
		query.prepare("INSERT INTO GROUPINFO (id, groupId, name, head, userId, identity) "
			"VALUES (?, ?, ?, ?, ?, ?);");
		query.bindValue(0, nIndex);
		query.bindValue(1, nGroupId);
		query.bindValue(2, name);
		query.bindValue(3, "1.bmp");
		query.bindValue(4, userId);
		query.bindValue(5, 1);

		query.exec();
	}

	// 构建 Json 对象
	QJsonObject json;
	json.insert("id", nGroupId);
	json.insert("name", name);
	json.insert("head", strHead);

	return json;
}

QJsonArray DatabaseManager::getGroupUsers(const int& groupId)
{
	QString strQuery = ("SELECT [id],[name] FROM USERINFO WHERE ");
	strQuery.append(" id=(SELECT [id] FROM GROUPINFO WHERE groupId=");
	strQuery.append(QString::number(groupId));
	strQuery.append(") AND status=");
	strQuery.append(QString::number(Offline));

	strQuery = "SELECT [userId] FROM GROUPINFO WHERE groupId=";
	strQuery.append(QString::number(groupId));

	QJsonArray jsonArr;
	QSqlQuery query(strQuery);
	jsonArr.append(groupId);
	// 查询
	while (query.next()) {
		int nId = query.value(0).toInt();
		strQuery = "SELECT [name],[head],[status] FROM USERINFO WHERE id=";
		strQuery.append(QString::number(nId));

		QSqlQuery queryUser(strQuery);
		if (queryUser.next()) {
			QJsonObject jsonObj;
			jsonObj.insert("id", nId);
			jsonObj.insert("name", queryUser.value(0).toString());
			jsonObj.insert("head", queryUser.value(1).toString());
			jsonObj.insert("status", queryUser.value(2).toInt());
			jsonArr.append(jsonObj);
		}
	}

	return jsonArr;
}

void DatabaseManager::changeAllUserStatus()
{
	QSqlQuery query("SELECT * FROM USERINFO ORDER BY id;");
	while (query.next()) {
		// 更新为下线状态
		updateUserStatus(query.value(0).toInt(), Offline);
	}
}

QString DatabaseManager::getUserName(const int& id) const
{
	QString strQuery = "SELECT [name] FROM USERINFO ";
	strQuery.append("WHERE id=");
	strQuery.append(QString::number(id));

	QSqlQuery query(strQuery);
	if (query.next()) {
		return query.value(0).toString();
	}

	return "";
}

QString DatabaseManager::getUserHead(const int& id) const
{
	QString strQuery = "SELECT [head] FROM USERINFO ";
	strQuery.append("WHERE id=");
	strQuery.append(QString::number(id));

	QSqlQuery query(strQuery);
	if (query.next()) {
		return query.value(0).toString();
	}

	return "1.bmp";
}

QJsonObject DatabaseManager::getUserInfo(const int& id) const
{
	QString strQuery = "SELECT * FROM USERINFO ";
	strQuery.append("WHERE id=");
	strQuery.append(QString::number(id));

	QJsonObject jsonObj;
	int nCode = -1;
	// 查询数据库
	QSqlQuery query(strQuery);
	// 构建用户的所有信息,不包括密码
	if (query.next()) {
		jsonObj.insert("id", query.value("id").toInt());
		jsonObj.insert("name", query.value("name").toString());
		jsonObj.insert("head", query.value("head").toString());
		jsonObj.insert("status", query.value("status").toInt());
		jsonObj.insert("groupId", query.value("status").toInt());
		jsonObj.insert("lasttime", query.value("lasttime").toString());
		// 结果代码
		nCode = 0;
	}
	jsonObj.insert("code", nCode);
	return jsonObj;
}

void DatabaseManager::queryAll()
{
	QSqlQuery query("select * from userinfo order by id");
	qDebug() << "query users";
	while (query.next()) {
		qDebug() << query.value(0).toInt() << query.value(1).toString()
			<< query.value(2).toString() << query.value(3).toString()
			<< query.value(4).toString() << query.value(5).toString();
 	}
	qDebug() << "query group";
	query = QSqlQuery("SELECT * FROM groupinfo ORDER BY id;");
	while (query.next()) {
		qDebug() << query.value(0).toInt() << query.value(1).toInt()
			<< query.value(2).toString() << query.value(3).toString()
			<< query.value(4).toInt() << query.value(5).toInt();
	}
	qDebug() << "query head";
	query = QSqlQuery("SELECT * FROM USERHEAD ORDER BY id;");
	while (query.next()) {
		qDebug() << query.value(0).toInt()
			<< query.value(1).toString() << query.value(2).toString().length()
			<< query.value(2).toString().length();
	}
}