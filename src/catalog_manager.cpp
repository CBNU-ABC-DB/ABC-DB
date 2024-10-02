#include "catalog_manager.h"

#include <fstream>

#include <boost/filesystem.hpp>


/*=======================CatalogManager=======================*/

CatalogManager::CatalogManager(std::string p) : path_(p) { ReadArchiveFile(); }

CatalogManager::~CatalogManager() { WriteArchiveFile(); }

void CatalogManager::ReadArchiveFile() {
  std::string file_name = path_ + "catalog";
  boost::filesystem::path file_path(file_name);

  file_path.imbue(std::locale("en_US.UTF-8"));

  if (boost::filesystem::exists(file_path)) {
    std::ifstream ifs;
    ifs.open(file_name.c_str(), std::ios::binary);
    boost::archive::binary_iarchive iar(ifs);
    iar >> (*this);
    ifs.close();
  }
}

void CatalogManager::WriteArchiveFile() {
  std::string file_name = path_ + "catalog";

  std::ofstream ofs;
  ofs.open(file_name.c_str(), std::ios::binary);
  boost::archive::binary_oarchive oar(ofs);
  oar << (*this);
  ofs.close();
}

void CatalogManager::CreateDatabase(std::string dbname) {
  dbs_.push_back(Database(dbname));
}

void CatalogManager::DeleteDatabase(std::string dbname) {
  for (unsigned int i = 0; i < dbs_.size(); i++) {
    if (dbs_[i].db_name() == dbname) {
      dbs_.erase(dbs_.begin() + i);
    }
  }
}

Database *CatalogManager::GetDB(std::string db_name) {
  for (unsigned int i = 0; i < dbs_.size(); ++i) {
    if (dbs_[i].db_name() == db_name) {
      return &dbs_[i];
    }
  }
  return NULL;
}

/*=======================Database=============================*/

Database::Database(std::string dbname) : db_name_(dbname) {}

void Database::CreateTable(SQLCreateTable &st) {
    int record_length = 0;
    Table tb(st.tb_name());

    // TODO::가변형일경우 최대크기(기본값) 지정
    for (const auto& attr : st.attrs()) {
        tb.AddAttribute(attr);
        record_length += attr.length(); 
    }

    tb.set_record_length(record_length);
    tbs_.push_back(std::move(tb));  
}


void Database::DropTable(SQLDropTable &st) {
  std::vector<Table>::iterator i;
  for (std::vector<Table>::iterator i = tbs_.begin(); i != tbs_.end(); i++) {
    if (i->tb_name() == st.tb_name()) {
      tbs_.erase(i);
      return;
    }
  }
}

Table *Database::GetTable(std::string tb_name) {
  for (unsigned int i = 0; i < tbs_.size(); ++i) {
    if (tbs_[i].tb_name() == tb_name) {
      return &tbs_[i];
    }
  }
  return NULL;
}

/*=======================Table===============================*/

Attribute *Table::GetAttribute(std::string name) {
  for (unsigned int i = 0; i < ats_.size(); ++i) {
    if (ats_[i].attr_name() == name) {
      return &ats_[i];
    }
  }
  return NULL;
}