
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <cstring> 
#include <fstream> 

#include "page.h"

void Slot::SetRecordInfo(size_t offset, size_t length) {
    offset_ = offset;
    length_ = length;
};

size_t Slot::GetOffset() const {
    return offset_;
};

size_t Slot::GetLength() const {
    return length_;
};

bool Slot::IsEmpty() const {
    return offset_ == 0 && length_ == 0;
};

bool Slot:: IsDeleted() const {
    return is_del_;
};

void Slot:: Clear() {
    offset_ = 0;
    length_ = 0;
    is_del_ = false;
};

bool Page::HasEnoughSpace(int record_size) const {
    return slot_offset_ + sizeof(Slot) <= record_offset_ - record_size;
};

bool Page::InsertRecord(const char* record, int record_size) {
    if (!HasEnoughSpace(record_size)){
            std::cerr<< "페이지에 남은 공간이 부족합니다."<<std::endl;
            return false;
        }

        record_offset_ -= record_size;
        std::memcpy(&data_[record_offset_], record, record_size);


        Slot new_slot(record_offset_, record_size);
        std::memcpy(&data_[slot_offset_], &new_slot, sizeof(Slot));

        slot_offset_ += sizeof(Slot);
        return true;
};

void Page::WriteToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
        boost::archive::binary_oarchive oar(ofs);
        oar << *this;  
        ofs.close();
};

void Page::ReadFromFile(const std::string& filename) {
    boost::filesystem::path file_path(filename);
       
       file_path.imbue(std::locale("en_US.UTF-8"));

       if(boost::filesystem::exists(file_path)){
            std::ifstream ifs;
            ifs.open(filename.c_str(), std::ios::binary);
            boost::archive::binary_iarchive iar(ifs);
            iar >> (*this);
            ifs.close();
       }
}

// 특정 인덱스에서 가변 길이 레코드를 읽는 함수
std::string Page::ReadRecordFromOffset(int offset, int length) const {
    if (offset < 0 || offset + length > data_.size()) {
            throw std::out_of_range("잘못된 오프셋 또는 길이");
        }
        return std::string(data_.begin() + offset, data_.begin() + offset + length);
};

void Page::PrintRecord() const{
    int current_slot_offset = HEADER_SIZE;

        while (current_slot_offset < slot_offset_) {
            Slot slot;
            std::memcpy(&slot, &data_[current_slot_offset], sizeof(Slot));

            // 삭제되지 않은 레코드만 출력
            if (!slot.IsDeleted()) {
                std::string record = ReadRecordFromOffset(slot.GetOffset(), slot.GetLength());
                std::cout << "레코드 데이터: " << record << std::endl;
            }

            // 다음 슬롯으로 이동
            current_slot_offset += sizeof(Slot);
        }
};
