#include "detection.hpp"


facedetector::facedetector ():
    face_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" ),
    eyes_glass_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ),
    eyes_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml" )

{
    if( !face_cascade_.load( face_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading face cascade";
    }
    if( !eyes_cascade_.load( eyes_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
    if( !eyes_glass_cascade_.load( eyes_glass_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
}

facedetector::~facedetector ()
{

}

void facedetector::detect_face( cv::Mat& img, std::vector < cv::Mat >& faces_mat )
{
    std::vector < cv::Rect > faces;
    std::vector < cv::Rect > faces_tmp;
    std::vector<cv::Rect> eyes;
    std::vector<cv::Rect> eyes_glass;

    const cv::Size min_size = cv::Size(48, 48);
    const cv::Size max_size = cv::Size();


    cv::Mat frame_gray;
    cv::imshow( "window_name", img );

    cv::cvtColor( img, frame_gray, cv::COLOR_BGR2GRAY );
    cv::equalizeHist( frame_gray, frame_gray );


    //-- Detect faces
    face_cascade_.detectMultiScale( frame_gray, faces_tmp, 1.5, 2, 0|cv::CASCADE_SCALE_IMAGE, min_size, max_size );

    for ( size_t i = 0; i < faces_tmp.size(); i++ )
    {
        cv::Point center( faces_tmp[i].x + faces_tmp[i].width/2, faces_tmp[i].y + faces_tmp[i].height/2 );
//        cv::ellipse( img, center, cv::Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0 );

        cv::Mat faceROI = frame_gray( faces_tmp[i] );
        //-- In each face, detect eyes
        eyes_cascade_.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |cv::CASCADE_SCALE_IMAGE, cv::Size(15, 15) );
        if ( eyes. size() >= 1 )
        {
            faces.push_back( faces_tmp[i] );
        }
        eyes_glass_cascade_.detectMultiScale( faceROI, eyes_glass, 1.1, 2, 0 |cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );
        if ( eyes_glass.size() >= 1 )
        {
            faces.push_back( faces_tmp[i] );
        }
    }

    for ( unsigned int i = 0; i < faces.size(); ++i )
    {
        cv::Mat ROI = img( faces[i] );
        faces_mat.push_back( ROI );
        cv::Rect face = faces[i];
        cv::rectangle( img, face.tl(), face.br() , cv::Scalar(0, 255, 0), 2, 8, 0 );
    }

}


