#include "sql_statement_visitor.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "commons.h"
#include "exceptions.h"

using namespace std;

antlrcpp::Any SQLStatementVisitor::visitSqlStatement(SQLParser::SqlStatementContext *ctx)
{
    // SQL 문장을 방문하고, 하위 노드를 방문하여 SQL 객체를 생성
    return visitChildren(ctx);
}

antlrcpp::Any SQLStatementVisitor::visitCreateDatabase(SQLParser::CreateDatabaseContext *ctx)
{
    SQLCreateDatabase *stmt = new SQLCreateDatabase();
    stmt->set_db_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitDropDatabase(SQLParser::DropDatabaseContext *ctx)
{
    SQLDropDatabase *stmt = new SQLDropDatabase();
    stmt->set_db_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitCreateTable(SQLParser::CreateTableContext *ctx)
{
    SQLCreateTable *stmt = new SQLCreateTable();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    std::vector<Attribute> attrs;

    // 컬럼 정의를 처리
    for (auto colDefCtx : ctx->columnDefinition())
    {
        Attribute attr;
        attr.set_attr_name(colDefCtx->IDENTIFIER()->getText());

        // 데이터 타입을 설정
        if (colDefCtx->dataType()->INT())
        {
            attr.set_data_type(T_INT);
            attr.set_length(4);
        }
        else if (colDefCtx->dataType()->FLOAT())
        {
            attr.set_data_type(T_FLOAT);
            attr.set_length(4);
        }
        else if (colDefCtx->dataType()->CHAR())
        {
            attr.set_data_type(T_CHAR);
            std::string lenStr = colDefCtx->dataType()->NUMERIC_LITERAL()->getText();
            attr.set_length(std::atoi(lenStr.c_str()));
        }
        else
        {
            throw SyntaxErrorException();
        }

        attrs.push_back(attr);
    }

    // 기본 키 정의를 처리
    if (ctx->primaryKeyDefinition())
    {
        std::string pkName = ctx->primaryKeyDefinition()->IDENTIFIER()->getText();
        bool found = false;
        for (auto &attr : attrs)
        {
            if (attr.attr_name() == pkName)
            {
                attr.set_attr_type(1); // 기본 키로 설정
                found = true;
                break;
            }
        }
        if (!found)
        {
            throw SyntaxErrorException();
        }
    }

    stmt->set_attrs(attrs);
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitDropTable(SQLParser::DropTableContext *ctx)
{
    SQLDropTable *stmt = new SQLDropTable();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitCreateIndex(SQLParser::CreateIndexContext *ctx)
{
    SQLCreateIndex *stmt = new SQLCreateIndex();
    stmt->set_index_name(ctx->IDENTIFIER(0)->getText());
    stmt->set_tb_name(ctx->IDENTIFIER(1)->getText());
    stmt->set_col_name(ctx->IDENTIFIER(2)->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitDropIndex(SQLParser::DropIndexContext *ctx)
{
    SQLDropIndex *stmt = new SQLDropIndex();
    stmt->set_idx_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitUseDatabase(SQLParser::UseDatabaseContext *ctx)
{
    SQLUse *stmt = new SQLUse();
    stmt->set_db_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitInsertInto(SQLParser::InsertIntoContext *ctx)
{
    SQLInsert *stmt = new SQLInsert();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    std::vector<SQLValue> values;

    for (auto valCtx : ctx->valueList()->value())
    {
        SQLValue sql_value;
        if (valCtx->STRING_LITERAL())
        {
            std::string val = valCtx->STRING_LITERAL()->getText();
            val = val.substr(1, val.length() - 2); // 따옴표 제거
            sql_value.data_type = 2;               // 문자열 타입
            sql_value.value = val;
        }
        else if (valCtx->NUMERIC_LITERAL())
        {
            std::string val = valCtx->NUMERIC_LITERAL()->getText();
            if (val.find('.') != std::string::npos)
            {
                sql_value.data_type = 1; // 실수 타입
            }
            else
            {
                sql_value.data_type = 0; // 정수 타입
            }
            sql_value.value = val;
        }
        else
        {
            throw SyntaxErrorException();
        }
        values.push_back(sql_value);
    }

    stmt->set_values(values);
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitSelectStatement(SQLParser::SelectStatementContext *ctx)
{
    SQLSelect *stmt = new SQLSelect();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    std::vector<SQLWhere> wheres;

    if (ctx->WHERE())
    {
        for (size_t i = 0; i < ctx->condition().size(); ++i)
        {
            SQLWhere where;
            auto condCtx = ctx->condition(i);
            where.key = condCtx->IDENTIFIER()->getText();
            std::string comp = condCtx->comparator()->getText();

            // 비교 연산자를 설정
            if (comp == "=")
                where.sign_type = SIGN_EQ;
            else if (comp == "<>")
                where.sign_type = SIGN_NE;
            else if (comp == "<")
                where.sign_type = SIGN_LT;
            else if (comp == "<=")
                where.sign_type = SIGN_LE;
            else if (comp == ">")
                where.sign_type = SIGN_GT;
            else if (comp == ">=")
                where.sign_type = SIGN_GE;
            else
                throw SyntaxErrorException();

            // 값 설정
            if (condCtx->value()->STRING_LITERAL())
            {
                std::string val = condCtx->value()->STRING_LITERAL()->getText();
                val = val.substr(1, val.length() - 2); // 따옴표 제거
                where.value = val;
            }
            else if (condCtx->value()->NUMERIC_LITERAL())
            {
                where.value = condCtx->value()->NUMERIC_LITERAL()->getText();
            }
            else
            {
                throw SyntaxErrorException();
            }
            wheres.push_back(where);
        }
    }

    stmt->set_wheres(wheres);
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitDeleteStatement(SQLParser::DeleteStatementContext *ctx)
{
    SQLDelete *stmt = new SQLDelete();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    std::vector<SQLWhere> wheres;

    if (ctx->WHERE())
    {
        for (size_t i = 0; i < ctx->condition().size(); ++i)
        {
            SQLWhere where;
            auto condCtx = ctx->condition(i);
            where.key = condCtx->IDENTIFIER()->getText();
            std::string comp = condCtx->comparator()->getText();

            if (comp == "=")
                where.sign_type = SIGN_EQ;
            else if (comp == "<>")
                where.sign_type = SIGN_NE;
            else if (comp == "<")
                where.sign_type = SIGN_LT;
            else if (comp == "<=")
                where.sign_type = SIGN_LE;
            else if (comp == ">")
                where.sign_type = SIGN_GT;
            else if (comp == ">=")
                where.sign_type = SIGN_GE;
            else
                throw SyntaxErrorException();

            if (condCtx->value()->STRING_LITERAL())
            {
                std::string val = condCtx->value()->STRING_LITERAL()->getText();
                val = val.substr(1, val.length() - 2);
                where.value = val;
            }
            else if (condCtx->value()->NUMERIC_LITERAL())
            {
                where.value = condCtx->value()->NUMERIC_LITERAL()->getText();
            }
            else
            {
                throw SyntaxErrorException();
            }
            wheres.push_back(where);
        }
    }

    stmt->set_wheres(wheres);
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitUpdateStatement(SQLParser::UpdateStatementContext *ctx)
{
    SQLUpdate *stmt = new SQLUpdate();
    stmt->set_tb_name(ctx->IDENTIFIER()->getText());
    std::vector<SQLKeyValue> keyvalues;

    // SET 절의 할당문 처리
    for (auto assignCtx : ctx->assignmentList()->assignment())
    {
        SQLKeyValue kv;
        kv.key = assignCtx->IDENTIFIER()->getText();

        if (assignCtx->value()->STRING_LITERAL())
        {
            std::string val = assignCtx->value()->STRING_LITERAL()->getText();
            val = val.substr(1, val.length() - 2); // 따옴표 제거
            kv.value = val;
        }
        else if (assignCtx->value()->NUMERIC_LITERAL())
        {
            kv.value = assignCtx->value()->NUMERIC_LITERAL()->getText();
        }
        else
        {
            throw SyntaxErrorException();
        }
        keyvalues.push_back(kv);
    }

    stmt->set_keyvalues(keyvalues);

    // WHERE 절의 조건 처리
    std::vector<SQLWhere> wheres;
    if (ctx->WHERE())
    {
        for (size_t i = 0; i < ctx->condition().size(); ++i)
        {
            SQLWhere where;
            auto condCtx = ctx->condition(i);
            where.key = condCtx->IDENTIFIER()->getText();
            std::string comp = condCtx->comparator()->getText();

            if (comp == "=")
                where.sign_type = SIGN_EQ;
            else if (comp == "<>")
                where.sign_type = SIGN_NE;
            else if (comp == "<")
                where.sign_type = SIGN_LT;
            else if (comp == "<=")
                where.sign_type = SIGN_LE;
            else if (comp == ">")
                where.sign_type = SIGN_GT;
            else if (comp == ">=")
                where.sign_type = SIGN_GE;
            else
                throw SyntaxErrorException();

            if (condCtx->value()->STRING_LITERAL())
            {
                std::string val = condCtx->value()->STRING_LITERAL()->getText();
                val = val.substr(1, val.length() - 2);
                where.value = val;
            }
            else if (condCtx->value()->NUMERIC_LITERAL())
            {
                where.value = condCtx->value()->NUMERIC_LITERAL()->getText();
            }
            else
            {
                throw SyntaxErrorException();
            }
            wheres.push_back(where);
        }
    }

    stmt->set_wheres(wheres);
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitExecStatement(SQLParser::ExecStatementContext *ctx)
{
    SQLExec *stmt = new SQLExec();
    stmt->set_file_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitShowDatabases(SQLParser::ShowDatabasesContext *ctx)
{
    SQL *stmt = new SQL(40);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitShowTables(SQLParser::ShowTablesContext *ctx)
{
    SQL *stmt = new SQL(41);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitHelpStatement(SQLParser::HelpStatementContext *ctx)
{
    SQL *stmt = new SQL(20);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitQuitStatement(SQLParser::QuitStatementContext *ctx)
{
    SQL *stmt = new SQL(10);
    return stmt;
}
