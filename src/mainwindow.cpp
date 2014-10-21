#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "helper.hpp"
#include "easylogging++.h"


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
    QString file_imgOne = QFileDialog::getOpenFileName(
                this,
                "Select an image to sort according to one person",
                "/home",
                "Images (*.png *.xpm *.jpg)");

    if ( helper::check_input( file_imgOne.toStdString() ) == helper::data_type::existing_file )
    {
      QPixmap pix( file_imgOne );
      int w = ui->label_compareImage->width();
      int h = ui->label_compareImage->height();
      ui ->label_compareImage->setPixmap( pix.scaled( w, h, Qt::KeepAspectRatio ) );
    }
    else
    {
      LOG( ERROR ) << "Not existing file chosen! Try again";
    }
}


void MainWindow::on_pushButton_photoInput_clicked()
{
    QStringList selection = QFileDialog::getOpenFileNames(
                this,
                "Select more than one images to sort",
                "/home",
                "Images (*.png *.xpm *.jpg)" );

    for ( unsigned int i = 0; i < selection.size(); ++i )
    {
      helper::check_input( selection[i].toStdString() );
    }

    const std::string output = ui->textEdit_output->toPlainText().toStdString();
    std::cout << output << std::endl;

}
