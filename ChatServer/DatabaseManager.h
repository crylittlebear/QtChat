/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#pragma once

#include <QObject>
#include "qsqldatabase.h"

class DatabaseManager  : public QObject
{
	Q_OBJECT

public:
    /*!
    * @brief 构造函数
    */
    explicit DatabaseManager(QObject* parent = nullptr);

    /*!
    * @brief 析构函数
    */
	~DatabaseManager();

    /*!
    * @brief 创建并打开名为databaseName的数据库
    */
	bool openDb(const QString& databaseName);

    /*!
    * @brief 关闭数据库
    */
	void closeDb();

    /*!
    * @brief 更新用户状态
    */
    void updateUserStatus(const int& id, const quint8& status);

    /*!
    * @brief 更新用户状态
    */
    void updateUserStatus(const QString& name, const quint8& status);

    /*!
    * @brief 更新用户密码
    */
    bool updateUserPasswd(const QString& name, const QString& passwd);

    /*!
    * @brief 更新用户头像
    */
    void updateUserHead(const int& id, const QString& strHead);

    /*!
    * @brief 
    */
    void testHeadPic(const int& id, const QString& name, const QString& strHead);

    /*!
    * @brief 获取所有的用户
    */
    QJsonArray getAllUsers();

    /*!
    * @brief 获取用户的状态
    */
    QJsonObject getUserStatus(const int& id) const;

    /*!
    * @brief  获取用户的在线状态
    */
    int getUserLineStatus(const int& id) const;

    /*!
    * @brief 对用户发来的登录请求进行校验
    */
    QJsonObject checkUserLogin(const QString& name, const QString& passwd);

    /*!
    * @brief 用户注册
    */
    int registerUser(const QString& name, const QString& passwd);

    /*!
    * @brief 删除用户
    */
    int removeUser(const QString& name);

    /*!
    * @brief 添加好友
    */
    QJsonObject addFriend(const QString& name);

    /*!
    * @brief 添加群组
    */
    QJsonObject addGroup(const int& userId, const QString& name);

    /*!
    * @brief 创建群组
    */
    QJsonObject createGroup(const int& userId, const QString& name);

    /*!
    * @brief 查询当前群组下的好友
    */
    QJsonArray  getGroupUsers(const int& groupId);

    /*!
    * @brief 更新所有用户的状态
    */
    void changeAllUserStatus();

    /*!
    * @brief 通过id获取用户的名字
    */
    QString getUserName(const int& id) const;

    /*!
    * @brief 通过id获取用户的头像
    */
    QString getUserHead(const int& id) const;

    /*!
    * @brief 获取用户的信息
    */
    QJsonObject getUserInfo(const int& id) const;

    /*!
    * @brief 单例模式，获取唯一的对象实例
    */
    static DatabaseManager* instance() {
        static DatabaseManager manager;
        return &manager;
    }

    /*!
    * @brief 执行select * 语句，打印每个表的信息
    */
    void queryAll();

private:
    QSqlDatabase database_;
};



