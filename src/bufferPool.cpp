#include "bufferPool.h"

void BufferPool::InsertPage(std::shared_ptr<Page> page)
{
    if (!page)
    {
        std::cerr << "Error: Attempted to insert a null page." << std::endl;
        return;
    }

    if (infreq.size() < MAX_INFREQ_SIZE)
    {
        infreq.push_back(page);
        std::cout << "\n[BufferPool Insert]Page inserted into infreq. page Table : " <<page->GetFilename() <<"\tinfreq size : "<<infreq.size()<< std::endl;
    }
    else if (freq.size() < MAX_FREQ_SIZE)
    {
        freq.push_back(page);
        std::cout << "[BufferPool Insert]Page inserted into freq. page Table : " <<page->GetFilename()<<"freq size : "<<freq.size()<<std::endl;
    }
    else
    {
        std::cerr << "Error: Buffer pool is full." << std::endl;
    }
    
}
