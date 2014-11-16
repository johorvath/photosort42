#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QFile>


class RecognitionModel      //class for using the database
{
    public:
    int id;
    QString firstname;
    QString lastname;
    int mask;
};

class facerecognizer
{
public:
    facerecognizer( std::string const& test );
    ~facerecognizer();

public:                 //database
    bool openDB();
    bool deleteDB();
    QSqlError lastError();
    bool createModelTable();
    int insertModel(QString firstname, QString lastname, int mask);
    bool getModel(int id, RecognitionModel*& model);

private:
        QSqlDatabase db;

    void recognize_face ( cv::Mat const& face, cv::Mat const& comp_face );
};
