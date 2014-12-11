#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <vector>

#include "libs/flandmark_detector.h"
#include "libs/linreg.h"
#include "libs/easylogging++.h"


class face_alignement
{
public:
    face_alignement();
    ~face_alignement();

    cv::Rect detect_face ( cv::Mat const& image, cv::CascadeClassifier& cascade);

    void get_rotated_points ( std::vector<cv::Point2d> const& points, std::vector<cv::Point2d>& dst_points, cv::Mat const& rot_mat);

    void show_landmarks ( std::vector<cv::Point2d> const& landmarks, cv::Mat const& image, string const& named_window);

    double align( cv::Mat& image, cv::Mat& dst_image, std::vector<cv::Point2d>& landmarks, std::vector<cv::Point2d>& dst_landmarks);

    std::vector<cv::Point2d> detectLandmarks(FLANDMARK_Model* model, cv::Mat const& image, cv::Rect const& face);

    void CropFace ( cv::Mat& img );

    void test ();


private:
    void ScaleRotateTranslate ( cv::Mat& img );





};
