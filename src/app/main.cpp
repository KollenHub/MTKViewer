#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include "vtkWindowTest.h"
#include <memory>
#include "dicom/DicomData.h"

int main(int argc, char **argv)
{
    //测试加载dicom文件夹
    QApplication app(argc, argv);
    
    // 注册自定义类型
    // qRegisterMetaType<std::shared_ptr<DicomData>>();

    MainWindow win;
    win.show();
    app.exec();
    return 0;
}