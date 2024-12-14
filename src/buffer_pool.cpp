#include "buffer_pool.h"

void BufferPool::InsertPage(std::shared_ptr<Page> page)
{
    if (!page)
    {
        std::cerr << "Error: Attempted to insert a null page." << std::endl;
        return;
    }

    if (infreq.size() < MAX_INFREQ_SIZE) //infreq에 먼저 삽입 시도
    {
        infreq.push_back(page);
    }
    else if (freq.size() < MAX_FREQ_SIZE) // infreq가 다 찼으면 freq로 삽입
    {
        freq.push_front(page);
    }
    else // LRU 알고리즘 적용해서 inreq에서 tail에서 빼고 infreq 두 번째에 삽입 (첫 번째에는 infreqHead가 있음)
    {
        infreq.pop_back();
        auto it = infreq.begin();
        if (it != infreq.end()) {
            ++it; // 두 번째 위치로 이동
        }
        infreq.insert(it, page);
    }
    
}

void BufferPool::PromotePage(std::shared_ptr<Page> page)
{
    if (!page)
    {
        std::cerr << "Error: Attempted to promote a null page." << std::endl;
        return;
    }

    if (infreq.size() > MAX_INFREQ_SIZE)
    {
        std::cerr << "Error: Attempted to promote a page in infreq." << std::endl;
        return;
    }

    if (freq.size() >= MAX_FREQ_SIZE)
    {
        std::cerr << "Error: Attempted to promote a page in freq when freq is full." << std::endl;
        return;
    }

    infreq.remove(page);
    freq.push_front(page);
}


void BufferPool::DebugBufferPool()
{
    std::cout << "[Debug Buffer Pool]" << std::endl;
    
    TraverseBufferPoolVoid([](const std::shared_ptr<Page> &page) -> void {
        char *record = nullptr;
        if (page->GetFilename() != "freqTail" && page->GetFilename() != "infreqHead")
        {
            // page->PrintRecord();
        }
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Page Index      : " << page->GetPageIdx() << std::endl;
        std::cout << "Filename        : " << page->GetFilename() << std::endl;
        std::cout << "Slot Offset     : " << page->GetSlotOffset() << std::endl;
        std::cout << "Free Space      : " << page->GetFreeSpace() << std::endl;
        std::cout << "Dirty           : " << (page->IsDirty() ? "Yes" : "No") << std::endl;
        // std::cout << "Data            : " << (record ? record : "NO DATA") << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "  |\n  v\n";
    });
}

/**
 * @brief 버퍼 풀 순회 함수 : void 반환, shared_ptr<Page> 인자 전달형
 */
void BufferPool::TraverseBufferPoolVoid(std::function<void(const std::shared_ptr<Page>&)> callback)
{
    for (const auto &page : freq)
    {
        callback(page);
    }
    for (const auto &page : infreq)
    {
        callback(page);
    }
}

/**
 * @brief 버퍼 풀 순회 함수 : shared_ptr<Page> 반환, Page 전달형
 */
std::shared_ptr<Page> BufferPool::TraverseBufferPool(std::function<std::shared_ptr<Page>(std::shared_ptr<Page>)> callback)
{
    for (const auto &page : freq)
    {
        auto result = callback(page);
        if (result != nullptr)
        {
            return result;
        }
    }
    for (const auto &page : infreq)
    {
        auto result = callback(page);
        if (result != nullptr)
        {
            return result;
        }
    }
    return nullptr;
}