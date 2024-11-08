#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <list>
#include <memory>
#include "page.h"

#define MAX_FREQ_SIZE (PAGE_AMOUNT*(5.0/8.0))
#define MAX_INFREQ_SIZE (PAGE_AMOUNT*(3.0/8.0))
#define PAGE_AMOUNT 80
/**
 * @brief 버퍼 풀 클래스 [freq->tail]->[head->infreq]
 */
class BufferPool
{
private:
    std::list<std::shared_ptr<Page>> freq;
    std::list<std::shared_ptr<Page>> infreq;

public:
    BufferPool()
    :freq(std::list<std::shared_ptr<Page>>()), infreq(std::list<std::shared_ptr<Page>>())
    {   
        std::shared_ptr<Page> freqTail = std::make_shared<Page>();
        std::shared_ptr<Page> infreqHead = std::make_shared<Page>();
        infreqHead->SetNext(NULL);

        // freq : freqTail infreq : infreqHead
        // freqTail <-> infreqHead
        freqTail->SetFilename("freqTail");
        infreqHead->SetFilename("infreqHead");
        freq.push_back(freqTail);
        infreq.push_back(infreqHead);
        freq.back()->SetNext(infreq.front());
        infreq.front()->SetPrev(freq.back());
    }
    ~BufferPool()
    {
        freq.clear();
        infreq.clear();
    }
    /**
     * @brief 버퍼 풀에 페이지를 삽입. infreq부터 삽입하고 다차면 freq로 넘어감
     */
    void InsertPage(std::shared_ptr<Page> page);
    void PromotePage(std::shared_ptr<Page> page);
    void ReplacePage(std::shared_ptr<Page> page);
    

    /**
     * @brief shraed_ptr<Page> 반환, shared_ptr<Page> 인자 전달형 버퍼풀 순회함수
    */
    std::shared_ptr<Page> TraverseBufferPool(std::function<std::shared_ptr<Page>(std::shared_ptr<Page>)> callback);

    /**
     * @brief void 반환, shared_ptr<Page> 인자 전달형 버퍼풀 순회함수
     */
    void TraverseBufferPoolVoid(std::function<void(const std::shared_ptr<Page>&)> callback);
    void DebugBufferPool();

    std::list<std::shared_ptr<Page>> GetFreq() { return freq; }
    std::list<std::shared_ptr<Page>> GetInfreq() { return infreq; }

};

#endif