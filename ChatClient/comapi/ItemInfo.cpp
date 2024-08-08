#include "ItemInfo.h"

#include "comapi/unit.h"

ItemInfo::ItemInfo()
	: strMsgName_("")
	, strMsgDate_("")
	, strHeadPic_("")
	, strMsgContent_("")
	, strSize_("")
	, orientation_(Orientation::Left)
	, msgType_(MessageType::Text)
	, msgHeight_(40)
{

}

ItemInfo::ItemInfo(const QString& name, 
				   const QString& date, 
				   const QString& headPic, 
	               const QString& content, 
				   const QString& size, 
				   const quint8& orientation, 
				   const quint8& type) 
	: strMsgName_(name)
	, strMsgDate_(date)
	, strHeadPic_(headPic)
	, strMsgContent_(content)
	, strSize_(size)
	, orientation_(orientation)
	, msgType_(type)
	, msgHeight_(40)
{

}

ItemInfo::~ItemInfo()
{

}

void ItemInfo::setName(const QString & text){
	strMsgName_ = text;
}

QString ItemInfo::getName() const {
	return strMsgName_;
}

void ItemInfo::setDatetime(const QString& text) {
	strMsgDate_ = text;
}

QString ItemInfo::getDatetime() const {
	return strMsgDate_;
}

void ItemInfo::setHeadPic(const QString& pixmap) {
	strHeadPic_ = pixmap;
}

QString ItemInfo::getStrPic() const {
	return strHeadPic_;
}

void ItemInfo::setMsgContent(const QString& text) {
	strMsgContent_ = text;
}

QString ItemInfo::getMsgContent() const {
	return strMsgContent_;
}

void ItemInfo::setFileSizeString(const QString& strSize) {
	strSize_ = strSize;
}

QString ItemInfo::getFileSizeString() const {
	return strSize_;
}

void ItemInfo::setOrientation(quint8 orientation) {
	orientation_ = orientation;
}

quint8 ItemInfo::getOrientation() const {
	return orientation_;
}

void ItemInfo::setMsgType(const quint8& msgType) {
	msgType_ = msgType;
}

quint8 ItemInfo::getMsgType() const {
	return msgType_;
}

void ItemInfo::setItemHeight(qreal itemHeight) {
	msgHeight_ = itemHeight;
}

qreal ItemInfo::getItemHeight() const {
	return msgHeight_;
}

void ItemInfo::setBubbleRect(const QRectF& bubbleRect) {
	bubbleRect_ = bubbleRect;
}

QRectF ItemInfo::getBubbleRect() const {
	return bubbleRect_;
}
