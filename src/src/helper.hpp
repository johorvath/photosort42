#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "libs/easylogging++.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace helper {

  bool check_file( std::string const& filename );
  bool check_dir ( std::string const& path );
  void resize_face ( cv::Mat& face );
  void read_csv( std::string const& filename, std::vector<cv::Mat>& images, std::vector<int>& labels);

}

#endif // HELPER_HPP



