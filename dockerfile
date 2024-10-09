# Use the official Alpine Linux as the base image
FROM alpine:latest

# Install necessary packages
RUN apk update && apk add --no-cache \
    build-base \
    cmake \
    git \
    make \
    wget \
    openjdk17 \
    libstdc++ \
    libgcc \
    boost-dev \
    linux-headers

# Set environment variables for ANTLR4
ENV ANTLR_VERSION=4.13.2
ENV ANTLR_JAR=antlr-${ANTLR_VERSION}-complete.jar

# Download ANTLR4 tool
RUN mkdir /opt/antlr4 && \
    wget -O /opt/antlr4/${ANTLR_JAR} https://www.antlr.org/download/${ANTLR_JAR}

# Set up ANTLR4 C++ runtime
RUN git clone https://github.com/antlr/antlr4.git /antlr4 && \
    cd /antlr4/runtime/Cpp && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && make && make install && \
    rm -rf /antlr4

# Set up the working directory
WORKDIR /app

# Copy your project files to the container
COPY . .

# Make the build.sh script executable
RUN chmod +x build.sh

# Build the project using the build.sh script
RUN ./build.sh

# Set environment variables for runtime
ENV CLASSPATH="/opt/antlr4/${ANTLR_JAR}:/app"
ENV LD_LIBRARY_PATH="/usr/local/lib:/usr/lib"

# Define the entry point for the container
CMD ["./QueryParser"]
