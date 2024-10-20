FROM alpine:latest

# 필수 패키지 설치
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
    --no-cache readline-dev \
    zlib-dev

# ANTLR4 설정
ENV ANTLR_VERSION=4.13.2
ENV ANTLR_JAR=antlr-${ANTLR_VERSION}-complete.jar

# ANTLR4 다운로드
RUN mkdir /opt/antlr4 && \
    wget -O /opt/antlr4/${ANTLR_JAR} https://www.antlr.org/download/${ANTLR_JAR}

# ANTLR4 C++ 런타임 설치
RUN git clone https://github.com/antlr/antlr4.git /antlr4 && \
    cd /antlr4/runtime/Cpp && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && make && make install && \
    rm -rf /antlr4

# Boost 라이브러리 설치
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz && \
    tar -xzf boost_1_82_0.tar.gz && \
    cd boost_1_82_0 && \
    ./bootstrap.sh && \
    ./b2 install --with-serialization --with-filesystem --prefix=/usr/local

# 프로젝트 복사
WORKDIR /usr/src/app
COPY . .

# SQL.g4로 ANTLR4 파서 생성
RUN java -jar /opt/antlr4/${ANTLR_JAR} -Dlanguage=Cpp -no-listener -visitor SQL.g4

# C++ 소스코드 컴파일
RUN g++ -std=c++17 -I/usr/local/include/antlr4-runtime -I/usr/local/include/boost *.cc *.cpp -o main \
-lantlr4-runtime -lboost_serialization -lboost_filesystem -lboost_iostreams -lboost_system -lreadline

# 실행 명령어 설정
CMD ["./main"]
