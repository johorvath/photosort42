#include "src/helper.hpp"

namespace helper {

  bool check_file( std::string const& filename )
  {
    const boost::filesystem::path dir ( filename );
    const bool is_file = boost::filesystem::is_regular_file( dir );
    if ( is_file )
    {
      LOG(INFO) << dir.filename() << "File exists!";
    }
    else
    {
      LOG(ERROR) << dir.filename() << "doesn't exists!";
    }
    return is_file;
  }

  bool check_is_dir( std::string const& path )
  {
    const boost::filesystem::path dir ( path );
    bool status_dir;
    if ( boost::filesystem::is_directory( dir ) )
    {
      if ( dir.empty() )
      {
        status_dir = true;
        LOG(INFO) << "Directory is empty!";
      }
      else
      {
        status_dir = false;
        LOG(WARNING) << "Directory already exists with files!";
      }
    }
    else
    {
      status_dir = true;
      LOG(WARNING) << "Directory will be created!";
      boost::filesystem::create_directory( dir );
    }
    return status_dir;
  }

  void resize_face ( std::vector < cv::Mat > & face )
  {

  }

  void read_csv( std::string const& filename, std::vector<cv::Mat>& images, std::vector<int>& labels ) {
      char separator = ';';
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


}

