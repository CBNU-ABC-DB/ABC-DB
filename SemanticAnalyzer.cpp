#include "SemanticAnalyzer.h"

bool semanticAnalysis(const SelectStmtNode& selectStmt, const SystemCatalog& catalog, std::vector<std::string>& errors) {
    bool success = true;

    // 테이블 존재 여부 확인
    if (!catalog.tableExists(selectStmt.tableName)) {
        errors.push_back("Semantic Error: Table '" + selectStmt.tableName + "' does not exist.");
        success = false;
    }
    else {
        // 각 컬럼 존재 여부 확인
        for (const std::string& column : selectStmt.columns) {
            if (!catalog.columnExists(selectStmt.tableName, column)) {
                errors.push_back("Semantic Error: Column '" + column + "' does not exist in table '" + selectStmt.tableName + "'.");
                success = false;
            }
        }

        // WHERE 절의 데이터 타입 일치 여부 확인
        if (selectStmt.whereClause) {
            const auto& condition = selectStmt.whereClause;

            // 컬럼 존재 여부 확인
            if (!catalog.columnExists(selectStmt.tableName, condition->column)) {
                errors.push_back("Semantic Error: Column '" + condition->column + "' does not exist in table '" + selectStmt.tableName + "'.");
                success = false;
            }
            else {
                // 데이터 타입 일치 여부 확인
                std::string columnType = catalog.getColumnType(selectStmt.tableName, condition->column);
                std::string literalType = condition->literalType;

                // 데이터 타입 매핑 (단순화된 예시)
                auto mapType = [](const std::string& type) -> std::string {
                    if (type == "INT" || type == "NUMBER") return "NUMBER";
                    if (type == "VARCHAR" || type == "STRING") return "STRING";
                    return type;
                };

                if (mapType(columnType) != mapType(literalType)) {
                    errors.push_back("Semantic Error: Type mismatch between column '" + condition->column + "' (" + columnType +
                        ") and literal value '" + condition->literalValue + "' (" + literalType + ").");
                    success = false;
                }
            }
        }
    }

    return success;
}
