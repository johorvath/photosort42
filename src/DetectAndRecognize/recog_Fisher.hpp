#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"


class facerecognizer_fisher
{
public:
    facerecognizer_fisher( std::string const& test );
    ~facerecognizer_fisher();

    void recognize_face ( cv::Mat const& face, cv::Mat const& comp_face );
};
