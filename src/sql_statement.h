#ifndef SQL_STATEMENT_H_
#define SQL_STATEMENT_H_

#include <string>
#include <vector>
#include "catalog_manager.h"

class CatalogManager;
class Database;
class Table;
class Attribute;
class Index;

enum SignType
{
  SIGN_EQ, // =
  SIGN_NE, // <>
  SIGN_LT, // <
  SIGN_LE, // <=
  SIGN_GT, // >
  SIGN_GE  // >=
};
typedef struct
{
  int data_type;
  std::string value;
} SQLValue;

class TKey
{
private:
  int key_type_;
  char *key_;
  int length_;

public:
  // 생성자
  TKey(int keytype, int length);

  // 복사 생성자
  TKey(const TKey &t1);

  // 대입 연산자
  TKey &operator=(const TKey &t1);

  // 값 읽기
  void ReadValue(const char *content);
  void ReadValue(std::string str);

  int key_type() const{ return key_type_; }
  char *key() const { return key_; };
  int length() const { return length_; }

  // 소멸자
  ~TKey();

  // 연산자 오버로딩
  friend std::ostream &operator<<(std::ostream &out, const TKey &object);

  bool operator<(const TKey &t1) const;
  bool operator>(const TKey &t1) const;
  bool operator<=(const TKey &t1) const;
  bool operator>=(const TKey &t1) const;
  bool operator==(const TKey &t1) const;
  bool operator!=(const TKey &t1) const;
};

class SQL
{
protected:
  int sql_type_;

public:
  SQL() : sql_type_(-1) {}
  SQL(int sqltype) { sql_type_ = sqltype; }
  virtual ~SQL() {}
  int sql_type() { return sql_type_; }
  void set_sql_type(int sqltype) { sql_type_ = sqltype; }
  int ParseDataType(std::vector<std::string> sql_vector, Attribute &attr,
                    unsigned int pos);
};

class SQLCreateDatabase : public SQL
{
private:
  std::string db_name_;

public:
  SQLCreateDatabase() { sql_type_ = 30; }
  std::string db_name() { return db_name_; }
  void set_db_name(std::string dbname) { db_name_ = dbname; }
};

class SQLUse : public SQL
{
private:
  std::string db_name_;

public:
  SQLUse() { sql_type_ = 60; }
  std::string db_name() { return db_name_; }
  void set_db_name(std::string dbname) { db_name_ = dbname; }
};

class SQLCreateTable : public SQL
{
private:
  std::string tb_name_;
  std::vector<Attribute> attrs_;

public:
  SQLCreateTable() { sql_type_ = 31; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::vector<Attribute> attrs() { return attrs_; };
  void set_attrs(std::vector<Attribute> att) { attrs_ = att; }
};
class SQLDescTable : public SQL
{
private:
  std::string tb_name_;
public:
  SQLDescTable() { sql_type_ = 42;}
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
};

class SQLInsert : public SQL
{
private:
  std::string tb_name_;
  std::vector<SQLValue> values_;

public:
  SQLInsert() { sql_type_ = 70; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::vector<SQLValue> &values() { return values_; }
  void set_values(const std::vector<SQLValue> &vals) { values_ = vals; }
};

class SQLExec : public SQL
{
private:
  std::string file_name_;

public:
  SQLExec() { sql_type_ = 80; }
  std::string file_name() { return file_name_; }
  void set_file_name(std::string fname) { file_name_ = fname; }
};

typedef struct
{
  std::string key;
  int sign_type;
  std::string value;
} SQLWhere;

class SQLSelect : public SQL
{
private:
  std::string tb_name_;
  std::vector<SQLWhere> wheres_;

public:
  SQLSelect() { sql_type_ = 90; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::vector<SQLWhere> &wheres() { return wheres_; }
  void set_wheres(const std::vector<SQLWhere> &ws) { wheres_ = ws; }
};

class SQLTestRecord : public SQL
{
private:
  int record_count_;
  std::string tb_name_;
  std::vector<SQLValue> values_;

public:
  SQLTestRecord() { sql_type_ = 120; }
  int record_count() const { return record_count_; }
  void set_record_count(int count) { record_count_ = count; }
  std::string tb_name() const { return tb_name_; }
  void set_tb_name(const std::string &name) { tb_name_ = name; }
  std::vector<SQLValue> &values() { return values_; }
  void set_values(const std::vector<SQLValue> &vals) { values_ = vals; }
};

#endif