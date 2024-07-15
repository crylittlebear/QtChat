#include "MainWindow.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
	: CustomWidget(parent)
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
