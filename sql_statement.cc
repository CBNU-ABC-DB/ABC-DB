#include "sql_statement.h"

#include <iomanip>
#include <iostream>

#include <boost/algorithm/string.hpp>

#include "commons.h"
#include "exceptions.h"

using namespace boost::algorithm;
using namespace std;

TKey::TKey(int keytype, int length)
{
  key_type_ = keytype;
  if (keytype == 2)
    length_ = length;
  else
    length_ = 4;
  key_ = new char[length_];
}

TKey::TKey(const TKey &t1)
{
  key_type_ = t1.key_type_;
  length_ = t1.length_;
  key_ = new char[length_];
  memcpy(key_, t1.key_, length_);
}

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

TKey::~TKey()
{
  if (key_ != NULL)
    delete[] key_;
}

std::ostream &operator<<(std::ostream &out, const TKey &object)
{
  switch (object.key_type_)
  {
  case 0:
  {
    int a;
    memcpy(&a, object.key_, object.length_);
    cout << setw(9) << left << a;
  }
  break;
  case 1:
  {
    float a;
    memcpy(&a, object.key_, object.length_);
    cout << setw(9) << left << a;
  }
  break;
  case 2:
  {
    cout << setw(9) << left << object.key_;
  }
  break;
  }

  return out;
}

bool TKey::operator<(const TKey t1)
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

bool TKey::operator>(const TKey t1)
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

bool TKey::operator<=(const TKey t1) { return !(operator>(t1)); }

bool TKey::operator>=(const TKey t1) { return !(operator<(t1)); }

bool TKey::operator==(const TKey t1)
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

bool TKey::operator!=(const TKey t1)
{
  switch (t1.key_type_)
  {
  case 0:
    return *(int *)key_ != *(int *)t1.key_;
  case 1:
    return *(float *)key_ != *(float *)t1.key_;
  case 2:
    return (strncmp(key_, t1.key_, length_) != 0);
  default:
    return false;
  }
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
