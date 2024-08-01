#include "customLineEdit.h"

#include "qsizepolicy.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include "qregexp.h"
#include "qvalidator.h"
#include "qevent.h"
#include "qdebug.h"

IpLineEdit::IpLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->setContentsMargins(2, 2, 2, 2);
	hBoxLayout->setSpacing(5);
	// 设置IP输入正则表达式
	QRegExp regExp("(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})");

	QLabel* dots[3] = { nullptr };
	for (int i = 0; i < 4; ++i) {
		lineEdits_[i] = new QLineEdit(this);
		// 去掉lineEdit无边框
		lineEdits_[i]->setFrame(false);
		// 设置每个lineEdit可输入数字个数
		lineEdits_[i]->setMaxLength(3);
		// 设置lineEdit输入居中
		lineEdits_[i]->setAlignment(Qt::AlignCenter);
		// 设置sizePolicy
		lineEdits_[i]->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
		// 验证输入
		lineEdits_[i]->setValidator(new QRegExpValidator(regExp, this));
		// 安装事件过滤器,过滤子控件事件，截获控件按键、鼠标事件
		lineEdits_[i]->installEventFilter(this);

		lineEdits_[i]->setStyleSheet(
			"font-family: '微软雅黑';" 
			"font-size: 20px;"
			"font-weight: bold;"
			"background-color: #f2f2f2"
		);
		// 添加到布局中
		hBoxLayout->addWidget(lineEdits_[i]);
		if (i < 3) {
			dots[i] = new QLabel(this);
			dots[i]->setText(".");
			dots[i]->setStyleSheet(
				"font-size: 14pt;"
				"font-weight: bold;"
			);
			dots[i]->setFixedWidth(5);
			hBoxLayout->addWidget(dots[i]);
		}
		this->setReadOnly(true);
		lineEdits_[0]->setFocus();
		lineEdits_[0]->selectAll();
	}
}

IpLineEdit::~IpLineEdit() {}

QString IpLineEdit::text() {
	QString res;
	for (int i = 0; i < 4; ++i) {
		res += lineEdits_[i]->text();
		if (i < 3) {
			res += ".";
		}
	}
	return res;
}

bool IpLineEdit::eventFilter(QObject * obj, QEvent * event) {
	if (children().contains(obj) && QEvent::KeyPress == event->type()) {
		QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(obj);
		switch (keyEvent->key()) {
		case Qt::Key_0:
		case Qt::Key_1:
		case Qt::Key_2:
		case Qt::Key_3:
		case Qt::Key_4:
		case Qt::Key_5:
		case Qt::Key_6:
		case Qt::Key_7:
		case Qt::Key_8:
		case Qt::Key_9:
		{
			QString str = lineEdit->text();
			if (str.length() == 3 || str.toInt() * 10 > 255) {
				int index = getIndex(lineEdit);
				if (index != -1 && index != 3) {
					lineEdits_[index + 1]->setFocus();
				}
			}
			return QLineEdit::eventFilter(obj, event);
		}
		break;
		case Qt::Key_Left:
		{
			if (!lineEdit->cursorPosition()) {
				int index = getIndex(lineEdit);
				if (index > 0) {
					lineEdits_[index - 1]->setFocus();
					int len = lineEdits_[index - 1]->text().length();
					lineEdits_[index - 1]->setCursorPosition(len ? len : 0);
				}
			}
			return QLineEdit::eventFilter(obj, event);
		}
		break;
		case Qt::Key_Right:
		{
			int len = lineEdit->text().size();
			if (lineEdit->cursorPosition() == len) {
				int index = getIndex(lineEdit);
				if (index != -1 && index < 3) {
					lineEdits_[index + 1]->setFocus();
					lineEdits_[index + 1]->setCursorPosition(0);
				}
			}
			return QLineEdit::eventFilter(obj, event);
		}
		break;
		case Qt::Key_Backspace:
		{
			if (!lineEdit->cursorPosition()) {
				int index = getIndex(lineEdit);
				if (index > 0) {
					lineEdits_[index - 1]->setFocus();
					int len = lineEdits_[index - 1]->text().length();
					lineEdits_[index - 1]->setCursorPosition(len ? len : 0);
				}
			}
			return QLineEdit::eventFilter(obj, event);
		}
		break;
		case Qt::Key_Period:
			int index = getIndex(lineEdit);
			if (index != -1 && index != 3) {
				lineEdits_[index + 1]->setFocus();
				lineEdits_[index + 1]->selectAll();
			}
			return QLineEdit::eventFilter(obj, event);
		}
	}
	return false;
}

int IpLineEdit::getIndex(QLineEdit* lineEdit) {
	for (int i = 0; i < 4; ++i) {
		if (lineEdit == lineEdits_[i]) {
			return i;
		}
	}
	return -1;
}


/*
* ////////////////////////////////////////////////////////////////////////
*							IconLineEdit
* ////////////////////////////////////////////////////////////////////////
*/

IconLineEdit::IconLineEdit(QWidget* parent) : QLineEdit(parent){
	label_ = new QLabel(this);
	label_->setMinimumSize(20, 20);
	label_->setVisible(false);
}

IconLineEdit::~IconLineEdit() {}

void IconLineEdit::setPic(const QPixmap& pixmap) {
	if (pixmap.isNull()) return;
	label_->setPixmap(pixmap);
	label_->setScaledContents(true);
	label_->setVisible(true);
	label_->setGeometry(10, 10, 25, 25);
	this->setTextMargins(35, 1, 1, 1);
}