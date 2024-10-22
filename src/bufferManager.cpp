#include "bufferManager.h"
#include <string>
/**
 * @brief 버퍼 매니저가 소멸할 때 디스크로 작성.
 */
BufferManager::~BufferManager()
{
    
    bufferPool->TraverseBufferPoolVoid([this](const std::shared_ptr<Page> &page) -> void {
        if (page->IsDirty())
        {
            File file(page->GetFilename());
            auto dir = file.GetPageDir(0); // TODO : DIR INDEX가 달라질 수도 있음
            FlushPageToDisk(*dir, page); 
        }
    });
    delete bufferPool;
}
/**
 * @brief 디스크에서 페이지를 가져오고 버퍼 풀에 삽입하는 함수
 * 
 * @details 파일 내부 Page는 Page Directory로 관리되며 Page Directory는 배열로 이루어짐.
 *         배열은 Page Directory Entry로 이루어져 있으며 각 Page Directory Entry는 Page의 offset을 가지고 있음. 
 *         PageDirectory -> PageEntry -> Page로 접근 하는 과정을 거침.
 * @param dir 가져올 페이지가 속한 디렉토리
 * @param pageIdx 가져올 페이지의 인덱스. Page Directory Entry의 인덱스
 * @return 디스크에서 가져온 페이지의 스마트 포인터
 */
std::shared_ptr<Page> BufferManager::GetPageFromDisk(PageDirectory &dir,unsigned int pageIdx)
{
    std::cout<<"[Get Page From Disk]"<<std::endl;
    if (pageIdx < 0)
    {
        std::cerr << "잘못된 페이지 인덱스" << std::endl;
        return nullptr;
    }
    File file(file_);
    std::shared_ptr<Page>diskPage=file.GetPage(dir,pageIdx);
    bufferPool->InsertPage(diskPage); // 버퍼 풀에 페이지 삽입
    return diskPage;
}


/**
 * @brief 버퍼 풀에서 페이지를 가져오는 함수
 * 
 * @param fileName 가져올 페이지가 속한 파일 이름
 * @param pageIdx 가져올 페이지의 인덱스
 */
std::shared_ptr<Page> BufferManager::GetPageFromBufferPool(std::string fileName,unsigned int pageIdx)
{   
    std::cout << "[Get Page From BufferPool]" << std::endl;
    return bufferPool->TraverseBufferPool
    ([=](std::shared_ptr<Page> page) -> std::shared_ptr<Page> 
    {
        if (page->GetPageIdx() == pageIdx && page->GetFilename() == fileName)
        {
            return page;
        }
        return nullptr;
    });
}

/**
 * @brief 버퍼 풀에서 여유 공간이 있는 페이지를 가져오는 함수
 * 
 */
std::shared_ptr<Page> BufferManager::GetEnoughSpacePage(std::string path, int length)
{   
    std::cout<<"[Get Enough Page From BufferPool]"<<std::endl;
    return bufferPool->TraverseBufferPool
    ([=](std::shared_ptr<Page> page) -> std::shared_ptr<Page> 
    {
        if (page->HasEnoughSpace(length) && page->GetFilename() == path)
        {
            return page;
        }
        return nullptr;
    });
}

/**
 * @brief page를 디스크로 내려 쓰게 하는 함수
 * @param dir 페이지가 속한 디렉토리
 * @param page 내려 쓸 페이지
*/
void BufferManager::FlushPageToDisk(PageDirectory dir, const std::shared_ptr<Page> &page)
{
    File f(file_);
    f.WritePageToFile(dir,*page);
}

/**
 * @brief 페이지에 값을 변경
 * 
 * @param page 변경할 페이지
 */
void BufferManager::WriteBlock(std::shared_ptr<Page> page,const char *content,int length)
{
    page->SetDirty(true);
    page->InsertRecord(content,length);
}
/**
 * @brief 버퍼풀에 있는 모든 데이터 출력
 * 
 */
void BufferManager::DebugAllBufferPool()
{
    bufferPool->DebugBufferPool();
}
void BufferManager::DebugTableBufferPool(std::string fileName)
{
    
}
void BufferManager::DebugTableBufferPool(std::string fileName,int pageIdx)
{

}