#include "sql_statement_visitor.h"

#include <iostream>
#include <sstream>
#include <cstdlib>

#include "commons.h"
#include "exceptions.h"

using namespace std;

// 데이터 타입 값을 문자열로 변환하는 함수
std::string DataTypeToString(int data_type)
{
    switch (data_type)
    {
    case T_INT:
        return "int";
    case T_FLOAT:
        return "float";
    case T_CHAR:
        return "char";
    default:
        return "unknown";
    }
}

antlrcpp::Any SQLStatementVisitor::visitSqlStatement(SQLParser::SqlStatementContext *ctx)
{
    // SQL 문장을 방문하고, 하위 노드를 방문하여 SQL 객체를 생성
    return visitChildren(ctx);
}

antlrcpp::Any SQLStatementVisitor::visitCreateDatabase(SQLParser::CreateDatabaseContext *ctx)
{
    cout << "Parsing CREATE DATABASE statement" << endl;

    SQLCreateDatabase *stmt = new SQLCreateDatabase();
    stmt->set_db_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitCreateTable(SQLParser::CreateTableContext *ctx)
{
    cout << "Parsing CREATE TABLE statement" << endl;

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

    // 파싱된 SQL 구조를 출력
    cout << "Parsed CREATE TABLE statement: " << endl;
    cout << "Table Name: " << stmt->tb_name() << endl;
    cout << "Attributes: " << endl;
    for (const auto &attr : stmt->attrs())
    {
        cout << " - Name: " << attr.attr_name()
             << ", Type: " << DataTypeToString(attr.data_type())
             << ", Length: " << attr.length();
    }

    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitUseDatabase(SQLParser::UseDatabaseContext *ctx)
{
    cout << "Parsing USE DATABASE statement" << endl;

    SQLUse *stmt = new SQLUse();
    stmt->set_db_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitInsertInto(SQLParser::InsertIntoContext *ctx)
{
    cout << "Parsing INSERT INTO statement" << endl;

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

    // 파싱된 SQL 구조를 출력
    cout << "Parsed INSERT INTO statement: " << endl;
    cout << "Table Name: " << stmt->tb_name() << endl;
    cout << "Values: " << endl;
    for (const auto &val : stmt->values())
    {
        cout << " - Data Type: " << val.data_type
             << ", Value: " << val.value << endl;
    }

    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitSelectStatement(SQLParser::SelectStatementContext *ctx)
{
    cout << "Parsing SELECT statement" << endl;

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

    // 파싱된 SQL 구조를 출력
    cout << "Parsed SELECT statement: " << endl;
    cout << "Table Name: " << stmt->tb_name() << endl;
    if (!wheres.empty())
    {
        cout << "Conditions: " << endl;
        for (const auto &where : wheres)
        {
            cout << " - Key: " << where.key
                 << ", Operator: " << where.sign_type
                 << ", Value: " << where.value << endl;
        }
    }

    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitExecStatement(SQLParser::ExecStatementContext *ctx)
{
    cout << "Parsing EXEC statement" << endl;

    SQLExec *stmt = new SQLExec();
    stmt->set_file_name(ctx->IDENTIFIER()->getText());
    return static_cast<SQL *>(stmt);
}

antlrcpp::Any SQLStatementVisitor::visitShowDatabases(SQLParser::ShowDatabasesContext *ctx)
{
    cout << "Parsing SHOW DATABASES statement" << endl;

    SQL *stmt = new SQL(40);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitShowTables(SQLParser::ShowTablesContext *ctx)
{
    cout << "Parsing SHOW TABLES statement" << endl;

    SQL *stmt = new SQL(41);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitHelpStatement(SQLParser::HelpStatementContext *ctx)
{
    cout << "Parsing HELP statement" << endl;

    SQL *stmt = new SQL(20);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitQuitStatement(SQLParser::QuitStatementContext *ctx)
{
    cout << "Parsing QUIT statement" << endl;

    SQL *stmt = new SQL(10);
    return stmt;
}

antlrcpp::Any SQLStatementVisitor::visitTestRecordStatement(SQLParser::TestRecordStatementContext *ctx)
{
    cout << "Parsing TEST RECORD statement" << endl;

    SQLTestRecord *stmt = new SQLTestRecord();

    // 레코드 수
    std::string numStr = ctx->NUMERIC_LITERAL()->getText();

    // 정수 또는 실수 여부 판단
    int record_count;
    if (numStr.find('.') != std::string::npos)
    {
        // 실수가 입력된 경우 에러 처리
        throw SyntaxErrorException();
    }
    else
    {
        // 정수인 경우 변환
        record_count = std::atoi(numStr.c_str());
    }
    stmt->set_record_count(record_count);

    stmt->set_tb_name(ctx->IDENTIFIER()->getText());

    std::vector<SQLValue> values;

    // valueList를 처리하여 여러 개의 값을 저장
    for (auto valCtx : ctx->valueList()->value())
    {
        SQLValue sql_value;
        if (valCtx->STRING_LITERAL())
        {
            std::string val = valCtx->STRING_LITERAL()->getText();
            // 따옴표 제거
            if ((val.front() == '\'' && val.back() == '\'') || (val.front() == '"' && val.back() == '"'))
            {
                val = val.substr(1, val.length() - 2);
            }
            sql_value.data_type = 2; // 문자열 타입
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

    // 파싱된 SQL 구조를 출력
    cout << "Parsed TEST RECORD statement: " << endl;
    cout << "Record Count: " << stmt->record_count() << endl;
    cout << "Table Name: " << stmt->tb_name() << endl;
    cout << "Values: " << endl;
    for (const auto &val : stmt->values())
    {
        cout << " - Data Type: " << val.data_type
             << ", Value: " << val.value << endl;
    }

    return static_cast<SQL *>(stmt);
}
