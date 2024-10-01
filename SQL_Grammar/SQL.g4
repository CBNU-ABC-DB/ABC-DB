grammar SQL;

sql_stmt: select_stmt; // 문법의 최상위 규칙 (현재는 SELECT 하나로 구성)

select_stmt: 'SELECT' column_list 'FROM' IDENTIFIER (where_clause)?; // SELECT SQL문 정의

column_list: IDENTIFIER (',' IDENTIFIER)*; // COLUMN 이름에 대한 규칙

where_clause: 'WHERE' condition;

condition: IDENTIFIER comparator literal_value;

comparator: '=' | '<' | '>' | '<=' | '>=' | '!=';

literal_value: NUMBER | STRING_LITERAL;

IDENTIFIER: [a-zA-Z_][a-zA-Z_0-9]*; // 식별자 정의 (첫 글자는 영문, 이후에는 영문 및 숫자 가능)

NUMBER: [0-9]+;

STRING_LITERAL: '\'' (~['\r\n])* '\''; // 작은 따옴표로 감싼 문자열

WS: [ \t\r\n]+ -> skip;  // 공백, 탭, 줄 바꿈을 무시하는 규칙
