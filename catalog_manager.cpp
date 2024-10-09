#include "catalog_manager.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

using namespace std;

//=======================CatalogManager=======================//

CatalogManager::CatalogManager(std::string p) : path_(p)
{
    ReadArchiveFile(); // 기존 데이터베이스 정보를 로드
}

CatalogManager::~CatalogManager()
{
    WriteArchiveFile(); // 데이터베이스 정보를 저장
}

void CatalogManager::ReadArchiveFile()
{
    std::string file_name = path_ + "catalog";
    boost::filesystem::path file_path(file_name);

    if (boost::filesystem::exists(file_path))
    {
        std::ifstream ifs(file_name, std::ios::binary);
        boost::archive::binary_iarchive iar(ifs);
        iar >> (*this);
        ifs.close();
    }
}

void CatalogManager::WriteArchiveFile()
{
    std::string file_name = path_ + "catalog";
    std::ofstream ofs(file_name, std::ios::binary);
    boost::archive::binary_oarchive oar(ofs);
    oar << (*this);
    ofs.close();
}

Database *CatalogManager::GetDB(std::string db_name)
{
    for (Database &db : dbs_)
    {
        if (db.db_name() == db_name)
        {
            return &db;
        }
    }
    return nullptr;
}

// CREATE TABLE
void CatalogManager::CreateTable(CreateTableStmtNodePtr createStmt)
{
    Database *db = GetDB("default"); // 기본 DB 사용
    if (db)
    {
        Table tb;
        tb.set_tb_name(createStmt->tableName);
        for (const auto &col : createStmt->columns)
        {
            Attribute attr;
            attr.set_attr_name(col.first);
            if (col.second == "INT")
            {
                attr.set_data_type(1); // INT = 1
                attr.set_length(4);    // INT 크기
            }
            else if (col.second.find("VARCHAR") != std::string::npos)
            {
                attr.set_data_type(2);                                                 // VARCHAR = 2
                int length = std::stoi(col.second.substr(8, col.second.length() - 9)); // VARCHAR 길이 추출
                attr.set_length(length);
            }
            tb.AddAttribute(attr);
        }
        db->tbs().push_back(tb);
    }
}

// DROP TABLE
void CatalogManager::DropTable(DropTableStmtNodePtr dropStmt)
{
    Database *db = GetDB("default");
    if (db)
    {
        db->DropTable(dropStmt->tableName);
    }
}

// DELETE FROM TABLE
void CatalogManager::DeleteFromTable(DeleteStmtNodePtr deleteStmt)
{
    Database *db = GetDB("default");
    if (!db)
        return;

    Table *table = db->GetTable(deleteStmt->tableName);
    if (table)
    {
        if (deleteStmt->whereClause)
        {
            cout << "Deleting rows from " << deleteStmt->tableName
                 << " where " << deleteStmt->whereClause->column
                 << " " << deleteStmt->whereClause->comparator
                 << " " << deleteStmt->whereClause->literalValue << endl;
            // 실제로 조건에 맞는 행 삭제 로직을 여기에 구현
        }
        else
        {
            cout << "Deleting all rows from " << deleteStmt->tableName << endl;
            // 테이블의 모든 행 삭제 로직을 여기에 구현
        }
    }
}

//=======================Database=============================//

Database::Database(std::string dbname) : db_name_(dbname) {}

void Database::CreateTable(const std::string &tableName, const std::vector<Attribute> &attrs)
{
    Table tb;
    tb.set_tb_name(tableName);
    tb.ats() = attrs;
    tbs_.push_back(tb);
}

void Database::DropTable(const std::string &tableName)
{
    tbs_.erase(std::remove_if(tbs_.begin(), tbs_.end(), [&tableName](Table &t)
                              { return t.tb_name() == tableName; }),
               tbs_.end());
}

Table *Database::GetTable(std::string tb_name)
{
    for (Table &tb : tbs_)
    {
        if (tb.tb_name() == tb_name)
        {
            return &tb;
        }
    }
    return nullptr;
}

//=======================Table===============================//

Attribute *Table::GetAttribute(const std::string &name)
{
    for (Attribute &attr : ats_)
    {
        if (attr.attr_name() == name)
        {
            return &attr;
        }
    }
    return nullptr;
}
