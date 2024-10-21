#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <string>
#include <vector>

#include "api.h"
#include "sql_statement.h"

#include "antlr4-runtime.h"
#include "SQLLexer.h"
#include "SQLParser.h"
#include "sql_statement_visitor.h"

class Interpreter
{
private:
  API *api;
  std::string sql_statement_;
  void RunSQLStatement(SQL *sqlStatement);

public:
  Interpreter();
  ~Interpreter();
  void ExecSQL(std::string statement);
};

#endif