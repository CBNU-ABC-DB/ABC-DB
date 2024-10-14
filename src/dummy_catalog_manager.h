#ifndef DUMMY_CATALOG_MANAGER_H_
#define DUMMY_CATALOG_MANAGER_H_

#include <iostream>
#include <vector>
#include <string>

// 더미 데이터베이스 및 테이블 클래스 정의
class Table {
public:
    std::string tb_name_;
    Table(std::string name) : tb_name_(name) {}
    std::string tb_name() const { return tb_name_; }
};

class Database {
public:
    std::string db_name_;
    std::vector<Table> tables_;
    Database(std::string name) : db_name_(name) {}
    void AddTable(Table table) { tables_.push_back(table); }
    std::string db_name() const { return db_name_; }
    Table* GetTable(const std::string& tb_name);
};

class CatalogManager {
public:
    std::vector<Database> dbs_;
    CatalogManager() = default;  // 기본 생성자 사용
    CatalogManager(const std::string&) {}  // 인자 받는 생성자 무시 (에러 방지용)

    // 데이터베이스 생성
    void CreateDatabase(const std::string& db_name);
    
    // 테이블 생성
    void CreateTable(const std::string& db_name, const std::string& table_name);

    // 데이터베이스 검색
    Database* GetDB(const std::string& db_name);

    // 모든 데이터베이스 가져오기
    const std::vector<Database>& GetDBs() const { return dbs_; }
};

#endif  // DUMMY_CATALOG_MANAGER_H_
