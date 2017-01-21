CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -pedantic

PRG = conjunction
TEST = conjunction_test
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/[A-Z]*.cpp)
CLASSES = $(patsubst %.cpp, %.o, $(CPP_FILES))
OBJ = src/main.o $(CLASSES)

all: $(PRG)

$(PRG): $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(GIT_HEADER): .git/HEAD
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

.PHONY : test
test: $(TEST)
	./$(TEST)
	rm $(TEST) HI_multi_locus_l1_c1_n1_[12].tsv

$(TEST): $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $@ ./test/conjunction_test.cpp $(CLASSES)

.PHONY : clean
clean:
	rm -f $(PRG) $(OBJ) $(TEST)
