#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "libs/easylogging++.h"
#include <opencv2/core/core.hpp>


namespace helper {

  bool check_file( std::string const& filename );
  bool check_dir ( std::string const& path );
  void resize_face ( cv::Mat& face );

}

#endif // HELPER_HPP



