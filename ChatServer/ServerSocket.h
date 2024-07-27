/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#pragma once

#include <QObject>
#include "qtcpsocket.h"
#include "qfile.h"

class ServerSocket  : public QObject
{
	Q_OBJECT

public:
    /*!
    * @brief 构造函数
    */
	explicit ServerSocket(QTcpSocket* socket, QObject* parent = nullptr);

    /*!
    * @brief 析构函数
    */
	~ServerSocket();

    /*!
    * @brief 获取当前用户的id
    */
	int getUserId() const;

    /*!
    * @brief 关闭套接字
    */
	void close();

signals:
    /*!
    * @brief 连接成功信号
    */
	void sigConnected();

    /*!
    * @brief 断开连接信号
    */
	void sigDisconnected();

    /*!
    * @brief 下载文件信号
    */
	void sigDownloadFile(const QJsonValue& json);

    /*!
    * @brief 发送消息到客户端信号
    */
	void sigMsgToClient(const quint8& type, const int& id, const QJsonValue& dataVal);

public slots:
    /*!
    * @brief 发送消息槽函数
    */
    void sltSendMessage(const quint8& type, const QJsonValue& json);

private:
    /*!
    * @brief 处理登录请求操作
    */
	void parseLogin(const QJsonValue& dataVal);

    /*!
    * @brief 用户上线
    */
	void parseUserOnline(const QJsonValue& dataVal);

    /*!
    * @brief 处理登出操作
    */
    void parseLogout(const QJsonValue& dataVal);

    /*!
    * @brief 处理更新用户头像操作
    */
    void parseUpdateUserHead(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户注册
    */
    void parseRegister(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户添加朋友
    */
    void parseAddFriend(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户加入群组
    */
    void parseAddGroup(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户创建群组
    */
    void parseCreateGroup(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户获取朋友信息
    */
    void parseGetMyFriend(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户获取群组信息
    */
    void parseGetMyGroups(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户刷新朋友信息
    */
    void parseRefreshFriend(const QJsonValue& dataVal);

    /*!
    * @brief 处理用户刷新群组信息
    */
    void parseRefreshGroups(const QJsonValue& dataVal);

    /*!
    * @brief 处理朋友的消息
    */
    void parseFriendMessages(const QByteArray& reply);

    /*!
    * @brief 处理群消息
    */
    void parseGroupMessages(const QByteArray& reply);

    /*!
    * @brief 处理表情消息
    */
    void parseFaceMessages(const QByteArray& reply);

private slots:
    /*!
    * @brief 连接成功槽函数
    */
    void sltConnected();

    /*!
    * @brief 断开连接槽函数
    */
    void sltDisconnected();

    /*!
    * @brief 准备读取消息槽函数
    */
    void sltReadyRead();

private:
    QTcpSocket* tcpSocket_;
    int id_;
};

/*!
* =======================================================================
*                           文件套接字
* =======================================================================
*/

class ServerFileSocket : public QObject {
    Q_OBJECT
public:
    explicit ServerFileSocket(QObject* parent = nullptr, QTcpSocket* socket = nullptr);
    ~ServerFileSocket();

    void close();
    bool checkUserId(const qint32 id, const qint32& winId);

    void fileTransferFinished();
    void startTransferFile(const QString& fileName);

signals:
    void sigConnected();
    void sigDisconnected();
    void sigRecvFinished(int id, const QJsonValue& json);

private:
    void initSocket();

private slots:
    void sltDisplayError(QAbstractSocket::SocketError);
    void sltReadyRead();
    void sltUpdateClientProcess(quint64 numBytes);

private:
    // 文件接收
    quint64 loadSize_;
    quint64 bytesReceived_;
    quint64 fileNameSize_;
    QString fileReadName_;
    QByteArray inBlock_;
    quint64 totalRecvBytes_;
    QFile* fileToRecv_;

    QTcpSocket* tcpSocket_;

    // 文件发送
    //quint16 blockSize_;
    QFile* fileToSend_;
    quint64 totalSendBytes_;
    quint64 bytesWritten_;
    quint64 bytesToWrite_;
    QByteArray outBlock_;

    bool busy_;
    quint32 userId_;
    quint32 msgToId_;
};