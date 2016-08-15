CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall -pedantic


PRG = conjunction
TEST = conjunction_test
CLASSES = ./src/Chromosome.o ./src/Individual.o ./src/Imigrant.o ./src/Deme.o ./src/SelectionModel.o ./src/World.o ./src/SimulationSetting.o ./src/Simulation.o ./src/SettingHandler.o
OBJ = ./src/main.o $(CLASSES)

all: $(PRG)

$(PRG): $(OBJ) ./include/gitversion.h
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

.cpp.o:
	$(CXX) -o $@ -c $< $(CXXFLAGS)

./include/gitversion.h:
	echo "#define GITVERSION \"$(shell git rev-parse HEAD)\"" > ./include/gitversion.h

test: $(TEST)
	./$(TEST)
	rm $(TEST)

$(TEST): $(CLASSES)
	$(CXX) $(CXXFLAGS) -o $(TEST) ./test/conjunction_test.cpp $(CLASSES)

clean:
	rm -f $(PRG) $(OBJ) $(TEST)
