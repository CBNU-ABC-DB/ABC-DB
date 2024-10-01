
// Generated from SQL.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SQLVisitor.h"


/**
 * This class provides an empty implementation of SQLVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  SQLBaseVisitor : public SQLVisitor {
public:

  virtual std::any visitSql_stmt(SQLParser::Sql_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelect_stmt(SQLParser::Select_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitColumn_list(SQLParser::Column_listContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitWhere_clause(SQLParser::Where_clauseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCondition(SQLParser::ConditionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitComparator(SQLParser::ComparatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral_value(SQLParser::Literal_valueContext *ctx) override {
    return visitChildren(ctx);
  }


};

