#ifndef ASTNODES_H
#define ASTNODES_H

#include <string>
#include <vector>
#include <memory>

class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

class ExpressionNode : public ASTNode
{
public:
    virtual ~ExpressionNode() = default;
};

class ConditionNode : public ExpressionNode
{
public:
    std::string column;
    std::string comparator;
    std::string literalValue;
    std::string literalType; // 'NUMBER' 또는 'STRING'

    ConditionNode(const std::string &col, const std::string &comp, const std::string &value, const std::string &type)
        : column(col), comparator(comp), literalValue(value), literalType(type) {}
};

class SelectStmtNode : public ASTNode
{
public:
    std::vector<std::string> columns;
    std::string tableName;
    std::shared_ptr<ConditionNode> whereClause; // WHERE 절이 있을 수 있음
};

using ASTNodePtr = std::shared_ptr<ASTNode>;
using SelectStmtNodePtr = std::shared_ptr<SelectStmtNode>;

#endif // ASTNODES_H