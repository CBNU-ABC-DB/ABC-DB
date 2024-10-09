#include "SQLASTBuilder.h"

std::any SQLASTBuilder::visitSql_stmt(SQLParser::Sql_stmtContext *ctx)
{
    if (ctx->select_stmt())
    {
        return visit(ctx->select_stmt());
    }
    else if (ctx->create_db_stmt())
    {
        return visit(ctx->create_db_stmt());
    }
    else if (ctx->drop_db_stmt())
    {
        return visit(ctx->drop_db_stmt());
    }
    else if (ctx->delete_table_stmt())
    {
        return visit(ctx->delete_table_stmt());
    }
    else if (ctx->delete_stmt())
    {
        return visit(ctx->delete_stmt());
    }
    return nullptr;
}

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

std::any SQLASTBuilder::visitCreate_db_stmt(SQLParser::Create_db_stmtContext *ctx)
{
    std::string dbName = ctx->IDENTIFIER()->getText();
    return std::make_shared<CreateDatabaseStmtNode>(dbName);
}

std::any SQLASTBuilder::visitDrop_db_stmt(SQLParser::Drop_db_stmtContext *ctx)
{
    std::string dbName = ctx->IDENTIFIER()->getText();
    return std::make_shared<DropDatabaseStmtNode>(dbName);
}

std::any SQLASTBuilder::visitDelete_table_stmt(SQLParser::Delete_table_stmtContext *ctx)
{
    std::string tableName = ctx->IDENTIFIER()->getText();
    return std::make_shared<DeleteTableStmtNode>(tableName);
}

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

std::any SQLASTBuilder::visitColumn_list(SQLParser::Column_listContext *ctx)
{
    std::vector<std::string> columns;
    for (auto id : ctx->IDENTIFIER())
    {
        columns.push_back(id->getText());
    }
    return columns;
}

std::any SQLASTBuilder::visitWhere_clause(SQLParser::Where_clauseContext *ctx)
{
    return visit(ctx->condition());
}

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
        // 문자열 리터럴에서 작은 따옴표 제거
        std::string rawString = ctx->literal_value()->STRING_LITERAL()->getText();
        literalValue = rawString.substr(1, rawString.length() - 2);
        literalType = "STRING";
    }

    auto conditionNode = std::make_shared<ConditionNode>(column, comparator, literalValue, literalType);
    return conditionNode;
}
