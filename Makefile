CXX=g++

TARGET=Lab2

SOURCES=main.cpp

OBJECTS=$(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean
