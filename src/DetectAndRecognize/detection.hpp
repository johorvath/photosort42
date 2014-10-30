#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"


class facedetector
{
public:
    facedetector( std::string const& test );
    ~facedetector();

    void detect_face (cv::Mat &img, std::vector<cv::Mat> &faces_mat);


private:
    std::string face_cascade_name_;
    std::string eyes_cascade_name_;
    std::string eyes_glass_cascade_name_;
    std::string left_eye_cascade_name_;
    std::string right_eye_cascade_name_;
    std::string profile_face_cascade_name_;
    std::string face1_cascade_name_;
    std::string face2_cascade_name_;
    std::string face3_cascade_name_;
    std::string face4_cascade_name_;
    cv::CascadeClassifier eyes_glass_cascade_;
    cv::CascadeClassifier face_cascade_;
    cv::CascadeClassifier eyes_cascade_;
    cv::CascadeClassifier left_eye_cascade_;
    cv::CascadeClassifier right_eye_cascade_;
    cv::CascadeClassifier profile_face_cascade_;
    cv::CascadeClassifier face1_cascade_;
    cv::CascadeClassifier face2_cascade_;
    cv::CascadeClassifier face3_cascade_;
    cv::CascadeClassifier face4_cascade_;


    void detectAndDisplay ( cv::Mat const& img );

};
