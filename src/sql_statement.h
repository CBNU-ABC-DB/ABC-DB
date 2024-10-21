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

  int key_type() { return key_type_; }
  char *key() { return key_; };
  int length() { return length_; }

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

class SQLDropDatabase : public SQL
{
private:
  std::string db_name_;

public:
  SQLDropDatabase() { sql_type_ = 50; }
  std::string db_name() { return db_name_; }
  void set_db_name(std::string dbname) { db_name_ = dbname; }
};

class SQLDropTable : public SQL
{
private:
  std::string tb_name_;

public:
  SQLDropTable() { sql_type_ = 51; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
};

class SQLDropIndex : public SQL
{
private:
  std::string idx_name_;

public:
  SQLDropIndex() { sql_type_ = 52; }
  std::string idx_name() { return idx_name_; }
  void set_idx_name(std::string idxname) { idx_name_ = idxname; }
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

typedef struct
{
  int data_type;
  std::string value;
} SQLValue;

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

class SQLCreateIndex : public SQL
{
private:
  std::string index_name_;
  std::string tb_name_;
  std::string col_name_;

public:
  SQLCreateIndex() { sql_type_ = 32; }
  std::string index_name() { return index_name_; }
  void set_index_name(std::string idxname) { index_name_ = idxname; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::string col_name() { return col_name_; }
  void set_col_name(std::string colname) { col_name_ = colname; }
};

class SQLDelete : public SQL
{
private:
  std::string tb_name_;
  std::vector<SQLWhere> wheres_;

public:
  SQLDelete() { sql_type_ = 100; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::vector<SQLWhere> &wheres() { return wheres_; }
  void set_wheres(const std::vector<SQLWhere> &ws) { wheres_ = ws; }
};

typedef struct
{
  std::string key;
  std::string value;
} SQLKeyValue;

class SQLUpdate : public SQL
{
private:
  std::string tb_name_;
  std::vector<SQLWhere> wheres_;
  std::vector<SQLKeyValue> keyvalues_;

public:
  SQLUpdate() { sql_type_ = 110; }
  std::string tb_name() { return tb_name_; }
  void set_tb_name(std::string tbname) { tb_name_ = tbname; }
  std::vector<SQLWhere> &wheres() { return wheres_; }
  void set_wheres(const std::vector<SQLWhere> &ws) { wheres_ = ws; }
  std::vector<SQLKeyValue> &keyvalues() { return keyvalues_; }
  void set_keyvalues(const std::vector<SQLKeyValue> &kvs) { keyvalues_ = kvs; }
};

#endif