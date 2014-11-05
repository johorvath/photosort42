#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"


class facerecognizer_eigen
{
public:
    facerecognizer_eigen( std::string const& test );
    ~facerecognizer_eigen();

    void recognize_face ( std::vector < cv::Mat >& faces, cv::Mat const& comp_face );
    void test();

private:
    void scale_mats ( std::vector < cv::Mat >& imgs );

    int xmin_;
    int xmax_;
    int ymin_;
    int ymax_;
    int xresize_;
    int yresize_;

};
