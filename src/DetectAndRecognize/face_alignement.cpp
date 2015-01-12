#include "face_alignement.hpp"
#include "libs/flandmark_detector.h"
#include "libs/linreg.h"

/*
face_alignement::face_alignement ( std::string const& test )
{
}

face_alignement::~face_alignement ()
{

}
*/
/*
 * Copyright (c) 2013. Alberto Fernandez Villan <alberto[dot]fernandez[at]fundacionctic[dot]org>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */

#include "libs/flandmark_detector.h"
#include "libs/linreg.h"
#include <opencv2/opencv.hpp>

//------------------------------------------------------------------------------
// Record the execution time of some code, in milliseconds. By Shervin Emami, May 4th 2011.
// eg:
//	DECLARE_TIMING(myTimer);
//	START_TIMING(myTimer);
//	  printf("A slow calc = %f\n", 1.0/sqrt(2.0) );
//	STOP_TIMING(myTimer);
//	SHOW_TIMING(myTimer, "My Timer");
//------------------------------------------------------------------------------
//#define DECLARE_TIMING(s)	int64 timeStart_##s; int64 timeDiff_##s; int64 timeTally_##s = 0; int64 countTally_##s = 0
//#define START_TIMING(s)		timeStart_##s = cvGetTickCount()
//#define STOP_TIMING(s)		timeDiff_##s = (cvGetTickCount() - timeStart_##s); timeTally_##s += timeDiff_##s; countTally_##s++
//#define GET_TIMING(s)		(double)(0.001 * ( (double)timeDiff_##s / (double)cvGetTickFrequency() ))
//#define GET_AVERAGE_TIMING(s)	(double)(countTally_##s ? 0.001 * ( (double)timeTally_##s / ((double)countTally_##s * cvGetTickFrequency()) ) : 0)
//#define GET_TIMING_COUNT(s)	(int)(countTally_##s)
//#define CLEAR_AVERAGE_TIMING(s)	timeTally_##s = 0; countTally_##s = 0
//#define SHOW_TIMING(s, msg)	printf("%s time: \t %dms \t (%dms average across %d runs).\n", msg, cvRound(GET_TIMING(s)), cvRound(GET_AVERAGE_TIMING(s)), GET_TIMING_COUNT(s) )

/** enum with all landmarks (0-7)--> flandmarks and 8,9 added */
enum landmark_pos {
    FACE_CENTER = 0,
    LEFT_EYE_INNER = 1,
    RIGHT_EYE_INNER = 2,
    MOUTH_LEFT = 3,
    MOUTH_RIGHT = 4,
    LEFT_EYE_OUTER = 5,
    RIGHT_EYE_OUTER = 6,
    NOSE_CENTER = 7,
    LEFT_EYE_ALIGN = 8,
    RIGHT_EYE_ALIGN = 9
};

face_alignement::face_alignement( std::string& flandmark_path ):
    flandmark_path_ ( flandmark_path )
{

}

/** detect biggest face */
cv::Rect face_alignement::detect_face(const cv::Mat& image, cv::CascadeClassifier& cascade){
    const double scale_factor = 1.1;
    const int min_neighbours = 2;
    const int flags = CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_SCALE_IMAGE;
    const Size min_size = Size(48, 48);
    const Size max_size = Size();

    if(cascade.empty() || image.empty()){
        return Rect();
    }

    vector<Rect> faces;
    cascade.detectMultiScale(image, faces, scale_factor, min_neighbours, flags, min_size, max_size);

    if (faces.empty())
        return Rect();

    return faces.front();
}

/** rotate points based on rot_mat */
void face_alignement::get_rotated_points(const std::vector<cv::Point2d> &points, std::vector<cv::Point2d> &dst_points, const cv::Mat &rot_mat){

    for(int i=0; i<points.size(); i++){

        Mat point_original(3,1,CV_64FC1);
        point_original.at<double>(0,0) = points[i].x;
        point_original.at<double>(1,0) = points[i].y;
        point_original.at<double>(2,0) = 1;

        Mat result(2,1,CV_64FC1);

        gemm(rot_mat,point_original, 1.0, cv::Mat(), 0.0, result);

        Point point_result(cvRound(result.at<double>(0,0)),cvRound(result.at<double>(1,0)));

        dst_points.push_back(point_result);
    }
}

/** show landmarks in an image */
void face_alignement::show_landmarks(const std::vector<cv::Point2d> &landmarks, const cv::Mat& image, const string &named_window){

    Mat result;
    cvtColor( image, result, CV_GRAY2BGR );

    for(int i=0; i<landmarks.size()-2; i++){
        circle(result,landmarks[i], 1,Scalar(255,0,0),1);
    }

    circle(result,landmarks[LEFT_EYE_ALIGN], 1,Scalar(255,255,0),1);
    circle(result,landmarks[RIGHT_EYE_ALIGN], 1,Scalar(255,255,0),1);

    imshow(named_window,result);
    imwrite("aligned_landmarks.png",result);
}


/** aligns the face based on the recalculated positions of the eyes and aligns also the landmarks*/
double face_alignement::align(cv::Mat& image, cv::Mat& dst_image,
                              std::vector<cv::Point2d> &landmarks,
                              std::vector<cv::Point2d> &dst_landmarks){
    const double DESIRED_LEFT_EYE_X = 0.27;     // Controls how much of the face is visible after preprocessing.
    const double DESIRED_LEFT_EYE_Y = 0.4;

    // Use square faces.
    int desiredFaceWidth = 144;
    int desiredFaceHeight = desiredFaceWidth;

    Point2d leftEye = landmarks[LEFT_EYE_ALIGN];
    Point2d rightEye = landmarks[RIGHT_EYE_ALIGN];

    // Get the center between the 2 eyes.
    Point2f eyesCenter = Point2f( (leftEye.x + rightEye.x) * 0.5f, (leftEye.y + rightEye.y) * 0.5f );
    // Get the angle between the 2 eyes.
    double dy = (rightEye.y - leftEye.y);
    double dx = (rightEye.x - leftEye.x);
    double len = sqrt(dx*dx + dy*dy);
    double angle = atan2(dy, dx) * 180.0/CV_PI; // Convert from radians to degrees.

    // Hand measurements shown that the left eye center should ideally be at roughly (0.19, 0.14) of a scaled face image.
    const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X);
    // Get the amount we need to scale the image to be the desired fixed size we want.
    double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X) * desiredFaceWidth;
    double scale = desiredLen / len;
    // Get the transformation matrix for rotating and scaling the face to the desired angle & size.
    Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);
    // Shift the center of the eyes to be the desired center between the eyes.
    rot_mat.at<double>(0, 2) += desiredFaceWidth * 0.5f - eyesCenter.x;
    rot_mat.at<double>(1, 2) += desiredFaceHeight * DESIRED_LEFT_EYE_Y - eyesCenter.y;

    // Rotate and scale and translate the image to the desired angle & size & position!
    // Note that we use 'w' for the height instead of 'h', because the input face has 1:1 aspect ratio.
    dst_image = Mat(desiredFaceHeight, desiredFaceWidth, CV_8U, Scalar(128)); // Clear the output image to a default grey.
    warpAffine(image, dst_image, rot_mat, dst_image.size());

    //rotate landmarks
    get_rotated_points(landmarks,dst_landmarks, rot_mat);

    return angle;
}

/** detects landmarks using flandmakrs and add two more landmakrs to be used to alignt the face*/
vector<cv::Point2d> face_alignement::detectLandmarks(FLANDMARK_Model* model, const Mat & image, const Rect & face){

    vector<Point2d> landmarks;

    // 1. get landmarks (using flandmarks)
    int bbox[4] = { face.x, face.y, face.x + face.width, face.y + face.height };
    double *points = new double[2 * model->data.options.M];

    if(flandmark_detect(new IplImage(image), bbox, model,points) > 0){
        return landmarks;
    }

    for (int i = 0; i < model->data.options.M; i++) {
        landmarks.push_back(Point2f(points[2 * i], points[2 * i + 1]));
    }

    // 2. get a linar regresion using the four points of the eyes
    LinearRegression lr;
    lr.addPoint(Point2D(landmarks[LEFT_EYE_OUTER].x,landmarks[LEFT_EYE_OUTER].y));
    lr.addPoint(Point2D(landmarks[LEFT_EYE_INNER].x,landmarks[LEFT_EYE_INNER].y));
    lr.addPoint(Point2D(landmarks[RIGHT_EYE_INNER].x,landmarks[RIGHT_EYE_INNER].y));
    lr.addPoint(Point2D(landmarks[RIGHT_EYE_OUTER].x,landmarks[RIGHT_EYE_OUTER].y));

    double coef_determination = lr.getCoefDeterm();
    double coef_correlation = lr.getCoefCorrel();
    double standar_error_estimate = lr.getStdErrorEst();

    double a = lr.getA();
    double b = lr.getB();

    // 3. get two more landmarks based on this linear regresion to be used to align the face
    cv::Point pp1(landmarks[LEFT_EYE_OUTER].x, landmarks[LEFT_EYE_OUTER].x*b+a);
    cv::Point pp2(landmarks[RIGHT_EYE_OUTER].x, landmarks[RIGHT_EYE_OUTER].x*b+a);

    landmarks.push_back(pp1);
    landmarks.push_back(pp2);

    delete[] points;
    points = 0;
    return landmarks;
}

void face_alignement::align_face( cv::Mat& face )
{
    string flandmarks_model_name = "/home/johannes/work/photosort42/src/libs/flandmark_model.dat";
    string fn_haar = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";

    Mat gray_image;

    if ( face.channels() > 1 )
    {
        cv::cvtColor(face, gray_image, CV_BGR2GRAY);
    }
    else
        gray_image = face.clone();

    FLANDMARK_Model* model = flandmark_init(flandmarks_model_name.c_str());

    vector<cv::Point2d> aligned_landmarks;

    CascadeClassifier face_cascade;
    face_cascade.load(fn_haar);

    if(face_cascade.empty()){
        cout<<"error loading face_cascade. Check path"<<endl;
        exit(1);
    }

    // Find face in image:
    Rect r = detect_face(face, face_cascade);

    Mat aligned_image;

    vector<cv::Point2d> landmarks = detectLandmarks(model, gray_image, Rect(r.x,r.y,r.width,r.height));

    if(landmarks.size() == 0){
        cout<<"landmarks not found on the face"<<endl;
//        exit(1);
    }
    else
    {

        std::cout << "landmarks found" << std::endl;

        //align the face
        align(gray_image,aligned_image,landmarks,aligned_landmarks);

    //    STOP_TIMING(alignTimer);
    //    SHOW_TIMING(alignTimer, "Alignment of image and landmark points");

        /*
        if(!aligned_image.empty()){
            show_landmarks(aligned_landmarks,aligned_image,"aligned landmarks");
        }
        */
        face = aligned_image.clone();

    }

}

/*
void face_alignement::test ()
{

    Mat image = imread("/home/johannes/Pictures/mueller.jpg");
    string fn_haar = "/home/johannes/work/photosort42/src/libs/haarcascade_frontalface_alt.xml";
    string flandmarks_model_name = "/home/johannes/work/photosort42/src/libs/flandmark_model.dat";

    if(image.empty()){
        cout<<"error loading image. Check path"<<endl;
        exit(1);
    }
    imshow("original",image);

    // Get grayscale image
    Mat gray_image;
    cvtColor(image, gray_image, CV_BGR2GRAY);

    // Get cascade for face detection
    CascadeClassifier face_cascade;
    face_cascade.load(fn_haar);

    if(face_cascade.empty()){
        cout<<"error loading face_cascade. Check path"<<endl;
        exit(1);
    }

    // Initialize flandmakrs with the model
    FLANDMARK_Model* model = flandmark_init(flandmarks_model_name.c_str());

    // Find face in image:
    Rect r = detect_face(image,face_cascade);

    if(r == Rect()){
        cout<<"No faces found on image. Try with another"<<endl;
        exit(1);
    }

    Mat aligned_image;
    vector<cv::Point2d> aligned_landmarks;

    cv::imshow( "landmarks", gray_image );
    waitKey(0);
    vector<cv::Point2d> landmarks = detectLandmarks(model, gray_image, Rect(r.x,r.y,r.width,r.height));

//    DECLARE_TIMING(alignTimer);
//    START_TIMING(alignTimer);

    // Detect landmarks


    if(landmarks.size() == 0){
        cout<<"landmarks not found on the face"<<endl;
        exit(1);
    }

    //align the face
    align(gray_image,aligned_image,landmarks,aligned_landmarks);

//    STOP_TIMING(alignTimer);
//    SHOW_TIMING(alignTimer, "Alignment of image and landmark points");

    if(!aligned_image.empty()){
        show_landmarks(aligned_landmarks,aligned_image,"aligned landmarks");
    }

//    waitKey(0);

}
*/


/*
enum landmark_pos {
    FACE_CENTER = 0,
    LEFT_EYE_INNER = 1,
    RIGHT_EYE_INNER = 2,
    MOUTH_LEFT = 3,
    MOUTH_RIGHT = 4,
    LEFT_EYE_OUTER = 5,
    RIGHT_EYE_OUTER = 6,
    NOSE_CENTER = 7,
    LEFT_EYE_ALIGN = 8, //pp1
    RIGHT_EYE_ALIGN = 9 //pp2
};

void show_landmarks(const std::vector<cv::Point2d> &landmarks, const cv::Mat& image, const std::string &named_window){
    cv::Mat copy_image;
    image.copyTo(copy_image);

    for(int i=0; i<landmarks.size()-2; i++){
        circle(copy_image,landmarks[i], 1,cv::Scalar(255,0,0),2);
    }

    circle(copy_image,landmarks[LEFT_EYE_ALIGN], 1,cv::Scalar(255,255,0),2);
    circle(copy_image,landmarks[RIGHT_EYE_ALIGN], 1,cv::Scalar(255,255,0),2);
}

std::vector<cv::Point2d> detectLandmarks(const cv::Mat & image, const cv::Rect & face){

    std::vector<cv::Point2d> landmarks;

    int bbox[4] = { face.x, face.y, face.x + face.width, face.y + face.height };
    double *points = new double[2 * this->model->data.options.M];

        //http://cmp.felk.cvut.cz/~uricamic/flandmark/
    if(flandmark_detect(new IplImage(image), bbox, this->model,points) > 0){
        return landmarks;
    }

    for (int i = 0; i < this->model->data.options.M; i++) {
        landmarks.push_back(Point2f(points[2 * i], points[2 * i + 1]));
    }

    std::LinearRegression lr;
    lr.addPoint(Point2D(landmarks[LEFT_EYE_OUTER].x,landmarks[LEFT_EYE_OUTER].y));
    lr.addPoint(Point2D(landmarks[LEFT_EYE_INNER].x,landmarks[LEFT_EYE_INNER].y));
    lr.addPoint(Point2D(landmarks[RIGHT_EYE_INNER].x,landmarks[RIGHT_EYE_INNER].y));
    lr.addPoint(Point2D(landmarks[RIGHT_EYE_OUTER].x,landmarks[RIGHT_EYE_OUTER].y));

    double coef_determination = lr.getCoefDeterm();
    double coef_correlation = lr.getCoefCorrel();
    double standar_error_estimate = lr.getStdErrorEst();

    double a = lr.getA();
    double b = lr.getB();

    cv::Point pp1(landmarks[LEFT_EYE_OUTER].x, landmarks[LEFT_EYE_OUTER].x*b+a);
    cv::Point pp2(landmarks[RIGHT_EYE_OUTER].x, landmarks[RIGHT_EYE_OUTER].x*b+a);

    landmarks.push_back(pp1); //landmarks[LEFT_EYE_ALIGN]
    landmarks.push_back(pp2); //landmarks[RIGHT_EYE_ALIGN]

    delete[] points;
    points = 0;
    return landmarks;
}

double align(const Mat &image, Mat &dst_image, std::vector<Point2d> &landmarks, std::vector<Point2d> &dst_landmarks){
    const double DESIRED_LEFT_EYE_X = 0.27;     // Controls how much of the face is visible after preprocessing.
    const double DESIRED_LEFT_EYE_Y = 0.4;

    int desiredFaceWidth = 144;
    int desiredFaceHeight = desiredFaceWidth;

    Point2d leftEye = landmarks[LEFT_EYE_ALIGN];
    Point2d rightEye = landmarks[RIGHT_EYE_ALIGN];

    // Get the center between the 2 eyes.
    Point2f eyesCenter = Point2f( (leftEye.x + rightEye.x) * 0.5f, (leftEye.y + rightEye.y) * 0.5f );
    // Get the angle between the 2 eyes.
    double dy = (rightEye.y - leftEye.y);
    double dx = (rightEye.x - leftEye.x);
    double len = sqrt(dx*dx + dy*dy);
    double angle = atan2(dy, dx) * 180.0/CV_PI; // Convert from radians to degrees.

    // Hand measurements shown that the left eye center should ideally be at roughly (0.19, 0.14) of a scaled face image.
    const double DESIRED_RIGHT_EYE_X = (1.0f - DESIRED_LEFT_EYE_X);
    // Get the amount we need to scale the image to be the desired fixed size we want.
    double desiredLen = (DESIRED_RIGHT_EYE_X - DESIRED_LEFT_EYE_X) * desiredFaceWidth;
    double scale = desiredLen / len;
    // Get the transformation matrix for rotating and scaling the face to the desired angle & size.
    Mat rot_mat = getRotationMatrix2D(eyesCenter, angle, scale);
    // Shift the center of the eyes to be the desired center between the eyes.
    rot_mat.at<double>(0, 2) += desiredFaceWidth * 0.5f - eyesCenter.x;
    rot_mat.at<double>(1, 2) += desiredFaceHeight * DESIRED_LEFT_EYE_Y - eyesCenter.y;

    // Rotate and scale and translate the image to the desired angle & size & position!
    // Note that we use 'w' for the height instead of 'h', because the input face has 1:1 aspect ratio.
    dst_image = Mat(desiredFaceHeight, desiredFaceWidth, CV_8U, Scalar(128)); // Clear the output image to a default grey.
    warpAffine(image, dst_image, rot_mat, dst_image.size());

    //don't forget to rotate landmarks also!!!
    get_rotated_points(landmarks,dst_landmarks, rot_mat);

    if(!dst_image.empty()){
        //show landmarks
        show_landmarks(dst_landmarks,dst_image,"rotate landmarks");
    }

    return angle;
}

void face_alignement::CropFace( cv::Mat& img )
{
    //Step 1: detect landmarks over the detected face
    std::vector<cv::Point2d> landmarks = landmark_detector->detectLandmarks(img_gray,Rect(r.x,r.y,r.width,r.height));
    //Step 2: align face
    Mat aligned_image;
    std::vector<cv::Point2d> aligned_landmarks;
    aligner->align(img_gray,aligned_image,landmarks,aligned_landmarks);
    //Step 3: normalize region
    Mat normalized_region = normalizer->process(aligned_image,Rect(r.x,r.y,r.width,r.height),aligned_landmarks);
    //Step 4: tan&&triggs
    normalized_region = ((FlandMarkFaceAlign *)normalizer)->tan_triggs_preprocessing(normalized_region, gamma_correct,dog,contrast_eq);
.


}



void face_alignement::ScaleRotateTranslate( cv::Mat& img )
{

}

*/
