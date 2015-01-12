#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "src/helper.hpp"
#include "libs/easylogging++.h"
#include "properties.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    facedetect_ ( new facedetector() ),
    face_aligner_ ( ),
    facerecognizer_eigen_ ( new facerecognizer(cv::createEigenFaceRecognizer())),
    facerecognizer_fisher_ ( new facerecognizer(cv::createFisherFaceRecognizer())),
    facerecognizer_lbp_ ( new facerecognizer(cv::createLBPHFaceRecognizer()))
{

    ui->setupUi(this);

    //DEBUG
    //TODO: remove
    ui->textEdit_photoInput->setText( "/home/johannes/Pictures/dfb.png");
    ui->textEdit_compareInput->setText( "/home/johannes/Pictures/dfb.png");
    ui->centralWidget->setFixedSize(this->width(), this->height());
    ui->groupBox->setDisabled(true);
    ui->pushButton_compareInput->setDisabled(true);
    std::string file_name =  "/home/johannes/Pictures/test.csv";
    helper::read_csv( file_name, faces_, labels_ );

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_sort_clicked()       //Sortierbutton
{
    for ( unsigned int i = 0; i < faces_.size(); ++i )
    {
        face_aligner_->align_face( faces_[i] );
    }
    facerecognizer_eigen_->train_model( faces_, labels_ );
    facerecognizer_fisher_->train_model( faces_, labels_ );
    facerecognizer_lbp_->train_model( faces_, labels_ );

    /*
    comp_file_ = ui->textEdit_compareInput->toPlainText().toStdString();
    comp_img_ = cv::imread( comp_file_, CV_LOAD_IMAGE_COLOR );
    img_ = cv::imread( ui->textEdit_photoInput->toPlainText().toStdString(), CV_LOAD_IMAGE_COLOR );
    */
    for ( unsigned int i = 0; i < input_files_.size(); ++i )
    {
        cv::Mat img;
        std::vector < cv::Mat > faces;

        img = cv::imread( input_files_[i], CV_LOAD_IMAGE_COLOR );

        facedetect_->detect_face( img, faces );
        for ( unsigned int j = 0; j < faces.size(); ++j )
        {
            int label;
            face_aligner_->align_face( img );
            facerecognizer_eigen_->recognize( img, label);
            std::cout << label << std::endl;
        }
    }

//    face_aligner_->test();
//    face_eigen_->recognize_face( faces, comp_img_ );

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
        comp_file_ = file_imgOne.toStdString();
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

    for ( int i = 0; i < selection.size(); ++i )
    {
        input_files_.push_back( selection[i].toStdString() );
        helper::check_file( input_files_[i] );
    }

    if(!selection.isEmpty())
    {
        ui->groupBox->setDisabled(false);
    }
}

void MainWindow::on_pushButton_photoOutput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Open Directory"),
                "/home",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void MainWindow::on_checkBox_onePerson_toggled(bool checked)
{
    if(checked == true)
    {
        ui->pushButton_compareInput->setDisabled(false);

    }
    else
    {
        ui->pushButton_compareInput->setDisabled(true);

    }
}


void MainWindow::on_pushButton_2_clicked()
{
    Properties prop;
    prop.setModal(true);
    prop.exec();

}

void MainWindow::on_pushButton_data_clicked()
{
    QString recogfile = QFileDialog::getOpenFileName(
                this,
                "Select a *csv file with models for sorting",
                "/home",
                "data (*.csv)");
}
