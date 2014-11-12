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
    facerecognizer( std::string const& test );
    ~facerecognizer();

    void update_model ( cv::Mat& face, int label );

    void recognize_face ( cv::Mat const& face, cv::Mat const& comp_face, recognize_mode const& mode );

    void train_model ( std::vector < cv::Mat > const& images, std::vector < int > const& label );

private:
    void load_model ( recognize_mode const& mode,, cv::Ptr <cv::FaceRecognizer>& model );
    std::string model_path_;
    cv::Ptr <cv::FaceRecognizer> model_;


};
