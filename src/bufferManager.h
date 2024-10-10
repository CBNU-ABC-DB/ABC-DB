#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "bufferPool.h"
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
        // PageHandler *pageHandler;
        BufferPool *bufferPool;
        File *file;
    public:
        BufferManager(std::string path)
            : file(new File(path)),bufferPool(new BufferPool())
        {}
        BufferManager():bufferPool(new BufferPool()){}
        ~BufferManager();
        std::shared_ptr<Page> GetPageFromDisk(PageDirectory &dir,unsigned int pageIdx);
        std::shared_ptr<Page> GetPageFromBufferPool(std::string fileName,unsigned int pageIdx);
        std::shared_ptr<Page> GetEnoughSpacePage(std::string fileName,int length);

        void WriteBlock(std::shared_ptr<Page> page,const char *content,int length);
        void FlushPageToDisk(PageDirectory dir, std::shared_ptr<Page> &page);

        /**
         * @brief 페이지가 버퍼 풀에 다 찼을 때 last Page evicton 실시
         */
        void ReplacePage(Page *page);
        void SetFile(File *file){this->file=file;}
        
};

#endif