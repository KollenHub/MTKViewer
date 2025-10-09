#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
