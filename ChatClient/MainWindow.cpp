#include "MainWindow.h"

#include "qpixmap.h"
#include "qevent.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent)
	: CustomWidget(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);
	// ���ô��ڶ�������
	setWindowFlags(Qt::FramelessWindowHint);
	setWindowIcon(QIcon(":/resource/ico/chatting.png"));
	// ���ô���ͼ��
	ui->labelWinIcon->setPixmap(QPixmap(":/resource/ico/chatting.png"));
	// ���ô��ڶ���widget��ʽ
	ui->widgetWinTitle->setStyleSheet("background-color: #0da2af;");
	ui->widgetHeadInfo->setStyleSheet("background-color: #0da2af;");
	ui->labelWinIcon->setScaledContents(true);
	ui->labelUserName->setText(QString::fromLocal8Bit("������"));
	ui->labelUserName->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelSigUp->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelSigDown->setStyleSheet("color: #FFFFFF; font-weight: bold");
	ui->labelContacts->setPixmap(QPixmap(":/resource/images/contacts.png"));
	ui->labelContacts->setScaledContents(true);
	ui->labelWeather->setPixmap(QPixmap(":/resource/images/cloudy.png"));
	ui->labelWeather->setScaledContents(true);

	//�����źŲ�
	connect(ui->btnWinClose, &QPushButton::clicked, this, &QWidget::close);
}

MainWindow::~MainWindow()
{
	delete ui;
}
