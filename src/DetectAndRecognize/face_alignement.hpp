#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include "libs/easylogging++.h"


class face_alignement
{
public:
    face_alignement( std::string const& test );
    ~face_alignement();

    void CropFace ( cv::Mat img );


private:
    void ScaleRotateTranslate ( cv::Mat img, );





};
