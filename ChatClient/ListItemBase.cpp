#include "ListItemBase.h"

#include <qstyleoption.h>
#include <qpainter.h>

ListItemBase::ListItemBase(QWidget *parent)
	: QWidget(parent)
	, itemType_(ChatUserItem)
{

}

ListItemBase::~ListItemBase()
{

}

void ListItemBase::setItemType(ListItemType type){
	itemType_ = type;
}

ListItemType ListItemBase::getItemType() const{
	return itemType_;
}

void ListItemBase::paintEvent(QEvent* event) {
	QStyleOption option;
	option.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
