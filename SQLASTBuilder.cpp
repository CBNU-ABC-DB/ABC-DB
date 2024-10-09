#include "SQLASTBuilder.h"

std::any SQLASTBuilder::visitSql_stmt(SQLParser::Sql_stmtContext *ctx)
{
    return visit(ctx->select_stmt());
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