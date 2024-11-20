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
    | testRecordStatement
    ;

testRecordStatement
    : TEST RECORD NUMERIC_LITERAL INTO IDENTIFIER VALUES LPAREN valueList RPAREN
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

CREATE: [cC][rR][eE][aA][tT][eE];
DATABASE: [dD][aA][tT][aA][bB][aA][sS][eE];
TABLE: [tT][aA][bB][lL][eE];
INDEX: [iI][nN][dD][eE][xX];
ON: [oO][nN];
DROP: [dD][rR][oO][pP];
USE: [uU][sS][eE];
INSERT: [iI][nN][sS][eE][rR][tT];
INTO: [iI][nN][tT][oO];
VALUES: [vV][aA][lL][uU][eE][sS];
SELECT: [sS][eE][lL][eE][cC][tT];
FROM: [fF][rR][oO][mM];
WHERE: [wW][hH][eE][rR][eE];
AND: [aA][nN][dD];
OR: [oO][rR];
NOT: [nN][oO][tT];
SET: [sS][eE][tT];
UPDATE: [uU][pP][dD][aA][tT][eE];
DELETE: [dD][eE][lL][eE][tT][eE];
EXEC: [eE][xX][eE][cC];
SHOW: [sS][hH][oO][wW];
HELP: [hH][eE][lL][pP];
QUIT: [qQ][uU][iI][tT];
DATABASES: [dD][aA][tT][aA][bB][aA][sS][eE][sS];
TABLES: [tT][aA][bB][lL][eE][sS];
PRIMARY: [pP][rR][iI][mM][aA][rR][yY];
KEY: [kK][eE][yY];
INT: [iI][nN][tT];
FLOAT: [fF][lL][oO][aA][tT];
CHAR: [cC][hH][aA][rR];
NULL: [nN][uU][lL][lL];
IS: [iI][sS];
IN: [iI][nN];
STAR: '*';

TEST: [tT][eE][sS][tT];
RECORD: [rR][eE][cC][oO][rR][dD];

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
    | '"' ( ~'"' | '""' )* '"'
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
