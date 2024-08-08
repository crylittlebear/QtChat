#pragma once

#include <QWidget>

class ItemInfo
{
public:
	ItemInfo();
	ItemInfo(const QString &name, 
			 const QString& date,
			 const QString &headPic, 
			 const QString& content,
			 const QString & size, 
			 const quint8& orientation,
			 const quint8& type);
	~ItemInfo();

    void setName(const QString &text);
    QString getName() const;

    void setDatetime(const QString &text);
    QString getDatetime() const;

    void setHeadPic(const QString &pixmap);
    QString getStrPic() const;

    void setMsgContent(const QString& text);
    QString getMsgContent() const;

    void setFileSizeString(const QString& strSize);
    QString getFileSizeString() const;

    void setOrientation(quint8 orientation);
    quint8 getOrientation() const;

    void setMsgType(const quint8 &msgType);
    quint8 getMsgType() const;

    void setItemHeight(qreal itemHeight);
    qreal getItemHeight() const;

    void setBubbleRect(const QRectF &bobbleRect);
    QRectF getBubbleRect() const;

private:
	// 消息标题
	QString strMsgName_;
	// 消息日期
	QString strMsgDate_;
	// 头像图片
	QString strHeadPic_;
	// 消息内容
	QString strMsgContent_;
	// 如果是文件会显示文件大小
	QString strSize_;
	// 气泡方向
	quint8 orientation_;
	// 消息类型
	quint8 msgType_;
	// 消息气泡高度
	qreal msgHeight_;
	// 窗口坐标
	QRectF bubbleRect_;
	//// 消息状态
	//quint8 msgStatus_;
};
