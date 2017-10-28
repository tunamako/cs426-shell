CXX=g++

CXXFLAGS = -std=c++17 -g -Wextra -lgtest -pthread

MAIN_OBJECTS = src/rash.o src/main.o src/operator.o
TEST_OBJECTS = src/rash.o src/test_hw.o src/operator.o

all: bin/testRunner bin/main


bin/main: $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) -o bin/main $(MAIN_OBJECTS)

bin/testRunner: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o bin/testRunner $(TEST_OBJECTS)


src/test_hw.o: src/test_hw.cc
	$(CXX) $(CXXFLAGS) -c src/test_hw.cc -o src/test_hw.o

src/main.o: src/main.cc
	$(CXX) $(CXXFLAGS) -c src/main.cc -o src/main.o

src/rash.o: src/rash.cc src/rash.h
	$(CXX) $(CXXFLAGS) -c src/rash.cc -o src/rash.o

src/operator.o: src/operator.cc src/operator.h
	$(CXX) $(CXXFLAGS) -c src/operator.cc -o src/operator.o

clean:
	rm src/*.o bin/testRunner bin/main 