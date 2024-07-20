/*!
*  @Author: crylittlebear
*  @Data  : 2024-7-20
*/

#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWindow.h"

#include "baseWidget/CustomWidget.h"
#include "baseWidget/RotateStackedWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; };
QT_END_NAMESPACE

class MainWindow : public CustomWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
