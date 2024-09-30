#include "bufferManager.h"
#include <string>

/**
 * @brief 디스크에서 페이지를 가져오는 함수
 * 
 * @details 파일 내부 Page는 Page Directory로 관리되며 Page Directory는 배열로 이루어짐.
 *         배열은 Page Directory Entry로 이루어져 있으며 각 Page Directory Entry는 Page의 offset을 가지고 있음. 
 *         PageDirectory -> PageEntry -> Page로 접근 하는 과정을 거침.
 * @param pageIdx 가져올 페이지의 인덱스. Page Directory Entry의 인덱스
 */
Page *BufferManager::GetPageFromDisk(unsigned int pageIdx)
{
    // TODO 매개변수나 생성자를 통해 특정 파일을 가져오더록 해야함. 현재는 생성자를 통해 파일을 가져오고 모든 페이지를 가져옴

    if (pageIdx < 0)
    {
        std::cerr << "잘못된 페이지 인덱스" << std::endl;
        return nullptr;
    }
    
    std::shared_ptr<PageDirectory> dir=file->GetPageDir();
    std::shared_ptr<Page>p=file->GetPage(*dir,pageIdx);
    Page *page=pageHandler->FillBufferPoolPage();
    page->SetZeroAge();

    return p.get();
}

/**
 * @brief 버퍼 풀에서 페이지를 가져오는 함수
 * 
 * @param fileName 가져올 페이지가 속한 파일 이름
 * @param pageIdx 가져올 페이지의 인덱스
 * 
 */
Page *BufferManager::GetPageFromBufferPool(std::string fileName,unsigned int pageIdx)
{   
    // 어디 파일의 몇 번째 인덱스인지 알아야함.
    Page *iter=pageHandler->getFirstPage();
    
    while (iter->GetNext()!=NULL)
    {
        // 원하는 페이지를 찾아야함
        std::string curFileName=iter->GetFile()->GetFileName();
        std::shared_ptr<PageDirectory>pd =iter->GetFile()->GetPageDir();
        if(curFileName==fileName && iter->GetPageIdx()==pageIdx)
        {
            return iter;
        }
        else
        {
            iter=iter->GetNext();
        }
        
    }
    
}

/**
 * @brief 페이지에 값을 변경
 * 
    * @param page 변경할 페이지
 */
void BufferManager::WriteBlock(Page *page)
{
    page->SetDirty(true);
}

/**
 * @brief 페이지를 디스크에 다시 씀
 */
void BufferManager::FlushPageToDisk(PageDirectory dir, Page &page)
{
    file->WritePageToFile(dir,page);
}

/**
 * @brief 버퍼풀이 꽉 찼을 때 페이지를 교체하는 함수
 * @param page 교체할 페이지
 */
void BufferManager::ReplacePage(Page *page)
{
    Page *last=pageHandler->getLastPage();
    bool dirty=last->IsDirty();
    if (dirty) // 페이지가 변경되었을 경우
    {   
        // 페이지를 디스크에 씀
        FlushPageToDisk(*(last->GetFile()->GetPageDir()),*last);
    }
    pageHandler->FreePage(last);
    
}