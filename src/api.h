#ifndef ABCDB_API_H_
#define ABCDB_API_H_

#include <string>

#include "catalog_manager.h"
#include "bufferManager.h"
#include "sql_statement.h"

class API
{
private:
  std::string path_;
  CatalogManager *cm_;
  BufferManager *bm_;
  std::string curr_db_;

public:
  API(std::string p);
  ~API();
  void Quit();
  void Help();
  void CreateDatabase(SQLCreateDatabase &st);
  void ShowDatabases();
  void Use(SQLUse &st);
  void CreateTable(SQLCreateTable &st);
  void ShowTables();
  void Insert(SQLInsert &st);
  void Select(SQLSelect &st);
};

#endif // ABCDB_API_H