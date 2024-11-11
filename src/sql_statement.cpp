#include "sql_statement.h"

#include <iomanip>
#include <iostream>
#include <cstring> // memcpy 사용을 위해 추가

#include <boost/algorithm/string.hpp>

#include "commons.h"
#include "exceptions.h"

using namespace boost::algorithm;
using namespace std;

// 생성자
TKey::TKey(int keytype, int length)
{
  key_type_ = keytype;
  if (keytype == 2)
    length_ = length;
  else
    length_ = 4;
  key_ = new char[length_];
}

// 복사 생성자
TKey::TKey(const TKey &t1)
{
  key_type_ = t1.key_type_;
  length_ = t1.length_;
  key_ = new char[length_];
  memcpy(key_, t1.key_, length_);
}

// 대입 연산자
TKey &TKey::operator=(const TKey &t1)
{
  if (this != &t1)
  { // 자기 자신과의 대입 방지
    key_type_ = t1.key_type_;
    length_ = t1.length_;
    delete[] key_;            // 기존 메모리 해제
    key_ = new char[length_]; // 새로운 메모리 할당
    memcpy(key_, t1.key_, length_);
  }
  return *this;
}

// 값 읽기 (char*)
void TKey::ReadValue(const char *content)
{
  switch (key_type_)
  {
  case 0:
  {
    int a = std::atoi(content);
    memcpy(key_, &a, length_);
  }
  break;
  case 1:
  {
    float a = std::atof(content);
    memcpy(key_, &a, length_);
  }
  break;
  case 2:
  {
    memcpy(key_, content, length_);
  }
  break;
  }
}

// 값 읽기 (std::string)
void TKey::ReadValue(std::string str)
{
  switch (key_type_)
  {
  case 0:
  {
    int a = std::atoi(str.c_str());
    memcpy(key_, &a, length_);
  }
  break;
  case 1:
  {
    float a = std::atof(str.c_str());
    memcpy(key_, &a, length_);
  }
  break;
  case 2:
  {
    memcpy(key_, str.c_str(), length_);
  }
  break;
  }
}

// 소멸자
TKey::~TKey()
{
  if (key_ != NULL)
    delete[] key_;
}

// 스트림 연산자 오버로딩
std::ostream &operator<<(std::ostream &out, const TKey &object)
{
  switch (object.key_type_)
  {
  case 0:
  {
    int a;
    memcpy(&a, object.key_, object.length_);
    out << std::setw(9) << std::left << a;
  }
  break;
  case 1:
  {
    float a;
    memcpy(&a, object.key_, object.length_);
    out << std::setw(9) << std::left << a;
  }
  break;
  case 2:
  {
    out << std::setw(9) << std::left << object.key_;
  }
  break;
  }

  return out;
}

// 비교 연산자 오버로딩
bool TKey::operator<(const TKey &t1) const
{
  switch (t1.key_type_)
  {
  case 0:
    return *(int *)key_ < *(int *)t1.key_;
  case 1:
    return *(float *)key_ < *(float *)t1.key_;
  case 2:
    return (strncmp(key_, t1.key_, length_) < 0);
  default:
    return false;
  }
}

bool TKey::operator>(const TKey &t1) const
{
  switch (t1.key_type_)
  {
  case 0:
    return *(int *)key_ > *(int *)t1.key_;
  case 1:
    return *(float *)key_ > *(float *)t1.key_;
  case 2:
    return (strncmp(key_, t1.key_, length_) > 0);
  default:
    return false;
  }
}

bool TKey::operator<=(const TKey &t1) const
{
  return !(operator>(t1));
}

bool TKey::operator>=(const TKey &t1) const
{
  return !(operator<(t1));
}

bool TKey::operator==(const TKey &t1) const
{
  switch (t1.key_type_)
  {
  case 0:
    return *(int *)key_ == *(int *)t1.key_;
  case 1:
    return *(float *)key_ == *(float *)t1.key_;
  case 2:
    return (strncmp(key_, t1.key_, length_) == 0);
  default:
    return false;
  }
}

bool TKey::operator!=(const TKey &t1) const
{
  return !(operator==(t1));
}

int SQL::ParseDataType(std::vector<std::string> sql_vector, Attribute &attr,
                       unsigned int pos)
{
  boost::algorithm::to_lower(sql_vector[pos]);

  if (sql_vector[pos] == "int")
  {
    std::cout << "TYPE: "
              << "int" << std::endl;
    attr.set_data_type(T_INT);
    attr.set_length(4);
    pos++;
    if (sql_vector[pos] == ",")
    {
      pos++;
    }
  }
  else if (sql_vector[pos] == "float")
  {
    std::cout << "TYPE: "
              << "float" << std::endl;
    attr.set_data_type(T_FLOAT);
    attr.set_length(4);
    pos++;
    if (sql_vector[pos] == ",")
    {
      pos++;
    }
  }
  else if (sql_vector[pos] == "char")
  {
    attr.set_data_type(T_CHAR);
    pos++;
    if (sql_vector[pos] == "(")
    {
      pos++;
    }
    attr.set_length(atoi(sql_vector[pos].c_str()));
    pos++;
    if (sql_vector[pos] == ")")
    {
      pos++;
    }
    if (sql_vector[pos] == ",")
    {
      pos++;
    }
  }
  else
  {
    throw SyntaxErrorException();
  }

  return pos;
}