#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"

class facedetector
{
public:
    facedetector();
    ~facedetector();

    void detect_face (cv::Mat const& img);


private:
    std::string face_cascade_name_;
    std::string eyes_cascade_name_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eyes_cascade_;
    
};
