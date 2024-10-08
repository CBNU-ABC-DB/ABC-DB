/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include "interpreter.h"

using namespace std;

int main(int argc, const char *argv[]) {

  string sql;
  Interpreter itp;

  char *line;
  size_t found;

  using_history();

  while (true) {
    line = readline("ABCDB> ");
    sql = string(line);
    free(line);
    boost::algorithm::trim(sql);

    if (sql.compare(0, 4, "exit") == 0 || sql.compare(0, 4, "quit") == 0) {
      itp.ExecSQL("quit");
      break;
    }

    while ((found = sql.find(";")) == string::npos) {
      line = readline("");
      sql += "\n" + string(line);
      free(line);
    }

    if (sql.length() != 0) {
      add_history(sql.c_str());
    }

    itp.ExecSQL(sql);
    std::cout << std::endl;
  }
  return 0;
}