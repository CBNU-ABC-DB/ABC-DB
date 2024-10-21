#ifndef ABCDB_FILE_HANDLER_H_
#define ABCDB_FILE_HANDLER_H_

#include "file.h"

#include <string>

class File;
class PageDirectory;
class Page;

class FileHandler{
    private:
        File* file_;

    public:
        FileHandler(const std::string& file_name) : file_(new File(file_name)) {}
        ~FileHandler() {delete file_;}

        void WritePage(PageDirectory& dir, const Page& page);
        void WritePageDir(const PageDirectory& dir);
};

#endif