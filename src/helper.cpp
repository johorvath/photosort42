#include "helper.hpp"

namespace helper {



  /**
   * @brief check_input
   * @param input
   * @return
   *
   * file handling
   * check different possibilities
   *
   */
  data_type check_input (const std::string& input )
  {
    const boost::filesystem::path dir ( input );
    data_type data;
    if ( boost::filesystem::is_regular_file( dir ) )
    {
      data = existing_file;
      LOG(INFO) << "File exists!";
    }
    else if ( boost::filesystem::is_directory( dir ) )
    {
      if ( dir.empty() )
      {
        data = empty_dir;
        LOG(INFO) << "Directory is empty!";
      }
      else
      {
        data = existing_dir;
        LOG(WARNING) << "Directory already exists with files!";
      }
    }
    else
    {
      LOG(WARNING) << "Directory will be created!";
      data = nexisting_dir;
      boost::filesystem::create_directory( dir );
    }
    return data;
  }
}

