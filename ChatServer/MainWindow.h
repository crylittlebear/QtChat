/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#pragma once

#include <QtWidgets/QWidget>
#include "qsystemtrayicon.h"
#include <QModelIndex>
#include "qstandarditemmodel.h"
#include "ui_MainWindow.h"

#include "baseWidget/CustomWidget.h"
#include "baseWidget/RotateStackedWidget.h"
#include "TcpServer.h"
#include "MyApp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; };
QT_END_NAMESPACE

class MainWindow : public CustomWidget
{
    Q_OBJECT

public:
    /*!
    * @brief 构造函数
    */
    explicit MainWindow(QWidget *parent = nullptr);

    /*!
    * @brief 析构函数
    */
    ~MainWindow();

protected:
    /*!
    * @brief 改变事件
    */
    virtual void changeEvent(QEvent* event) override;

private slots:
    /*!
    * @brief 改变页面
    */
    void sltChangePages(int index);

    /*!
    * @brief 登陆界面退出按钮点击
    */
    void sltBtnLoginExitClicked();

    /*!
    * @brief 标题栏关闭按钮点击
    */
    void sltBtnWinCloseClicked();

    /*!
    * @brief 功能界面退出按钮点击
    */
    void sltBtnWinMinClicked();

    /*!
    * @brief 备份按钮点击
    */
    void sltBtnBackupClicked();

    /*!
    * @brief 数据还原按钮点击
    */
    void sltBtnDataUdoClicked();

    /*!
    * @brief 登录按钮点击
    */
    void sltBtnLoginClicked();

    /*!
    * @brief 功能界面退出按钮点击
    */
    void sltBtnExitClicked();

    /*!
    * @brief 系统托盘图标点击
    */
    void sltTrayIconClicked(QSystemTrayIcon::ActivationReason reason);

    /*!
    * @brief 系统托盘菜单项点击
    */
    void sltTrayIconMenuClicked(QAction* action);

    /*!
    * @brief 显示用户状态
    */
    void sltShowUserStatus(const QString& text);

    /*!
    * @brief 
    */
    void sltTableClicked(const QModelIndex& index);

    /*!
    * @brief 用户刷新按钮点击
    */
    void sltBtnUserRefreshClicked();

    /*!
    * @brief 用户插入按钮点击
    */
    void sltBtnUserInserClicked();

protected:
    /*!
    * @brief 关闭事件,点击桌面右上角关闭按钮缩小到托盘
    * 将关闭事件拦截，不关闭窗口
    */
    virtual void closeEvent(QCloseEvent* event) override;

    /*!
    * @brief 定时器事件
    */
    virtual void timerEvent(QTimerEvent* event) override;

protected:
    int timerId_;       /// 定时器ID

private:
    /*!
    * @brief 初始化网络
    */
    void initNetwork();

    /*!
    * @brief 设置用户身份
    */
    void setUserIdentity(const int& identity);

private:
    Ui::MainWindow *ui;

    QButtonGroup* buttonGroup_;
    QStandardItemModel* itemModel_;

    // 服务器配置
    TcpMsgServer* msgServer_;

    // 系统菜单
    QSystemTrayIcon* sysTrayIcon_;
};
