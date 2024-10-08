# 컴파일러 설정
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# 링크 옵션 (라이브러리 경로와 라이브러리)
LDFLAGS = -L/usr/local/lib -lboost_serialization -lboost_filesystem -lboost_iostreams -lboost_system -lreadline

# 소스 파일 및 오브젝트 파일 목록
SRCS = main.cpp page.cpp api.cpp catalog_manager.cpp file.cpp disk_manager.cpp sql_statement.cpp interpreter.cpp file_handler.cpp bufferManager.cpp bufferPool.cpp
OBJS = $(SRCS:.cpp=.o)

# 실행 파일 이름
TARGET = ABC

# 기본 빌드 규칙
all: $(TARGET)

# 링크하여 실행 파일 생성
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# 개별 소스 파일을 컴파일하여 오브젝트 파일 생성
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 클린 규칙 (오브젝트 파일 및 실행 파일 삭제)
clean:
	rm -f $(OBJS) $(TARGET)
