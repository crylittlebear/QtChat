/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#include "CustomWidget.h"

#include "qevent.h"
#include "qpushbutton.h"
#include "qlayout.h"
#include "qtimer.h"

CustomWidget::CustomWidget(QWidget* parent)
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
		//move(event->globalX() - mousePoint_.x(), event->globalY() - mousePoint_.y());
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
	this->setMinimumSize(360, 240);
	this->setAttribute(Qt::WA_DeleteOnClose);

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
	labelWinTitle_->setText(tr("自定义系统提示框"));

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
		//labelWinIcon_->setScaledContents(true);
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
	labelMsgContent_->setText(tr("恭喜你, 中了500万!"));

	// 设置水平布局
	QHBoxLayout* hBoxLayout = new QHBoxLayout();
	hBoxLayout->setContentsMargins(60, 2, 60, 2);
	hBoxLayout->setSpacing(10);
	hBoxLayout->addWidget(labelIcon_);
	hBoxLayout->addWidget(labelMsgContent_, 1);

	// 设置窗口按钮
	btnOk_ = new QPushButton(widgetBody_);
	btnOk_->setFocusPolicy(Qt::NoFocus);
	btnOk_->setFixedSize(80, 40);
	btnOk_->setText(tr("确定"));

	btnCancel_ = new QPushButton(widgetBody_);
	btnCancel_->setFocusPolicy(Qt::NoFocus);
	btnCancel_->setFixedSize(80, 40);
	btnCancel_->setText(tr("取消"));

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
	connect(btnOk_, &QPushButton::clicked, this, &QDialog::reject);
	connect(btnCancel_, &QPushButton::clicked, this, &QDialog::accept);

	// 倒计时定时器
	timerCount_ = 10;
	timer_ = new QTimer(this);
	connect(timer_, &QTimer::timeout, this, &CustomMessageBox::sltTimerOut);

	this->setWinTitle(tr("提示"));
	//this->setWindowTitle(tr("提示"));
}

CustomMessageBox::~CustomMessageBox() { }

void CustomMessageBox::showMessage(const QString& content, const quint8& msgType, const QString& title) {
	if (content.isEmpty()) return;
	if (!title.isEmpty()) this->setWinTitle(title);
	labelMsgContent_->setText(content);
	// 如果提示窗口是问题窗口, 设置取消按钮不可见
	btnCancel_->setVisible(msgType == CustomMessageBox::Question);
	if (msgType == CustomMessageBox::Information) {
		labelIcon_->setPixmap(QPixmap(":/resource/buttonIcon/infoDialog.png"));
	}
	else if (msgType == CustomMessageBox::Question) {
		labelIcon_->setPixmap(QPixmap(":/resource/buttonIcon/questionDialog.png"));
	}
	else if (msgType == CustomMessageBox::Warning) {
		labelIcon_->setPixmap(QPixmap(":/resource/buttonIcon/warning.png"));
	}
}

void CustomMessageBox::startTimer() {
	if (timer_->isActive()) timer_->stop();
	timer_->start(1000);
}

void CustomMessageBox::sltTimerOut() {
	btnOk_->setText(tr("确定(%1)").arg(timerCount_--));
	if (timerCount_ < 0) {
		timer_->stop();
		this->accept();
	}
}

int CustomMessageBox::information(QWidget* parent, const QString& content, const QString& title) {
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Information, title);
	msgBox->startTimer();
	msgBox->sltTimerOut();
	return msgBox->exec();
}

int CustomMessageBox::question(QWidget* parent, const QString& content, const QString& title)
{
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Question, title);
	return msgBox->exec();
}

int CustomMessageBox::warning(QWidget* parent, const QString& content, const QString& title)
{
	CustomMessageBox* msgBox = new CustomMessageBox(parent);
	msgBox->showMessage(content, CustomMessageBox::Warning, title);
	return msgBox->exec();
}

/*!
* =================================================================================================
*									CustomInputDialog
* =================================================================================================
*/

CustomInputDialog::CustomInputDialog(QWidget* parent) {
	this->setAttribute(Qt::WA_DeleteOnClose, false);
	labelText_ = new QLabel(widgetBody_);
	labelText_->setText(tr("输入"));
	
	lineEditInput_ = new QLineEdit(widgetBody_);
	lineEditInput_->setEchoMode(QLineEdit::Normal);

	btnCancel_ = new QPushButton(widgetBody_);
	btnCancel_->setFocusPolicy(Qt::NoFocus);
	btnCancel_->setText(tr("取消"));

	btnOk_ = new QPushButton(widgetBody_);
	btnOk_->setFocusPolicy(Qt::NoFocus);
	btnOk_->setText(tr("确定"));

	QHBoxLayout* horLayoutBtn = new QHBoxLayout();
	horLayoutBtn->setContentsMargins(10, 10, 10, 10);
	horLayoutBtn->setSpacing(10);
	horLayoutBtn->addStretch();
	horLayoutBtn->addWidget(btnCancel_);
	horLayoutBtn->addWidget(btnOk_);

	QVBoxLayout* verLayout = new QVBoxLayout(widgetBody_);
	verLayout->setContentsMargins(10, 10, 10, 10);
	verLayout->addWidget(labelText_);
	verLayout->addWidget(lineEditInput_);
	verLayout->addLayout(horLayoutBtn);

	// 信号和槽
	connect(btnCancel_, &QPushButton::clicked, this, &QDialog::reject);
	connect(btnOk_, &QPushButton::clicked, this, &QDialog::accept);
}

CustomInputDialog::~CustomInputDialog() { }

QString CustomInputDialog::getInputText(QWidget* parent, const QString& text, 
										const QString& title, QLineEdit::EchoMode mode) {
	CustomInputDialog inputDlg(parent);
	inputDlg.setInputText(text);
	inputDlg.setEchoMode(mode);
	inputDlg.setWinTitle(title);
	
	if (inputDlg.exec() == QDialog::Accepted) {
		return inputDlg.getText();
	}
	return QString();
}

QString CustomInputDialog::getText() const { return lineEditInput_->text(); }

void CustomInputDialog::setInputText(const QString& text) {
	if (text.isEmpty()) return;
	lineEditInput_->setText(text);
	lineEditInput_->setFocus();
	lineEditInput_->selectAll();
}

void CustomInputDialog::setEchoMode(QLineEdit::EchoMode mode) { lineEditInput_->setEchoMode(mode); }
