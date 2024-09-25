# Compiler and flags
CXX = g++
CXXFLAGS = -I ./include -pthread -Wall -g -std=c++17 

# Source files and target
SRC = src/*.cpp 
TARGET = build/server.out

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) 

# Clean rule to remove the output binary
clean:
	rm -f $(TARGET)

# Run rule to execute output after build
run: $(TARGET)
	./$(TARGET)
