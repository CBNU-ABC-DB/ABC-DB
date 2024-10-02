#include "file_handler.h"

void FileHandler::WritePage(PageDirectory& dir, const Page& page){
    file_->WritePageToFile(dir, page);
};
void FileHandler::WritePageDir(const PageDirectory& dir){
    file_->WritePageDirToFile(dir);
};