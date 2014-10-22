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
}

