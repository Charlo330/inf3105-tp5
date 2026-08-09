CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic
TARGET = tp5
SRC = main.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC) graphe.h algographe.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)