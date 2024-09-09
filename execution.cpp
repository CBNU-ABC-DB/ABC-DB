#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <cstring>  // strdup 사용을 위해 필요

struct Row {
    std::unordered_map<std::string, std::string> data;  // 컬럼명과 데이터 매핑
};

struct Page {
    void* header;       // 페이지 헤더
    char** records;     // 레코드 배열 
    int numRecords;     // 페이지 내 레코드 개수
};

// AST 노드 정의 (AST 생성 모듈에서 동일하게 사용해야 함)
struct ASTNode {
    std::string nodeName;
    std::vector<ASTNode*> children;

    ASTNode(const std::string& name) : nodeName(name) {}

    void addChild(ASTNode* child) {
        children.push_back(child);
    }
};

// AST에서 열 목록을 파싱하는 함수
std::vector<std::string> parseColumns(ASTNode* ast) {
    std::vector<std::string> columns;
    for (ASTNode* child : ast->children) {
        if (child->nodeName == "COLUMNS") {
            for (ASTNode* column : child->children) {
                columns.push_back(column->nodeName);
            }
        }
    }
    return columns;
}

// AST에서 테이블 이름을 파싱하는 함수
std::string parseTable(ASTNode* ast) {
    for (ASTNode* child : ast->children) {
        if (child->nodeName == "TABLE") {
            return child->children[0]->nodeName;
        }
    }
    return "";
}

// 테이블 스캔 연산자
class TableScanOperator {
public:
    TableScanOperator(const std::vector<Page>& pages) : pages_(pages), currentPage_(0), currentRecord_(0) {}

    // 페이지에서 다음 레코드를 가져오는 함수
    bool next(Row& row, const std::vector<std::string>& selectedColumns) {
        while (currentPage_ < pages_.size()) {
            if (currentRecord_ < pages_[currentPage_].numRecords) {
                
                // 현재 페이지의 현재 레코드를 읽어서 Row로 변환하는 부분 추가 필요

            }
            // 다음 페이지로 이동
            currentPage_++;
            currentRecord_ = 0;
        }
        return false;  // 더 이상 읽을 레코드가 없음
    }

private:
    std::vector<Page> pages_;  // 페이지 목록
    size_t currentPage_;       // 현재 페이지 인덱스
    size_t currentRecord_;     // 현재 페이지 내 레코드 인덱스
};

// 실행 엔진 함수
void runSelectEngine(ASTNode* ast, const std::vector<Page>& pages) {
    // AST에서 SELECT 절 파싱
    std::vector<std::string> selectedColumns = parseColumns(ast);
    std::string tableName = parseTable(ast);

    // 테이블 스캔 연산자 초기화
    TableScanOperator table_scan(pages);

    // 테이블의 모든 행을 순차적으로 읽어서 출력
    Row row;
    while (table_scan.next(row, selectedColumns)) {
        for (const auto& col : selectedColumns) {
            std::cout << col << ": " << row.data[col] << ", ";
        }
        std::cout << std::endl;
    }
}

// 외부에서 데이터를 메모리로 로드하는 부분
std::vector<Page> loadData() {
    

}

int main() {
    // 외부로부터 데이터를 메모리로 로드
    std::vector<Page> pages = loadData();

    // AST 트리를 전달받았다고 가정
    ASTNode* ast = new ASTNode("SELECT");
    ASTNode* columnsNode = new ASTNode("COLUMNS");
    columnsNode->addChild(new ASTNode("name"));
    columnsNode->addChild(new ASTNode("age"));
    ASTNode* tableNode = new ASTNode("TABLE");
    tableNode->addChild(new ASTNode("students"));
    ast->addChild(columnsNode);
    ast->addChild(tableNode);

    // 실행 엔진 실행
    runSelectEngine(ast, pages);

    delete ast;
}
