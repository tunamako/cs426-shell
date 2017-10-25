CXX=g++

CXXFLAGS = -std=c++17 -g -Wextra -lgtest -pthread

OBJECTS = rash.o test_hw.o


all: testRunner

clean:
	rm *.o testRunner

testRunner: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o testRunner $(OBJECTS)

test_hw.o: test_hw.cc
	$(CXX) $(CXXFLAGS) -c test_hw.cc -o test_hw.o

gtest_main.o: gtest_main.cc
	$(CXX) $(CXXFLAGS) -c gtest_main.cc -o gtest_main.o

rash.o: rash.cc rash.h
	$(CXX) $(CXXFLAGS) -c rash.cc -o rash.o
