#include "gui/mainwindow.h"
#include <QApplication>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include "libs/easylogging++.h"

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
  LOG(INFO) << "first log";
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
