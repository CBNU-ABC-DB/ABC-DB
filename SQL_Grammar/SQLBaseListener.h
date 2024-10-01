
// Generated from SQL.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "SQLListener.h"


/**
 * This class provides an empty implementation of SQLListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  SQLBaseListener : public SQLListener {
public:

  virtual void enterSql_stmt(SQLParser::Sql_stmtContext * /*ctx*/) override { }
  virtual void exitSql_stmt(SQLParser::Sql_stmtContext * /*ctx*/) override { }

  virtual void enterSelect_stmt(SQLParser::Select_stmtContext * /*ctx*/) override { }
  virtual void exitSelect_stmt(SQLParser::Select_stmtContext * /*ctx*/) override { }

  virtual void enterColumn_list(SQLParser::Column_listContext * /*ctx*/) override { }
  virtual void exitColumn_list(SQLParser::Column_listContext * /*ctx*/) override { }

  virtual void enterWhere_clause(SQLParser::Where_clauseContext * /*ctx*/) override { }
  virtual void exitWhere_clause(SQLParser::Where_clauseContext * /*ctx*/) override { }

  virtual void enterCondition(SQLParser::ConditionContext * /*ctx*/) override { }
  virtual void exitCondition(SQLParser::ConditionContext * /*ctx*/) override { }

  virtual void enterComparator(SQLParser::ComparatorContext * /*ctx*/) override { }
  virtual void exitComparator(SQLParser::ComparatorContext * /*ctx*/) override { }

  virtual void enterLiteral_value(SQLParser::Literal_valueContext * /*ctx*/) override { }
  virtual void exitLiteral_value(SQLParser::Literal_valueContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

