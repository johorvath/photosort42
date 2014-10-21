#ifndef HELPER_HPP
#define HELPER_HPP

#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include "easylogging++.h"


namespace helper {

  enum data_type {
    existing_file,
    empty_dir,
    existing_dir,
    nexisting_dir
  };


  data_type check_input ( std::string const& input );
  bool check_is_file ( std::string const& filename );
  bool check_is_dir_empty ( std::string const& path );

}

#endif // HELPER_HPP



