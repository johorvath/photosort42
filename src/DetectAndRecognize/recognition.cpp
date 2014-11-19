#include "recognition.hpp"


facerecognizer::facerecognizer (std::string const& test, recognize_mode const& mode ):
    mode_( mode )
{
    model_path_ ="/home/johannes/tmp";
}

facerecognizer::~facerecognizer ()
{

}

void facerecognizer::update_model( recognize_mode const& mode, std::vector<cv::Mat>& images, std::vector<int>& label )
{
    switch ( mode )
    {
        case EIGEN:
        {
            model_eigen_->update( images, label );

            break;
        }
        case FISHER:
        {
            model_fisher_->update( images, label );
            break;
        }
        case LBP:
        {
            model_lbp_->update( images, label );
            break;
        }
        default:
        {
            break;
        }
    }
}

void facerecognizer::scale_mats ( std::vector < cv::Mat >& imgs )
{
    for ( unsigned int i = 0; i < imgs.size(); ++i )
    {
        cv::Size img_size = imgs[i].size();
        xsum_ += img_size.width;
        ysum_ += img_size.height;
        if ( img_size.height > ymax_ )
        {
            ymax_ = img_size.height;
        }
        if ( img_size.height < ymin_ )
        {
            ymin_ = img_size.height;
        }
        if ( img_size.width > xmax_ )
        {
            xmax_ = img_size.width;
        }
        if ( img_size.width < xmin_ )
        {
            xmin_ = img_size.width;
        }
        std::cout << " " << std::endl;
    }
    xresize_ = xsum_ / imgs.size();
    yresize_ = ysum_ / imgs.size();
//    xresize_ = (xmax_ + xmin_) / 2;
//    yresize_ = (ymax_ + ymin_) / 2;
    cv::Size size( xresize_, yresize_ );

    for ( unsigned int i = 0; i < imgs.size(); ++i )
    {
        cv::resize( imgs[i], imgs[i], size );
        std::cout << " " << std::endl;
    }

}

void facerecognizer::load_model ( recognize_mode const& mode, cv::Ptr <cv::FaceRecognizer>& model )
{
    boost::filesystem::path path ( model_path_ );
    switch ( mode )
    {
        case EIGEN:
        {
            model->load( (path / "model_eigen.yml").string );

            break;
        }
        case FISHER:
        {
            model->load( (path / "model_fisher.yml").string );
            break;
        }
        case LBP:
        {
            model->load( (path / "model_lbp.yml").string );
            break;
        }
        default:
        {
            break;
        }
    }

}

void facerecognizer::train_model ( recognize_mode const& mode, std::vector < cv::Mat > const& images, std::vector < int > const& label )
{
    boost::filesystem::path path ( model_path_ );
    switch ( mode )
    {
        case EIGEN:
        {
            model->train( (path / "model_eigen.yml").string );

            break;
        }
        case FISHER:
        {
            model->train( (path / "model_fisher.yml").string );
            break;
        }
        case LBP:
        {
            model->train( (path / "model_lbp.yml").string );
            break;
        }
        default:
        {
            break;
        }
    }

}




static cv::Mat norm_0_255( cv::InputArray _src) {
    cv::Mat src = _src.getMat();
    // Create and return normalized image:
    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}








void facerecognizer::recognize ( std::vector <cv::Mat>& images, std::vector <int>& labels)
{
    std::string fn_csv = "/home/johannes/Documents/test.csv";
    try {
            helper::read_csv( fn_csv, images, labels );
        } catch (cv::Exception& e) {
            std::cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << std::endl;
            // nothing more we can do
            exit(1);
        }

    scale_mats( images );


    cv::Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();

    cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer( 10 );

    model->train(images, labels);
    model->save( "test.yml" );
    int predictedLabel = model->predict(testSample);


    std::cout << predictedLabel << " " << testLabel << std::endl;
}
    //int height = images[0].rows;
    /* ONLY BLABLA
    // Quit if there are not enough images for this demo.
    if(images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size:
    int height = images[0].rows;
    // The following lines simply get the last images from
    // your dataset and remove it from the vector. This is
    // done, so that the training data (which we learn the
    // cv::FaceRecognizer on) and the test data we test
    // the model with, do not overlap.
    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();
    // The following lines create an Fisherfaces model for
    // face recognition and train it with the images and
    // labels read from the given CSV file.
    // If you just want to keep 10 Fisherfaces, then call
    // the factory method like this:
    //
    //      cv::createFisherFaceRecognizer(10);
    //
    // However it is not useful to discard Fisherfaces! Please
    // always try to use _all_ available Fisherfaces for
    // classification.
    //
    // If you want to create a FaceRecognizer with a
    // confidence threshold (e.g. 123.0) and use _all_
    // Fisherfaces, then call it with:
    //
    //      cv::createFisherFaceRecognizer(0, 123.0);
    //
    */

//    cv::Ptr<cv::FaceRecognizer> model = cv::createFisherFaceRecognizer();
//    model->train(images, labels);
//    // The following line predicts the label of a given
//    // test image:
//    int predictedLabel = model->predict(testSample);
//    //
//    // To get the confidence of a prediction call the model with:
//    //
//    //      int predictedLabel = -1;
//    //      double confidence = 0.0;
//    //      model->predict(testSample, predictedLabel, confidence);
//    //
    /*
    std::string result_message = cv::format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    std::cout << result_message << std::endl;
    // Here is how to get the eigenvalues of this Eigenfaces model:
    cv::Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    cv::Mat W = model->getMat("eigenvectors");
    // Get the sample mean from the training data
    cv::Mat mean = model->getMat("mean");
    // Display or save:
    cv::imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
    // Display or save the first, at most 16 Fisherfaces:
    for (int i = 0; i < std::min(16, W.cols); i++) {
        std::string msg = cv::format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        std::cout << msg << std::endl;
        // get eigenvector #i
        cv::Mat ev = W.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        cv::Mat grayscale = norm_0_255(ev.reshape(1, height));
        // Show the image & apply a Bone colormap for better sensing.
        cv::Mat cgrayscale;
        cv::applyColorMap(grayscale, cgrayscale, cv::COLORMAP_BONE);
        // Display or save:
        cv::imshow(cv::format("fisherface_%d", i), cgrayscale);
    }
    // Display or save the image reconstruction at some predefined steps:
    for(int num_component = 0; num_component < std::min(16, W.cols); num_component++) {
        // Slice the Fisherface from the model:
        cv::Mat ev = W.col(num_component);
        cv::Mat projection = subspaceProject(ev, mean, images[0].reshape(1,1));
        cv::Mat reconstruction = subspaceReconstruct(ev, mean, projection);
        // Normalize the result:
        reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
        // Display or save:
        cv::imshow(cv::format("fisherface_reconstruction_%d", num_component), reconstruction);
       }
    cv::imshow( "end", testSample );
    // Display if we are not writing to an output folder:

}
*/
/*
void facerecognizer::train_face( const cv::Mat &face, const cv::Mat &comp_face, recognize_mode const& mode )
{
    switch ( mode )
    {
        case EIGEN:
        {
            cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
            model->train(images, labels);

            break;
        }
        case FISHER:
        {
            cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
            model->train(images, labels);
            break;
        }
        case LBP:
        {
            cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
            model->train(images, labels);
            break;
        }
        default:
        {
            break;
        }
    }

}
*/
