FROM alpine:latest

RUN apk update && apk add --no-cache \
    figlet \
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

# ANTLR4 파서 생성 (생성 경로 바꿈)
RUN java -jar /opt/antlr4/${ANTLR_JAR} -Dlanguage=Cpp -no-listener -visitor -o /app/src /app/SQL.g4

# 소스 코드 복사
COPY ./src /app/src

ENV APP=/app
# RUN g++ -std=c++17 /app/src/*.cpp \
#     -I/usr/local/include/antlr4-runtime -I/usr/local/include/boost \
#     -I/app/src \
#     -o main \
#     -L/usr/local/lib -lantlr4-runtime -lboost_serialization \
#     -lboost_filesystem -lboost_iostreams -lboost_system -lreadline

# 실행 명령어 설정
# CMD ["./main"]