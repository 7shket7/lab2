CXX = g++
CXXFLAGS = -std=c++11 -Wall
TARGET = cipher
SOURCES = main.cpp RouteCipher.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run
