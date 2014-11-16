#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"
#include <boost/filesystem.hpp>


enum recognize_mode {
    EIGEN,
    FISHER,
    LBP
};

class facerecognizer
{
public:
    facerecognizer( std::string const& test, recognize_mode const& mode );
    ~facerecognizer();

    void update_model (  recognize_mode const& mode, cv::Mat& face, int label );

    void recognize_face ( cv::Mat const& face, cv::Mat const& comp_face, recognize_mode const& mode );

    void recognize ( std::vector <cv::Mat>& images, std::vector <int>& labels);

    void train_model ( recognize_mode const& mode, std::vector < cv::Mat > const& images, std::vector < int > const& label );

private:
    void load_model ( recognize_mode const& mode, cv::Ptr <cv::FaceRecognizer>& model );
    void scale_mats ( std::vector < cv::Mat >& imgs );
    std::string model_path_;
    cv::Ptr <cv::FaceRecognizer> model_;
    recognize_mode mode_;

    int xmin_;
    int xmax_;
    int ymin_;
    int ymax_;
    int xresize_;
    int yresize_;
    int xsum_;
    int ysum_;


};
