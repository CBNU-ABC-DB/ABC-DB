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
    std::shared_ptr<Page> firstPage; //자주 접근 페이지
    /**
     * @brief 버퍼 풀을 만드는 함수 first, mid, last page를 설정
     * @param page first page
     */
    std::shared_ptr<Page> InitFreeBlock(std::shared_ptr<Page> page);
    int pCurrnetSize;  // 현재 사용 중인 페이지 수
    int pTotalSize;   // 총 페이지 수
    int midPoint;
public:
    PageHandler()
        : firstPage(std::make_shared<Page>()), pTotalSize(PAGE_AMOUNT), pCurrnetSize(1)
        {
            InitFreeBlock(firstPage);
        }
    /**
     * @brief 버퍼 풀로 부터 페이지를 가져옴
     */
    std::shared_ptr<Page> GetFreeBufferPoolPage();

    /**
     * @brief 페이지를 버퍼 풀에 반환
     * 
     * @param page 반환 페이지
     */
    void FreePage(std::shared_ptr<Page> page);

    void InsertBufferPool(std::shared_ptr<Page> src,std::shared_ptr<Page> dst);

    std::shared_ptr<Page> getFirstPage() { return firstPage; }

    int GetpCurrnetSize() { return pCurrnetSize; }
    int GetpTotalSize() { return pTotalSize; }

};


#endif