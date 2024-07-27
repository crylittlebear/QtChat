#include "rotatestackedwidget.h"

#include "qpixmap.h"
#include "qpainter.h"

RotateStackedWidget::RotateStackedWidget(QWidget *parent)
	: QStackedWidget(parent)
{
	nextIndex_ = 0;
	isAnimating_ = false;
	rotateVal_ = 0;
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

RotateStackedWidget::~RotateStackedWidget() {}

void RotateStackedWidget::rotate(int index) {
	if (isAnimating_) { return; }
	nextIndex_ = index;
	int offsetX = frameRect().width();
	int offsetY = frameRect().height();
	this->widget(nextIndex_)->setGeometry(0, 0, offsetX, offsetY);

	QPropertyAnimation* anim = new QPropertyAnimation(this, "rotateVal");
	anim->setDuration(350);
	anim->setEasingCurve(QEasingCurve::Linear);
	anim->setStartValue(0);
	anim->setEndValue(180);
	connect(anim, &QPropertyAnimation::valueChanged, this, &RotateStackedWidget::valChanged);
	connect(anim, &QPropertyAnimation::finished, this, &RotateStackedWidget::animationDone);
	currentWidget()->hide();
	isAnimating_ = true;
	anim->start();
}

void RotateStackedWidget::paintEvent(QPaintEvent* event) {
	if (isAnimating_) {
		if (rotateVal_ > 90) {
			QPixmap pixmap(this->widget(nextIndex_)->size());
			this->widget(nextIndex_)->render(&pixmap);
			QPainter painter(this);

			QTransform transform;
			/*!
			* translate的作用是平移坐标原点的位置，如果不进行平移
			* 窗口会以左上角为坐标原点
			*/
			transform.translate(width() / 2, 0);
			transform.rotate(rotateVal_ + 180, Qt::YAxis);
			painter.setTransform(transform);
			painter.drawPixmap(-1 * width() / 2, 0, pixmap);
		}
		else {
			QPixmap pixmap(currentWidget()->size());
			currentWidget()->render(&pixmap);
			QPainter painter(this);

			QTransform transform;
			transform.translate(width() / 2, 0);
			transform.rotate(rotateVal_, Qt::YAxis);
			painter.setTransform(transform);
			painter.drawPixmap(-1 * width() / 2, 0, pixmap);
		}
	}
	else {
		QWidget::paintEvent(event);
	}
}

void RotateStackedWidget::nextPage() {
	rotate((currentIndex() + 1) >= count() ? 0 : (currentIndex() + 1));
}

void RotateStackedWidget::valChanged(const QVariant& value) {
	repaint();
}

void RotateStackedWidget::animationDone() {
	rotateVal_ = 0;
	isAnimating_ = false;
	this->widget(nextIndex_)->show();
	this->widget(nextIndex_)->raise();
	setCurrentWidget(this->widget(nextIndex_));
	repaint();
}