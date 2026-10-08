CXX=g++
CXXFLAGS=-O3 -march=native -std=c++20 -Wall -Wextra -Wpedantic
TARGET=fix_demo
SRC=main.cpp fix_parser.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
