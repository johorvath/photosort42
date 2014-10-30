#include "detection.hpp"


facedetector::facedetector ( std::string const& test ):
    face_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml" ),
    eyes_glass_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ),
    eyes_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml" ),
    left_eye_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_lefteye_2split.xml" ),
    right_eye_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascade_righteye_2splits.xml" ),
    face1_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascadefrontalface_alt2.xml"),
    face2_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascadefrontalface_alt_tree.xml"),
    face3_cascade_name_ ( "/usr/local/share/OpenCV/haarcascades/haarcascadefrontalface_default.xml")
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
    if( !face1_cascade_.load( face1_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
    if( !face2_cascade_.load( face2_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
    if( !face3_cascade_.load( face3_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
    if( !left_eye_cascade_.load( left_eye_cascade_name_ ) )
    {
        LOG(ERROR) << "Error loading eye cascade";
    }
    if( !right_eye_cascade_.load( right_eye_cascade_name_ ) )
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

    cv::Mat frame_gray;
    cv::imshow( "window_name", img );

    cv::cvtColor( img, frame_gray, cv::COLOR_BGR2GRAY );
    cv::imshow( "window_name", frame_gray );
    cv::equalizeHist( frame_gray, frame_gray );
    cv::imshow( "window_name", frame_gray );

    //-- Detect faces
    face_cascade_.detectMultiScale( frame_gray, faces_tmp, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

    for ( size_t i = 0; i < faces_tmp.size(); i++ )
    {
        cv::Point center( faces_tmp[i].x + faces_tmp[i].width/2, faces_tmp[i].y + faces_tmp[i].height/2 );
//        cv::ellipse( img, center, cv::Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0 );

        cv::Mat faceROI = frame_gray( faces_tmp[i] );



        //-- In each face, detect eyes
        left_eye_cascade_.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |cv::CASCADE_SCALE_IMAGE, cv::Size(15, 15) );
        if ( eyes. size() >= 1 )
        {
            faces.push_back( faces_tmp[i] );
        }

        /*
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            cv::Point eye_center( faces_tmp[i].x + eyes[j].x + eyes[j].width/2, faces_tmp[i].y + eyes[j].y + eyes[j].height/2 );
//            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//          cv::circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        */

        eyes_glass_cascade_.detectMultiScale( faceROI, eyes_glass, 1.1, 2, 0 |cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );
        if ( eyes_glass.size() >= 1 )
        {
            faces.push_back( faces_tmp[i] );
        }
        /*
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            cv::Point eye_center( faces_tmp[i].x + eyes_glass[j].x + eyes_glass[j].width/2, faces_tmp[i].y + eyes_glass[j].y + eyes_glass[j].height/2 );
//            int radius = cvRound( (eyes_glass[j].width + eyes_glass[j].height)*0.25 );
//          cv::circle( frame, eye_center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        */
        cv::Mat ROI = img( faces[i] );
        for ( unsigned int i = 0; i < eyes.size(); ++i )
        {
            cv::rectangle( ROI, eyes[i], cv::Scalar(255, 0, 255), 2, 8, 0 );
        }
        for ( unsigned int i = 0; i < eyes_glass.size(); ++i )
        {
            cv::rectangle( ROI, eyes_glass[i], cv::Scalar(255, 0, 0), 2, 8, 0 );
        }
        faces_mat.push_back( ROI );
//        cv::imshow( "roi", ROI );
//        cv::waitKey(0);
    }
    for ( unsigned int i = 0; i < faces.size(); ++i )
    {
        cv::Rect face = faces[i];
        cv::rectangle( img, face.tl(), face.br() , cv::Scalar(0, 255, 0), 2, 8, 0 );
    }

    cv::imshow( "window_name", img );
    cv::waitKey( 0 );



    std::cout << faces.size() << std::endl;
}




//using namespace std;
//using namespace cv;


///** Function Headers */
//void detectAndDisplay( Mat f""rame );

///** Global variables */
//String face_cascade_name = "haarcascade_frontalface_alt.xml";
//String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
//CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;
//String window_name = "Capture - Face detection";

///** @function main */
//int test( void )
//{
//    VideoCapture capture;
//    Mat frame;

//    //-- 1. Load the cascades
//    if( !face_cascade.load( face_cascade_name ) )
//    { printf("--(!)Error loading face cascade\n"); return -1; };
//    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading eyes cascade\n"); return -1; };

//    //-- 2. Read the video stream
//    capture.open( -1 );
//    if ( ! capture.isOpened() ) { printf("--(!)Error opening video capture\n"); return -1; }

//    while ( capture.read(frame) )
//    {
//        if( frame.empty() )
//        {
//            printf(" --(!) No captured frame -- Break!");
//            break;
//        }

//        //-- 3. Apply the classifier to the frame
//        detectAndDisplay( frame );

//        int c = waitKey(10);
//        if( (char)c == 27 ) { break; } // escape
//    }
//    return 0;
//}

///** @function detectAndDisplay */
void facedetector::detectAndDisplay( cv::Mat const& img )
{
    std::vector<cv::Rect> faces;
    cv::Mat frame_gray;

    cv::cvtColor( img, frame_gray, cv::COLOR_BGR2GRAY );
    cv::equalizeHist( frame_gray, frame_gray );

    //-- Detect faces
    face_cascade_.detectMultiScale( frame_gray, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

    for ( size_t i = 0; i < faces.size(); i++ )
    {
        cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//        cv::ellipse( img, center, cv::Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, cv::Scalar( 255, 0, 255 ), 4, 8, 0 );

        cv::Mat faceROI = frame_gray( faces[i] );
        std::vector<cv::Rect> eyes;

        //-- In each face, detect eyes
        eyes_cascade_.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            cv::Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
//          cv::circle( img, eye_center, radius, cv::Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
    }
    //-- Show what you got
    cv::imshow( "window_name", img );
}
