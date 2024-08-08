#include "ChatBubble.h"

#include "comapi/ItemInfo.h"
#include "comapi/unit.h"
#include "comapi/global.h"

#include <qfileinfo.h>
#include <qfile.h>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qevent.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpainterpath.h>
#include <qfileiconprovider.h>
#include <qboxlayout.h>
#include <qscrollbar.h>
#include <qmenu.h>

BubbleList::BubbleList(QWidget *parent)
	: QWidget(parent)
{
	initVariables();
	initWidgets();
	initStages();
	initConnections();
}

BubbleList::~BubbleList()
{

}

void BubbleList::addItem(ItemInfo * item) {
	privateBubbleList_->addItem(item);
}

void BubbleList::addItems(QVector<ItemInfo*> items) {
	privateBubbleList_->addItems(items);
}

void BubbleList::clear() {
	privateBubbleList_->clear();
}

void BubbleList::render() {
	privateBubbleList_->render();
}

void BubbleList::calcGeo() {

}

void BubbleList::setCurrentItemIndex(int itemIndex) {
	scrollBar_->setValue(itemIndex);
}

QSize BubbleList::sizeHint() const {
	return QSize(SIZE_HINT);
}

void BubbleList::resizeEvent(QResizeEvent* event) {

}

void BubbleList::initVariables() {

}

void BubbleList::initWidgets() {
	mainLayout_ = new QVBoxLayout(this);
	scrollBar_ = new QScrollBar(this);
	privateBubbleList_ = new BubbleListPrivate(this);
	this->setMinimumWidth(300);

	mainLayout_->addWidget(privateBubbleList_);
	mainLayout_->addWidget(scrollBar_);
	setLayout(mainLayout_);
}

void BubbleList::initStages() {
	mainLayout_->setContentsMargins(0, 0, 0, 0);
	mainLayout_->setSpacing(0);
	scrollBar_->setRange(0, 0);
	scrollBar_->hide();
}

void BubbleList::initConnections() {
	connect(scrollBar_, &QScrollBar::valueChanged, privateBubbleList_, 
		&BubbleListPrivate::sltSetCurrentIndex);
	connect(privateBubbleList_, &BubbleListPrivate::sigSetMaximum, this, 
		&BubbleList::sltSetMaximum);
	connect(privateBubbleList_, &BubbleListPrivate::sigSetCurrentIndex, 
		scrollBar_, &QScrollBar::setValue);
	connect(privateBubbleList_, &BubbleListPrivate::sigItemClicked, 
		this, &BubbleList::sigItemClicked);
	connect(privateBubbleList_, &BubbleListPrivate::sigDownloadFile, 
		this, &BubbleList::sigDownloadFile);
}

void BubbleList::sltSetMaximum(int max) {
	if (max == 0) {
		scrollBar_->hide();
	} else {
		scrollBar_->show();
	}
	scrollBar_->setRange(0, max);
}

/*!
* ================================================
*				BubbleListPrivate
* ================================================
*/

BubbleListPrivate::BubbleListPrivate(QWidget* parent) {
	initVariables();
	initSettings();
}

BubbleListPrivate::~BubbleListPrivate() {
	clear();
}

void BubbleListPrivate::addItem(ItemInfo* item) {
	itemVec_.push_front(item);
	currentIndex_ = 0;
	update();
}

void BubbleListPrivate::addItems(QVector<ItemInfo*> items) {
	itemVec_.clear();
	itemVec_ = std::move(items);
	currentIndex_ = 0;
	update();
}

void BubbleListPrivate::clear() {
	itemVec_.clear();
	currentIndex_ = 0;
	visiableItemCount_ = 0;
	itemCount_ = 0;
	allJobsDone_ = false;
	update();
}

void BubbleListPrivate::render() {
	update();
}

int BubbleListPrivate::itemCount() const {
	return itemVec_.size();
}

void BubbleListPrivate::sltSetCurrentIndex(int curIndex) {
	if (curIndex = currentIndex_) {
		return;
	}
	currentIndex_ = curIndex;
}

void BubbleListPrivate::initVariables() {
	currentIndex_ = 0;
	visiableItemCount_ = 0;
	itemCount_ = 0;
	allJobsDone_ = false;
	hover_ = false;
	font_ = QFont("微软雅黑", 12);
	// 图片右键菜单
	picRightButtonMenu_ = new QMenu(this);
	picRightButtonMenu_->addAction("保存图片");
	picRightButtonMenu_->addSeparator();
	picRightButtonMenu_->addAction("复制到剪切板");
	picRightButtonMenu_->hide();
	// 文件右键菜单
	fileRightButtonMenu_ = new QMenu(this);
	fileRightButtonMenu_->addAction("下载文件");
	picRightButtonMenu_->addSeparator();
	fileRightButtonMenu_->addAction("打开文件");
	picRightButtonMenu_->addSeparator();
	fileRightButtonMenu_->addAction("打开文件目录");
	fileRightButtonMenu_->hide();
	// 菜单项触发信号与槽函数
	connect(fileRightButtonMenu_, &QMenu::triggered, this, 
		&BubbleListPrivate::sltFileMenuClicked);
}

void BubbleListPrivate::initSettings() {
	setMouseTracking(true);
}

void BubbleListPrivate::calcGeo() {
	visiableItemCount_ = height() / (ITEM_HEIGHT + ITEM_SPACE + 10) + 1;
	int inVisiableCount = itemVec_.size() - visiableItemCount_;
	if (inVisiableCount >= 0) {
		emit sigSetMaximum(inVisiableCount);
	}
}

void BubbleListPrivate::wheelUp() {
	++currentIndex_;
	if (currentIndex_ > itemVec_.size()) {
		currentIndex_ = itemVec_.size() - 1;
	}
	update();
	emit sigSetCurrentIndex(currentIndex_);
}

void BubbleListPrivate::wheelDown() {
	--currentIndex_;
	if (currentIndex_ < 0) {
		currentIndex_ = 0;
	}
	update();
	emit sigSetCurrentIndex(currentIndex_);
}

void BubbleListPrivate::drawBackground(QPainter* painter) {
	painter->save();
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush("#ECECEC"));
	painter->drawRect(rect());
	painter->restore();
}

void BubbleListPrivate::drawItems(QPainter* painter) {
	if (itemVec_.size() == 0) {
		return;
	}
	painter->save();
	qreal itemY = 0;
	int width = this->width();
	width = width % 2 == 0 ? width : width + 1;
	itemY = ITEM_SPACE + ITEM_TITLE_HEIGHT;
	for (int i = currentIndex_; i < itemVec_.size(); ++i) {
		if (itemY > this->height()) {
			break;
		}
		QString strMsg = itemVec_.at(i)->getMsgContent();
		quint8 msgType = itemVec_.at(i)->getMsgType();
		quint8 orientation = itemVec_.at(i)->getOrientation();
		QString strPic = itemVec_.at(i)->getStrPic();
		qreal bubbleWidth = 0;
		qreal bubbleHeight = 0;
		
		int nx = 0;
		int ny = 0;
		QPixmap pixmap;
		QPainterPath path;
		// 气泡和内容的矩形框
		QRectF msgRect;
		QRectF bubbleRect;
		switch (msgType) {
			case Text:
			{
				QFontMetrics fm(font_);
				//bubbleWidth = fm.width(strMsg);
				bubbleWidth = fm.horizontalAdvance(strMsg);
				bubbleHeight = fm.height();
				QStringList strMsgList = strMsg.split("\n");
				qreal lineWidth = 0;
				qreal lineHeight = 0;
				for (int i = 0; i < strMsgList.size(); ++i) {
					if (lineWidth < fm.horizontalAdvance(strMsgList.at(i))) {
						lineWidth = fm.horizontalAdvance(strMsgList.at(i));
						lineWidth = lineWidth < (width * 2 / 3) ? lineWidth : width * 2 / 3;
					}
					lineHeight = lineWidth < (width * 2 / 3) ? fm.height() * 1.5 : 
						lineWidth / ((width * 2 / 3) + 1) * ITEM_HEIGHT / 3;
				}
				bubbleWidth = lineWidth;
				ny = this->height() - itemY - bubbleHeight;
			}
				break;
			case Audio:
				break;
			case Picture:
			{
				pixmap = QPixmap(strMsg);
				if (pixmap.isNull()) {
					pixmap = QPixmap(":/resource/background/image.png");
					pixmap.scaled(QSize(64, 64));
				}
				if (pixmap.width() > 200 || pixmap.height() > 100) {
					pixmap = pixmap.scaled(200, 100);
				}
				bubbleWidth = pixmap.width();
				bubbleHeight = pixmap.height() + 10;
				ny = this->height() - itemY - bubbleHeight;
			}
				break;
			case Files:
			{
				QFileInfo fileInfo(strMsg);
				QString strSize = itemVec_.at(i)->getFileSizeString();
				QPixmap pixmap(QString(":/resource/pixmap/zip.png"));
				// 判断文件是否存在
				if (QFile::exists(strMsg)) {
					strSize = "文件大小: ";
					strSize += myHelper::CalcSize(fileInfo.size());
#ifdef Q_OS_WIN32
					QFileIconProvider provider;
					QIcon icon = provider.icon(fileInfo);
					pixmap = icon.pixmap(48, 48);
#endif
				} else {
					strSize = QString::fromLocal8Bit("文件未下载");
				}
				// 计算文字的宽度
				QFont font("微软雅黑", 10);
				QFontMetrics fm(font);
				// 显示文件名,路径不显示
				strMsg = fileInfo.fileName();
				// 计算气泡的size
				if (fm.horizontalAdvance(strMsg) < fm.horizontalAdvance(strSize)) {
					bubbleWidth = fm.horizontalAdvance(strSize) + pixmap.width() + 20;
				} else {
					bubbleWidth = fm.horizontalAdvance(strMsg) + pixmap.width() + 20;
				}
				bubbleHeight = 60;
				bubbleWidth = bubbleWidth < (width * 2 / 3) ? bubbleWidth : (width * 2 / 3);
				bubbleWidth = bubbleWidth < 60 ? 60 : bubbleWidth;

				strMsg += "\n";
				strMsg += strSize;
				// 文字初始化告诉
				ny = this->height() - itemY - bubbleHeight;
			}
				break;
			default:
				break;
		} // end switch

		//--------------------绘制气泡和文字----------------------------------//

		if (orientation == Orientation::Right) {
			painter->save();
			painter->setPen(QPen(QColor("#D3D3D3"), 1));
			painter->drawRoundedRect(width - 55, ny, 50, 50, 2, 2);
			QPixmap pixmap(strPic);
			if (!pixmap.isNull()) {
				painter->drawPixmap(width - 55 + 1, ny + 1, 48, 48, pixmap);
			}
			painter->restore();
			// 绘制名字和当前时间
			painter->save();
			// 计算文字的宽度
			QFont font("微软雅黑", 8);
			QFontMetrics fm(font);
			QString strTitle = itemVec_.at(i)->getDatetime() + "  ";
			strTitle += itemVec_.at(i)->getName();
			painter->setPen(QColor("#666666"));
			painter->setFont(font);
			painter->drawText(width - 55 - fm.horizontalAdvance(strTitle) - 10, ny + fm.height() / 2, strTitle);
			painter->restore();
			// 根据消息内容计算气泡区域
			nx = width - 95 - bubbleWidth;
			if (nx < 0) {
				nx = -bubbleWidth - 95 + width;
			}
			// 计算气泡框
			ny += ITEM_TITLE_HEIGHT;
			bubbleRect = QRectF(nx, ny, bubbleWidth + 20, bubbleHeight);
			msgRect = QRectF(nx + 10, ny, bubbleWidth, bubbleHeight);
			painter->save();
			// 计算气泡右边的三角
			path.addRoundedRect(bubbleRect, 10, 10);
			path.moveTo(width - 75, ny + 15);
			path.lineTo(width - 65, ny + 20);
			path.lineTo(width - 75, ny + 25);
			painter->setPen(Qt::red);
			painter->fillPath(path, QColor("#FFFFFF"));
			painter->restore();
		} else if (orientation == Orientation::Left) {
			// 绘制头像
			painter->save();
			painter->setPen(QColor("#D3D3D3"));
			// 绘制边框
			painter->drawRoundedRect(5, ny, 50, 50, 2, 2);
			// 绘制头像
			QPixmap pixmap(strPic);
			if (!pixmap.isNull()) {
				painter->drawPixmap(6, ny + 1, 48, 48, pixmap);
			}
			painter->restore();
			nx = 75;
			// 绘制名字和当前时间
			painter->save();
			// 计算文字的宽度
			QFont font("微软雅黑", 8);
			QFontMetrics fm(font);

			QString strTitle = itemVec_.at(i)->getName() + "  ";
			strTitle += itemVec_.at(i)->getDatetime();

			painter->setPen(QColor("#666666"));
			painter->setFont(font);
			painter->drawText(nx , ny + fm.height() / 2, strTitle);
			painter->restore();

			// 绘制气泡区域
			painter->save();
			ny += ITEM_TITLE_HEIGHT;
			bubbleRect = QRectF(nx, ny, bubbleWidth + 20, bubbleHeight);
			// 绘制文字区域
			msgRect = QRectF(nx + 10, ny, bubbleWidth, bubbleHeight);
			// 左边三角
			path.addRoundedRect(bubbleRect, 5, 5);
			path.moveTo(nx, ny + 15);
			path.lineTo(nx - 10, ny + 20);
			path.lineTo(nx, ny + 25);
			// 背景色
			painter->fillPath(path, QColor("#9EE656"));
			painter->restore();
		}
		painter->setPen(QColor("#000000"));
		painter->setFont(font_);

		if (msgType == Text) {
			painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);
		} else if (msgType == Picture) {
			painter->drawPixmap(nx + 10, ny + 5, pixmap.width(), pixmap.height(), pixmap);
		} else if (msgType == Files) {
			painter->drawPixmap(nx + 10, ny + 5 + (48 - pixmap.height()) / 2, pixmap.width(), pixmap.height(), pixmap);
			painter->save();
			painter->setPen(Qt::blue);
			QFont font("微软雅黑", 10);
			font.setUnderline(hover_ && (hoverItemIndex_ == i));
			painter->setFont(font);
			painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);
			painter->restore();
		}
		itemVec_.at(i)->setBubbleRect(bubbleRect);
		itemY += (bubbleRect.height() + ITEM_SPACE + ITEM_TITLE_HEIGHT);
	} // end for
	painter->restore();
}

void BubbleListPrivate::drawHoverRect(QPainter* painter) {

}

void BubbleListPrivate::sltFileMenuClicked(QAction* action) {
	QString strText = itemVec_.at(currentIndex_)->getMsgContent();
	QFileInfo fileInfo(strText);
	// 菜单操作
	if (action->text() == QString::fromLocal8Bit("下载文件")) {
		if (QFile::exists(strText)) {
			QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
			return;
		}
		// 文件不存在,下载文件
		emit sigDownloadFile(fileInfo.fileName());
	} else if (action->text() == QString::fromLocal8Bit("打开文件")) {
		// 如果文件存在
		if (QFile::exists(strText)) {
			QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
			return;
		}
	} else if (action->text() == QString::fromLocal8Bit("打开文件目录")){
		QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
	}
}

void BubbleListPrivate::paintEvent(QPaintEvent* event) {
	QPainter painter(this);
	// 将painter对象的渲染设置为同时开启高质量抗锯齿和普通抗锯齿
	painter.setRenderHints(QPainter::Antialiasing);
	drawBackground(&painter);
	drawItems(&painter);
}

void BubbleListPrivate::mouseMoveEvent(QMouseEvent* event) {
	int itemY = ITEM_SPACE;
	for (int i = currentIndex_; i < itemVec_.size(); ++i) {
		// 如果气泡之间的间距大于当前窗口的高度
		if (itemY > this->height()) {
			return;
		}
		int ny = this->height() - itemY;
		QRectF bubbleRect = itemVec_.at(currentIndex_)->getBubbleRect();
		if (event->pos().y() < ny && event->pos().y() > ny - bubbleRect.height() &&
			event->pos().x() > bubbleRect.x() && event->x() < bubbleRect.x() + bubbleRect.width()) {
			quint8 type = itemVec_.at(i)->getMsgType();
			if (type == MessageType::Picture || type == MessageType::Files) {
				this->setCursor(Qt::PointingHandCursor);
				hover_ = true;
				hoverItemIndex_ = i;
				update();
				return;
			}
		}
		if (hover_) {
			hover_ = false;
			hoverItemIndex_ = -1;
			update();
			return;
		}
		itemY += bubbleRect.height() + ITEM_SPACE;
	}
	this->setCursor(Qt::ArrowCursor);
}

void BubbleListPrivate::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::RightButton) {
		int itemY = ITEM_SPACE;
		for (int i = currentIndex_; i < itemVec_.size(); ++i) {
			if (itemY > this->height()) {
				break;
			}
			int ny = this->height() - itemY;
			QRectF bubbleRect = itemVec_.at(i)->getBubbleRect();
			if (event->pos().y() < ny && event->pos().y() > ny - bubbleRect.height() &&
				event->pos().x() > bubbleRect.x() && event->x() < bubbleRect.x() + bubbleRect.width()) {
				// 记录当前选中的index
				selectedIndex_ = i;
				// 如果是图片或文件,可以直接打开
				if (itemVec_.at(i)->getMsgType() == Picture) {
					picRightButtonMenu_->popup(event->globalPos());
					return;
				} else if (itemVec_.at(i)->getMsgType() == Files) {
					fileRightButtonMenu_->popup(event->globalPos());
					return;
				}
			}
			itemY += bubbleRect.height() + ITEM_SPACE;
		}
	}
}

void BubbleListPrivate::mouseDoubleClickEvent(QMouseEvent* event) {
	int itemY = ITEM_SPACE;
	for (int i = 0; i < itemVec_.size(); ++i) {
		if (itemY > this->height()) {
			break;
		}
		int ny = this->height() - itemY;
		QRectF bubbleRect = itemVec_.at(i)->getBubbleRect();
		if (event->pos().y() < ny && event->pos().y() > ny - bubbleRect.height() &&
			event->pos().x() > bubbleRect.x() && event->x() < bubbleRect.x() + bubbleRect.width()) {
			if (itemVec_.at(i)->getMsgType() == Picture || itemVec_.at(i)->getMsgType() == Files) {
				QString strFile = itemVec_.at(i)->getMsgContent();
				QFileInfo fileInfo(strFile);
				if (QFile::exists(strFile)) {
					QDesktopServices::openUrl(QUrl(fileInfo.absolutePath()));
				}
				return;
			}
			emit sigItemClicked(itemVec_.at(i)->getMsgContent());
			break;
		}
		itemY += bubbleRect.height() + ITEM_SPACE;
	}
}

void BubbleListPrivate::resizeEvent(QResizeEvent* event) {
	calcGeo();
}

void BubbleListPrivate::leaveEvent(QEvent* event) {
	hoverRect_ = QRectF();
}

void BubbleListPrivate::showEvent(QShowEvent* event) {
	calcGeo();
}

void BubbleListPrivate::wheelEvent(QWheelEvent* event) {
	if (event->angleDelta().y() > 0) {
		wheelUp();
	} else {
		wheelDown();
	}
}