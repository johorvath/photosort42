#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <opencv2/core/core.hpp>

#include "DetectAndRecognize/detection.hpp"
#include "DetectAndRecognize/recognition.hpp"
#include "DetectAndRecognize/face_alignement.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_sort_clicked();    

    void on_pushButton_compareInput_clicked();

    void on_pushButton_photoInput_clicked();

    void on_pushButton_photoOutput_clicked();

    void on_checkBox_onePerson_toggled(bool checked);



    void on_pushButton_2_clicked();

    void on_pushButton_data_clicked();

private:
    Ui::MainWindow *ui;
    std::vector < std::string > input_files_;
    facedetector *facedetect_;
    std::string comp_file_;
    cv::Mat comp_img_;
    cv::Mat img_;
    cv::Size size_;
    std::vector <cv::Mat> faces_;
    std::vector <int> labels_;
    facerecognizer *facerecognizer_eigen_;
    facerecognizer *facerecognizer_fisher_;
    facerecognizer *facerecognizer_lbp_;
    cv::Ptr <cv::FaceRecognizer> model_eigen_;
    cv::Ptr <cv::FaceRecognizer> model_fisher_;
    cv::Ptr <cv::FaceRecognizer> model_lbp_;
    face_alignement *face_aligner_;
};

#endif // MAINWINDOW_H
