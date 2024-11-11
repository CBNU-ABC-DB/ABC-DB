#ifndef ABCDB_EXECUTION_ENGINE_H_
#define ABCDB_EXECUTION_ENGINE_H_

#include <string>
#include <vector>
#include "catalog_manager.h"
#include "sql_statement.h"
#include "file.h"
#include "exceptions.h"
#include "buffer_manager.h"

class ExecutionEngine {
private:
    CatalogManager *cm_;
    BufferManager *bm_;
    std::string db_name_;

    std::vector<TKey> ParseRecord(Table *tbl, std::vector<char> &data, int offset);
    
    bool EvaluateConditions(const std::vector<TKey> &record, Table *tbl, const std::vector<SQLWhere> &wheres);

public:
    ExecutionEngine(CatalogManager *cm, std::string db, BufferManager *bm)
        : cm_(cm), db_name_(db), bm_(bm) {}
    ~ExecutionEngine() {}

    void Insert(SQLInsert &st);
    void Select(SQLSelect &st);
};

#endif