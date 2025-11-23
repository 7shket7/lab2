CXX = g++
CXXFLAGS = -std=c++11
TARGET = cipher
SOURCES = main.cpp modAlphaCipher.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: clean
