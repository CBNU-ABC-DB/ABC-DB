#ifndef MINIDB_CATALOG_MANAGER_H_
#define MINIDB_CATALOG_MANAGER_H_

#include <string>
#include <vector>
#include <memory>
#include "ASTNodes.h" // 새롭게 정의한 AST 노드들을 포함

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

class Database;
class Table;
class Attribute;

class CatalogManager
{
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & dbs_;
    }
    std::string path_;
    std::vector<Database> dbs_;

public:
    CatalogManager(std::string p);
    ~CatalogManager();

    std::vector<Database> &dbs() { return dbs_; }
    std::string path() { return path_; }
    Database *GetDB(std::string db_name);

    // 새로운 AST 구조에 맞춘 함수들
    void CreateTable(CreateTableStmtNodePtr createStmt); // CREATE TABLE 지원
    void DropTable(DropTableStmtNodePtr dropStmt);       // DROP TABLE 지원
    void DeleteFromTable(DeleteStmtNodePtr deleteStmt);  // DELETE 지원

    void ReadArchiveFile();
    void WriteArchiveFile();
    void CreateDatabase(std::string dbname);
    void DeleteDatabase(std::string dbname);
};

class Database
{
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & db_name_;
        ar & tbs_;
    }
    std::string db_name_;
    std::vector<Table> tbs_;

public:
    Database() {}
    Database(std::string dbname);
    ~Database() {}
    Table *GetTable(std::string tb_name);
    std::string db_name() { return db_name_; }

    void CreateTable(const std::string &tableName, const std::vector<Attribute> &attrs);
    void DropTable(const std::string &tableName);
    std::vector<Table> &tbs() { return tbs_; }
};

class Table
{
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & tb_name_;
        ar & record_length_;
        ar & first_block_num_;
        ar & first_rubbish_num_;
        ar & block_count_;
        ar & ats_;
    }

    std::string tb_name_;
    int record_length_;

    int first_block_num_;
    int first_rubbish_num_;
    int block_count_;

    std::vector<Attribute> ats_;

public:
    Table()
        : tb_name_(""), record_length_(-1), first_block_num_(-1),
          first_rubbish_num_(-1), block_count_(0) {}
    ~Table() {}

    std::string tb_name() { return tb_name_; }
    void set_tb_name(const std::string &tbname) { tb_name_ = tbname; }

    int record_length() { return record_length_; }
    void set_record_length(int len) { record_length_ = len; }

    std::vector<Attribute> &ats() { return ats_; }
    void AddAttribute(const Attribute &attr) { ats_.push_back(attr); }

    Attribute *GetAttribute(const std::string &name);

    // Block 관련 기능
    int first_block_num() { return first_block_num_; }
    void set_first_block_num(int num) { first_block_num_ = num; }
    int first_rubbish_num() { return first_rubbish_num_; }
    void set_first_rubbish_num(int num) { first_rubbish_num_ = num; }
    int block_count() { return block_count_; }
    void IncreaseBlockCount() { block_count_++; }
};

class Attribute
{
private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & attr_name_;
        ar & data_type_;
        ar & length_;
    }

    std::string attr_name_;
    int data_type_; // 1 = INT, 2 = VARCHAR
    int length_;

public:
    Attribute() : attr_name_(""), data_type_(-1), length_(-1) {}
    ~Attribute() {}

    std::string attr_name() { return attr_name_; }
    void set_attr_name(const std::string &name) { attr_name_ = name; }

    int data_type() { return data_type_; }
    void set_data_type(int type) { data_type_ = type; }

    int length() { return length_; }
    void set_length(int len) { length_ = len; }
};

#endif // MINIDB_CATALOG_MANAGER_H_
