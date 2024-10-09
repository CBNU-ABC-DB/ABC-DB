#!/bin/sh

# Generate lexer and parser from SQL.g4 using ANTLR4
java -jar /opt/antlr4/antlr-4.13.2-complete.jar -Dlanguage=Cpp -visitor -o SQL_Grammar SQL.g4

# Compile the project with the generated lexer and parser files using C++17
g++ -std=c++17 \
    -I/usr/local/include/antlr4-runtime \
    -I./SQL_Grammar \
    -I. \
    main.cpp \
    catalog_manager.cpp \
    SQLASTBuilder.cpp \
    SQLSemanticAnalyzer.cpp \
    SQL_Grammar/*.cpp \
    -lantlr4-runtime \
    -lboost_serialization \
    -lboost_system \
    -lboost_filesystem \
    -o QueryParser
