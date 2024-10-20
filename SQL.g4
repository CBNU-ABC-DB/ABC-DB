grammar SQL;

options {
    language = Cpp;
}

sqlStatement
    : createDatabase
    | dropDatabase
    | createTable
    | dropTable
    | createIndex
    | dropIndex
    | useDatabase
    | insertInto
    | selectStatement
    | deleteStatement
    | updateStatement
    | execStatement
    | showDatabases
    | showTables
    | helpStatement
    | quitStatement
    ;

createDatabase
    : CREATE DATABASE IDENTIFIER
    ;

dropDatabase
    : DROP DATABASE IDENTIFIER
    ;

createTable
    : CREATE TABLE IDENTIFIER LPAREN columnDefinition (COMMA columnDefinition)* (COMMA primaryKeyDefinition)? RPAREN
    ;

dropTable
    : DROP TABLE IDENTIFIER
    ;

createIndex
    : CREATE INDEX IDENTIFIER ON IDENTIFIER LPAREN IDENTIFIER RPAREN
    ;

dropIndex
    : DROP INDEX IDENTIFIER
    ;

useDatabase
    : USE IDENTIFIER
    ;

insertInto
    : INSERT INTO IDENTIFIER VALUES LPAREN valueList RPAREN
    ;

valueList
    : value (COMMA value)*
    ;

value
    : STRING_LITERAL
    | NUMERIC_LITERAL
    ;

selectStatement
    : SELECT STAR FROM IDENTIFIER (WHERE condition (AND condition)*)?
    ;

condition
    : IDENTIFIER comparator value
    ;

comparator
    : EQ
    | NEQ
    | LT
    | LEQ
    | GT
    | GEQ
    ;

deleteStatement
    : DELETE FROM IDENTIFIER (WHERE condition (AND condition)*)?
    ;

updateStatement
    : UPDATE IDENTIFIER SET assignmentList (WHERE condition (AND condition)*)?
    ;

assignmentList
    : assignment (COMMA assignment)*
    ;

assignment
    : IDENTIFIER EQ value
    ;

execStatement
    : EXEC IDENTIFIER
    ;

showDatabases
    : SHOW DATABASES
    ;

showTables
    : SHOW TABLES
    ;

helpStatement
    : HELP
    ;

quitStatement
    : QUIT
    ;

columnDefinition
    : IDENTIFIER dataType
    ;

dataType
    : INT
    | FLOAT
    | CHAR LPAREN NUMERIC_LITERAL RPAREN
    ;

primaryKeyDefinition
    : PRIMARY KEY LPAREN IDENTIFIER RPAREN
    ;

CREATE: 'CREATE';
DATABASE: 'DATABASE';
TABLE: 'TABLE';
INDEX: 'INDEX';
ON: 'ON';
DROP: 'DROP';
USE: 'USE';
INSERT: 'INSERT';
INTO: 'INTO';
VALUES: 'VALUES';
SELECT: 'SELECT';
FROM: 'FROM';
WHERE: 'WHERE';
AND: 'AND';
OR: 'OR';
NOT: 'NOT';
SET: 'SET';
UPDATE: 'UPDATE';
DELETE: 'DELETE';
EXEC: 'EXEC';
SHOW: 'SHOW';
HELP: 'HELP';
QUIT: 'QUIT';
DATABASES: 'DATABASES';
TABLES: 'TABLES';
PRIMARY: 'PRIMARY';
KEY: 'KEY';
INT: 'INT';
FLOAT: 'FLOAT';
CHAR: 'CHAR';
NULL: 'NULL';
IS: 'IS';
IN: 'IN';
STAR: '*';

EQ: '=';
NEQ: '<>';
LT: '<';
LEQ: '<=';
GT: '>';
GEQ: '>=';

LPAREN: '(';
RPAREN: ')';
COMMA: ',';
SEMICOLON: ';';

STRING_LITERAL
    : '\'' ( ~'\'' | '\'\'' )* '\''
    ;

NUMERIC_LITERAL
    : [0-9]+ ('.' [0-9]+)?
    ;

IDENTIFIER
    : [a-zA-Z_] [a-zA-Z0-9_]*
    ;

WS
    : [ \t\r\n]+ -> skip
    ;
