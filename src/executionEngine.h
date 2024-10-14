#ifndef EXECUTION_ENGINE_H_
#define EXECUTION_ENGINE_H_

#include "dummy_catalog_manager.h"
#include "bufferManager.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class ExecutionEngine {
private:
    CatalogManager* catalog_manager_;
    BufferManager* buffer_manager_;

public:
    ExecutionEngine() = delete;
    // 생성자: 카탈로그 매니저와 버퍼 매니저 연결
    ExecutionEngine(CatalogManager* catalog_manager, BufferManager* buffer_manager)
        : catalog_manager_(catalog_manager), buffer_manager_(buffer_manager) {}
    // ExecutionEngine() 빈원
    //     :catalog_manager_(new catalog_manager_), buffer_manager_(new buffer_manager_)

    // 특정 데이터베이스와 테이블을 읽어오는 함수
    void SearchTable(const std::string& db_name, const std::string& tableName);

    // 전체 데이터베이스와 테이블 정보를 출력하는 함수
    void ShowAllTables();

    // SELECT 
    void SelectFromTable(const std::string& db_name, const std::string& tableName,
                         const std::unordered_map<std::string, std::string>& conditions = {});
};

#endif  // EXECUTION_ENGINE_H_

