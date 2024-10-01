#ifndef SYSTEMCATALOG_H
#define SYSTEMCATALOG_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <map>

class SystemCatalog
{
public:
    // 테이블 이름을 키로 하고, 컬럼 이름과 데이터 타입을 매핑하는 맵을 값으로 갖는 맵
    std::unordered_map<std::string, std::map<std::string, std::string>> tables;

    SystemCatalog()
    {
        // 임시 데이터 초기화
        tables["student"] = {
            {"id", "INT"},
            {"name", "VARCHAR"},
            {"age", "INT"}};

        tables["course"] = {
            {"course_id", "INT"},
            {"title", "VARCHAR"},
            {"credits", "INT"}};
    }

    bool tableExists(const std::string &tableName) const
    {
        return tables.find(tableName) != tables.end();
    }

    bool columnExists(const std::string &tableName, const std::string &columnName) const
    {
        auto it = tables.find(tableName);
        if (it != tables.end())
        {
            const auto &columns = it->second;
            return columns.find(columnName) != columns.end();
        }
        return false;
    }

    // 컬럼의 데이터 타입을 반환하는 함수
    std::string getColumnType(const std::string &tableName, const std::string &columnName) const
    {
        auto tableIt = tables.find(tableName);
        if (tableIt != tables.end())
        {
            const auto &columns = tableIt->second;
            auto colIt = columns.find(columnName);
            if (colIt != columns.end())
            {
                return colIt->second;
            }
        }
        return ""; // 컬럼이나 테이블이 존재하지 않는 경우 빈 문자열 반환
    }
};

#endif // SYSTEMCATALOG_H