#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "src/helper.hpp"
#include "libs/easylogging++.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    facedetect_ ( new facedetector )
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_sort_clicked()       //Sortierbutton
{
//    facedetect_->detect_face();

}


void MainWindow::on_pushButton_compareInput_clicked()
{
    //    QStringList selection = QFileDialog::getOpenFileNames(
    //              this,
    //              "Select more than one images to sort",
    //              "/home",
    //              "Images (*.png *.xpm *.jpg)" );

    ///just for my debug use
    QString file_imgOne = QFileDialog::getOpenFileName(
                this,
                "Select an image to sort according to one person",
                "/home/johannes/Pictures",
                "Images (*.png *.xpm *.jpg)");
    if ( helper::check_file( file_imgOne.toStdString() ) )
    {
        QPixmap pix( file_imgOne );
        int w = ui->label_compareImage->width();
        int h = ui->label_compareImage->height();
        ui ->label_compareImage->setPixmap( pix.scaled( w, h, Qt::KeepAspectRatio ) );
    }
}


void MainWindow::on_pushButton_photoInput_clicked()
{

//    QStringList selection = QFileDialog::getOpenFileNames(
//              this,
//              "Select more than one images to sort",
//              "/home",
//              "Images (*.png *.xpm *.jpg)" );

    ///just for my debug use
    QStringList selection = QFileDialog::getOpenFileNames(
              this,
              "Select more than one images to sort",
              "/home/johannes/Pictures",
              "Images (*.png *.xpm *.jpg)" );

    for ( unsigned int i = 0; i < selection.size(); ++i )
    {
        input_files_.push_back( selection[i].toStdString() );
        helper::check_file( input_files_[i] );
    }
}
