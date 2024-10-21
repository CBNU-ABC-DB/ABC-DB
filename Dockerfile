FROM alpine:latest

RUN apk update && apk add --no-cache \
    build-base \
    alpine-sdk \
    g++ \
    make \
    git \
    cmake \
    boost-dev \
    bzip2-dev \
    openjdk17 \
    libstdc++ \
    libgcc \
    readline-dev \
    zlib-dev

WORKDIR /app

ENV ANTLR_VERSION=4.13.2
ENV ANTLR_JAR=antlr-${ANTLR_VERSION}-complete.jar

RUN mkdir /opt/antlr4 && \
    wget -O /opt/antlr4/${ANTLR_JAR} https://www.antlr.org/download/${ANTLR_JAR}

RUN git clone https://github.com/antlr/antlr4.git /antlr4 && \
    cd /antlr4/runtime/Cpp && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && make && make install && \
    rm -rf /antlr4

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz && \
    tar -xzf boost_1_82_0.tar.gz && \
    cd boost_1_82_0 && \
    ./bootstrap.sh && \
    ./b2 install --with-serialization --with-filesystem --prefix=/usr/local

# SQL.g4 복사 (루트 디렉터리에서 /app으로)
COPY SQL.g4 /app/

# ANTLR4 파서 생성 (SQL.g4의 경로 명시)
RUN java -jar /opt/antlr4/${ANTLR_JAR} -Dlanguage=Cpp -no-listener -visitor /app/SQL.g4

# ANTLR4가 생성한 파일 복사
RUN mkdir -p /app/generated && mv /app/*.h /app/*.cpp /app/generated/

# 소스 코드 복사
COPY ./src /app/src

# RUN g++ -std=c++17 -I/usr/local/include/antlr4-runtime -I/usr/local/include/boost *.cc *.cpp -o main \
#     -lantlr4-runtime  -lboost_iostreams -lboost_system -lreadline


RUN g++ /app/src/main.cpp /app/src/file.cpp /app/src/page.cpp \
    /app/src/bufferManager.cpp  /app/src/interpreter.cpp \
    /app/src/bufferPool.cpp  /app/src/api.cpp /app/src/catalog_manager.cpp \
    /app/src/disk_manager.cpp /app/src/file_handler.cpp\
    /app/generated/SQLLexer.cpp\
    /app/generated/SQLParser.cpp  \
    -o main \
    -I/usr/local/include/antlr4-runtime -I/usr/local/include/boost \
    -I/app/generated \
    -L/usr/local/lib -lantlr4-runtime -lboost_serialization \
    -lboost_filesystem -lboost_iostreams -lboost_system -lreadline


# 실행 명령어 설정
CMD ["./main"]