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

int main(int argc, char* argv[]){
	if(strcmp( argv[1], "--test") == 0){
		cerr << "Running all tests..." << endl;
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
		cerr << "Deme: implemented, not tested" << endl;
		cerr << "Universe: not implemented" << endl;
		cerr << "Wrapper: not implemented" << endl;
		return 0;
	}
	return 0;
}
