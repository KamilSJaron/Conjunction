CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -pedantic

PRG = conjunction
TEST = conjunction_test
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/[A-Z]*.cpp)
CLASSES = $(patsubst %.cpp, %.o, $(CPP_FILES))
OBJ = src/main.o $(CLASSES)
INSTAL_PREFIX = /usr/local

.PHONY: all
all: $(PRG)

$(PRG): $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(GIT_HEADER): .git/COMMIT_EDITMSG .git/HEAD
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@
FORCE:

.PHONY : test
test: $(TEST)
	./$(TEST)
	rm $(TEST) HI_multi_locus_l1_c1_n1_[12].tsv

$(TEST): test/conjunction_test.cpp $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(CLASSES)

.PHONY : install
install : $(PRG)
	install -C $< $(INSTAL_PREFIX)/bin

.PHONY : uninstall
uninstall :
	-rm $(INSTAL_PREFIX)/bin/$(PRG)

.PHONY : clean
clean:
	-rm -f $(PRG) $(OBJ) $(TEST) $(GIT_HEADER)
