#include <iostream>
#include <any> // std::any를 사용하기 위해 추가
#include "antlr4-runtime.h"
#include "SQLLexer.h"
#include "SQLParser.h"
#include "ASTNodes.h"
#include "SQLASTBuilder.h"
#include "SystemCatalog.h"
#include "SemanticAnalyzer.h"

using namespace antlr4;

class CustomErrorListener : public BaseErrorListener {
public:
    void syntaxError(Recognizer* recognizer, Token* offendingSymbol, size_t line,
        size_t charPositionInLine, const std::string& msg, std::exception_ptr e) override {
        std::cerr << "Syntax Error at line " << line << ", column " << charPositionInLine << ": " << msg << std::endl;
    }
};

// 파싱 트리를 재귀적으로 순회하면서 출력하는 함수
void printParseTree(tree::ParseTree* tree, const SQLParser& parser, int depth = 0) {
    std::string indent(depth * 2, ' ');
    std::string nodeText = tree->getText();

    if (auto* ruleContext = dynamic_cast<RuleContext*>(tree)) {
        std::cout << indent << "Rule: " << parser.getRuleNames()[ruleContext->getRuleIndex()]
            << " -> " << nodeText << std::endl;
    }
    else {
        std::cout << indent << "Terminal: " << nodeText << std::endl;
    }

    for (size_t i = 0; i < tree->children.size(); ++i) {
        printParseTree(tree->children[i], parser, depth + 1);
    }
}

int main(int argc, const char* argv[]) {
    // SQL 구문 입력
    std::string sqlQuery = "SELECT name, age FROM student WHERE age >= 20";

    ANTLRInputStream input(sqlQuery);

    // SQLLexer 생성 (토큰 생성기)
    SQLLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    // SQLParser 생성
    SQLParser parser(&tokens);

    // 에러 리스너 추가
    CustomErrorListener errorListener;
    parser.removeErrorListeners(); // 기존 에러 리스너 제거
    parser.addErrorListener(&errorListener); // 커스텀 에러 리스너 추가

    // SQL 문 파싱 및 파싱 트리 생성
    tree::ParseTree* tree = parser.sql_stmt();

    // 파싱 트리 출력
    if (tree != nullptr) {
        std::cout << "Parse Tree:" << std::endl;
        printParseTree(tree, parser);
    }
    else {
        std::cout << "Parsing failed!" << std::endl;
        return -1;
    }

    // AST 빌드
    SQLASTBuilder astBuilder;
    std::any result = astBuilder.visit(tree);

    // 추출된 AST 노드
    SelectStmtNodePtr selectStmt;
    try {
        selectStmt = std::any_cast<SelectStmtNodePtr>(result);
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "Error: Failed to cast AST node. " << e.what() << std::endl;
        return -1;
    }

    // System Catalog 생성
    SystemCatalog catalog;

    // 의미 분석
    std::vector<std::string> semanticErrors;
    bool semanticSuccess = semanticAnalysis(*selectStmt, catalog, semanticErrors);

    if (!semanticSuccess) {
        for (const std::string& err : semanticErrors) {
            std::cerr << err << std::endl;
        }
        return -1;
    }

    // 의미 분석 성공
    std::cout << "Semantic analysis successful." << std::endl;

    // Execution Engine에 전달 (여기서는 출력으로 대체)
    std::cout << "Executing query..." << std::endl;
    std::cout << "Table: " << selectStmt->tableName << std::endl;
    std::cout << "Columns: ";
    for (const auto& col : selectStmt->columns) {
        std::cout << col << " ";
    }
    std::cout << std::endl;

    if (selectStmt->whereClause) {
        std::cout << "Where: " << selectStmt->whereClause->column << " "
            << selectStmt->whereClause->comparator << " "
            << selectStmt->whereClause->literalValue << std::endl;
    }

    return 0;
}
