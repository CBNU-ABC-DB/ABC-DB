#include "pageHandler.h"
/**
 * @brief 버퍼 풀을 만드는 함수 
 * @param page first page
 * @return 재귀 함수를 통해 최대 페이지 수에 도달할 때까지 페이지를 생성
 */
std::shared_ptr<Page> PageHandler::InitFreeBlock(std::shared_ptr<Page> page) {
    std::shared_ptr<Page> adder=std::make_shared<Page>();
    adder->SetNext(page->GetNext());
    page->SetNext(adder);
    pCurrnetSize++;

    if(pCurrnetSize==PAGE_AMOUNT) // 마지막 페이지 설정 및 종결 조건
    {
        return adder;
    }
    else // 마지막 페이지 설정 전까지 재귀
    {
        return InitFreeBlock(adder);
    }
}

std::shared_ptr<Page> PageHandler::GetFreeBufferPoolPage() {
    if(pCurrnetSize==0)
    {
        return NULL; // TODO : 예외처리
    }
    std::shared_ptr<Page>p=firstPage->GetNext(); // first 페이지 반환
    p->SetZeroAge(); // age 초기화
    pCurrnetSize--; // 사용 중인 free 페이지 수 감소
    p->SetNext(NULL);
    return p;
}

/**
 * @brief 페이지를 버퍼 풀에 반환
 * @param page 반환 페이지
 */
void PageHandler::FreePage(std::shared_ptr<Page> page)
{
    if(pCurrnetSize==0) // 남은 페이지가 없을 때 first를 page로 설정
    {
        firstPage=page;
        page->SetNext(page);
    }
    else{
        page->SetNext(firstPage->GetNext()); 
        firstPage->SetNext(page);
    }
    pCurrnetSize++; //여유 블록 사이즈 증가
}