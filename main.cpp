#include <iostream>
#include <any>
#include "antlr4-runtime.h"
#include "SQLLexer.h"
#include "SQLParser.h"
#include "ASTNodes.h"
#include "SQLASTBuilder.h"
#include "SQLSemanticAnalyzer.h"
#include "catalog_manager.h"

using namespace antlr4;

class CustomErrorListener : public BaseErrorListener
{
public:
    void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                     size_t charPositionInLine, const std::string &msg, std::exception_ptr e) override
    {
        std::cerr << "Syntax Error at line " << line << ", column " << charPositionInLine << ": " << msg << std::endl;
    }
};

int main(int argc, const char *argv[])
{
    // 무한 입력 루프
    while (true)
    {
        std::string sqlQuery;
        std::cout << "Enter SQL query (or type 'exit' to quit): ";
        std::getline(std::cin, sqlQuery);

        // 종료 명령어 처리
        if (sqlQuery == "exit")
        {
            break;
        }

        try
        {
            ANTLRInputStream input(sqlQuery);

            // SQLLexer 생성 (토큰 생성기)
            SQLLexer lexer(&input);
            CommonTokenStream tokens(&lexer);

            // SQLParser 생성
            SQLParser parser(&tokens);

            // 에러 리스너 추가
            CustomErrorListener errorListener;
            parser.removeErrorListeners();           // 기존 에러 리스너 제거
            parser.addErrorListener(&errorListener); // 커스텀 에러 리스너 추가

            // SQL 문 파싱 및 파싱 트리 생성
            tree::ParseTree *tree = parser.sql_stmt();

            // 파싱 트리 출력 및 AST 빌드
            SQLASTBuilder astBuilder;
            std::any result = astBuilder.visit(tree);

            // 추출된 AST 노드
            SelectStmtNodePtr selectStmt;
            try
            {
                selectStmt = std::any_cast<SelectStmtNodePtr>(result);
            }
            catch (const std::bad_any_cast &e)
            {
                std::cerr << "Error: Failed to cast AST node. " << e.what() << std::endl;
                continue; // 에러 발생 시 계속 입력을 받을 수 있도록 함
            }

            // CatalogManager 초기화 (파일 경로)
            CatalogManager catalogManager("path/to/catalog");

            // Semantic Analyze 수행
            SQLSemanticAnalyzer semanticAnalyzer(catalogManager);
            if (!semanticAnalyzer.analyzeSelectStmt(selectStmt))
            {
                std::cerr << "Semantic analysis failed." << std::endl;
                continue; // 에러 발생 시 계속 입력을 받을 수 있도록 함
            }

            // 성공적으로 분석되었을 때 출력
            std::cout << "Semantic analysis successful." << std::endl;
            std::cout << "Table: " << selectStmt->tableName << std::endl;
            std::cout << "Columns: ";
            for (const auto &col : selectStmt->columns)
            {
                std::cout << col << " ";
            }
            std::cout << std::endl;

            if (selectStmt->whereClause)
            {
                std::cout << "Where: " << selectStmt->whereClause->column << " "
                          << selectStmt->whereClause->comparator << " "
                          << selectStmt->whereClause->literalValue << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
    }

    return 0;
}
