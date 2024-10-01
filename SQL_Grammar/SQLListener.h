
// Generated from SQL.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SQLParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by SQLParser.
 */
class  SQLListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterSql_stmt(SQLParser::Sql_stmtContext *ctx) = 0;
  virtual void exitSql_stmt(SQLParser::Sql_stmtContext *ctx) = 0;

  virtual void enterSelect_stmt(SQLParser::Select_stmtContext *ctx) = 0;
  virtual void exitSelect_stmt(SQLParser::Select_stmtContext *ctx) = 0;

  virtual void enterColumn_list(SQLParser::Column_listContext *ctx) = 0;
  virtual void exitColumn_list(SQLParser::Column_listContext *ctx) = 0;

  virtual void enterWhere_clause(SQLParser::Where_clauseContext *ctx) = 0;
  virtual void exitWhere_clause(SQLParser::Where_clauseContext *ctx) = 0;

  virtual void enterCondition(SQLParser::ConditionContext *ctx) = 0;
  virtual void exitCondition(SQLParser::ConditionContext *ctx) = 0;

  virtual void enterComparator(SQLParser::ComparatorContext *ctx) = 0;
  virtual void exitComparator(SQLParser::ComparatorContext *ctx) = 0;

  virtual void enterLiteral_value(SQLParser::Literal_valueContext *ctx) = 0;
  virtual void exitLiteral_value(SQLParser::Literal_valueContext *ctx) = 0;


};

