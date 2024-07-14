#include "login.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"
#include "qlineedit.h"
#include "qfile.h"
#include "qdebug.h"

Login::Login(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::LoginClass())
{
	ui->setupUi(this);
	// ȥ��QT�Դ��ı�����
	setWindowFlags(Qt::FramelessWindowHint);
	// ʹ͸����Ч
	//setAttribute(Qt::WA_TranslucentBackground);
	setWindowIcon(QIcon(":/resource/ico/chatting.png"));
	// ����page1
	ui->btnClose->setIcon(QIcon(":/resource/common/ic_close_white.png"));
	ui->btnMenu->setIcon(QIcon(":/resource/common/ic_login_cfg.png"));
	ui->btnMin->setIcon(QIcon(":/resource/common/ic_min_white.png"));
	ui->labelWinIcon->setPixmap(QPixmap(":/resource/ico/chatting.png"));
	ui->labelWinIcon->setScaledContents(true);
	ui->labelWelcome->setPixmap(QPixmap(":/resource/background/welcome.png"));
	ui->labelUserHead->setPixmap(QPixmap(":/resource/head/hacker.png"));
	ui->labelUserHead->setScaledContents(true);
	ui->labelWelcome->setScaledContents(true);
	ui->labelWinTitle->setText(QString::fromLocal8Bit("��¼����"));
	ui->lineEditUser->setPlaceholderText(QString::fromLocal8Bit("�������˺�"));
	ui->lineEditPasswd->setPlaceholderText(QString::fromLocal8Bit("����������"));
	ui->lineEditPasswd->setEchoMode(QLineEdit::Password);

	// ����page2
	ui->btnClose2->setIcon(QIcon(":/resource/common/ic_close_white.png"));
	ui->btnMin2->setIcon(QIcon(":/resource/common/ic_min_white.png"));
	ui->lineEditMsgServerPort->setAlignment(Qt::AlignCenter);
	ui->lineEditFileServerPort->setAlignment(Qt::AlignCenter);

	// ͨ����ʽ���ļ�������ʽ
	QFile file(":/qss/resource/qss/loadwindow.css");
	file.open(QIODevice::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	// �����źŲ�
	connect(ui->btnClose, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnClose2, &QPushButton::clicked, this, &QWidget::close);
	connect(ui->btnMin, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMin2, &QPushButton::clicked, this, &QWidget::showMinimized);
	connect(ui->btnMenu, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	connect(ui->btnCancel, &QPushButton::clicked, ui->stackedWidget, &RotateStackedWidget::nextPage);
	connect(ui->btnOk, &QPushButton::clicked, [this]() {
		qDebug() << ui->lineEditServerAddress->text();
	});
}

Login::~Login()
{
	delete ui;
}

void Login::mousePressEvent(QMouseEvent* event) {
	// �ж��Ƿ����϶�����
	if (event->button() == Qt::LeftButton && isInDraggableArea(event->pos())) {
		qDebug() << event->pos().x() << ", " << event->pos().y();
		dragging_ = true;
		// ��ȡ�����λ������ڴ������Ͻǵ�ƫ����
		dragStartPosition_ = event->pos();
	}
	else if (event->buttons() == Qt::LeftButton && isInScaleArea(event->pos())) {
		scaling_ = true;
	}
	event->accept();
}

void Login::mouseMoveEvent(QMouseEvent* event) {
	// ��������϶������ƶ�����
	if (dragging_ && (event->buttons() & Qt::LeftButton)) {
		move(QPoint(event->globalPos().x() - dragStartPosition_.x(),
			event->globalPos().y() - dragStartPosition_.y()));
	}
	if (scaling_ && (event->buttons() & Qt::LeftButton)) {
		resize(event->globalX() - geometry().x(), event->globalY() - geometry().y());
	}
	event->accept();
}

void Login::mouseReleaseEvent(QMouseEvent* event) {
	// ����ͷţ��������draging_Ϊ��
	if (event->button() == Qt::LeftButton) {
		dragging_ = false;
		scaling_ = false;
	}
	event->accept();
}

bool Login::isInDraggableArea(const QPoint& pos) {
	return (pos.x() >= 0 && pos.x() <= width() && pos.y() >= 0 && pos.y() <= 35);
}

bool Login::isInScaleArea(const QPoint& pos) {
	return (pos.x() >= width() - 30 && pos.x() <= width() 
			&& pos.y() > 35 && pos.y() <= height()) 
			|| (pos.x() >= 0 && pos.x() <= width() 
			&& pos.y() >= height() - 30 && pos.y() <= height());
}


