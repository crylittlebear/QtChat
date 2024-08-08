#pragma once

#include <qwidget.h>
#include <qvector.h>
#include <qboxlayout.h>

class ItemInfo;
class BubbleListPrivate;
class QMenu;
class QScrollBar;

class BubbleList  : public QWidget
{
	Q_OBJECT

public:
	/*!
	* @brief 构造函数
	*/
	BubbleList(QWidget *parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~BubbleList();

	/*!
	* @brief 添加聊天条目
	*/
	void addItem(ItemInfo* item);

	/*!
	* @brief 添加若干聊天条目
	*/
	void addItems(QVector<ItemInfo*> items);

	/*!
	* @brief 清空气泡列表
	*/
	void clear();

	/*!
	* @brief 构造函数
	*/
	void render();

	/*!
	* @brief 构造函数
	*/
	void calcGeo();

	/*!
	* @brief 设置当前条目索引
	*/
	void setCurrentItemIndex(int itemIndex);

signals:
	/*!
	* @brief 设置当前条目索引信号
	*/
	void sigSetCurrentIndex(int curIdnex);

	/*!
	* @brief 条目点击信号
	*/
	void sigItemClicked(const QString& str);

	/*!
	* @brief 文件下载信号
	*/
	void sigDownloadFile(const QString& fileName);

protected:
	/*!
	* @brief 重写窗口示意函数，提供默认的窗口大小攻父窗口调用
	*/
	virtual QSize sizeHint() const override;

	/*!
	* @brief 调整窗口的大小时间
	*/
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	/*!
	* @brief 初始化数据成员
	*/
	void initVariables();

	/*!
	* @brief 初始化窗口布局等信息
	*/
	void initWidgets();

	/*!
	* @brief 初始化窗口状态信息
	*/
	void initStages();

	/*!
	* @brief 初始化信号槽
	*/
	void initConnections();

private slots:
	/*!
	* @brief 设置最大条目个数
	*/
	void sltSetMaximum(int max);

private:
	// 主布局
	QVBoxLayout* mainLayout_;
	// 滚动条
	QScrollBar* scrollBar_;
	// 具体的bubbleList实现类
	BubbleListPrivate* privateBubbleList_;
};

class BubbleListPrivate : public QWidget {
	Q_OBJECT
public:
	/*!
	* @brief 构造函数
	*/
	explicit BubbleListPrivate(QWidget* parent = nullptr);

	/*!
	* @brief 析构函数
	*/
	~BubbleListPrivate();

	/*!
	* @brief 添加条目
	*/
	void addItem(ItemInfo* item);

	/*!
	* @brief 添加一系列条目
	*/
	void addItems(QVector<ItemInfo*> items);

	/*!
	* @brief 清空条目
	*/
	void clear();

	/*!
	* @brief 构造函数
	*/
	void render();

	/*!
	* @brief 条目个数
	*/
	int itemCount() const;

signals:
	/*!
	* @brief 设置最大条目个数信号
	*/
	void sigSetMaximum(int max);

	/*!
	* @brief 设置当前索引信号
	*/
	void sigSetCurrentIndex(int index);

	/*!
	* @brief 条目点击信号
	*/
	void sigItemClicked(const QString& str);

	/*!
	* @brief 文件下载信号
	*/
	void sigDownloadFile(const QString& fileName);

public slots:
	/*!
	* @brief 设置当前索引槽函数
	*/
	void sltSetCurrentIndex(int curIndex);

protected:
	void paintEvent(QPaintEvent* event) override;

	void mouseMoveEvent(QMouseEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;

	void mouseDoubleClickEvent(QMouseEvent* event) override;

	void resizeEvent(QResizeEvent* event) override;

	void leaveEvent(QEvent* event) override;

	void showEvent(QShowEvent* event) override;

	void wheelEvent(QWheelEvent* event) override;

private: 
	/*!
	* @brief 初始化数据成员
	*/
	void initVariables();

	/*!
	* @brief 初始化设置
	*/
	void initSettings();

	/*!
	* @brief 构造函数
	*/
	void calcGeo();

	/*!
	* @brief 鼠标滚轮向上
	*/
	void wheelUp();

	/*!
	* @brief 鼠标滚轮向下
	*/
	void wheelDown();

private:
    /// painting functions
    void drawBackground(QPainter* painter);

    void drawItems(QPainter* painter);

    void drawHoverRect(QPainter* painter);

private slots:
	/*!
	* @brief 文件菜单项点击槽函数
	*/
	void sltFileMenuClicked(QAction* action);

private:
	QVector<ItemInfo*> itemVec_;
	int currentIndex_;
	int selectedIndex_;
	int visiableItemCount_;
	int itemCount_;
	bool allJobsDone_;
	QRectF hoverRect_;
	QFont font_;
	bool hover_;
	int hoverItemIndex_;

	QMenu* picRightButtonMenu_;
	QMenu* fileRightButtonMenu_;
};
