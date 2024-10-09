#include "SQLASTBuilder.h"

// sql_stmt 방문: 여러 SQL 문에 대한 진입점
std::any SQLASTBuilder::visitSql_stmt(SQLParser::Sql_stmtContext *ctx)
{
    if (ctx->select_stmt())
    {
        return visit(ctx->select_stmt());
    }
    else if (ctx->create_stmt())
    {
        return visit(ctx->create_stmt());
    }
    else if (ctx->drop_stmt())
    {
        return visit(ctx->drop_stmt());
    }
    else if (ctx->delete_stmt())
    {
        return visit(ctx->delete_stmt());
    }
    else if (ctx->delete_table_stmt())
    {
        return visit(ctx->delete_table_stmt());
    }
    else if (ctx->insert_stmt())
    {
        return visit(ctx->insert_stmt());
    }
    else if (ctx->create_db_stmt())
    {
        return visit(ctx->create_db_stmt());
    }
    else if (ctx->drop_db_stmt())
    {
        return visit(ctx->drop_db_stmt());
    }

    return nullptr;
}

// SELECT 문 방문
std::any SQLASTBuilder::visitSelect_stmt(SQLParser::Select_stmtContext *ctx)
{
    auto selectNode = std::make_shared<SelectStmtNode>();
    selectNode->columns = std::any_cast<std::vector<std::string>>(visit(ctx->column_list()));
    selectNode->tableName = ctx->IDENTIFIER()->getText();

    if (ctx->where_clause())
    {
        selectNode->whereClause = std::any_cast<std::shared_ptr<ConditionNode>>(visit(ctx->where_clause()));
    }

    return selectNode;
}

// CREATE 문 방문 (TABLE 또는 DATABASE)
std::any SQLASTBuilder::visitCreate_stmt(SQLParser::Create_stmtContext *ctx)
{
    if (ctx->IDENTIFIER())
    {                                                         // CREATE TABLE
        std::string tableName = ctx->IDENTIFIER()->getText(); // 테이블 이름 가져오기
        auto columns = std::any_cast<std::vector<std::pair<std::string, std::string>>>(visit(ctx->column_def_list()));
        return std::make_shared<CreateTableStmtNode>(tableName, columns);
    }
    else
    {                                                      // CREATE DATABASE
        std::string dbName = ctx->IDENTIFIER()->getText(); // 데이터베이스 이름 가져오기
        return std::make_shared<CreateDatabaseStmtNode>(dbName);
    }
}

// DROP 문 방문 (TABLE 또는 DATABASE)
std::any SQLASTBuilder::visitDrop_stmt(SQLParser::Drop_stmtContext *ctx)
{
    if (ctx->IDENTIFIER())
    {                                                         // DROP TABLE
        std::string tableName = ctx->IDENTIFIER()->getText(); // 테이블 이름 가져오기
        return std::make_shared<DropTableStmtNode>(tableName);
    }
    else
    {                                                      // DROP DATABASE
        std::string dbName = ctx->IDENTIFIER()->getText(); // 데이터베이스 이름 가져오기
        return std::make_shared<DropDatabaseStmtNode>(dbName);
    }
}

// DELETE 문 방문 (데이터 삭제)
std::any SQLASTBuilder::visitDelete_stmt(SQLParser::Delete_stmtContext *ctx)
{
    std::string tableName = ctx->IDENTIFIER()->getText();
    std::shared_ptr<ConditionNode> whereClause = nullptr;

    if (ctx->where_clause())
    {
        whereClause = std::any_cast<std::shared_ptr<ConditionNode>>(visit(ctx->where_clause()));
    }

    return std::make_shared<DeleteStmtNode>(tableName, whereClause);
}

// DELETE TABLE 문 방문 (테이블 삭제)
std::any SQLASTBuilder::visitDelete_table_stmt(SQLParser::Delete_table_stmtContext *ctx)
{
    std::string tableName = ctx->IDENTIFIER()->getText();
    return std::make_shared<DeleteTableStmtNode>(tableName);
}

// INSERT INTO 문 방문
std::any SQLASTBuilder::visitInsert_stmt(SQLParser::Insert_stmtContext *ctx)
{
    std::string tableName = ctx->IDENTIFIER()->getText();
    auto columns = std::any_cast<std::vector<std::string>>(visit(ctx->column_list()));
    auto values = std::any_cast<std::vector<std::string>>(visit(ctx->value_list()));

    return std::make_shared<InsertStmtNode>(tableName, columns, values);
}

// 컬럼 목록 방문
std::any SQLASTBuilder::visitColumn_list(SQLParser::Column_listContext *ctx)
{
    std::vector<std::string> columns;
    for (auto id : ctx->IDENTIFIER())
    {
        columns.push_back(id->getText());
    }
    return columns;
}

// 값 목록 방문
std::any SQLASTBuilder::visitValue_list(SQLParser::Value_listContext *ctx)
{
    std::vector<std::string> values;
    for (auto val : ctx->literal_value())
    {
        values.push_back(val->getText());
    }
    return values;
}

// WHERE 절 방문
std::any SQLASTBuilder::visitWhere_clause(SQLParser::Where_clauseContext *ctx)
{
    return visit(ctx->condition());
}

// 조건절 방문 (조건문 파싱)
std::any SQLASTBuilder::visitCondition(SQLParser::ConditionContext *ctx)
{
    std::string column = ctx->IDENTIFIER()->getText();
    std::string comparator = ctx->comparator()->getText();
    std::string literalValue;
    std::string literalType;

    if (ctx->literal_value()->NUMBER())
    {
        literalValue = ctx->literal_value()->NUMBER()->getText();
        literalType = "NUMBER";
    }
    else if (ctx->literal_value()->STRING_LITERAL())
    {
        std::string rawString = ctx->literal_value()->STRING_LITERAL()->getText();
        literalValue = rawString.substr(1, rawString.length() - 2); // 작은 따옴표 제거
        literalType = "STRING";
    }

    auto conditionNode = std::make_shared<ConditionNode>(column, comparator, literalValue, literalType);
    return conditionNode;
}

// 컬럼 정의 목록 방문
std::any SQLASTBuilder::visitColumn_def_list(SQLParser::Column_def_listContext *ctx)
{
    std::vector<std::pair<std::string, std::string>> columns;
    for (auto def : ctx->column_def())
    {
        columns.push_back(std::any_cast<std::pair<std::string, std::string>>(visit(def)));
    }
    return columns;
}

// 컬럼 정의 방문
std::any SQLASTBuilder::visitColumn_def(SQLParser::Column_defContext *ctx)
{
    std::string columnName = ctx->IDENTIFIER()->getText();
    std::string dataType = ctx->data_type()->getText();
    return std::make_pair(columnName, dataType);
}
