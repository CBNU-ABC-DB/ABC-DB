#ifndef ABCDB_PAGE_H_
#define ABCDB_PAGE_H_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

class Slot {
private:
    size_t offset_;  // 페이지 내의 레코드 시작 위치 (바이트 단위)
    size_t length_;  // 저장된 레코드의 길이
    bool is_del_;   // 레코드 삭제여부
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & offset_;
        ar & length_;
        ar & is_del_;
    }
public:
    Slot() : offset_(0), length_(0), is_del_(false) {}
    Slot(size_t offset, size_t length) : offset_(offset), length_(length), is_del_(false) {}
    // 레코드가 저장될 위치와 크기를 설정
    void SetRecordInfo(size_t offset, size_t length);
    // 레코드의 시작 위치 반환
    size_t GetOffset() const;
    // 레코드의 크기 반환
    size_t GetLength() const;
    // 슬롯이 비어있는지 여부 확인 (offset이 -1이면 비어있음)
    bool IsEmpty() const;
    // 레코드가 삭제 되었는지 여부 확인
    bool IsDeleted() const ;
    // 슬롯 비우기
    void Clear();
};

class Page {
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & page_num_;
        ar & record_offset_;
        ar & slot_offset_;
        ar & data_;
    }
    static const int PAGE_SIZE= 4 * 1024;
    static const int HEADER_SIZE = 128;
    // 페이지 헤더
    long age_;
    bool dirty_;
    int page_num_;
    int record_offset_;                  // 데이터가 추가될 위치
    int slot_offset_;                   // 슬롯이 추가될 위치
    // 데이터
    std::vector<char> data_;           // 페이지 데이터 (실제 데이터 저장소)

public:
    Page(int page_number) :page_num_(page_number), slot_offset_(HEADER_SIZE),record_offset_(PAGE_SIZE) {
        data_.resize(PAGE_SIZE);
    }
    // 충분한 저장공간이 있는지 확인
    bool HasEnoughSpace(int record_size) const;
    // 빈 슬롯에 데이터 저장 (페이지 끝에서부터 채워짐)
    bool InsertRecord(const char* record, int record_size);
    // 바이너리 파일에 페이지와 슬롯 정보 쓰기
    void WriteToFile(const std::string& filename) const ;
    // 바이너리 파일에서 페이지와 슬롯 정보 읽기
    void ReadFromFile(const std::string& filename);
    // 특정 인덱스에서 가변 길이 레코드를 읽는 함수
    std::string ReadRecordFromOffset(int offset, int length) const;
    // 슬롯의 레코드 데이터 출력
    void PrintRecord() const ;
};

#endif