CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall


PRG = forsim
TEST = forsim_test
CLASSES = ./src/Chromosome.o ./src/Individual.o ./src/Imigrant.o ./src/Deme.o ./src/SelectionModel.o
OBJ = ./src/main.o $(CLASSES)

all: $(PRG)

$(PRG): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

test: $(TEST)

$(TEST): $(CLASSES)
	$(CXX) -Wall -pedantic -o $(TEST) ./test/forsim_test.cpp $(CLASSES)

clean:
	rm -f $(PRG) $(OBJ) $(TEST)
