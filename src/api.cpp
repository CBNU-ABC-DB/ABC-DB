#include <fstream>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "api.h"
#include "catalog_manager.h"
#include "exceptions.h"
#include "execution_engine.h"
#include "buffer_manager.h"

using namespace std;

API::API(std::string p) : path_(p)
{
  cm_ = new CatalogManager(p);
  bm_ = new BufferManager();
}

API::~API()
{
  // hdl_ is initialized in #Use#
  // delete hdl_;
  std::cout << "API DIE" << std::endl;
  delete cm_;
  delete bm_;
}

void API::Quit()
{
  // delete hdl_;
  delete cm_;
  delete bm_;
  std::cout << "Quiting..." << std::endl;
}

void API::Help()
{
  std::cout << "AbcDB 1.0.0" << std::endl;
  std::cout << "Implemented SQL types:" << std::endl;
  std::cout << "#QUIT#" << std::endl;
  std::cout << "#HELP#" << std::endl;
  std::cout << "#EXEC#" << std::endl;
  std::cout << "#CREATE DATABASE#" << std::endl;
  std::cout << "#SHOW DATABASES#" << std::endl;
  std::cout << "#USE#" << std::endl;
  std::cout << "#CREATE TABLE#" << std::endl;
  std::cout << "#SHOW TABLES#" << std::endl;
  std::cout << "#SELECT#" << std::endl;
  std::cout << "#INSERT#" << std::endl;
}

void API::CreateDatabase(SQLCreateDatabase &st)
{
  std::cout << "Creating database: " << st.db_name() << std::endl;
  std::string folder_name(path_ + st.db_name());
  std::cout<<path_<<std::endl;
  boost::filesystem::path folder_path(folder_name);

  folder_path.imbue(std::locale("en_US.UTF-8"));

  if (cm_->GetDB(st.db_name()) != NULL)
  {
    throw DatabaseAlreadyExistsException();
  }

  if (boost::filesystem::exists(folder_path))
  {
    boost::filesystem::remove_all(folder_path);
    std::cout << "Database folder exists and deleted!" << std::endl;
  }

  boost::filesystem::create_directories(folder_path);
  std::cout << "Database folder created!" << std::endl;

  cm_->CreateDatabase(st.db_name());
  std::cout << "Catalog written!" << std::endl;
  cm_->WriteArchiveFile();
}

void API::ShowDatabases()
{
  std::vector<Database> dbs = cm_->dbs();
  std::cout << "DATABASE LIST:" << std::endl;
  for (unsigned int i = 0; i < dbs.size(); ++i)
  {
    Database db = dbs[i];
    std::cout << "\t" << db.db_name() << std::endl;
  }
}

void API::Use(SQLUse &st)
{
  Database *db = cm_->GetDB(st.db_name());

  if (db == NULL)
  {
    throw DatabaseNotExistException();
  }

  if (curr_db_.length() != 0)
  {
    std::cout << "Closing the old database: " << curr_db_ << std::endl;
    cm_->WriteArchiveFile();
    // delete hdl_;
  }
  curr_db_ = st.db_name();
  // hdl_ = new BufferManager(path_);
}

void API::CreateTable(SQLCreateTable &st)
{
  std::cout << "Creating table: " << st.tb_name() << std::endl;
  if (curr_db_.length() == 0)
  {
    throw NoDatabaseSelectedException();
  }

  Database *db = cm_->GetDB(curr_db_);
  if (db == NULL)
  {
    throw DatabaseNotExistException();
  }

  if (db->GetTable(st.tb_name()) != NULL)
  {
    throw TableAlreadyExistsException();
  }

  std::string file_name(path_ + curr_db_ + "/" + st.tb_name() + ".bin");
  boost::filesystem::path folder_path(file_name);

  if (boost::filesystem::exists(file_name))
  {
    boost::filesystem::remove(file_name);
    std::cout << "Table file already exists and deleted!" << std::endl;
  }

  ofstream ofs(file_name);
  ofs.close();
  std::cout << "Table file created!" << std::endl;

  db->CreateTable(st,file_name);
  std::cout << "Catalog written!" << std::endl;
  cm_->WriteArchiveFile();
}

void API::ShowTables()
{
  if (curr_db_.length() == 0)
  {
    throw NoDatabaseSelectedException();
  }

  Database *db = cm_->GetDB(curr_db_);
  if (db == NULL)
  {
    throw DatabaseNotExistException();
  }
  std::cout << "CURRENT DATABASE: " << curr_db_ << std::endl;
  std::cout << "TABLE LIST:" << std::endl;
  for (int i = 0; i < db->tbs().size(); ++i)
  {
    Table &tb = db->tbs()[i];
    std::cout << "\t" << tb.tb_name() << std::endl;
  }
}

void API::Insert(SQLInsert &st)
{
  if (curr_db_.length() == 0)
  {
    throw NoDatabaseSelectedException();
  }

  Database *db = cm_->GetDB(curr_db_);
  if (db == NULL)
  {
    throw DatabaseNotExistException();
  }
  ExecutionEngine *ee = new ExecutionEngine(cm_, curr_db_, bm_);
  ee->Insert(st);
  delete ee;
}

void API::Select(SQLSelect &st)
{
  if (curr_db_.length() == 0)
  {
    throw NoDatabaseSelectedException();
  }

  std::string file_name(path_ + curr_db_ + "/" + st.tb_name() + ".bin");
  Table *tb = cm_->GetDB(curr_db_)->GetTable(file_name);
  if (tb == NULL)
  {
    throw TableNotExistException();
  }

  ExecutionEngine *ee = new ExecutionEngine(cm_, curr_db_, bm_);
  ee->Select(st);
  delete ee;
}

void API::AddTestRecord(SQLTestRecord &st){
  if (curr_db_.length() == 0)
  {
    throw NoDatabaseSelectedException();
  }

  Database *db = cm_->GetDB(curr_db_);
  if (db == NULL)
  {
    throw DatabaseNotExistException();
  }
  ExecutionEngine *ee = new ExecutionEngine(cm_, curr_db_, bm_);
  ee->AddTestRecord(st);
  delete ee;
}