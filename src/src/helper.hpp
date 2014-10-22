#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "libs/easylogging++.h"


namespace helper {

  bool check_file( std::string const& filename );
  bool check_dir ( std::string const& path );

}

#endif // HELPER_HPP



