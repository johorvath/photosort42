#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_sort_clicked()       //Sortierbutton
{

}


void MainWindow::on_pushButton_compareInput_clicked()
{
    QString imgOne=QFileDialog::getOpenFileName(
                this,
                "Select an image to sort according to one person",
                "/home",
                "Images (*.png *.xpm *.jpg)");

    QPixmap pix(imgOne);
    int w = ui->label_compareImage->width();
    int h = ui->label_compareImage->height();
    ui ->label_compareImage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));


}


void MainWindow::on_pushButton_photoInput_clicked()
{
    QStringList img=QFileDialog::getOpenFileNames(
                this,
                "Select more than one images to sort",
                "/home",
                "Images (*.png *.xpm *.jpg)");

}
