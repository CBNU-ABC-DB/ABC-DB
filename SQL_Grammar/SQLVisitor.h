
// Generated from SQL.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SQLParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by SQLParser.
 */
class  SQLVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by SQLParser.
   */
    virtual std::any visitSql_stmt(SQLParser::Sql_stmtContext *context) = 0;

    virtual std::any visitSelect_stmt(SQLParser::Select_stmtContext *context) = 0;

    virtual std::any visitColumn_list(SQLParser::Column_listContext *context) = 0;

    virtual std::any visitWhere_clause(SQLParser::Where_clauseContext *context) = 0;

    virtual std::any visitCondition(SQLParser::ConditionContext *context) = 0;

    virtual std::any visitComparator(SQLParser::ComparatorContext *context) = 0;

    virtual std::any visitLiteral_value(SQLParser::Literal_valueContext *context) = 0;


};

