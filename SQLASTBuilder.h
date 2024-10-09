#ifndef SQLASTBUILDER_H
#define SQLASTBUILDER_H

#include "SQLBaseVisitor.h"
#include "ASTNodes.h"
#include <any> // std::any를 사용하기 위해 추가

class SQLASTBuilder : public SQLBaseVisitor
{
public:
    std::any visitSql_stmt(SQLParser::Sql_stmtContext *ctx) override;
    std::any visitSelect_stmt(SQLParser::Select_stmtContext *ctx) override;
    std::any visitCreate_stmt(SQLParser::Create_stmtContext *ctx) override;
    std::any visitDrop_stmt(SQLParser::Drop_stmtContext *ctx) override;
    std::any visitDelete_stmt(SQLParser::Delete_stmtContext *ctx) override;
    std::any visitDelete_table_stmt(SQLParser::Delete_table_stmtContext *ctx) override;
    std::any visitInsert_stmt(SQLParser::Insert_stmtContext *ctx) override;
    std::any visitColumn_list(SQLParser::Column_listContext *ctx) override;
    std::any visitValue_list(SQLParser::Value_listContext *ctx) override;
    std::any visitWhere_clause(SQLParser::Where_clauseContext *ctx) override;
    std::any visitCondition(SQLParser::ConditionContext *ctx) override;
    std::any visitColumn_def_list(SQLParser::Column_def_listContext *ctx) override;
    std::any visitColumn_def(SQLParser::Column_defContext *ctx) override;
};

#endif // SQLASTBUILDER_H
