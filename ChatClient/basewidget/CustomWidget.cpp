#include "CustomWidget.h"

#include "qevent.h"

CustomWidget::CustomWidget(QWidget *parent)
	: QWidget(parent)
{
	dragHeight_ = height() / 4;
}

CustomWidget::~CustomWidget()
{
	
}

void CustomWidget::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		if (isInDragingArea(event->pos())) {
			isDraging_ = true;
			dragStartPoint_ = event->pos();
		}
		else if (isInScalingArea(event->pos())) {
			isScaling_ = true;
		}
	}
	event->accept();
}

void CustomWidget::mouseMoveEvent(QMouseEvent* event) {
	if (event->buttons() & Qt::LeftButton) {
		if (isDraging_) {
			move(QPoint(event->globalPos().x() - dragStartPoint_.x(),
				event->globalPos().y() - dragStartPoint_.y()));
		}
		if (isScaling_) {
			if (isInRightScalingArea(event->pos())) {
				this->resize(event->globalPos().x() - frameGeometry().x(), height());
			}
			else if (isInBottomScalingArea(event->pos())) {
				this->resize(width(), event->globalPos().y() - geometry().y());
			}
			else {
				this->resize(event->globalPos().x() - geometry().x(),
					event->globalPos().y() - geometry().y());
			}
		}
	}
	event->accept();
}

void CustomWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		if (isDraging_) {
			isDraging_ = false;
		}
		if (isScaling_) {
			isScaling_ = false;
		}
	}
	event->accept();
}

bool CustomWidget::isInDragingArea(const QPoint& point) const {
	int dragWidth = width();
	return point.x() >= 0 && point.x() <= dragWidth
		&& point.y() >= 0 && point.y() <= dragHeight_;
}

bool CustomWidget::isInScalingArea(const QPoint& point) const {
	return isInRightScalingArea(point) || isInBottomScalingArea(point) ||
		isInBottomRightScalingArea(point);
}

bool CustomWidget::isInRightScalingArea(const QPoint& point) const {
	return point.x() >= width() - 10 && point.x() <= width() && point.y() > dragHeight_
		&& point.y() <= height() - 10;
}

bool CustomWidget::isInBottomScalingArea(const QPoint& point) const {
	return point.x() >= 0 && point.x() <= width() - 30 && point.y() >= height() - 30
		&& point.y() <= height();
}

bool CustomWidget::isInBottomRightScalingArea(const QPoint& point) const {
	return point.x() > width() - 30 && point.x() <= width() && point.y() >= height() - 30
		&& point.y() <= height();
}