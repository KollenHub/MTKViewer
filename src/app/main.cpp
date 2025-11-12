#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include "vtkWindowTest.h"

int main(int argc, char **argv)
{
    //测试加载dicom文件夹
    QApplication app(argc, argv);
    
    MainWindow win;
    win.show();
    app.exec();
    return 0;
}