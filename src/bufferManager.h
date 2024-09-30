#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "pageHandler.h"
#include "file.h"

/**
 * @brief Buffer Manager class
 * Buffer Manager class is a class that manages the buffer pool
 * It has a block handler class
 * It communicate with disk to read and write data
 */
class BufferManager
{
    private:
        PageHandler *pageHandler;
        File *file;

    public:
        BufferManager(std::string path)
            : file(new File(path)), pageHandler(new PageHandler())
        {}
        ~BufferManager()
        {
            delete file;
            delete pageHandler;
        }
        Page *GetPageFromDisk(unsigned int pageIdx);
        Page *GetPageFromBufferPool(std::string fileName,unsigned int pageIdx);
        PageHandler *GetPageHandler() { return pageHandler; }
        void WriteBlock(Page *page);
        
        /**
         * @brief 페이지가 버퍼 풀에 다 찼을 때 last Page evicton 실시
         */
        void ReplacePage(Page *page);
        void FlushPageToDisk(PageDirectory dir, Page &page);
};

#endif