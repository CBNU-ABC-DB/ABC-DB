#include "dummy_catalog_manager.h"  // dummy_catalog_manager 헤더 포함
#include "bufferManager.h"
#include "executionEngine.h"
#include <iostream>
#include <memory>

int main() {
    // 1. 기본 생성자를 사용하여 더미 카탈로그 매니저 생성
    CatalogManager catalog;

    // 2. 데이터베이스 및 테이블 생성 (테스트용)
    catalog.CreateDatabase("student_db");
    catalog.CreateTable("student_db", "student_table");

    // 3. 버퍼 매니저 초기화 (기존과 동일)
    BufferManager buffer_manager("student.bin");

    // 4. ExecutionEngine 생성자를 사용하여 객체 생성 (포인터 전달)
    ExecutionEngine engine(&catalog, &buffer_manager);

    // 5. 전체 데이터베이스 및 테이블 출력 (테스트용)
    std::cout << "\n==== Show All Tables ====" << std::endl;
    engine.ShowAllTables();

    // 6. SELECT * 기능 테스트
    std::cout << "\n==== Execute SELECT * on Table ====" << std::endl;
    engine.SelectFromTable("student_db", "student_table");

    return 0;
}