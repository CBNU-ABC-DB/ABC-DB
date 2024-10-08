#ifndef ABCDB_INTERPRETER_H_
#define ABCDB_INTERPRETER_H_

#include <string>
#include <vector>

#include "api.h"
#include "sql_statement.h"

class Interpreter {
private:
  API *api;
  std::string sql_statement_;
  std::vector<std::string> sql_vector_;
  int sql_type_;
  void TellSQLType();
  void FormatSQL();
  void Run();

public:
  Interpreter();
  ~Interpreter();
  void ExecSQL(std::string statement);
};

#endif /* ABCDB_INTERPRETER_H_ */