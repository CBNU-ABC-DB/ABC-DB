#include "executionEngine.h"


//db_name, table_name

// class SelectStmtNode : public ASTNode
// {
// public:
//     std::vector<std::string> columns;
//     std::shared_ptr<ConditionNode> whereClause; // WHERE 절이 있을 수 있음
// };

// ast 노드가 어떤 클래스로 구현되어있는지 확인 해야함 


// 특정 테이블을 대상으로 데이터를 검색하여 출력
void ExecutionEngine::SearchTable(const std::string& db_name, const std::string& tableName) {
    // 데이터베이스 검색
    Database* db = catalog_manager_->GetDB(db_name);
    if (db == nullptr) {
        std::cerr << "Database " << db_name << "Not Found" << std::endl;
        return;
    }

    // 테이블 검색
    Table* table = db->GetTable(tableName);
    if (table == nullptr) {
        std::cerr << "Table " << tableName << " Not Found in database " << db_name << std::endl;
        return;
    }

    std::cout << "Reading records from table: " << tableName << std::endl;

    // 버퍼 매니저를 통해 테이블의 첫 번째 페이지 가져오기
    Page* page = buffer_manager_->GetPageFromBufferPool(db_name, 0);

    // 모든 페이지를 순회하면서 데이터 출력
    while (page != nullptr) {
        std::string tableName=page->GetFileName();
        if(tableName==table->tb_name())
        {
            std::cout << "Page Index: " << page->GetPageIdx() << std::endl;
            page->PrintRecord();  // 각 페이지 내의 레코드 출력
        }
        page = page->GetNext();
    }
}

// SELECT * 기능: 특정 테이블의 모든 데이터를 출력하거나, 조건 검색 수행
void ExecutionEngine::SelectFromTable(const std::string& db_name, const std::string& tableName,
                                      const std::unordered_map<std::string, std::string>& conditions) {
    // 데이터베이스와 테이블 검색
    Database* db = catalog_manager_->GetDB(db_name);
    if (db == nullptr) {
        std::cerr << "Database " << db_name << " not found!" << std::endl;
        return;
    }

    Table* table = db->GetTable(tableName);
    if (table == nullptr) {
        std::cerr << "Table " << tableName << " not found!" << std::endl;
        return;
    }

    std::cout << "Executing SELECT on table: " << tableName << std::endl;

    // 버퍼 매니저를 사용하여 해당 테이블의 첫 번째 페이지 가져오기
    Page* page = buffer_manager_->GetPageFromBufferPool(db_name, 0);

    // 모든 페이지를 순회하며 모든 레코드를 출력 또는 조건을 만족하는 레코드만 출력
    while (page != nullptr) {
        std::cout << "Page Index: " << page->GetPageIdx() << std::endl;

        // 페이지 내의 모든 레코드 출력 (현재는 조건이 없는 경우 모든 레코드 출력)
        page->PrintRecord();  // 각 페이지의 모든 레코드 출력
        page = page->GetNext();  // 다음 페이지로 이동
    }
}
