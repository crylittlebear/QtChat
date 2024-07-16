#pragma once

#include <QtWidgets/QWidget>
#include "ui_MainWindow.h"

#include "baseWidget/CustomWidget.h"

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