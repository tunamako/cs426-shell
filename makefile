CXX=g++

CXXFLAGS = -std=c++17 -g -Wextra -lgtest -pthread

MAIN_OBJECTS = rash.o main.o
TEST_OBJECTS = rash.o test_hw.o

all: testRunner main


main: $(MAIN_OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(MAIN_OBJECTS)

testRunner: $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o testRunner $(TEST_OBJECTS)


test_hw.o: test_hw.cc
	$(CXX) $(CXXFLAGS) -c test_hw.cc -o test_hw.o

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc -o main.o

rash.o: rash.cc rash.h
	$(CXX) $(CXXFLAGS) -c rash.cc -o rash.o


clean:
	rm *.o testRunner main