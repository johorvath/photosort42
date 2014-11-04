#include "recog_Eigen.hpp"
#include <opencv2/contrib/contrib.hpp>
#include <iostream>


/**
 * @brief facerecognizer_eigen::facerecognizer_eigen
 * @param test
 */
facerecognizer_eigen::facerecognizer_eigen ( std::string const& test )
{
}

facerecognizer_eigen::~facerecognizer_eigen ()
{

}

void facerecognizer_eigen::recognize_face( std::vector < cv::Mat >& faces, cv::Mat const& comp_face )
{

    cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
    std::vector < int > labels;

    for ( unsigned int i = 0; i < faces.size(); ++i )
    {
        labels.push_back( i );
    }

    //training the faces
    model->train(faces, labels);

    int predictedLabel = model->predict(comp_face);
    LOG(INFO) << predictedLabel;



}

static void read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (!file) {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) {
        std::stringstream liness(line);
        std::getline(liness, path, separator);
        std::getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(cv::imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void facerecognizer_eigen::scale ( std::vector < cv::Mat > imgs )
{

}

void facerecognizer_eigen::test ()
{
    std::vector<cv::Mat> images;
    std::vector<int> labels;
    std::string fn_csv = "/home/johannes/Documents/test.csv";
    try {
            read_csv(fn_csv, images, labels);
        } catch (cv::Exception& e) {
            std::cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << std::endl;
            // nothing more we can do
            exit(1);
        }

    cv::Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();

    cv::Ptr<cv::FaceRecognizer> model = cv::createEigenFaceRecognizer();
    model->train(images, labels);
    int predictedLabel = model->predict(testSample);


    std::cout << predictedLabel << " " << testLabel << std::endl;
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
//    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
//    cout << result_message << endl;
//    // Here is how to get the eigenvalues of this Eigenfaces model:
//    Mat eigenvalues = model->getMat("eigenvalues");
//    // And we can do the same to display the Eigenvectors (read Eigenfaces):
//    Mat W = model->getMat("eigenvectors");
//    // Get the sample mean from the training data
//    Mat mean = model->getMat("mean");
//    // Display or save:
//    if(argc == 2) {
//        imshow("mean", norm_0_255(mean.reshape(1, images[0].rows)));
//    } else {
//        imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, images[0].rows)));
//    }
//    // Display or save the first, at most 16 Fisherfaces:
//    for (int i = 0; i < min(16, W.cols); i++) {
//        string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
//        cout << msg << endl;
//        // get eigenvector #i
//        Mat ev = W.col(i).clone();
//        // Reshape to original size & normalize to [0...255] for imshow.
//        Mat grayscale = norm_0_255(ev.reshape(1, height));
//        // Show the image & apply a Bone colormap for better sensing.
//        Mat cgrayscale;
//        applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
//        // Display or save:
//        if(argc == 2) {
//            imshow(format("fisherface_%d", i), cgrayscale);
//        } else {
//            imwrite(format("%s/fisherface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
//        }
//    }
//    // Display or save the image reconstruction at some predefined steps:
//    for(int num_component = 0; num_component < min(16, W.cols); num_component++) {
//        // Slice the Fisherface from the model:
//        Mat ev = W.col(num_component);
//        Mat projection = subspaceProject(ev, mean, images[0].reshape(1,1));
//        Mat reconstruction = subspaceReconstruct(ev, mean, projection);
//        // Normalize the result:
//        reconstruction = norm_0_255(reconstruction.reshape(1, images[0].rows));
//        // Display or save:
//        if(argc == 2) {
//            imshow(format("fisherface_reconstruction_%d", num_component), reconstruction);
//        } else {
//            imwrite(format("%s/fisherface_reconstruction_%d.png", output_folder.c_str(), num_component), reconstruction);
//        }
//    }
//    // Display if we are not writing to an output folder:
//    if(argc == 2) {
//        waitKey(0);
//    }
//    return 0;
}
