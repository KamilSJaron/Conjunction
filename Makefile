CXXFLAGS = -std=c++11 -O3 -Wall -pedantic

PRG = conjunction
GIT_HEADER = include/gitversion.h
CPP_FILES = $(wildcard src/[A-Z]*.cpp)
TEST_FILES = $(wildcard test/[A-Z]*.cpp)
CLASSES = $(patsubst %.cpp, %.o, $(CPP_FILES))
OBJ = src/main.o $(CLASSES)
INSTAL_PREFIX = /usr/local

.PHONY: all
all: $(PRG)

$(PRG): $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

# .git/COMMIT_EDITMSG is a file that get updated with every commit
# .git/HEAD is a file that get updated with every switch of branches
# if any of those have changed, commit in binary have to be updated
$(GIT_HEADER): .git/HEAD .git/COMMIT_EDITMSG
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

# if repository is cloned, it does not contain last commit message
# to make a git header regarless if absence of the last commit message
# we create a blank file with last commit message
.git/COMMIT_EDITMSG :
	touch $@

# compile testing binary, run test and clean if everything succedes.
.PHONY : test
test: conjunction_test
	./$< && rm $< HI_multi_locus_l1_c1_n1_[12].tsv

# testing binary recepie
conjunction_test : test/conjunction_test.cpp $(CLASSES) $(TEST_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(CLASSES)

# install Conjunciton to /usr/local/bin
.PHONY : install
install : $(PRG)
	install -C $< $(INSTAL_PREFIX)/bin

# remove Conjunciton from /usr/local/bin
.PHONY : uninstall
uninstall :
	-rm $(INSTAL_PREFIX)/bin/$(PRG)

# clean repo
.PHONY : clean
clean:
	-rm -f $(PRG) $(OBJ) $(TEST) $(GIT_HEADER)
