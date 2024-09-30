#include "pageHandler.h"
/**
 * @brief 버퍼 풀을 만드는 함수 first, mid, last page를 설정
 * @param page first page
 * @return 재귀 함수를 통해 최대 페이지 수에 도달할 때까지 페이지를 생성
 */
Page* PageHandler::InitTempBufferPool(Page *page) {
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
        return InitTempBufferPool(adder);
    }
}

Page *PageHandler::FillBufferPoolPage() {
    Page *p=midPage->GetNext();
    
    if(pCurrnetSize==0)
    {
        return NULL; // TODO : 예외처리
    }
    
    // last페이지에 도달하면 mid에서 first로 이동하면서 삽입
    if (p == lastPage)
    {
        p=midPage->GetPrev();
        midPage->SetPrev(midPage->GetPrev()->GetPrev());
        p->SetZeroAge();
        p->SetNext(NULL);
    }
    // 가운데부터 삽입하기 시작
    else{
        midPage->SetNext(midPage->GetNext()->GetNext());
        p->SetZeroAge();
        p->SetNext(NULL);
    }
    

    pCurrnetSize--;
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
    // Page Free시 mid -> next에 삽입
    else{
        // before : mid <-> next
        // after : mid <-> page <-> next
        page->SetNext(midPage->GetNext()); 
        midPage->GetNext()->SetPrev(page);
        midPage->SetNext(page);
        page->SetPrev(midPage);

    }
    pCurrnetSize++; //여유 블록 사이즈 증가

}