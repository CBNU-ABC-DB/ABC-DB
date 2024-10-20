#include "interpreter.h"

#include <fstream>
#include <iostream>
#include <any>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "exceptions.h"
#include "minidb_api.h"

using namespace std;
using namespace antlr4;

Interpreter::Interpreter() : api(nullptr)
{
  string p = string(getenv("HOME")) + "/MiniDBData/";
  api = new MiniDBAPI(p);
}

Interpreter::~Interpreter() { delete api; }

void Interpreter::ExecSQL(std::string statement)
{
  sql_statement_ = statement;

  // ANTLR4를 사용하여 SQL 문을 파싱합니다.
  ANTLRInputStream input(sql_statement_);
  SQLLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  SQLParser parser(&tokens);

  // SQL 문을 파싱합니다.
  SQLParser::SqlStatementContext *tree = parser.sqlStatement();

  // 방문자를 생성하여 구문 트리를 순회하고 SQL 문 객체를 생성합니다.
  SQLStatementVisitor visitor;
  antlrcpp::Any result = visitor.visit(tree);

  SQL *sqlStatement = nullptr;

  try
  {
    sqlStatement = std::any_cast<SQL *>(result);
  }
  catch (const std::bad_any_cast &e)
  {
    std::cerr << "Bad any_cast: " << e.what() << std::endl;
    return;
  }

  if (sqlStatement != nullptr)
  {
    // SQL 문을 실행합니다.
    RunSQLStatement(sqlStatement);
    delete sqlStatement;
  }
  else
  {
    std::cerr << "Failed to parse SQL statement." << std::endl;
  }
}

void Interpreter::RunSQLStatement(SQL *sqlStatement)
{
  try
  {
    switch (sqlStatement->sql_type())
    {
    case 10:
      api->Quit();
      exit(0);
      break;
    case 20:
      api->Help();
      break;
    case 30:
    {
      SQLCreateDatabase *st = dynamic_cast<SQLCreateDatabase *>(sqlStatement);
      if (st)
        api->CreateDatabase(*st);
    }
    break;
    case 31:
    {
      SQLCreateTable *st = dynamic_cast<SQLCreateTable *>(sqlStatement);
      if (st)
        api->CreateTable(*st);
    }
    break;
    case 32:
    {
      SQLCreateIndex *st = dynamic_cast<SQLCreateIndex *>(sqlStatement);
      if (st)
        api->CreateIndex(*st);
    }
    break;
    case 40:
      api->ShowDatabases();
      break;
    case 41:
      api->ShowTables();
      break;
    case 50:
    {
      SQLDropDatabase *st = dynamic_cast<SQLDropDatabase *>(sqlStatement);
      if (st)
        api->DropDatabase(*st);
    }
    break;
    case 51:
    {
      SQLDropTable *st = dynamic_cast<SQLDropTable *>(sqlStatement);
      if (st)
        api->DropTable(*st);
    }
    break;
    case 52:
    {
      SQLDropIndex *st = dynamic_cast<SQLDropIndex *>(sqlStatement);
      if (st)
        api->DropIndex(*st);
    }
    break;
    case 60:
    {
      SQLUse *st = dynamic_cast<SQLUse *>(sqlStatement);
      if (st)
        api->Use(*st);
    }
    break;
    case 70:
    {
      SQLInsert *st = dynamic_cast<SQLInsert *>(sqlStatement);
      if (st)
        api->Insert(*st);
    }
    break;
    case 80:
    {
      SQLExec *st = dynamic_cast<SQLExec *>(sqlStatement);
      if (st)
      {
        string contents;
        ifstream in(st->file_name(), ios::in | ios::binary);
        if (!in)
        {
          cerr << "Could not open file: " << st->file_name() << endl;
          break;
        }
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        cout << endl;

        // 내용을 세미콜론으로 분할하여 실행합니다.
        vector<string> sqls;
        boost::split(sqls, contents, boost::is_any_of(";"));
        for (const auto &sql : sqls)
        {
          if (!sql.empty())
          {
            ExecSQL(sql);
          }
        }
      }
    }
    break;
    case 90:
    {
      SQLSelect *st = dynamic_cast<SQLSelect *>(sqlStatement);
      if (st)
        api->Select(*st);
    }
    break;
    case 100:
    {
      SQLDelete *st = dynamic_cast<SQLDelete *>(sqlStatement);
      if (st)
        api->Delete(*st);
    }
    break;
    case 110:
    {
      SQLUpdate *st = dynamic_cast<SQLUpdate *>(sqlStatement);
      if (st)
        api->Update(*st);
    }
    break;
    default:
      cerr << "Unknown SQL statement type." << endl;
      break;
    }
  }
  catch (SyntaxErrorException &e)
  {
    cerr << "Syntax Error!" << endl;
  }
  catch (NoDatabaseSelectedException &e)
  {
    cerr << "No database selected!" << endl;
  }
  catch (DatabaseNotExistException &e)
  {
    cerr << "Database doesn't exist!" << endl;
  }
  catch (DatabaseAlreadyExistsException &e)
  {
    cerr << "Database already exists!" << endl;
  }
  catch (TableNotExistException &e)
  {
    cerr << "Table doesn't exist!" << endl;
  }
  catch (OneIndexEachTableException &e)
  {
    cerr << "Each table could only have one index!" << endl;
  }
  catch (BPlusTreeException &e)
  {
    cerr << "BPlusTree exception!" << endl;
  }
  catch (TableAlreadyExistsException &e)
  {
    cerr << "Table already exists!" << endl;
  }
  catch (IndexAlreadyExistsException &e)
  {
    cerr << "Index already exists!" << endl;
  }
  catch (IndexNotExistException &e)
  {
    cerr << "Index doesn't exist!" << endl;
  }
  catch (IndexMustBeCreatedOnPKException &e)
  {
    cerr << "Index must be created on primary key!" << endl;
  }
  catch (PrimaryKeyConflictException &e)
  {
    cerr << "Primary key conflicts!" << endl;
  }
}
