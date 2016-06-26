#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

#include "../include/Chromosome.h"
#include "../include/Imigrant.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"
#include "../include/World.h"
#include "../include/SimulationSetting.h"
#include "../include/SettingHandler.h"
#include "../include/Simulation.h"


#include "ChromosomeTest.cpp"
#include "IndividualTest.cpp"
#include "DemeTest.cpp"
#include "WorldTest.cpp"
#include "GlobalTest.cpp"

int main(int argc, char* argv[]){
	int test_result = 0;
	int seed = 910401;
	srand (seed);
	cerr << "Running all tests with random seed: " << seed << endl;

	if(testChromosome() == 0){
		cerr << "Chromosome ... passed" << endl;
	} else {
		test_result++;
		cerr << "Chromosome: HAD A PROBLEM!" << endl;
	}
	if(testIndividual() == 0){
		cerr << "Individual ... passed" << endl;
	} else {
		test_result++;
		cerr << "Individual: HAD A PROBLEM!" << endl;
	}
	if(testDeme() == 0){
		cerr << "Deme ... passed" << endl;
	} else {
		test_result++;
		cerr << "Deme: HAD A PROBLEM!" << endl;
	}
	if(testWorld() == 0){
		cerr << "World ... passed" << endl;
	} else {
		test_result++;
		cerr << "World: HAD A PROBLEM!" << endl;
	}
	if(testWorld() == 0){
		cerr << "World ... passed" << endl;
	} else {
		test_result++;
		cerr << "World: HAD A PROBLEM!" << endl;
	}
	if(testParameterLoading() == 0){
		cerr << "Parameter loading ... passed" << endl;
	} else {
		test_result++;
		cerr << "Parameter loading: HAD A PROBLEM!" << endl;
	}
	if(test_result == 0){
		cerr << "HURRAY, everything seems to be working.\n";
	} else {
		cerr << "OUCH, " << test_result << " test/s failed!\n " <<
				"Contact kamil.jaron (at) gmail (dot) com for a support.\n";
	}

	return 0;
}
