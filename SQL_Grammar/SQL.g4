grammar SQL;

sql_stmt: select_stmt | create_stmt | drop_stmt | delete_stmt; // 여러 SQL 문 지원

select_stmt: 'SELECT' (column_list | STAR) 'FROM' IDENTIFIER (where_clause)?; // '*' 토큰 사용

create_stmt: 'CREATE' 'TABLE' IDENTIFIER '(' column_def_list ')'; // CREATE TABLE 문 정의

drop_stmt: 'DROP' 'TABLE' IDENTIFIER; // DROP TABLE 문 정의

delete_stmt: 'DELETE' 'FROM' IDENTIFIER (where_clause)?; // DELETE FROM 문 정의

column_list: IDENTIFIER (',' IDENTIFIER)*; // SELECT문 속성 목록

column_def_list: column_def (',' column_def)*; // CREATE문 속성 정의 목록

column_def: IDENTIFIER data_type; // 속성 정의

data_type: 'INT' | 'VARCHAR' '(' NUMBER ')'; // 지원하는 데이터 타입

where_clause: 'WHERE' condition;

condition: IDENTIFIER comparator literal_value;

comparator: '=' | '<' | '>' | '<=' | '>=' | '!=';

literal_value: NUMBER | STRING_LITERAL;

// '*' 기호를 위한 토큰 정의
STAR: '*'; 

IDENTIFIER: [a-zA-Z_][a-zA-Z_0-9]*; // 식별자 정의 (첫 글자는 영문, 이후에는 영문 및 숫자 가능)

NUMBER: [0-9]+;

STRING_LITERAL: '\'' (~['\r\n])* '\''; // 작은 따옴표로 감싼 문자열

WS: [ \t\r\n]+ -> skip;  // 공백, 탭, 줄 바꿈을 무시하는 규칙
