#ifndef SQL_STATEMENT_VISITOR_H_
#define SQL_STATEMENT_VISITOR_H_

#include "SQLBaseVisitor.h"
#include "sql_statement.h"
#include "antlr4-runtime.h"

class SQLStatementVisitor : public SQLBaseVisitor
{
public:
    virtual antlrcpp::Any visitSqlStatement(SQLParser::SqlStatementContext *ctx) override;
    virtual antlrcpp::Any visitCreateDatabase(SQLParser::CreateDatabaseContext *ctx) override;
    virtual antlrcpp::Any visitCreateTable(SQLParser::CreateTableContext *ctx) override;
    virtual antlrcpp::Any visitUseDatabase(SQLParser::UseDatabaseContext *ctx) override;
    virtual antlrcpp::Any visitInsertInto(SQLParser::InsertIntoContext *ctx) override;
    virtual antlrcpp::Any visitSelectStatement(SQLParser::SelectStatementContext *ctx) override;
    virtual antlrcpp::Any visitExecStatement(SQLParser::ExecStatementContext *ctx) override;
    virtual antlrcpp::Any visitShowDatabases(SQLParser::ShowDatabasesContext *ctx) override;
    virtual antlrcpp::Any visitShowTables(SQLParser::ShowTablesContext *ctx) override;
    virtual antlrcpp::Any visitDescTable(SQLParser::DescTableContext *ctx) override;
    virtual antlrcpp::Any visitHelpStatement(SQLParser::HelpStatementContext *ctx) override;
    virtual antlrcpp::Any visitQuitStatement(SQLParser::QuitStatementContext *ctx) override;
    virtual antlrcpp::Any visitTestRecordStatement(SQLParser::TestRecordStatementContext *ctx) override;
    virtual antlrcpp::Any visitTestBufferPoolStatement(SQLParser::TestBufferPoolStatementContext *ctx) override;
};

#endif // SQL_STATEMENT_VISITOR_H_