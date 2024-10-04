#include "file_handler.h"

/**
 * @brief caller file::WritePageToFile
 */
void FileHandler::WritePage(PageDirectory& dir, const Page& page){
    file_->WritePageToFile(dir, page);
};

/**
 * @brief caller file::WritePageDirToFile
 */
void FileHandler::WritePageDir(const PageDirectory& dir){
    file_->WritePageDirToFile(dir);
};

