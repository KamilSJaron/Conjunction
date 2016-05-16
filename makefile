CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall


PRG = forsim
OBJ = ./src/main.o ./src/Chromosome.o ./src/Individual.o ./src/Imigrant.o ./src/Deme.o ./src/SelectionModel.o

all: $(PRG)

$(PRG): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

test: forsim_test

forsim_test: forsim_test
	$(CXX) -o forsim_test

clean:
	rm -f $(PRG) $(OBJ) $(TEST)
