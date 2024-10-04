#include "bufferManager.h"
#include <string>

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
std::shared_ptr<Page> BufferManager::GetPageFromDisk(std::shared_ptr<PageDirectory>dir,unsigned int pageIdx)
{
    // TODO 매개변수나 생성자를 통해 특정 파일을 가져오더록 해야함. 현재는 생성자를 통해 파일을 가져오고 모든 페이지를 가져옴
    if (pageIdx < 0)
    {
        std::cerr << "잘못된 페이지 인덱스" << std::endl;
        return nullptr;
    }
    std::shared_ptr<Page>diskPage=file->GetPage(*dir,pageIdx);
    bufferPool->InsertPage(diskPage); // 버퍼 풀에 페이지 삽입
    return diskPage;
}


/**
 * @brief 버퍼 풀에서 페이지를 가져오는 함수
 * 
 * @param fileName 가져올 페이지가 속한 파일 이름
 * @param pageIdx 가져올 페이지의 인덱스
 * 
 */
std::shared_ptr<Page> BufferManager::GetPageFromBufferPool(std::string fileName,unsigned int pageIdx)
{   
    std::cout<<"[Get Page From BufferPool] Receive Page Index :"<<pageIdx<<"\tFile Name : "<<fileName<<std::endl;
    // 어디 파일의 몇 번째 인덱스인지 알아야함.
    std::shared_ptr<Page> iter=bufferPool->GetFreq().front(); // 버퍼 풀 첫번째 페이지부터 시작(최근 사용 순)
    std::cout<<iter->GetFilename()<<std::endl; 

    for(std::list<std::shared_ptr<Page>>::iterator it=bufferPool->GetFreq().begin();it!=bufferPool->GetFreq().end();it++)
    {
        std::cout<<"[Get Page From BufferPool for] freq iter index : "<<(*it)->GetPageIdx()<<std::endl;
        int infreqBridge=it->get()->GetPageIdx();
        if(it->get()->GetPageIdx()==-1)
        {
            for(std::list<std::shared_ptr<Page>>::iterator infreqIt=bufferPool->GetInfreq().begin();infreqIt!=bufferPool->GetInfreq().end();infreqIt++)
            {
                std::cout<<"[Get Page From BufferPool for] infreq iter index : "<<(*infreqIt)->GetPageIdx()<<std::endl;
                if((*infreqIt)->GetPageIdx()==pageIdx)
                {
                    std::cout<<"[Get Page From BufferPool] Found Page index : "<<(*infreqIt)->GetPageIdx()<<std::endl;
                    return *infreqIt;
                }
            }
        }
    }
    return NULL;    
}

/**
 * @brief 페이지에 값을 변경
 * 
    * @param page 변경할 페이지
 */
void BufferManager::WriteBlock(std::shared_ptr<Page> page)
{
    page->SetDirty(true);
}

/**
 * @brief 페이지를 디스크에 다시 씀
 */
// void BufferManager::FlushPageToDisk(PageDirectory dir, std::shared_ptr<Page> &page)
// {
//     file->WritePageToFile(dir,page);
// }

// void BufferManager::PrintAllPageFromBufferPool()
// {
//     std::cout<<"###  print all page from buffer pool  ###"<<std::endl;
//     std::shared_ptr<Page> iter=pageHandler->getFirstPage();
//     while (iter->GetNext()!=NULL)
//     {
//         std::cout<<iter->GetFilename()<<std::endl;
//         iter=iter->GetNext();
        
//     }
// }

// void BufferManager::PrintAllPageFromDisk(std::shared_ptr<PageDirectory>dir)
// {
//     std::cout<<dir->GetSize()<<std::endl;
//     int size=dir->GetSize();
//     // file->GetPage(*dir,1)->PrintRecord();
//     for(int i=1;i<size;i++)
//     {
//         std::shared_ptr<Page> diskPage=file->GetPage(*dir,i);
//         std::cout<<"page idx : "<<i<<std::endl;
//         std::shared_ptr<Page> bufferPage=pageHandler->GetFreeBufferPoolPage();  //버퍼 풀에서 블럭 가져오기
//         if(bufferPage->GetNext() ==NULL) // 버퍼풀의 마지막 페이지
//         {
//             bufferPage->GetPrev()->SetNext(diskPage);
//             continue;
//         }
//         if(bufferPage->GetPrev() ==NULL) // 버퍼 풀의 첫 번째 페이지
//         {
//             bufferPage->GetNext()->SetPrev(diskPage);
//             continue;
//         }
//         bufferPage->GetNext()->SetPrev(diskPage);
//         bufferPage->GetPrev()->SetNext(diskPage);
//     }

// }
// /**
//  * @brief 버퍼풀이 꽉 찼을 때 페이지를 교체하는 함수
//  * @param page 교체할 페이지
//  */
// void BufferManager::ReplacePage(Page *page)
// {
//     Page *last=pageHandler->getLastPage();
//     bool dirty=last->IsDirty();
//     if (dirty) // 페이지가 변경되었을 경우
//     {   
//         // 페이지를 디스크에 씀
//         FlushPageToDisk(*(last->GetFile()->GetPageDir()),*last);
//     }
//     pageHandler->FreePage(last);
// }