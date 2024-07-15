#include "MainWindow.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);
	// 设置窗口顶部隐藏
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowIcon(QIcon(":/resource/ico/chatting.png"));
	// 设置窗口图标
	ui->labelWinIcon->setPixmap(QPixmap(":/resource/ico/chatting.png"));
	// 设置窗口顶部widget样式
	ui->widgetWinTitle->setStyleSheet("background-color: #0da2af;");
	ui->widgetHeadInfo->setStyleSheet("background-color: #0da2af;");
	ui->labelWinIcon->setScaledContents(true);
	ui->btnWinMenu->setIcon(QIcon(":/resource/common/ic_login_cfg.png"));
	ui->btnWinMin->setIcon(QIcon(":/resource/common/ic_min_white.png"));
	ui->btnWinClose->setIcon(QIcon(":/resource/common/ic_close_white.png"));
	ui->labelUserName->setText(QString::fromLocal8Bit("李立恒"));
	ui->labelUserName->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelSigUp->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelSigDown->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelContacts->setPixmap(QPixmap(":/resource/images/contacts.png"));
	ui->labelContacts->setScaledContents(true);
	ui->labelWeather->setPixmap(QPixmap(":/resource/images/cloudy.png"));
	ui->labelWeather->setScaledContents(true);

	//设置信号槽
	connect(ui->btnWinClose, &QPushButton::clicked, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
	// 检测是否左键按下
	if (event->button() == Qt::LeftButton && isInDragingArea(event->pos())) {
		draging_ = true;
		startPoint_ = event->pos();
	}
	event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
	if (draging_ && (event->buttons() & Qt::LeftButton)) {
		move(QPoint(event->globalPos().x() - startPoint_.x(),
			event->globalPos().y() - startPoint_.y()));
	}
	event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		draging_ = false;
	}
	event->accept();
}

bool MainWindow::isInDragingArea(const QPoint& point) const {
	int dragingWidth = width();
	int dragingHeight = ui->widgetWinTitle->height();
	if (point.x() >= 0 && point.x() < dragingWidth && point.y() >= 0 &&
		point.y() <= dragingHeight) {
		qDebug() << "x: " << point.x() << ", y: " << point.y();
		return true;
	}
	return false;
}
