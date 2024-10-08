#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H

#include "file_handler.h"
#include "bufferPool.h"
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
        // FileHandler *fh;

    public:
        BufferManager()
            : bufferPool(new BufferPool())
        {}
        ~BufferManager()
        {
            delete bufferPool;
            // delete pageHandler;
        }
        std::shared_ptr<Page> GetPageFromDisk(PageDirectory &dir,unsigned int pageIdx);
        std::shared_ptr<Page> GetPageFromBufferPool(std::string fileName,unsigned int pageIdx);
        // PageHandler *GetPageHandler() { return pageHandler; }
        void WriteBlock(std::shared_ptr<Page> page);
        void PrintAllPageFromBufferPool();
        void PrintAllPageFromDisk(std::shared_ptr<PageDirectory>dir);
        /**
         * @brief 페이지가 버퍼 풀에 다 찼을 때 last Page evicton 실시
         */
        void ReplacePage(Page *page);
        void FlushPageToDisk(PageDirectory dir, std::shared_ptr<Page> &page);
        void SetFile(File *f){file=f;std::cout<<f->GetPageDir()->GetSize()<<std::endl;}
};

#endif