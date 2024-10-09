#ifndef MINIDB_CATALOG_MANAGER_H_
#define MINIDB_CATALOG_MANAGER_H_

#include <string>
#include <vector>
#include <memory>
#include "ASTNodes.h" // AST 노드 구조 정의를 포함하는 헤더 파일

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

// CatalogManager는 데이터베이스와 테이블을 관리하는 클래스
class Database;
class Table;
class Attribute;

// CatalogManager 클래스: 데이터베이스와 테이블의 메타데이터 관리
class CatalogManager
{
private:
    friend class boost::serialization::access; // Boost serialization을 위한 friend 선언

    // Boost serialization 메서드: 객체를 직렬화하는 함수
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & dbs_; // 데이터베이스 목록을 직렬화
    }

    std::string path_;          // 데이터베이스 파일 경로
    std::vector<Database> dbs_; // 데이터베이스 목록

public:
    CatalogManager(std::string p); // 생성자
    ~CatalogManager();             // 소멸자

    std::vector<Database> &dbs() { return dbs_; } // 데이터베이스 목록에 대한 참조 반환
    std::string path() { return path_; }          // 경로 반환
    Database *GetDB(std::string db_name) const;   // 특정 이름의 데이터베이스 반환

    // 데이터베이스 관련 함수들
    bool doesDatabaseExist(const std::string &dbname) const; // 데이터베이스 존재 여부 확인
    void createDatabase(const std::string &dbname);          // 데이터베이스 생성
    void dropDatabase(const std::string &dbname);            // 데이터베이스 삭제

    // AST 노드에 맞춘 테이블 관련 처리 함수들
    void HandleCreateTableStmt(CreateTableStmtNodePtr createStmt); // CREATE TABLE 문 처리
    void HandleDropTableStmt(DropTableStmtNodePtr dropStmt);       // DROP TABLE 문 처리
    void HandleDeleteStmt(DeleteStmtNodePtr deleteStmt);           // DELETE 문 처리

    // 테이블 삭제 및 데이터 삭제 함수
    void deleteTable(const std::string &tableName);                                                   // 테이블 삭제 처리
    void deleteData(const std::string &tableName, const std::shared_ptr<ConditionNode> &whereClause); // 조건에 맞는 데이터 삭제 처리

    // 메타데이터를 파일로부터 읽고 쓰는 함수들
    void ReadArchiveFile();  // 파일에서 데이터베이스 정보를 읽어오는 함수
    void WriteArchiveFile(); // 파일에 데이터베이스 정보를 저장하는 함수
};

// Database 클래스: 각 데이터베이스의 정보를 관리
class Database
{
private:
    friend class boost::serialization::access; // Boost serialization을 위한 friend 선언

    // Boost serialization 메서드
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & db_name_; // 데이터베이스 이름 직렬화
        ar & tbs_;     // 테이블 목록 직렬화
    }

    std::string db_name_;    // 데이터베이스 이름
    std::vector<Table> tbs_; // 테이블 목록

public:
    Database() {}                                    // 기본 생성자
    Database(std::string dbname);                    // 데이터베이스 이름을 받는 생성자
    ~Database() {}                                   // 소멸자
    Table *GetTable(std::string tb_name) const;      // 테이블 이름으로 테이블 객체 반환
    std::string db_name() const { return db_name_; } // 데이터베이스 이름 반환

    // 테이블 생성 및 삭제
    void CreateTable(const std::string &tableName, const std::vector<Attribute> &attrs); // 테이블 생성
    void DropTable(const std::string &tableName);                                        // 테이블 삭제
    std::vector<Table> &tbs() { return tbs_; }                                           // 테이블 목록 반환
};

// Table 클래스: 각 테이블의 정보를 관리
class Table
{
private:
    friend class boost::serialization::access; // Boost serialization을 위한 friend 선언

    // Boost serialization 메서드
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & tb_name_;           // 테이블 이름 직렬화
        ar & record_length_;     // 레코드 길이 직렬화
        ar & first_block_num_;   // 첫 번째 블록 번호 직렬화
        ar & first_rubbish_num_; // 첫 번째 삭제된 블록 번호 직렬화
        ar & block_count_;       // 블록 수 직렬화
        ar & ats_;               // 속성 목록 직렬화
    }

    std::string tb_name_;        // 테이블 이름
    int record_length_;          // 테이블의 레코드 길이
    int first_block_num_;        // 첫 번째 블록 번호
    int first_rubbish_num_;      // 삭제된 첫 번째 블록 번호
    int block_count_;            // 블록 개수
    std::vector<Attribute> ats_; // 테이블의 속성 목록

public:
    Table() : tb_name_(""), record_length_(-1), first_block_num_(-1), first_rubbish_num_(-1), block_count_(0) {} // 기본 생성자
    ~Table() {}                                                                                                  // 소멸자

    std::string tb_name() const { return tb_name_; }                   // 테이블 이름 반환
    void set_tb_name(const std::string &tbname) { tb_name_ = tbname; } // 테이블 이름 설정

    int record_length() const { return record_length_; }      // 레코드 길이 반환
    void set_record_length(int len) { record_length_ = len; } // 레코드 길이 설정

    std::vector<Attribute> &ats() { return ats_; }                     // 속성 목록 반환
    void AddAttribute(const Attribute &attr) { ats_.push_back(attr); } // 속성 추가

    Attribute *GetAttribute(const std::string &name); // 속성 이름으로 속성 반환

    // 블록 관련 함수들
    int first_block_num() const { return first_block_num_; }          // 첫 번째 블록 번호 반환
    void set_first_block_num(int num) { first_block_num_ = num; }     // 첫 번째 블록 번호 설정
    int first_rubbish_num() const { return first_rubbish_num_; }      // 첫 번째 삭제된 블록 번호 반환
    void set_first_rubbish_num(int num) { first_rubbish_num_ = num; } // 첫 번째 삭제된 블록 번호 설정
    int block_count() const { return block_count_; }                  // 블록 개수 반환
    void IncreaseBlockCount() { block_count_++; }                     // 블록 개수 증가
};

// Attribute 클래스: 각 속성의 정보를 관리
class Attribute
{
private:
    friend class boost::serialization::access; // Boost serialization을 위한 friend 선언

    // Boost serialization 메서드
    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & attr_name_; // 속성 이름 직렬화
        ar & data_type_; // 데이터 타입 직렬화
        ar & length_;    // 속성 길이 직렬화
    }

    std::string attr_name_; // 속성 이름
    int data_type_;         // 데이터 타입 (1: INT, 2: VARCHAR)
    int length_;            // 데이터 길이

public:
    Attribute() : attr_name_(""), data_type_(-1), length_(-1) {} // 기본 생성자
    ~Attribute() {}                                              // 소멸자

    std::string attr_name() const { return attr_name_; }               // 속성 이름 반환
    void set_attr_name(const std::string &name) { attr_name_ = name; } // 속성 이름 설정

    int data_type() const { return data_type_; }        // 데이터 타입 반환
    void set_data_type(int type) { data_type_ = type; } // 데이터 타입 설정

    int length() const { return length_; }      // 데이터 길이 반환
    void set_length(int len) { length_ = len; } // 데이터 길이 설정
};

#endif // MINIDB_CATALOG_MANAGER_H_
