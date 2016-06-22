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

#include "ChromosomeTest.cpp"
#include "IndividualTest.cpp"
#include "DemeTest.cpp"
#include "WorldTest.cpp"

int main(int argc, char* argv[]){

	int seed = time(NULL);
	srand (seed);
	cerr << "Running all tests with random seed: " << seed << endl;

	if(testChromosome() == 0){
		cerr << "Chromosome: passed" << endl;
	} else {
		cerr << "Chromosome: not passed" << endl;
	}
	if(testIndividual() == 0){
		cerr << "Individual: passed" << endl;
	} else {
		cerr << "Individual: not passed" << endl;
	}
	if(testDeme() == 0){
		cerr << "Deme: passed" << endl;
	} else {
		cerr << "Deme: not passed" << endl;
	}
	if(testWorld() == 0){
		cerr << "World: passed" << endl;
	} else {
		cerr << "World: not passed" << endl;
	}

	return 0;
}
