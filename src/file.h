#ifndef ABCDB_FILE_H_
#define ABCDB_FILE_H_

#include <cstring> 
#include <fstream> 
#include <array>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/array.hpp>
#include "page.h"

/**
 * @brief 페이지 디렉토리 entries에 들어가는 페이지를 참조하기위한 정보
 * 
 */
struct PageDirectoryEntry{
    size_t offset;  // 파일 내 페이지의 오프셋
    bool is_loaded = false; // 페이지가 메모리에 로드되어 있는지 여부
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & offset;
        ar & is_loaded;
    }
};

const int DIRECTORY_BODY_SIZE = PAGE_SIZE - sizeof(size_t) * 2 - sizeof(int);   // 페이지 디렉토리의 헤더를 제외한 사이즈
constexpr int MAX_ENTRIES_PER_DIR = DIRECTORY_BODY_SIZE / sizeof(PageDirectoryEntry);   // 최대 페이지 entry 저장 개수

/**
 * @brief 파일내 페이지의 offset을 entry로 관리
 * 
 */
class PageDirectory {
private:
    size_t directory_offset_;   // 페이지 디렉토리 offset
    size_t next_;   // 다음 페이지 디렉토리 offset
    int size_;  // 현재 entries 사이즈
    std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR> entries_;   // page offset을 저장하는 array

public:
    PageDirectory(const size_t offset);
    
    template<class Archive>
     void serialize(Archive& ar, const unsigned int version) {
        ar & directory_offset_;
        ar & next_;
        ar & size_;
        ar & boost::serialization::make_array(entries_.data(), entries_.size());
    }

    /**
     * @brief 페이지 디렉토리가 페이지를 하나라도 가지고 있는지 여부
     * 
     * @return true 하나 이상의 페이지 가지고 있음.
     * @return false 페이지 가지고 있지 않음.
     */
    bool HasPage();

    /**
     * @brief entries의 사이즈 증가
     * 
     */
    void IncrementSize();

    /**
     * @brief Get the Size object
     * 
     * @return int entries_ size
     */
    int GetSize();

    /**
     * @brief Get the Offset object
     * 
     * @return size_t 페이지 디렉토리 offset
     */
    size_t GetOffset() const;

    /**
     * @brief Get the Entries object
     * 
     * @return std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& 
     */
    std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& GetEntries();
    
    /**
     * @brief Get the Next object
     * 
     * @return size_t  다음 페이지 디렉토리 offset
     */
    size_t GetNext() const;

    /**
     * @brief Set the Next object
     * 
     * @param next 다음 페이지 디렉토리 offset
     */
    void SetNext(size_t next);
};

/**
 * @brief PageDirectory 및 Page를 관리
 * 
 */
class File {
private:
    std::fstream file_;

    /**
     * @brief 파일에서 페이지 읽어옴
     * 
     * @param offset 읽어올 Page의 offset
     * @return std::shared_ptr<Page>  Page의 스마트 포인터(참조 카운팅을 통해 마지막 참조가 해제되면 자동으로 객체 삭제)
     */
    std::shared_ptr<Page> LoadPageFromFile(size_t offset);
    /**
     * @brief 파일에서 페이지 디렉토리 읽어옴
     * 
     * @param offset PageDirectory의 offset
     * @return std::shared_ptr<PageDirectory> PageDirectory의 스마트 포인터(참조 카운팅을 통해 마지막 참조가 해제되면 자동으로 객체 삭제)
     */
    std::shared_ptr<PageDirectory> LoadPageDirFromFile(size_t offset);

public:
    File(const std::string& filename);
    /**
     * @brief 페이지 디렉토리 파일에 쓰기
     * 
     * @param dir File에 직렬화 시킬 PageDirectory
     */
    void WritePageDirectoryToFile(const PageDirectory& dir);
    /**
     * @brief 페이지 파일에 쓰기
     * 
     * @param dir Page를 관리하는 PageDirectory
     * @param page File에 직렬화 시킬 Page
     * @return size_t File에 작성된 Page의 offset
     */
    size_t WritePageToFile(PageDirectory& dir, const Page& page);
    /**
     * @brief 페이지 페이지 디렉토리에 추가
     * 
     * @param dir Page를 관리할 PageDirectory
     * @param page PageDirectory에 새롭게 추가할 Page
     */
    void AddPageToDirectory(PageDirectory& dir, Page& page);
    /**
     * @brief Get the Page object
     * 
     * @param dir 
     * @param page_index 
     * @return std::shared_ptr<Page> 
     */
    std::shared_ptr<Page> GetPage(PageDirectory& dir, int page_index);
    /**
     * @brief Get the Page Dir object
     * 
     * @param offset 
     * @return std::shared_ptr<PageDirectory> 
     */
    std::shared_ptr<PageDirectory> GetPageDir(size_t offset = 0);
    //
    ~File();
};

#endif  // ABCDB_FILE_H_
