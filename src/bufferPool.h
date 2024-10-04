#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H

#include <list>
#include <memory>
#include "page.h"
#include "pageHandler.h"

#define MAX_FREQ_SIZE (PAGE_AMOUNT*(5.0/8.0))
#define MAX_INFREQ_SIZE (PAGE_AMOUNT*(3.0/8.0))
/**
 * @brief 버퍼 풀 클래스 [freq->tail]->[head->infreq]
 */
class BufferPool
{
private:
    std::list<std::shared_ptr<Page>> freq;
    std::list<std::shared_ptr<Page>> infreq;
    // PageHandler *pageHandler;

public:
    BufferPool()
    :freq(std::list<std::shared_ptr<Page>>()), infreq(std::list<std::shared_ptr<Page>>())
    // pageHandler(new PageHandler())
    {
        std::shared_ptr<Page> freqTail = std::make_shared<Page>();
        std::shared_ptr<Page> infreqHead = std::make_shared<Page>();
        infreqHead->SetNext(NULL);

        // freq : freqTail infreq : infreqHead
        // freqTail <-> infreqHead
        freq.push_back(freqTail);
        infreq.push_back(infreqHead);
        freq.back()->SetNext(infreq.front());
        infreq.front()->SetPrev(freq.back());
    }

    std::shared_ptr<Page> GetPage();
    /**
     * @brief 버퍼 풀에 페이지를 삽입. infreq부터 삽입하고 다차면 freq로 넘어감
     */
    void InsertPage(std::shared_ptr<Page> page);
    
    std::list<std::shared_ptr<Page>> GetFreq() { return freq; }
    std::list<std::shared_ptr<Page>> GetInfreq() { return infreq; }
};

#endif