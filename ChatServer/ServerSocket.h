#pragma once

#include <QObject>
#include "qtcpsocket.h"

class ServerSocket  : public QObject
{
	Q_OBJECT

public:
    /*!
    * @brief ���캯��
    */
	explicit ServerSocket(QTcpSocket* socket, QObject* parent = nullptr);

    /*!
    * @brief ��������
    */
	~ServerSocket();

    /*!
    * @brief ��ȡ��ǰ�û���id
    */
	int getUserId() const;

    /*!
    * @brief �ر��׽���
    */
	void close();

signals:
    /*!
    * @brief ���ӳɹ��ź�
    */
	void sigConnected();

    /*!
    * @brief �Ͽ������ź�
    */
	void sigDisconnected();

    /*!
    * @brief �����ļ��ź�
    */
	void sigDownloadFile(const QJsonValue& json);

    /*!
    * @brief ������Ϣ���ͻ����ź�
    */
	void sigMsgToClient(const quint8& type, const int& id, const QJsonValue& dataVal);

public slots:
    /*!
    * @brief ������Ϣ�ۺ���
    */
    void sltSendMessage(const quint8& type, const QJsonValue& json);

private:
    /*!
    * @brief �����¼�������
    */
	void parseLogin(const QJsonValue& dataVal);

    /*!
    * @brief �û�����
    */
	void parseUserOnline(const QJsonValue& dataVal);

    /*!
    * @brief ����ǳ�����
    */
    void parseLogout(const QJsonValue& dataVal);

    /*!
    * @brief ��������û�ͷ�����
    */
    void parseUpdateUserHead(const QJsonValue& dataVal);

    /*!
    * @brief �����û�ע��
    */
    void parseRegister(const QJsonValue& dataVal);

    /*!
    * @brief �����û��������
    */
    void parseAddFriend(const QJsonValue& dataVal);

    /*!
    * @brief �����û�����Ⱥ��
    */
    void parseAddGroup(const QJsonValue& dataVal);

    /*!
    * @brief �����û�����Ⱥ��
    */
    void parseCreateGroup(const QJsonValue& dataVal);

    /*!
    * @brief �����û���ȡ������Ϣ
    */
    void parseGetMyFriend(const QJsonValue& dataVal);

    /*!
    * @brief �����û���ȡȺ����Ϣ
    */
    void parseGetMyGroups(const QJsonValue& dataVal);

    /*!
    * @brief �����û�ˢ��������Ϣ
    */
    void parseRefreshFriend(const QJsonValue& dataVal);

    /*!
    * @brief �����û�ˢ��Ⱥ����Ϣ
    */
    void parseRefreshGroups(const QJsonValue& dataVal);

    /*!
    * @brief �������ѵ���Ϣ
    */
    void parseFriendMessages(const QByteArray& reply);

    /*!
    * @brief ����Ⱥ��Ϣ
    */
    void parseGroupMessages(const QByteArray& reply);

    /*!
    * @brief ���������Ϣ
    */
    void parseFaceMessages(const QByteArray& reply);

private slots:
    /*!
    * @brief ���ӳɹ��ۺ���
    */
    void sltConnected();

    /*!
    * @brief �Ͽ����Ӳۺ���
    */
    void sltDisconnected();

    /*!
    * @brief ׼����ȡ��Ϣ�ۺ���
    */
    void sltReadyRead();

private:
    QTcpSocket* tcpSocket_;
    int id_;
};
