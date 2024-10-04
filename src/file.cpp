#include "file.h"
#include <iostream>
/*=======================================PageDirectory================================================ */

bool PageDirectory::HasPage() {
    return !entries_.empty();
}

void PageDirectory::IncrementSize() {
    size_++;
}

int PageDirectory::GetIdx(){
    return index_;
}

int PageDirectory::GetSize() {
    return size_;
}

size_t PageDirectory::GetOffset() const {
    return directory_offset_;
}

std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& PageDirectory::GetEntries() {
    return entries_;
}

size_t PageDirectory::GetNext() const {
    return next_;
}

void PageDirectory::SetNext(size_t next) {
    next_ = next;
}

/*=======================================File================================================ */
File::File(const std::string& filename) {
    file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_.is_open()) {
        file_.open(filename, std::ios::out | std::ios::binary);
        file_.close();
        file_.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }
    if(GetPageDir(0) == nullptr){
        PageDirectory directory(0,0);
        WritePageDirToFile(directory);
    }
}

std::shared_ptr<Page> File::LoadPageFromFile(size_t offset) {
    file_.seekg(offset);
    std::shared_ptr<Page> page = std::make_shared<Page>();
    boost::archive::binary_iarchive iar(file_);
    iar >> *page;
    return page;
}

std::shared_ptr<PageDirectory> File::LoadPageDirFromFile(size_t offset) {
    file_.seekg(offset);
    std::shared_ptr<PageDirectory> dir = std::make_shared<PageDirectory>(offset);
    try {
        boost::archive::binary_iarchive iar(file_);
        iar >> *dir;
    } catch (const boost::archive::archive_exception& e) {
        // std::cerr << "Error: Failed to deserialize PageDirectory. " << e.what() << std::endl;
        return nullptr;
    }
    return dir;
}

void File::WritePageDirToFile(const PageDirectory& dir) {
    file_.seekp(dir.GetOffset(), std::ios::beg); // 자신의 위치에 덮어쓴다.
    boost::archive::binary_oarchive oar(file_);
    oar << dir;
}

size_t File::WritePageToFile(PageDirectory& dir, const Page& page) {
    if (page.GetPageIdx() == -1) {  // 직렬화 안된 Page인 경우
        file_.seekp(0, std::ios::end);  // File 제일 뒤를 point
    } else {
        std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& entries = dir.GetEntries();
        file_.seekp(entries[page.GetPageIdx()].offset, std::ios::beg);  // Page의 위치를 point
    }
    size_t offset = file_.tellp();
    boost::archive::binary_oarchive oar(file_);
    oar << page;
    return offset;
}

std::shared_ptr<PageDirectory> File::AddPageToDirectory(PageDirectory& dir, Page& page) {
    size_t offset = WritePageToFile(dir,page);
    if (dir.GetSize() >= MAX_ENTRIES_PER_DIR) { // PageDirectory가 가득찬경우
        file_.seekp(0, std::ios::end);  // File의 제일 뒤를 point
        size_t offset = file_.tellp();
        PageDirectory new_dir(offset,dir.GetIdx()+1);  // 새로운 PageDirectory create
        WritePageDirToFile(new_dir);
        dir.SetNext(offset);    // 새로운 PageDirectory를 next로 기록
        WritePageDirToFile(dir);  // 현재 PageDirectory Write
        dir = new_dir;
    }
    // std::cout<<"[dir.size]"<<dir.GetSize()<<std::endl;
    page.SetPageIdx(dir.GetSize()); // Page는 entries에서의 본인 index저장
    dir.IncrementSize();
    std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& entries = dir.GetEntries();
    entries[dir.GetSize()] = {offset, false};  // 새로운 Page 관리
    return std::make_shared<PageDirectory>(dir);
}

std::shared_ptr<Page> File::GetPage(PageDirectory& dir, int page_index) {
    std::array<PageDirectoryEntry, MAX_ENTRIES_PER_DIR>& entries = dir.GetEntries();
    if (page_index >= dir.GetSize()) {
        throw std::out_of_range("잘못된 페이지 인덱스입니다.");
    }
    PageDirectoryEntry& entry = entries[page_index];
    std::shared_ptr<Page> page = LoadPageFromFile(entry.offset);
    entry.is_loaded = true;
    return page;
}

std::shared_ptr<Page> File::GetEnoughSpacePage(int length){
    std::shared_ptr<PageDirectory> dir = GetPageDir(0);
    do{
        for(int i=0;i<dir->GetSize();i++){
            std::shared_ptr<Page> page = GetPage(*dir,i);
            if(page->GetFreeSpace() > length){
                return page;
            }
        }
        size_t offset = dir->GetNext();
        dir = GetPageDir(offset);
    }while(dir->GetNext());
    return nullptr;
}

std::shared_ptr<PageDirectory> File::GetPageDir(size_t offset) {
    return LoadPageDirFromFile(offset);
}

File::~File() {
    if (file_.is_open()) {
        file_.close();
    }
}
