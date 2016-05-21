#include <iostream>
#include <map>
#include <vector>
#include <cmath>

using namespace std;

#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"

#include "ChromosomeTest.cpp"
#include "IndividualTest.cpp"
#include "DemeTest.cpp"

int main(int argc, char* argv[]){

	int seed = 0;
	//set SEED

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
	if(testIndividual() == 0){
		cerr << "Deme: passed" << endl;
	} else {
		cerr << "Deme: not passed" << endl;
	}
	cerr << "Universe: not implemented" << endl;
	cerr << "Wrapper: not implemented" << endl;
	return 0;
}
