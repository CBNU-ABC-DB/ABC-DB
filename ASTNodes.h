#ifndef ASTNODES_H
#define ASTNODES_H

#include <string>
#include <vector>
#include <memory>

// 추상 ASTNode 클래스
class ASTNode
{
public:
    virtual ~ASTNode() = default;
};

// 표현식용 클래스 (숫자, 문자열, 이항표현식 등등)
class ExpressionNode : public ASTNode
{
public:
    virtual ~ExpressionNode() = default;
};

// Where문 조건절 클래스 (단순 비교 >=, ==, <=)
class ConditionNode : public ExpressionNode
{
public:
    std::string column;       // 속성
    std::string comparator;   // 비교 연산자
    std::string literalValue; // 비교하는 값
    std::string literalType;  // 'NUMBER' 또는 'STRING'

    ConditionNode(const std::string &col, const std::string &comp, const std::string &value, const std::string &type)
        : column(col), comparator(comp), literalValue(value), literalType(type) {}
};

// Select 문에 대한 AST 노드
class SelectStmtNode : public ASTNode
{
public:
    std::string tableName;                      // 테이블 명
    std::vector<std::string> columns;           // 속성
    std::shared_ptr<ConditionNode> whereClause; // 조건절

    // 기존 생성자
    SelectStmtNode(const std::string &table, const std::vector<std::string> &cols, std::shared_ptr<ConditionNode> where = nullptr)
        : tableName(table), columns(cols), whereClause(where) {}

    // 기본 생성자 추가
    SelectStmtNode() = default;
};

// Create Table 문에 대한 AST 노드
class CreateTableStmtNode : public ASTNode
{
public:
    std::string tableName;                                    // 테이블 이름
    std::vector<std::pair<std::string, std::string>> columns; // 컬럼 이름과 타입

    CreateTableStmtNode(const std::string &table, const std::vector<std::pair<std::string, std::string>> &cols)
        : tableName(table), columns(cols) {}
};

// Drop Table 문에 대한 AST 노드
class DropTableStmtNode : public ASTNode
{
public:
    std::string tableName; // 삭제할 테이블 이름

    DropTableStmtNode(const std::string &table) : tableName(table) {}
};

// Delete 문에 대한 AST 노드
class DeleteStmtNode : public ASTNode
{
public:
    std::string tableName;                      // 테이블 이름
    std::shared_ptr<ConditionNode> whereClause; // WHERE 절

    DeleteStmtNode(const std::string &table, std::shared_ptr<ConditionNode> where = nullptr)
        : tableName(table), whereClause(where) {}
};

// 타입 정의
using ASTNodePtr = std::shared_ptr<ASTNode>;
using SelectStmtNodePtr = std::shared_ptr<SelectStmtNode>;
using CreateTableStmtNodePtr = std::shared_ptr<CreateTableStmtNode>;
using DropTableStmtNodePtr = std::shared_ptr<DropTableStmtNode>;
using DeleteStmtNodePtr = std::shared_ptr<DeleteStmtNode>;

#endif // ASTNODES_H
