#ifndef ABCDB_PAGE_H_
#define ABCDB_PAGE_H_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem.hpp>
#include <cstring> 
#include <fstream> 
#include <iostream>

constexpr int PAGE_SIZE= 4 * 1024;

/**
 * @brief Page내부에서 Record를 관리하는 Slot
 * 
 */
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
        /**
         * @brief Set the Record Info object
         * 
         * @param offset Record 시작위치
         * @param length Record 크기
         */
        void SetRecordInfo(size_t offset, size_t length);
        /**
         * @brief Get the Offset object
         * 
         * @return size_t Record offset
         */
        size_t GetOffset() const;
        /**
         * @brief Get the Length object
         * 
         * @return size_t Record 크기
         */
        size_t GetLength() const;
        /**
         * @brief 슬롯이 비어있는지 여부 확인
         * 
         * @return true empty
         * @return false 
         */
        bool IsEmpty() const;
        /**
         * @brief 레코드가 삭제 되었는지 여부 확인
         * 
         * @return true Record 삭제됨
         * @return false 
         */
        bool IsDeleted() const ;
        void Clear();
};

/**
 * @brief 실질적인 Record를 관리하는 Page
 * 
 */
class Page {
    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar & page_idx_;
            ar & age_;
            ar & dirty_;
            ar & pinned_;
            ar & record_offset_;
            ar & slot_offset_;
            ar & data_;
        }
        static const int HEADER_SIZE = 128;
        
        // 페이지 헤더
        long age_;          
        bool dirty_;                  // 페이지 변경 여부   
        bool pinned_;                        // 페이지 고정 여부
        int page_idx_;                     // 페이지 디렉터리내의 index  
        int record_offset_;                  // 데이터가 추가될 위치
        int slot_offset_;                   // 슬롯이 추가될 위치
        Page *next_;                        // 다음 페이지
        Page *prev_;                        // 이전 페이지
        // 데이터
        std::vector<char> data_;           // 페이지 데이터 (실제 데이터 저장소)

    public:
        Page() :page_idx_(-1), slot_offset_(HEADER_SIZE),record_offset_(PAGE_SIZE) {
            data_.resize(PAGE_SIZE);
        }

        /**
         * @brief  페이지 비교 연산자 overloading 참조 횟수를 기준으로 비교하기 위함
         * 
         */
        bool operator<(const Page& other) const {
            return this->age_ < other.age_;
        }   
        /**
         * @brief 충분한 저장공간이 있는지 확인
         * 
         * @param record_size 새롭게 저장할 Record의 크기
         * @return true 현재 페이지에 저장 가능
         * @return false 현재 페이지에 저장 불가능 
         */
        bool HasEnoughSpace(int record_size) const;

        /**
         * @brief 빈 슬롯에 데이터 저장 (페이지 끝에서부터 채워짐)
         * 
         * @param record 새롭게 저장할 Record
         * @param record_size 새롭게 저장할 Record의 크기
         * @return true 성공
         * @return false 실패
         */
        bool InsertRecord(const char* record, int record_size);

        /**
         * @brief 특정 인덱스에서 가변 길이 레코드를 읽는 함수
         * 
         * @param offset 읽어올 Record의 offset
         * @param length 읽어올 Record의 크기
         * @return std::string Record
         */
        std::string ReadRecordFromOffset(int offset, int length) const;

        /**
         * @brief Get the Page Idx object
         * 
         * @return int Page의 인덱스 
         */
        int GetPageIdx() const;

        /**
         * @brief Set the Page Idx object
         * 
         * @param index Page의 인덱스 
         */


        /**
         * @brief 페이지 변경 여부 확인
         * 
         *  @return true 변경됨
         *  @return false 변경되지 않음
         */
        bool IsDirty() const;

        /**
         * @brief 페이지 고정 여부 확인
         * 
         * @return true 고정됨
         * @return false 고정되지 않음
         */
        bool IsPinned() const;

        /**
         * @brief 페이지 참조 횟수 확인
         * 
         * @return dirty 변경 여부
         */
        int GetAge() const;


        void SetPageIdx(const int index);

        /**
         * @brief 페이지 변경 여부 설정 
         * 
         * @param  true 변경됨, 
         * @param  false 변경되지 않음
         */
        void SetDirty(bool dirty);

        /**
         * @brief 페이지 고정 여부 설정
         * 
         * @param  true 고정됨
         * @param  false 고정되지 않음
         */
        void SetPinned(bool pinned);

        /**
         * @brief 페이지 참조 횟수 +1 증가
         * 
         */
        void IncreaseAge(){this->age_++;};

        void SetZeroAge(){this->age_=0;};
        /**
         * @brief 슬롯의 레코드 데이터 출력
         * 
         */
        void PrintRecord() const ;
};

#endif