#include "pageHandler.h"
/**
 * @brief 버퍼 풀을 만드는 함수 first, mid, last page를 설정
 * @param page first page
 * @return 재귀 함수를 통해 최대 페이지 수에 도달할 때까지 페이지를 생성
 */
Page* PageHandler::InitBufferPool(Page *page) {
    Page *adder=new Page();
    adder->SetNext(page->GetNext());
    page->SetNext(adder);
    adder->SetPrev(page);
    pCurrnetSize++;

    if(pCurrnetSize==midPoint) // 중간 페이지 설정
    {
        midPage=adder;
        midPage->SetDirty(true);
    }

    if(pCurrnetSize==PAGE_AMOUNT) // 마지막 페이지 설정 및 종결 조건
    {
        lastPage=adder;
        return adder;
    }
    else // 마지막 페이지 설정 전까지 재귀
    {
        return InitBufferPool(adder);
    }
}

Page *PageHandler::GetBlockFromBufferPool() {
    if(pCurrnetSize==0)
    {
        return NULL;
    }
    Page *p=firstPage->GetNext();
    firstPage->SetNext(firstPage->GetNext()->GetNext());
    pCurrnetSize--;
    p->SetZeroAge();    
    p->SetNext(NULL);
    return p;
}


/**
 * @brief 페이지를 버퍼 풀에 반환
 * @param page 반환 페이지
 */
void PageHandler::FreePage(Page *page)
{
    if(pCurrnetSize==0) // 남은 페이지가 없을 때 first를 page로 설정
    {
        firstPage=page;
        page->SetNext(page);
    }
    else{
        // before : first <-> next
        // after : first <-> page <-> next
        page->SetNext(firstPage->GetNext()); 
        firstPage->GetNext()->SetPrev(page);
        firstPage->SetNext(page);
        page->SetPrev(firstPage);

    }
    pCurrnetSize++; //여유 블록 사이즈 증가

}