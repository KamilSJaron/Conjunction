#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

#include "../include/RandomGenerators.h"
#include "../include/Chromosome.h"
#include "../include/Imigrant.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"
#include "../include/World.h"
#include "../include/SimulationSetting.h"
#include "../include/SettingHandler.h"
#include "../include/Simulation.h"


const int RANDOM_SEED = 910401;

Context createTestContext() {
	return Context(RANDOM_SEED);
}

#include "ChromosomeTest.cpp"
#include "ImigrantTest.cpp"
#include "IndividualTest.cpp"
#include "DemeTest.cpp"
#include "WorldTest.cpp"
#include "GlobalTest.cpp"

int main(int argc, char* argv[]){
	int test_result = 0;
	cerr << "Running all tests with random seed: " << RANDOM_SEED << endl;

	typedef int (*test_t)();
	auto runTest = [&test_result](test_t test, std::string name) {
		if (test() == 0) {
			cerr << name << " ... passed\n";
		} else {
			test_result++;
			cerr << name << ": HAD A PROBLEM!\n";
		}
	};
	runTest(&testChromosome, "Chromosome");
//	runTest(&testImigrant, "Imigrant");
	runTest(&testIndividual, "Individual");
	runTest(&testDeme, "Deme");
	runTest(&testWorld, "World");
	runTest(&testParameterLoading, "Parameter loading");

	if(test_result == 0){
		cerr << "HURRAY, everything seems to be working.\n";
	} else {
		cerr << "OUCH, " << test_result << " test/s failed!\n " <<
				"Contact kamiljaron (at) gmail (dot) com for a support.\n";
		exit (EXIT_FAILURE);
	}

	return 0;
}
