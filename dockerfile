FROM alpine

RUN apk update && apk add --no-cache \
    alpine-sdk \
    g++ \
    cmake \
    make \
    git \
    boost-dev \
    bzip2-dev \
    zlib-dev

WORKDIR /app

RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz && \
    tar -xzf boost_1_82_0.tar.gz && \
    cd boost_1_82_0 && \
    ./bootstrap.sh && \
    ./b2 install --with-serialization --with-filesystem --prefix=/usr/local

COPY ./src .

RUN g++ main.cpp file.cpp page.cpp -o main -L/usr/local/lib -lboost_serialization -lboost_filesystem -lboost_iostreams -lboost_system
