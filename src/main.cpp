#include <iostream>
#include <QApplication>
#include "NewWindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    NewWindow win;
    win.show();
    app.exec();
    return 0;
}