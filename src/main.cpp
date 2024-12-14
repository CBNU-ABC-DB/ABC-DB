#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <readline/history.h>
#include <readline/readline.h>

#include "interpreter.h"

using namespace std;

std::string getNextCommandFromFile(std::ifstream &file) {
    std::string command;
    if (std::getline(file, command)) {
        boost::algorithm::trim(command);
        return command;
    }
    return "";
}

void processScriptFile(std::ifstream &scriptFile, Interpreter &itp) {
    std::string sql;
    while (true) {
        sql = getNextCommandFromFile(scriptFile);
        if (sql.empty()) {
            break;
        }

        size_t found;
        while ((found = sql.find(";")) == std::string::npos) {
            std::string nextLine = getNextCommandFromFile(scriptFile);
            if (nextLine.empty()) {
                break;
            }
            sql += " " + nextLine;
        }

        if (!sql.empty()) {
            itp.ExecSQL(sql);
            std::cout << std::endl;
        }
    }
}

int main(int argc, const char *argv[]) {

  string sql;
  Interpreter itp;
  char *line;
  size_t found;

  using_history();
  std::ifstream scriptFile;
  if (argc > 1 && std::string(argv[1]) == "-f" && argc > 2) {
      scriptFile.open(argv[2]);
      if (!scriptFile.is_open()) {
          std::cerr << "Error: Unable to open script file: " << argv[2] << std::endl;
          return 1;
      }
      processScriptFile(scriptFile, itp);
      scriptFile.close();
  }

  string text = "ABC DB";
  string font = "slant";
  string command = "figlet -f " + font + " " + text;
  system(command.c_str());

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