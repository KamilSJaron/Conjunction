CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall


PRG = forsim
OBJ = ./src/main.o ./src/chromosome.o ./src/individual.o ./src/imigrant.o ./src/SelectionModel.o ./test/forsim_test.o


all: $(PRG)

$(PRG): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(PRG) $(OBJ)
