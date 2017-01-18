CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -pedantic

PRG = conjunction
TEST = conjunction_test
GIT_HEADER = ./include/gitversion.h
CLASSES = ./src/Chromosome.o ./src/Individual.o ./src/Imigrant.o ./src/Deme.o ./src/SelectionModel.o ./src/World.o ./src/SimulationSetting.o ./src/Simulation.o ./src/SettingHandler.o
OBJ = ./src/main.o $(CLASSES)

all: $(PRG)

$(PRG): $(GIT_HEADER) $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(GIT_HEADER): .git/HEAD
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > $@

test: $(TEST)
	./$(TEST)
	rm $(TEST)

$(TEST): $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $@ ./test/conjunction_test.cpp $(CLASSES)

clean:
	rm -f $(PRG) $(OBJ) $(TEST)
