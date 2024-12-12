#ifndef ABCDB_COMMONS_H_
#define ABCDB_COMMONS_H_

// File Format
#define FORMAT_RECORD 0
#define FORMAT_INDEX 1

// Data Type
#define T_INT 0
#define T_FLOAT 1
#define T_CHAR 2

//=	<>	<	>	<=	>=
#define SIGN_EQ 0
#define SIGN_NE 1
#define SIGN_LT 2
#define SIGN_GT 3
#define SIGN_LE 4
#define SIGN_GE 5

// 데이터 타입 값을 문자열로 변환하는 함수
inline std::string DataTypeToString(int data_type)
{
    switch (data_type)
    {
    case T_INT:
        return "int";
    case T_FLOAT:
        return "float";
    case T_CHAR:
        return "char";
    default:
        return "unknown";
    }
}
#endif