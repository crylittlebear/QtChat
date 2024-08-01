#include "SlideStackedWidget.h"

#include <qpainter.h>
#include <qdebug.h>

SlideStackedWidget::SlideStackedWidget(QWidget *parent)
	: QStackedWidget(parent)
{
	isAnimating_ = false;
	currentValue_ = 0;
	currIndex_ = 0;
	preIndex_ = 0;
	slideType_ = SlideOrientationType::LeftToRight;
	// 初始化动画类
	animation_ = new QPropertyAnimation(this, QByteArray());
	animation_->setDuration(100);
	animation_->setEasingCurve(QEasingCurve::Linear);
	animation_->setStartValue(0);
	animation_->setEndValue(0);
	// 设置信号槽
	connect(animation_, &QPropertyAnimation::valueChanged, this, &SlideStackedWidget::sltValueChanged);
	connect(animation_, &QPropertyAnimation::finished, this, &SlideStackedWidget::sltAnimationFinished);
}


SlideStackedWidget::~SlideStackedWidget() {

}

QString SlideStackedWidget::getClassName() {
	return staticMetaObject.className();
}

void SlideStackedWidget::start(int index) {
	if (isAnimating_) return;
	preIndex_ = currIndex_;
	currIndex_ = index;

	int offsetX = frameRect().width();
	int offsetY = frameRect().height();
	widget(currIndex_)->setGeometry(0, 0, offsetX, offsetY);

	currentWidget()->hide();
	isAnimating_ = true;
	animation_->start();
}

void SlideStackedWidget::setIndex(int previos, int current) {
	preIndex_ = previos;
	currIndex_ = current;
}

void SlideStackedWidget::setLength(int length, SlideOrientationType type) {
	slideType_ = type;
	switch (type) {
		case BottomToTop:
		case LeftToRight:
			animation_->setStartValue(-length / 2);
			animation_->setEndValue(length / 2);
			break;
		case TopToBottom:
		case RightToLeft:
			animation_->setStartValue(length / 2);
			animation_->setEndValue(-length / 2);
			break;
		default:
			break;
	}
}

void SlideStackedWidget::setDuration(int duration) {
	animation_->setDuration(duration);
}

int SlideStackedWidget::getDuration() const {
	return animation_->duration();
}

void SlideStackedWidget::paintEvent(QPaintEvent* event) {
	if (isAnimating_) {
		QPainter painter(this);
		QTransform transform;
		renderCurrentWidget(painter, transform);
		renderPreviousWidget(painter, transform);
	} else {
		QWidget::paintEvent(event);
	}
}

void SlideStackedWidget::renderPreviousWidget(QPainter& painter, QTransform transform) {
	QWidget* w = widget(preIndex_);
	QPixmap pixmap(w->size());
	pixmap.fill(Qt::transparent);
	w->setAttribute(Qt::WA_TranslucentBackground, true);
	w->render(&pixmap);
	w->setAttribute(Qt::WA_TranslucentBackground, false);
	switch (slideType_) {
		case BottomToTop:
			painter.drawPixmap(0, height() / 2, pixmap);
			break;
		case TopToBottom:
			painter.drawPixmap(0, -height() / 2, pixmap);
			break;
		case LeftToRight :
			painter.drawPixmap(width()/2, 0, pixmap);
			break;
		case RightToLeft :
			painter.drawPixmap(-width()/2, 0, pixmap);
			break;
		default:
			break;
	}
}

void SlideStackedWidget::renderCurrentWidget(QPainter& painter, QTransform transform){
    QWidget *w = widget(currIndex_);
    QPixmap pixmap( w->size() );
    pixmap.fill(Qt::transparent);
    w->setAttribute(Qt::WA_TranslucentBackground, true);
    w->render(&pixmap);
    w->setAttribute(Qt::WA_TranslucentBackground, false);
    switch(slideType_) {
		case BottomToTop :
			transform.translate(0, currentValue_);
			painter.setTransform(transform);
			painter.drawPixmap(0, -height()/2, pixmap);
			break;
		case TopToBottom :
			transform.translate(0, currentValue_);
			painter.setTransform(transform);
			painter.drawPixmap(0, height()/2, pixmap);
			break;
		case LeftToRight :
			transform.translate(currentValue_, 0);
			painter.setTransform(transform);
			painter.drawPixmap(-width()/2, 0, pixmap);
			break;
		case RightToLeft :
			transform.translate(currentValue_, 0);
			painter.setTransform(transform);
			painter.drawPixmap(width()/2, 0, pixmap);
			break;
		default: break;
    }
}

void SlideStackedWidget::sltValueChanged(const QVariant& value) {
	currentValue_ = value.toFloat();
	update();
}

void SlideStackedWidget::sltAnimationFinished() {
	currentValue_ = 0;
	isAnimating_ = false;
	QWidget* wid = widget(currIndex_);
	wid->show();
	// 将窗口置于父窗口上面
	wid->raise();
	setCurrentWidget(wid);
	update();
}





