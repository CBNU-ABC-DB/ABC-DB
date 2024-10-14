#include "dummy_catalog_manager.h"

// 데이터베이스 검색
Database* CatalogManager::GetDB(const std::string& db_name) {
    for (auto& db : dbs_) {
        if (db.db_name() == db_name) {
            return &db;
        }
    }
    return nullptr;
}

// 데이터베이스 생성 함수
void CatalogManager::CreateDatabase(const std::string& db_name) {
    dbs_.push_back(Database(db_name));
}

// 테이블 생성 함수
void CatalogManager::CreateTable(const std::string& db_name, const std::string& table_name) {
    for (auto& db : dbs_) {
        if (db.db_name() == db_name) {
            db.AddTable(Table(table_name));
            return;
        }
    }
}

// 데이터베이스 내 테이블 검색
Table* Database::GetTable(const std::string& tb_name) {
    for (auto& table : tables_) {
        if (table.tb_name() == tb_name) {
            return &table;
        }
    }
    return nullptr;
}
