CXX := g++
CXXFLAGS := -O3
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := main_space

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)