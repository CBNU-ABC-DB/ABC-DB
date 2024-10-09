#ifndef SQLSEMANTICANALYZER_H
#define SQLSEMANTICANALYZER_H

#include "ASTNodes.h"
#include "catalog_manager.h"

class SQLSemanticAnalyzer
{
public:
    SQLSemanticAnalyzer(CatalogManager &catalogMgr);

    // 각 StmtNode에 대한 Semantic Analyze 수행
    bool analyzeSelectStmt(SelectStmtNodePtr selectStmt);
    bool analyzeCreateDatabaseStmt(CreateDatabaseStmtNodePtr createDbStmt);
    bool analyzeDropDatabaseStmt(DropDatabaseStmtNodePtr dropDbStmt);
    bool analyzeDeleteTableStmt(DeleteTableStmtNodePtr deleteTableStmt);
    bool analyzeDeleteStmt(DeleteStmtNodePtr deleteStmt);

private:
    CatalogManager &catalogMgr_;

    bool checkTableExists(const std::string &tableName);
    bool checkColumnsExist(const std::string &tableName, const std::vector<std::string> &columns);
    bool checkConditionIsValid(const std::string &tableName, std::shared_ptr<ConditionNode> whereClause);
};

#endif // SQLSEMANTICANALYZER_H
