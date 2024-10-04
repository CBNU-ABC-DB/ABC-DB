#ifndef ABCDB_DISK_MANAGER_H_
#define ABCDB_DISK_MANAGER_H_

#include <string>
#include "catalog_manager.h"
#include "sql_statement.h"
#include "file.h"
#include "exceptions.h"
#include "bufferManager.h"

class DiskManager{
    private:
        //BufferManager
        CatalogManager *cm_;
        BufferManager *bm_;
        std::string db_name_;


        std::vector<TKey> ParseRecord(Table *tbl, std::vector<char> &data, int offset);
    public:
        DiskManager(CatalogManager *cm, std::string db, BufferManager *bm)
            : cm_(cm),db_name_(db),bm_(bm){}
        ~DiskManager() {}
        void Insert(SQLInsert &st);
        void Select(SQLSelect &st);

};

#endif // ABCDB_DISK_MANAGER_H_