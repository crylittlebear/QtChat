#include "CustomWidget.h"

#include "qevent.h"
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qdebug.h>

CustomWidget::CustomWidget(QWidget *parent)
	: QWidget(parent)
{
	// 去掉QT自带的标题栏
	setWindowFlags(Qt::FramelessWindowHint);

	dragHeight_ = height() / 4;

	this->installEventFilter(this);
}

CustomWidget::~CustomWidget()
{
	
}

//void CustomWidget::mousePressEvent(QMouseEvent* event) {
//	if (event->button() == Qt::LeftButton) {
//		if (isInDragingArea(event->pos())) {
//			isDraging_ = true;
//			dragStartPoint_ = event->pos();
//		}
//	}
//	event->accept();
//}
//
//void CustomWidget::mouseMoveEvent(QMouseEvent* event) {
//	if (event->buttons() & Qt::LeftButton) {
//		if (isDraging_) {
//			move(QPoint(event->globalPos().x() - dragStartPoint_.x(),
//				event->globalPos().y() - dragStartPoint_.y()));
//		}
//	}
//	event->accept();
//}
//
//void CustomWidget::mouseReleaseEvent(QMouseEvent* event) {
//	if (event->button() == Qt::LeftButton) {
//		if (isDraging_) {
//			isDraging_ = false;
//		}
//	}
//	event->accept();
//}

bool CustomWidget::eventFilter(QObject* watched, QEvent* event) {
	if (event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if (mouseEvent->buttons() & Qt::LeftButton) {
			if (isInDragingArea(mouseEvent->pos())) {
				isDraging_ = true;
				dragStartPoint_ = mouseEvent->pos();
				return true;
			}
			if (isInScalingArea(mouseEvent->pos())) {
				isScaling_ = true;
				zoomLocation_ = geometry();
				zoomPos_ = mouseEvent->globalPos();
				return true;
			}
		}
	} else if (event->type() == QEvent::MouseMove) {
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if (mouseEvent->buttons() & Qt::LeftButton) {
			if (isDraging_) {
				move(QPoint(mouseEvent->globalPos().x() - dragStartPoint_.x(),
					mouseEvent->globalPos().y() - dragStartPoint_.y()));
			}
			if (isScaling_) {
				int dx = mouseEvent->globalPos().x() - zoomPos_.x();
				int dy = mouseEvent->globalPos().y() - zoomPos_.y();
				QRect rect = zoomLocation_;
				rect.setRight(rect.right() + dx);
				rect.setBottom(rect.bottom() + dy);
				setGeometry(rect);
				update();
				return true;
			}
		}
	} else if (event->type() == QEvent::MouseButtonRelease) {
		if (isDraging_) {
			isDraging_ = false;
			return true;
		}
		if (isScaling_) {
			isScaling_ = false;
			return true;
		}
	}
	return QObject::eventFilter(watched, event);
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
	return point.x() >= width() - 15 && point.x() <= width() && point.y() > dragHeight_
		&& point.y() <= height() - 15;
}

bool CustomWidget::isInBottomScalingArea(const QPoint& point) const {
	return point.x() >= 0 && point.x() <= width() - 15 && point.y() >= height() - 15
		&& point.y() <= height();
}

bool CustomWidget::isInBottomRightScalingArea(const QPoint& point) const {
	return point.x() > width() - 15 && point.x() <= width() && point.y() >= height() - 15
		&& point.y() <= height();
}

/*!
* =================================================================================================
*									CustomDialog
* =================================================================================================
*/
CustomDialog::CustomDialog(QWidget* parent) : QDialog(parent) {
	mousePressed_ = false;
	this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
}

CustomDialog::~CustomDialog() { }

void CustomDialog::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		mousePressed_ = true;
		mousePoint_ = event->pos();
	}
	event->accept();
}

void CustomDialog::mouseMoveEvent(QMouseEvent* event) {
	if (mousePressed_ && (event->buttons() & Qt::LeftButton)) {
		move(event->globalPos() - mousePoint_);
	}
	event->accept();
}

void CustomDialog::mouseReleaseEvent(QMouseEvent* event) {
	if (mousePressed_) {
		mousePressed_ = false;
	}
	event->accept();
}

/*!
* =================================================================================================
*									CustomBaseDialog
* =================================================================================================
*/

CustomBaseDialog::CustomBaseDialog(QWidget* parent) : CustomDialog(parent) {
	this->setObjectName("CustomBaseDialog");
	this->setMinimumSize(320, 200);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setStyleSheet("font-size: 15pt");

	// 重新构建界面
	widgetWindowTitle_ = new QWidget(this);
	widgetWindowTitle_->setFixedHeight(40);
	widgetWindowTitle_->setObjectName("widgetWinTitle");

	// window图标
	labelWinIcon_ = new QLabel(this);
	labelWinIcon_->setAlignment(Qt::AlignCenter);
	labelWinIcon_->setFixedSize(QSize(30, 30));
	labelWinIcon_->setObjectName("labelWinIcon");
	labelWinIcon_->setPixmap(QPixmap(":/resource/background/desktop.png"));
	labelWinIcon_->setScaledContents(true);

	// window标题
	labelWinTitle_ = new QLabel(this);
	labelWinTitle_->setObjectName("labelWinTitle");
	labelWinTitle_->setFixedHeight(30);
	labelWinTitle_->setText(QString::fromLocal8Bit("自定义系统提示框"));

	// 最小化和关闭按钮
	btnWinMin_ = new QPushButton(this);
	btnWinClose_ = new QPushButton(this);
	btnWinMin_->setFixedSize(30, 30);
	btnWinClose_->setFixedSize(30, 30);
	btnWinMin_->setObjectName("btnWinMin");
	btnWinClose_->setObjectName("btnWinClose");
	btnWinMin_->setIcon(QIcon(":/resource/common/ic_min.png"));
	btnWinClose_->setIcon(QIcon(":/resource/common/ic_close_white.png"));

	// 设置按钮水平布局
	QHBoxLayout* hBoxLayoutBtn = new QHBoxLayout();
	hBoxLayoutBtn->setContentsMargins(0, 0, 0, 0);
	hBoxLayoutBtn->setSpacing(2);
	hBoxLayoutBtn->addWidget(btnWinMin_);
	hBoxLayoutBtn->addWidget(btnWinClose_);

	// 设置水平布局
	QHBoxLayout* hBoxLayout = new QHBoxLayout(widgetWindowTitle_);
	hBoxLayout->setContentsMargins(0, 0, 0, 0);
	hBoxLayout->addSpacing(10);
	hBoxLayout->addWidget(labelWinIcon_, 0);
	hBoxLayout->addWidget(labelWinTitle_, 1);
	hBoxLayout->addStretch();
	hBoxLayout->addLayout(hBoxLayoutBtn);

	// 窗口体
	widgetBody_ = new QWidget(this);
	widgetBody_->setObjectName("widgetBody");
	widgetBody_->setStyleSheet("background-color: #cbc3cd");

	// 设置垂直布局
	QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
	vBoxLayout->setContentsMargins(2, 2, 2, 2);
	vBoxLayout->setSpacing(0);
	vBoxLayout->addWidget(widgetWindowTitle_);
	vBoxLayout->addWidget(widgetBody_, 1);

	// 信号和槽
	connect(btnWinMin_, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(btnWinClose_, &QPushButton::clicked, this, &QWidget::close);
}

CustomBaseDialog::~CustomBaseDialog() { }

void CustomBaseDialog::setWinIcon(QPixmap pixmap) {
	if (pixmap.isNull()) { return; }
	if (pixmap.width() > 30 || pixmap.height() > 30) {
		pixmap = pixmap.scaled(30, 30);
	}
	labelWinIcon_->setPixmap(pixmap);
}

void CustomBaseDialog::setWinTitle(const QString& text) {
	if (text.isEmpty()) { return; }
	labelWinTitle_->setText(text);
}

/*!
* =================================================================================================
*									CustomMessageBox
* =================================================================================================
*/

CustomMessageBox::CustomMessageBox(QWidget* parent) : CustomBaseDialog(parent) {
	// 提示窗口图标
	labelIcon_ = new QLabel(widgetBody_);
	labelIcon_->setFixedSize(64, 64);
	labelIcon_->setAlignment(Qt::AlignCenter);
	labelIcon_->setPixmap(QPixmap(":/resource/buttonIcon/infoDialog.png"));
	labelIcon_->setScaledContents(true);

	// 提示窗口内容
	labelMsgContent_ = new QLabel(widgetBody_);
	// 文本超出窗体长度自动换行
	labelMsgContent_->setWordWrap(true);
	labelMsgContent_->setMinimumHeight(64);
	labelMsgContent_->setObjectName("labelMsgContent");
	labelMsgContent_->setStyleSheet(QString("QLabel {font: 15pt;}"));
	labelMsgContent_->setText(QString::fromLocal8Bit("恭喜你, 中了500万!"));

	// 设置警告图标和警告消息水平布局
	QHBoxLayout* hBoxLayout = new QHBoxLayout();
	hBoxLayout->setContentsMargins(30, 2, 30, 2);
	hBoxLayout->setSpacing(10);
	hBoxLayout->addWidget(labelIcon_);
	hBoxLayout->addWidget(labelMsgContent_, 1);

	// 设置窗口按钮属性
	btnOk_ = new QPushButton(widgetBody_);
	btnOk_->setFocusPolicy(Qt::NoFocus);
	btnOk_->setFixedSize(80, 40);
	btnOk_->setText(QString::fromLocal8Bit("确定"));

	btnCancel_ = new QPushButton(widgetBody_);
	btnCancel_->setFocusPolicy(Qt::NoFocus);
	btnCancel_->setFixedSize(80, 40);
	btnCancel_->setText(QString::fromLocal8Bit("取消"));

	// 设置按钮的水平布局
	QHBoxLayout* hBoxLayoutBtn = new QHBoxLayout();
	hBoxLayoutBtn->setContentsMargins(2, 2, 2, 2);
	hBoxLayoutBtn->setSpacing(10);
	hBoxLayoutBtn->addStretch();
	hBoxLayoutBtn->addWidget(btnCancel_);
	hBoxLayoutBtn->addWidget(btnOk_);

	// 设置窗体图标及按钮的垂直布局
	QVBoxLayout* vBoxLayout = new QVBoxLayout(widgetBody_);
	vBoxLayout->setContentsMargins(6, 6, 6, 6);
	vBoxLayout->addLayout(hBoxLayout, 1);
	vBoxLayout->addLayout(hBoxLayoutBtn);

	// 信号和槽
	connect(btnOk_, &QPushButton::clicked, this, &QDialog::accept);
	connect(btnCancel_, &QPushButton::clicked, this, &QDialog::reject);

	// 倒计时定时器
	timerCount_ = 10;
	timer_ = new QTimer(this);
	connect(timer_, &QTimer::timeout, this, &CustomMessageBox::sltTimerOut);
	this->setWinTitle(QString::fromLocal8Bit("提示"));
}

CustomMessageBox::~CustomMessageBox() { }

void CustomMessageBox::showMessage(const QString& content, const quint8& msgType, const QString& title) {
	if (content.isEmpty()) return;
	if (!title.isEmpty()) this->setWinTitle(title);
	labelMsgContent_->setText(content);
	// 如果提示窗口是问题窗口, 设置取消按钮不可见
	btnCancel_->setVisible(msgType == CustomMessageBox::Question);
	if (msgType == CustomMessageBox::Information) {
		labelIcon_->setPixmap(QPixmap(":/resource/pixmap/infoDialog.png"));
		labelWinIcon_->setPixmap(QPixmap(":/resource/pixmap/infoDialog.png"));
	}
	else if (msgType == CustomMessageBox::Question) {
		labelIcon_->setPixmap(QPixmap(":/resource/pixmap/questionDialog.png"));
		labelWinIcon_->setPixmap(QPixmap(":/resource/pixmap/questionDialog.png"));
	}
	else if (msgType == CustomMessageBox::Warning) {
		labelIcon_->setPixmap(QPixmap(":/resource/pixmap/warning.png"));
		labelWinIcon_->setPixmap(QPixmap(":/resource/pixmap/warning.png"));
	}
	else if (msgType == CustomMessageBox::Error) {
		labelIcon_->setPixmap(QPixmap(":/resource/pixmap/error.png"));
		labelWinIcon_->setPixmap(QPixmap(":/resource/pixmap/error.png"));
	}
}

void CustomMessageBox::startTimer() {
	if (timer_->isActive()) timer_->stop();
	timer_->start(1000);
}

void CustomMessageBox::sltTimerOut() {
	btnOk_->setText(QString::fromLocal8Bit("确定(%1)").arg(timerCount_--));
	if (timerCount_ < 0) {
		timer_->stop();
		this->accept();
	}
}

int CustomMessageBox::information(QWidget* parent, const QString& content, const QString& title) {
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Information, title);
	return msgBox->exec();
}

int CustomMessageBox::question(QWidget* parent, const QString& content, const QString& title) {
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Question, title);
	return msgBox->exec();
}

int CustomMessageBox::warning(QWidget* parent, const QString& content, const QString& title) {
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Warning, title);
	return msgBox->exec();
}

int CustomMessageBox::error(QWidget* parent, const QString& content, const QString& title) {
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Error, title);
	return msgBox->exec();
}

/*!
* =================================================================================================
*									CustomInputDialog
* =================================================================================================
*/

CustomInputDialog::CustomInputDialog(QString title, QWidget* parent) : CustomBaseDialog(parent) {
	this->setAttribute(Qt::WA_DeleteOnClose, false);
	labelText1_ = new QLabel(widgetBody_);
	labelText1_->setText(QString::fromLocal8Bit("账号"));
	
	lineEditInput1_ = new QLineEdit(widgetBody_);
	lineEditInput1_->setEchoMode(QLineEdit::Normal);
	lineEditInput1_->setAlignment(Qt::AlignCenter);
	lineEditInput1_->setStyleSheet("background-color: #FFFFFF; border-radius: 8px; border: 3px solid #0da2af");

	QHBoxLayout* hLayout1 = new QHBoxLayout(this);
	hLayout1->addWidget(labelText1_);
	hLayout1->addWidget(lineEditInput1_);
	hLayout1->setStretch(0, 1);
	hLayout1->setStretch(1, 4);

	labelText2_ = new QLabel(widgetBody_);
	labelText2_->setText(QString::fromLocal8Bit("密码"));

	lineEditInput2_ = new QLineEdit(widgetBody_);
	lineEditInput2_->setEchoMode(QLineEdit::Normal);
	lineEditInput2_->setAlignment(Qt::AlignCenter);
	lineEditInput2_->setStyleSheet("background-color: #FFFFFF; border-radius: 8px; border: 3px solid #0da2af");

	QHBoxLayout* hLayout2 = new QHBoxLayout(this);
	hLayout2->addWidget(labelText2_);
	hLayout2->addWidget(lineEditInput2_);
	hLayout2->setStretch(0, 1);
	hLayout2->setStretch(1, 4);

	btnCancel_ = new QPushButton(widgetBody_);
	btnCancel_->setFocusPolicy(Qt::NoFocus);
	btnCancel_->setFixedSize(80, 40);
	btnCancel_->setText(QString::fromLocal8Bit("取消"));

	btnOk_ = new QPushButton(widgetBody_);
	btnOk_->setFocusPolicy(Qt::NoFocus);
	btnOk_->setFixedSize(80, 40);
	btnOk_->setText(QString::fromLocal8Bit("确定"));

	QHBoxLayout* horLayoutBtn = new QHBoxLayout();
	horLayoutBtn->setContentsMargins(0, 0, 0, 0);
	horLayoutBtn->setSpacing(10);
	horLayoutBtn->addStretch();
	horLayoutBtn->addWidget(btnCancel_);
	horLayoutBtn->addWidget(btnOk_);

	QVBoxLayout* verLayout = new QVBoxLayout();
	verLayout->setContentsMargins(40, 10, 40, 10);
	verLayout->addLayout(hLayout1);
	verLayout->addLayout(hLayout2);
	verLayout->setSpacing(12);
	//verLayout->addLayout(horLayoutBtn);
	//verLayout->setStretch(0, 2);
	//verLayout->setStretch(1, 2);
	//verLayout->setStretch(2, 1);

	QVBoxLayout* verLayout2 = new QVBoxLayout(widgetBody_);
	verLayout2->addLayout(verLayout);
	verLayout2->addLayout(horLayoutBtn);

	setTitle(title);

	// 信号和槽
	connect(btnCancel_, &QPushButton::clicked, this, &QDialog::reject);
	connect(btnOk_, &QPushButton::clicked, [this]() {
		list_ = getInputText();
		QDialog::accept();
	});
}

CustomInputDialog::~CustomInputDialog() { }

QStringList CustomInputDialog::getInputText() {
	QString name = lineEditInput1_->text();
	QString passwd = lineEditInput2_->text();
	if (!name.isEmpty() && !passwd.isEmpty()) {
		return QStringList() << name << passwd;
	}
	return QStringList();
}

QStringList CustomInputDialog::getStringList() const
{
	return list_;
}

void CustomInputDialog::setTitle(const QString& text) {
	setWinTitle(text);
	//if (text.isEmpty()) return;
	//lineEditInput_->setText(text);
	//lineEditInput_->setFocus();
	//lineEditInput_->selectAll();
}

void CustomInputDialog::setEchoMode(QLineEdit::EchoMode mode) { 
	//lineEditInput_->setEchoMode(mode); 
}

/*!
* =================================================================================================
*									CustomLabelLineEdit
* =================================================================================================
*/

LabelLineEdit::LabelLineEdit(QWidget* parent) : QLineEdit(parent) {
	label_ = new QLabel(this);
	label_->setVisible(false);
}

LabelLineEdit::~LabelLineEdit() {
}

void LabelLineEdit::setPixmap(const QString& pixmapPath) {
	label_->setPixmap(QPixmap(pixmapPath));
	label_->setScaledContents(true);
	label_->setVisible(true);
	label_->setGeometry(10, 15, this->height() - 30, this->height() - 30);
	this->setTextMargins(30, 1, 1, 1);
}

IconChangeButton::IconChangeButton(QWidget* parent) : QPushButton(parent) {
}

void IconChangeButton::setEnterLeaveIcon(const QString& enter, const QString& leave) {
	enter_ = enter;
	leave_ = leave;
}

void IconChangeButton::enterEvent(QEvent* event) {
	setIcon(QIcon(enter_));
}

void IconChangeButton::leaveEvent(QEvent* event) {
	setIcon(QIcon(leave_));
}

