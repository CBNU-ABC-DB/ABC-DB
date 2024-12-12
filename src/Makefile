CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/local/include/antlr4-runtime -I/usr/local/include/boost

LDFLAGS = -L/usr/local/lib -lboost_serialization -lboost_filesystem -lboost_iostreams -lboost_system -lreadline -lantlr4-runtime

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d) 

TARGET = ABC

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)