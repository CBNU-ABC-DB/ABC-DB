grammar SQL;

// 여러 SQL 문 지원
sql_stmt: select_stmt 
        | create_stmt 
        | drop_stmt 
        | delete_stmt 
        | create_db_stmt 
        | drop_db_stmt; 

// SELECT 문 정의
select_stmt: 'SELECT' (column_list | STAR) 'FROM' IDENTIFIER (where_clause)?; 

// CREATE 문 정의 (TABLE과 DATABASE 추가)
create_stmt: 'CREATE' 'TABLE' IDENTIFIER '(' column_def_list ')' // CREATE TABLE 문 정의
           | 'CREATE' 'DATABASE' IDENTIFIER; // CREATE DATABASE 문 정의

// DROP 문 정의 (TABLE과 DATABASE 추가)
drop_stmt: 'DROP' 'TABLE' IDENTIFIER // DROP TABLE 문 정의
         | 'DROP' 'DATABASE' IDENTIFIER; // DROP DATABASE 문 정의

// DELETE 문 정의 (테이블과 데이터 삭제 지원)
delete_stmt: 'DELETE' 'FROM' IDENTIFIER (where_clause)?; // DELETE 데이터 문 정의
delete_table_stmt: 'DELETE' 'TABLE' IDENTIFIER; // DELETE TABLE 문 정의

// INSERT 문 정의
insert_stmt: 'INSERT' 'INTO' IDENTIFIER '(' column_list ')' 'VALUES' '(' value_list ')'; 

// 컬럼 목록 정의
column_list: IDENTIFIER (',' IDENTIFIER)*; 

// 값 목록 정의
value_list: literal_value (',' literal_value)*;

// 컬럼 정의 목록
column_def_list: column_def (',' column_def)*; 

// 컬럼 정의
column_def: IDENTIFIER data_type; 

// 데이터 타입 정의
data_type: 'INT' | 'VARCHAR' '(' NUMBER ')'; 

// WHERE 절 정의
where_clause: 'WHERE' condition; 

// 조건 정의
condition: IDENTIFIER comparator literal_value; 

// 비교 연산자 정의
comparator: '=' | '<' | '>' | '<=' | '>=' | '!='; 

// 값 정의 (숫자 또는 문자열)
literal_value: NUMBER | STRING_LITERAL; 

// '*' 기호를 위한 토큰 정의
STAR: '*'; 

// 식별자 정의 (테이블명, 컬럼명 등)
IDENTIFIER: [a-zA-Z_][a-zA-Z_0-9]*; 

// 숫자 정의
NUMBER: [0-9]+; 

// 문자열 정의 (작은 따옴표로 감싼 문자열)
STRING_LITERAL: '\'' (~['\r\n])* '\''; 

// 공백, 탭, 줄 바꿈 무시
WS: [ \t\r\n]+ -> skip;  
