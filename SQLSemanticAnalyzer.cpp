#include "SQLSemanticAnalyzer.h"
#include <iostream>

SQLSemanticAnalyzer::SQLSemanticAnalyzer(CatalogManager &catalogMgr) : catalogMgr_(catalogMgr) {}

bool SQLSemanticAnalyzer::analyzeSelectStmt(SelectStmtNodePtr selectStmt)
{
    // 테이블이 존재하는지 확인
    if (!checkTableExists(selectStmt->tableName))
    {
        std::cerr << "Semantic Error: Table '" << selectStmt->tableName << "' does not exist." << std::endl;
        return false;
    }

    // 속성들이 유효한지 확인
    if (!checkColumnsExist(selectStmt->tableName, selectStmt->columns))
    {
        std::cerr << "Semantic Error: Invalid columns in table '" << selectStmt->tableName << "'." << std::endl;
        return false;
    }

    // 조건절이 유효한지 확인
    if (selectStmt->whereClause && !checkConditionIsValid(selectStmt->tableName, selectStmt->whereClause))
    {
        std::cerr << "Semantic Error: Invalid condition in WHERE clause for table '" << selectStmt->tableName << "'." << std::endl;
        return false;
    }

    return true;
}

bool SQLSemanticAnalyzer::analyzeCreateDatabaseStmt(CreateDatabaseStmtNodePtr createDbStmt)
{
    if (catalogMgr_.doesDatabaseExist(createDbStmt->databaseName))
    {
        std::cerr << "Semantic Error: Database '" << createDbStmt->databaseName << "' already exists." << std::endl;
        return false;
    }
    catalogMgr_.createDatabase(createDbStmt->databaseName);
    return true;
}

bool SQLSemanticAnalyzer::analyzeDropDatabaseStmt(DropDatabaseStmtNodePtr dropDbStmt)
{
    if (!catalogMgr_.doesDatabaseExist(dropDbStmt->databaseName))
    {
        std::cerr << "Semantic Error: Database '" << dropDbStmt->databaseName << "' does not exist." << std::endl;
        return false;
    }
    catalogMgr_.dropDatabase(dropDbStmt->databaseName);
    return true;
}

bool SQLSemanticAnalyzer::analyzeDeleteTableStmt(DeleteTableStmtNodePtr deleteTableStmt)
{
    if (!checkTableExists(deleteTableStmt->tableName))
    {
        std::cerr << "Semantic Error: Table '" << deleteTableStmt->tableName << "' does not exist." << std::endl;
        return false;
    }
    catalogMgr_.deleteTable(deleteTableStmt->tableName);
    return true;
}

bool SQLSemanticAnalyzer::analyzeDeleteStmt(DeleteStmtNodePtr deleteStmt)
{
    if (!checkTableExists(deleteStmt->tableName))
    {
        std::cerr << "Semantic Error: Table '" << deleteStmt->tableName << "' does not exist." << std::endl;
        return false;
    }
    // 조건절에 따른 데이터 삭제
    catalogMgr_.deleteData(deleteStmt->tableName, deleteStmt->whereClause);
    return true;
}

bool SQLSemanticAnalyzer::checkTableExists(const std::string &tableName)
{
    Database *db = catalogMgr_.GetDB("default"); // 기본 DB 이름 사용
    if (db && db->GetTable(tableName))
    {
        return true;
    }
    return false;
}

bool SQLSemanticAnalyzer::checkColumnsExist(const std::string &tableName, const std::vector<std::string> &columns)
{
    Database *db = catalogMgr_.GetDB("default");
    if (!db)
        return false;

    Table *table = db->GetTable(tableName);
    if (!table)
        return false;

    for (const auto &column : columns)
    {
        if (!table->GetAttribute(column))
        {
            std::cerr << "Column '" << column << "' not found in table '" << tableName << "'." << std::endl;
            return false;
        }
    }
    return true;
}

bool SQLSemanticAnalyzer::checkConditionIsValid(const std::string &tableName, std::shared_ptr<ConditionNode> whereClause)
{
    Database *db = catalogMgr_.GetDB("default");
    if (!db)
        return false;

    Table *table = db->GetTable(tableName);
    if (!table)
        return false;

    // 조건절에서 사용하는 컬럼이 테이블에 존재하는지 확인
    Attribute *attr = table->GetAttribute(whereClause->column);
    if (!attr)
    {
        std::cerr << "Column '" << whereClause->column << "' in WHERE clause does not exist in table '" << tableName << "'." << std::endl;
        return false;
    }

    // 데이터 타입이 일치하는지 확인 (NUMBER 또는 STRING)
    if ((whereClause->literalType == "NUMBER" && attr->data_type() != 1) ||
        (whereClause->literalType == "STRING" && attr->data_type() != 2))
    {
        std::cerr << "Type mismatch in WHERE clause for column '" << whereClause->column << "'." << std::endl;
        return false;
    }

    return true;
}
