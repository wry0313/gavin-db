#pragma once

#include <string>
#include "common/config.hpp"

namespace db {
class DiskManager {
   public:
  explicit DiskManager(const std::string &db_file); 
    void ShutDown();

   void WritePage(page_id_t page_id, const char *page_data);
   void ReadPage(page_id_t page_id, char *page_data);

};
}