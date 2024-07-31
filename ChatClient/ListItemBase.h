#pragma once

#include <QWidget>

#include "comapi/unit.h"

class ListItemBase  : public QWidget
{
	Q_OBJECT

public:
	ListItemBase(QWidget *parent);
	~ListItemBase();

	void setItemType(ListItemType type);

	ListItemType getItemType() const;

protected:
	virtual void paintEvent(QEvent* event);

private:
	ListItemType itemType_;
};
