#include "bufferManager.h"
#include <string>
BufferManager::~BufferManager()
{
    std::cout<<"[Buffer Manager Destructor]"<<std::endl;
    for(auto it=bufferPool->GetFreq().begin();it!=bufferPool->GetFreq().end();it++)
    {
        if((*it)->IsDirty())
        {
            auto dir=file->GetPageDir();
            FlushPageToDisk(*dir,*it);
        }   
    }
    for(auto it=bufferPool->GetInfreq().begin();it!=bufferPool->GetInfreq().end();it++)
    {
        if((*it)->IsDirty())
        {
            auto dir=file->GetPageDir();
            FlushPageToDisk(*dir,*it);
        }
    }
    delete bufferPool;
    delete file;
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
    std::shared_ptr<Page>diskPage=file->GetPage(dir,pageIdx);
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
    std::cout<<"[Get Page From BufferPool] Receive Page Index :"<<pageIdx<<"\tFile Name : "<<fileName<<std::endl;
    // 어디 파일의 몇 번째 인덱스인지 알아야함.
    std::shared_ptr<Page> iter=bufferPool->GetFreq().front(); // 버퍼 풀 첫번째 페이지부터 시작(최근 사용 순)
    std::cout<<iter->GetFilename()<<std::endl; 

    // freq 링크드 리스트 순회
    for(std::list<std::shared_ptr<Page>>::iterator it=bufferPool->GetFreq().begin();it!=bufferPool->GetFreq().end();it++)
    {
        int infreqBridge=it->get()->GetPageIdx(); //페이지 인덱스 확인
        // freq 순회
        // 찾는 페이지 인덱스와 파일 이름이 같으면
        if(it->get()->GetPageIdx()==pageIdx && it->get()->GetFilename()==fileName) // 찾는 페이지 인덱스와 같으면
        {
            std::cout<<"[Get Page From BufferPool] Found Page index : "<<it->get()->GetPageIdx()<<std::endl;
            return *it;
        }

        if(it->get()->GetPageIdx()==-1) // freq의 tail이면
        {
            // infreq로 이동
            for(std::list<std::shared_ptr<Page>>::iterator infreqIt=bufferPool->GetInfreq().begin();infreqIt!=bufferPool->GetInfreq().end();infreqIt++)
            {
                // 찾는 인덱스와 파일 이름이 같으면
                if((*infreqIt)->GetPageIdx()==pageIdx && it->get()->GetFilename()==fileName)
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
void BufferManager::WriteBlock(std::shared_ptr<Page> page,const char *content,int length)
{
    page->SetDirty(true);
    page->InsertRecord(content,length);
}

/**
 * @brief 버퍼 풀에서 여유 공간이 있는 페이지를 가져오는 함수
 * 
 */
std::shared_ptr<Page> BufferManager::GetEnoughSpacePage(std::string path, int length)
{
    // 어디 파일의 몇 번째 인덱스인지 알아야함.
    std::shared_ptr<Page> iter=bufferPool->GetFreq().front(); // 버퍼 풀 첫번째 페이지부터 시작(최근 사용 순)
    std::cout<<iter->GetFilename()<<std::endl; 

    // freq 링크드 리스트 순회
    for(std::list<std::shared_ptr<Page>>::iterator it=bufferPool->GetFreq().begin();it!=bufferPool->GetFreq().end();it++)
    {
        int infreqBridge=(*it)->GetPageIdx(); //페이지 인덱스 확인

        // 레코드 삽입 여유 페이지가 있고 테이블이 같으면
        if((*it)->HasEnoughSpace(length)!=0 && (*it)->GetFilename()==path)
        {
            std::cout<<"[Get Enough Page From BufferPool] Found Page index in freq : "<<(*it)->GetPageIdx()<<std::endl;
            return *it;
        }
            
        if(it->get()->GetPageIdx()==-1) // freq의 tail이면
        {
            // infreq 링크드 리스트 순회
            for(std::list<std::shared_ptr<Page>>::iterator infreqIt=bufferPool->GetInfreq().begin();infreqIt!=bufferPool->GetInfreq().end();infreqIt++)
            {
                if((*infreqIt)->HasEnoughSpace(length) &&(*infreqIt)->GetFilename()==path)
                {
                    std::cout<<"[Get Enough Page From BufferPool] Found Page index in infreq : "<<(*infreqIt)->GetPageIdx()<<std::endl;
                    return *infreqIt;
                }
            }
        }
        return NULL;    
    }
}

void BufferManager::FlushPageToDisk(PageDirectory dir, std::shared_ptr<Page> &page)
{
    file->WritePageToFile(dir,*page);
}