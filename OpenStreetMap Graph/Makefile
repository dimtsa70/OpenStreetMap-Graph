TARGET = hw5

CXX =g++
CXXFLAGS = -std=c++17 -Wall -g

SRCS = HW5.cpp Graph.cpp Vertex.cpp Edge.cpp tinyxml2.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)	