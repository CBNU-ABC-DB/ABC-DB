#include "catalog_manager.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace std;

//=======================CatalogManager=======================//

// 생성자: 데이터베이스 경로를 초기화하고, 기존 데이터베이스 메타데이터를 로드
CatalogManager::CatalogManager(std::string p) : path_(p)
{
    ReadArchiveFile(); // 데이터베이스 정보를 파일로부터 읽어옴
}

// 소멸자: 객체가 파괴될 때, 데이터베이스 정보를 저장
CatalogManager::~CatalogManager()
{
    WriteArchiveFile(); // 데이터베이스 정보를 파일에 저장
}

// 데이터베이스 정보를 파일에서 읽어오는 함수
void CatalogManager::ReadArchiveFile()
{
    std::string file_name = path_ + "catalog"; // 파일 경로 설정
    boost::filesystem::path file_path(file_name);

    // 파일이 존재하는지 확인한 후, 존재하면 파일을 읽어옴
    if (boost::filesystem::exists(file_path))
    {
        std::ifstream ifs(file_name, std::ios::binary); // 파일을 바이너리 모드로 엶
        boost::archive::binary_iarchive iar(ifs);       // Boost 아카이브 객체 생성
        iar >> (*this);                                 // 데이터를 역직렬화하여 CatalogManager 객체에 로드
        ifs.close();                                    // 파일 닫기
    }
}

// 데이터베이스 정보를 파일에 저장하는 함수
void CatalogManager::WriteArchiveFile()
{
    std::string file_name = path_ + "catalog";      // 파일 경로 설정
    std::ofstream ofs(file_name, std::ios::binary); // 파일을 바이너리 모드로 엶
    boost::archive::binary_oarchive oar(ofs);       // Boost 아카이브 객체 생성
    oar << (*this);                                 // 데이터를 직렬화하여 파일에 저장
    ofs.close();                                    // 파일 닫기
}

// 데이터베이스 이름을 받아 해당 데이터베이스를 반환하는 함수
Database *CatalogManager::GetDB(std::string db_name) const
{
    // 데이터베이스 목록에서 이름이 일치하는 데이터베이스를 찾아 반환
    for (const Database &db : dbs_)
    {
        if (db.db_name() == db_name)
        {
            return const_cast<Database *>(&db); // const 객체를 mutable로 반환
        }
    }
    return nullptr; // 찾지 못한 경우 nullptr 반환
}

// CREATE DATABASE: 데이터베이스가 존재하는지 확인
bool CatalogManager::doesDatabaseExist(const std::string &dbname) const
{
    return GetDB(dbname) != nullptr; // 데이터베이스가 존재하면 true 반환
}

// CREATE DATABASE: 데이터베이스가 없으면 생성
void CatalogManager::createDatabase(const std::string &dbname)
{
    if (!doesDatabaseExist(dbname))
    {
        dbs_.emplace_back(dbname); // 데이터베이스 목록에 새 데이터베이스 추가
    }
}

// DROP DATABASE: 데이터베이스를 삭제
void CatalogManager::dropDatabase(const std::string &dbname)
{
    // 데이터베이스 목록에서 주어진 이름을 가진 데이터베이스 삭제
    dbs_.erase(std::remove_if(dbs_.begin(), dbs_.end(), [&dbname](const Database &db)
                              { return db.db_name() == dbname; }),
               dbs_.end());
}

// CREATE TABLE: 주어진 테이블 생성 AST 노드를 처리
void CatalogManager::HandleCreateTableStmt(CreateTableStmtNodePtr createStmt)
{
    Database *db = GetDB("default"); // 기본 데이터베이스 사용
    if (db)
    {
        Table tb;                              // 새로운 테이블 객체 생성
        tb.set_tb_name(createStmt->tableName); // 테이블 이름 설정
        // 각 컬럼에 대한 속성을 설정
        for (const auto &col : createStmt->columns)
        {
            Attribute attr;
            attr.set_attr_name(col.first); // 컬럼 이름 설정
            if (col.second == "INT")
            {
                attr.set_data_type(1); // INT 데이터 타입 설정
                attr.set_length(4);    // INT 데이터 타입 크기 설정
            }
            else if (col.second.find("VARCHAR") != std::string::npos)
            {
                attr.set_data_type(2);                                                 // VARCHAR 데이터 타입 설정
                int length = std::stoi(col.second.substr(8, col.second.length() - 9)); // VARCHAR 길이 추출
                attr.set_length(length);                                               // VARCHAR 길이 설정
            }
            tb.AddAttribute(attr); // 테이블에 속성 추가
        }
        db->tbs().push_back(tb); // 테이블을 데이터베이스에 추가
    }
}

// DROP TABLE: 주어진 테이블 삭제 AST 노드를 처리
void CatalogManager::HandleDropTableStmt(DropTableStmtNodePtr dropStmt)
{
    Database *db = GetDB("default"); // 기본 데이터베이스 사용
    if (db)
    {
        db->DropTable(dropStmt->tableName); // 테이블 삭제
    }
}

// DELETE: 주어진 삭제 문을 처리
void CatalogManager::HandleDeleteStmt(DeleteStmtNodePtr deleteStmt)
{
    Database *db = GetDB("default"); // 기본 데이터베이스 사용
    if (!db)
        return;

    Table *table = db->GetTable(deleteStmt->tableName); // 삭제할 테이블 찾기
    if (table)
    {
        if (deleteStmt->whereClause) // 조건절이 있는 경우
        {
            cout << "Deleting rows from " << deleteStmt->tableName
                 << " where " << deleteStmt->whereClause->column
                 << " " << deleteStmt->whereClause->comparator
                 << " " << deleteStmt->whereClause->literalValue << endl;
            // 실제로 조건에 맞는 행을 삭제하는 로직은 여기 구현
        }
        else // 조건이 없는 경우
        {
            cout << "Deleting all rows from " << deleteStmt->tableName << endl;
            // 테이블의 모든 행을 삭제하는 로직은 여기 구현
        }
    }
}

//=======================Database=============================//

// 생성자: 데이터베이스 이름을 초기화
Database::Database(std::string dbname) : db_name_(dbname) {}

// 테이블 생성 함수: 주어진 테이블 이름과 속성 목록으로 테이블 생성
void Database::CreateTable(const std::string &tableName, const std::vector<Attribute> &attrs)
{
    Table tb;                  // 새로운 테이블 객체 생성
    tb.set_tb_name(tableName); // 테이블 이름 설정
    tb.ats() = attrs;          // 속성 목록 설정
    tbs_.push_back(tb);        // 테이블을 데이터베이스에 추가
}

// 테이블 삭제 함수: 주어진 테이블 이름에 해당하는 테이블을 삭제
void Database::DropTable(const std::string &tableName)
{
    tbs_.erase(std::remove_if(tbs_.begin(), tbs_.end(), [&tableName](Table &t)
                              { return t.tb_name() == tableName; }),
               tbs_.end());
}

// 테이블 반환 함수: 주어진 이름을 가진 테이블을 반환
Table *Database::GetTable(std::string tb_name) const
{
    for (const Table &tb : tbs_) // const 테이블로 반복
    {
        if (tb.tb_name() == tb_name) // 테이블 이름 비교
        {
            return const_cast<Table *>(&tb); // const 객체를 mutable로 반환
        }
    }
    return nullptr; // 테이블을 찾지 못한 경우 nullptr 반환
}

//=======================Table===============================//

// 속성 반환 함수: 주어진 이름을 가진 속성을 반환
Attribute *Table::GetAttribute(const std::string &name)
{
    for (Attribute &attr : ats_) // 속성 목록에서 검색
    {
        if (attr.attr_name() == name) // 이름이 일치하는 속성 찾기
        {
            return &attr; // 속성 반환
        }
    }
    return nullptr; // 속성을 찾지 못한 경우 nullptr 반환
}

// 테이블 삭제 함수 정의: 기본 데이터베이스에서 테이블 삭제
void CatalogManager::deleteTable(const std::string &tableName)
{
    Database *db = GetDB("default"); // 기본 데이터베이스 사용
    if (db)
    {
        db->DropTable(tableName); // 테이블 삭제
        std::cout << "Table " << tableName << " deleted from default database." << std::endl;
    }
    else
    {
        std::cout << "Default database not found." << std::endl; // 기본 DB가 없을 경우 에러 출력
    }
}

// 데이터 삭제 함수 정의: 주어진 조건에 맞는 데이터를 삭제
void CatalogManager::deleteData(const std::string &tableName, const std::shared_ptr<ConditionNode> &whereClause)
{
    Database *db = GetDB("default"); // 기본 데이터베이스 사용
    if (!db)
        return;

    Table *table = db->GetTable(tableName); // 삭제할 테이블 찾기
    if (!table)
    {
        std::cout << "Table " << tableName << " not found in default database." << std::endl;
        return;
    }

    if (whereClause) // 조건이 있는 경우
    {
        std::cout << "Deleting data from table " << tableName
                  << " where " << whereClause->column
                  << " " << whereClause->comparator
                  << " " << whereClause->literalValue << std::endl;
        // 조건에 맞는 데이터를 삭제하는 로직은 여기 추가
    }
    else // 조건이 없는 경우
    {
        std::cout << "Deleting all data from table " << tableName << std::endl;
        // 모든 데이터를 삭제하는 로직은 여기 추가
    }
}
