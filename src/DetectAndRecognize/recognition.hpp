#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QFile>
#include <boost/filesystem.hpp>

#include "src/helper.hpp"

enum recognize_mode {
    EIGEN,
    FISHER,
    LBP
};

class RecognitionModel      //class for using the database
{
    public:
    int id;
    QString firstname;
    QString lastname;
    QByteArray mask;
};

class facerecognizer
{
public:
    facerecognizer(cv::Ptr <cv::FaceRecognizer> model , const std::string &path , const std::string &file_name);
    facerecognizer(cv::Ptr <cv::FaceRecognizer> model , const std::string &file_name);
//    ~facerecognizer();

    void update_model ( cv::Mat const& face, int const& label);

    void train_model (std::vector < cv::Mat > const& images, std::vector < int > const& labels);

    int recognize_face ( recognize_mode const& mode, cv::Mat const& comp_face );

    void save_model ( std::string const& path );

    void load_model ( std::string const& path );

    void recognize_test ();

    void recognize ( std::vector <cv::Mat>& images, std::vector <int>& labels);

    cv::Ptr <cv::FaceRecognizer> model_;


private:
    void load_model (recognize_mode const& mode, std::string const& dir );
    void scale_mats ( std::vector < cv::Mat >& imgs );
    std::string model_path_;

    std::vector <cv::Mat> faces_;
    std::vector <int> labels_;

public:                 //database
    bool openDB();
    bool deleteDB();    
    bool createModelTable();
    int insertModel(QString firstname, QString lastname, QByteArray mask);
    bool getModel(int id, RecognitionModel*& model);
    QSqlError lastError();

private:
        QSqlDatabase db;

    void recognize_face ( cv::Mat const& face, cv::Mat const& comp_face );
    int xmin_;
    int xmax_;
    int ymin_;
    int ymax_;
    int xresize_;
    int yresize_;
    int xsum_;
    int ysum_;
};

class facerecognizer_eigen : public facerecognizer
{
public:
    facerecognizer_eigen();
    ~facerecognizer_eigen();
};

class facerecognizer_fisher : public facerecognizer
{
public:
    facerecognizer_fisher();
    ~facerecognizer_fisher();
};

class facerecognizer_lbp : public facerecognizer
{
public:
    facerecognizer_lbp();
    ~facerecognizer_lbp();
};

