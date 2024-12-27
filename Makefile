# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Targets
all: p1 p2 p3 p4

p1: p1.o image.o
	$(CXX) $(CXXFLAGS) -o p1 p1.o image.o

p2: p2.o image.o DisjSets.o
	$(CXX) $(CXXFLAGS) -o p2 p2.o image.o DisjSets.o

p3: p3.o image.o
	$(CXX) $(CXXFLAGS) -o p3 p3.o image.o

p4: p4.o image.o
	$(CXX) $(CXXFLAGS) -o p4 p4.o image.o

p1.o: p1.cc image.h
	$(CXX) $(CXXFLAGS) -c p1.cc

p2.o: p2.cc image.h DisjSets.h
	$(CXX) $(CXXFLAGS) -c p2.cc

p3.o: p3.cc image.h
	$(CXX) $(CXXFLAGS) -c p3.cc

p4.o: p4.cc image.h
	$(CXX) $(CXXFLAGS) -c p4.cc

image.o: image.cc image.h
	$(CXX) $(CXXFLAGS) -c image.cc

DisjSets.o: DisjSets.cc DisjSets.h
	$(CXX) $(CXXFLAGS) -c DisjSets.cc

clean:
	rm -f *.o p1 p2 p3 p4
