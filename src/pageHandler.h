#ifndef PAGEHANDLER_H
#define PAGEHANDLER_H
#include "page.h"
#define PAGE_AMOUNT 80

/**
 * @brief Page Handler class 버퍼 풀 클래스
 */
class PageHandler
{
private:
    Page *firstPage; //자주 접근 페이지
    Page *midPage; // 중간 페이지 : page 삽입 지점 last 방향으로 삽입
    Page *lastPage; //잘 접근 되지않는 페이지

    /**
     * @brief 버퍼 풀을 만드는 함수 first, mid, last page를 설정
     * @param page first page
     */
    Page *InitBufferPool(Page *page);
    int pCurrnetSize;  // 현재 사용 중인 페이지 수
    int pTotalSize;   // 총 페이지 수
    int midPoint;
public:
    PageHandler()
        : firstPage(new Page()), pTotalSize(PAGE_AMOUNT), pCurrnetSize(1),midPoint(PAGE_AMOUNT*5/8)
        {
            InitBufferPool(firstPage);
        }
    /**
     * @brief 버퍼 풀로 부터 페이지를 가져옴
     */
    Page* GetBlockFromBufferPool();

    /**
     * @brief 페이지를 버퍼 풀에 반환
     * 
     * @param page 반환 페이지
     */
    void FreePage(Page *page);

    Page* getFirstPage() { return firstPage; }

    Page* getMidPage() { return midPage; }

    Page* getLastPage() { return lastPage; }

};


#endif