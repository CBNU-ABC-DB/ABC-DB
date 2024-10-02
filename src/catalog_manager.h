#ifndef ABCDB_CATALOG_MANAGER_H_
#define ABCDB_CATALOG_MANAGER_H_

#include<string>
#include <vector>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include "file.h"
#include "sql_statement.h"

class Database;
class Table;
class Attribute;
class SQLCreateTable;
class SQLDropTable;

class CatalogManager {
private:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &dbs_;
  }
  std::string path_;
  std::vector<Database> dbs_;

public:
  CatalogManager(std::string p);
  ~CatalogManager();
  std::vector<Database> &dbs() { return dbs_; }
  std::string path() { return path_; }
  Database *GetDB(std::string db_name);
  void ReadArchiveFile();
  void WriteArchiveFile();
  void CreateDatabase(std::string dbname);
  void DeleteDatabase(std::string dbname);
};

class Database {
private:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &db_name_;
    ar &tbs_;
  }
  std::string db_name_;
  std::vector<Table> tbs_;

public:
  Database() {}
  Database(std::string dbname);
  ~Database() {}
  Table *GetTable(std::string tb_name);
  std::string db_name() { return db_name_; }
  void CreateTable(SQLCreateTable &st);
  void DropTable(SQLDropTable &st);
  std::vector<Table> &tbs() { return tbs_; }
};

class Table {
private:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &file_;
    ar &tb_name_;
  
    ar &ats_;
  }

  std::string file_;
  std::string tb_name_;
  int record_length_;

  std::vector<Attribute> ats_;

public:
  Table() {}
  Table(std::string name)
      : file_(name), tb_name_(name), record_length_(-1) {}
  ~Table() {}

  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }

  std::string GetFile() { return file_; }

  int record_length() { return record_length_; }
  void set_record_length(int len) { record_length_ = len; }

  std::vector<Attribute> &ats() { return ats_; }
  Attribute *GetAttribute(std::string name);
  
  unsigned long GetAttributeNum() { return ats_.size(); }
  void AddAttribute(const Attribute &attr) { ats_.push_back(attr); }
};

class Attribute {
private:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &attr_name_;
    ar &data_type_;
    ar &length_;
    ar &attr_type_;
  }

  std::string attr_name_;
  int data_type_;
  int length_;
  int attr_type_;

public:
  Attribute() : attr_name_(""), data_type_(-1), length_(-1), attr_type_(0) {}
  ~Attribute() {}

  std::string attr_name() { return attr_name_; }

  void set_attr_name(std::string name) { attr_name_ = name; }

  int attr_type() { return attr_type_; }
  void set_attr_type(int type) { attr_type_ = type; }

  int data_type() { return data_type_; }
  void set_data_type(int type) { data_type_ = type; }

  void set_length(int length) { length_ = length; }
  int length() const { return length_; }
};

#endif