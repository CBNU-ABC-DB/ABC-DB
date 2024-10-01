#ifndef SQLASTBUILDER_H
#define SQLASTBUILDER_H

#include "SQLBaseVisitor.h"
#include "ASTNodes.h"
#include <any> // std::any를 사용하기 위해 추가

class SQLASTBuilder : public SQLBaseVisitor {
public:
    std::any visitSql_stmt(SQLParser::Sql_stmtContext* ctx) override;
    std::any visitSelect_stmt(SQLParser::Select_stmtContext* ctx) override;
    std::any visitColumn_list(SQLParser::Column_listContext* ctx) override;
    std::any visitWhere_clause(SQLParser::Where_clauseContext* ctx) override;
    std::any visitCondition(SQLParser::ConditionContext* ctx) override;
};

#endif // SQLASTBUILDER_H
