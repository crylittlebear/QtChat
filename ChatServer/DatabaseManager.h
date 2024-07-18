#pragma once

#include <QObject>
#include "qsqldatabase.h"

class DatabaseManager  : public QObject
{
	Q_OBJECT

public:
    /*!
    * @brief ���캯��
    */
    explicit DatabaseManager(QObject* parent = nullptr);

    /*!
    * @brief ��������
    */
	~DatabaseManager();

    /*!
    * @brief ����������ΪdatabaseName�����ݿ�
    */
	bool openDb(const QString& databaseName);

    /*!
    * @brief �ر����ݿ�
    */
	void closeDb();

    /*!
    * @brief �����û�״̬
    */
    void updateUserStatus(const int& id, const quint8& status);

    /*!
    * @brief �����û�ͷ��
    */
    void updateUserHead(const int& id, const QString& strHead);

    /*!
    * @brief 
    */
    void testHeadPic(const int& id, const QString& name, const QString& strHead);

    /*!
    * @brief ��ȡ���е��û�
    */
    QJsonArray getAllUsers();

    /*!
    * @brief ��ȡ�û���״̬
    */
    QJsonObject getUserStatus(const int& id) const;

    /*!
    * @brief  ��ȡ�û�������״̬
    */
    int getUserLineStatus(const int& id) const;

    /*!
    * @brief ���û������ĵ�¼�������У��
    */
    QJsonObject checkUserLogin(const QString& name, const QString& passwd);

    /*!
    * @brief �û�ע��
    */
    int registerUser(const QString& name, const QString& passwd);

    /*!
    * @brief ��Ӻ���
    */
    QJsonObject addFriend(const QString& name);

    /*!
    * @brief ���Ⱥ��
    */
    QJsonObject addGroup(const int& userId, const QString& name);

    /*!
    * @brief ����Ⱥ��
    */
    QJsonObject createGroup(const int& userId, const QString& name);

    /*!
    * @brief ��ѯ��ǰȺ���µĺ���
    */
    QJsonArray  getGroupUsers(const int& groupId);

    /*!
    * @brief ���������û���״̬
    */
    void changeAllUserStatus();

    /*!
    * @brief ͨ��id��ȡ�û�������
    */
    QString getUserName(const int& id) const;

    /*!
    * @brief ͨ��id��ȡ�û���ͷ��
    */
    QString getUserHead(const int& id) const;

    /*!
    * @brief ��ȡ�û�����Ϣ
    */
    QJsonObject getUserInfo(const int& id) const;

    /*!
    * @brief ����ģʽ����ȡΨһ�Ķ���ʵ��
    */
    static DatabaseManager* instance() {
        static DatabaseManager manager;
        return &manager;
    }

    /*!
    * @brief ִ��select * ��䣬��ӡÿ�������Ϣ
    */
    void queryAll();

private:
    static DatabaseManager* self_;
    QSqlDatabase database_;
};



