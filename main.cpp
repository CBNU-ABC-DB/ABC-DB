#include <iostream>
#include <any> // std::any�� ����ϱ� ���� �߰�
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

// �Ľ� Ʈ���� ��������� ��ȸ�ϸ鼭 ����ϴ� �Լ�
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
    // SQL ���� �Է�
    std::string sqlQuery = "SELECT name, age FROM student WHERE age >= 20";

    ANTLRInputStream input(sqlQuery);

    // SQLLexer ���� (��ū ������)
    SQLLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    // SQLParser ����
    SQLParser parser(&tokens);

    // ���� ������ �߰�
    CustomErrorListener errorListener;
    parser.removeErrorListeners(); // ���� ���� ������ ����
    parser.addErrorListener(&errorListener); // Ŀ���� ���� ������ �߰�

    // SQL �� �Ľ� �� �Ľ� Ʈ�� ����
    tree::ParseTree* tree = parser.sql_stmt();

    // �Ľ� Ʈ�� ���
    if (tree != nullptr) {
        std::cout << "Parse Tree:" << std::endl;
        printParseTree(tree, parser);
    }
    else {
        std::cout << "Parsing failed!" << std::endl;
        return -1;
    }

    // AST ����
    SQLASTBuilder astBuilder;
    std::any result = astBuilder.visit(tree);

    // ����� AST ���
    SelectStmtNodePtr selectStmt;
    try {
        selectStmt = std::any_cast<SelectStmtNodePtr>(result);
    }
    catch (const std::bad_any_cast& e) {
        std::cerr << "Error: Failed to cast AST node. " << e.what() << std::endl;
        return -1;
    }

    // System Catalog ����
    SystemCatalog catalog;

    // �ǹ� �м�
    std::vector<std::string> semanticErrors;
    bool semanticSuccess = semanticAnalysis(*selectStmt, catalog, semanticErrors);

    if (!semanticSuccess) {
        for (const std::string& err : semanticErrors) {
            std::cerr << err << std::endl;
        }
        return -1;
    }

    // �ǹ� �м� ����
    std::cout << "Semantic analysis successful." << std::endl;

    // Execution Engine�� ���� (���⼭�� ������� ��ü)
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
