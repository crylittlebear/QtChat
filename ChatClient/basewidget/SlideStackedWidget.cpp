#include "SlideStackedWidget.h"

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
	animation_->setDuration(300);
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

	//int offsetx = frameRect()
}

void SlideStackedWidget::setIndex(int previos, int current) {

}

void SlideStackedWidget::setLength(int length, SlideOrientationType type) {

}

void SlideStackedWidget::setDuration(int duration) {

}

int SlideStackedWidget::getDuration() const {
	return 0;
}

void SlideStackedWidget::paintEvent(QPaintEvent* event)
{
}

void SlideStackedWidget::renderPreviousWidget(QPainter& painter, QTransform transform)
{
}

void SlideStackedWidget::rendCurrentWidget(QPainter& painter, QTransform transform)
{
}

void SlideStackedWidget::sltValueChanged(const QVariant& value) {
}

void SlideStackedWidget::sltAnimationFinished()
{
}





