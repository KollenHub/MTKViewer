#include <iostream>
#include <QApplication>
#include "NewWindow.h"
#include "vtkWindowTest.h"

int main(int argc, char **argv)
{
    //测试加载dicom文件夹
    QApplication app(argc, argv);
    NewWindow win;
    win.show();
    app.exec();
    return 0;
}