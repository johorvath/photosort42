#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"
#include <boost/filesystem.hpp>

#include "src/helper.hpp"

enum recognize_mode {
    EIGEN,
    FISHER,
    LBP
};

class facerecognizer
{
public:
    facerecognizer( std::string const& test );
    ~facerecognizer();

    void update_model ( recognize_mode const& mode, std::vector <cv::Mat>& face, std::vector <int>& label );

    int recognize_face ( recognize_mode const& mode, cv::Mat const& comp_face );

    void recognize_test ();

    void recognize ( std::vector <cv::Mat>& images, std::vector <int>& labels);

    void train_model ( recognize_mode const& mode, std::vector < cv::Mat > const& images, std::vector < int > const& label );

private:
    void load_model (recognize_mode const& mode, std::string const& dir );
    void scale_mats ( std::vector < cv::Mat >& imgs );
    std::string model_path_;
    cv::Ptr <cv::FaceRecognizer> model_eigen_;
    cv::Ptr <cv::FaceRecognizer> model_fisher_;
    cv::Ptr <cv::FaceRecognizer> model_lbp_;
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
