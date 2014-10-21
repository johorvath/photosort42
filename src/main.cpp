#include "mainwindow.h"
#include <QApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include "helper.hpp"

int main(int argc, char *argv[])
{
    helper::check_is_dir( "Hallo" );
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();    
}
