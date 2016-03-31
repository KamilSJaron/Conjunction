#include <iostream>
#include <map>
#include <vector>

#include "../include/chromosome.h"
#include "../include/individual.h"
#include "../test/forsim_test.h"
using namespace std;

int main(int argc, char **argv)
{
    if(argc > 1){
    	if(strcmp( argv[1], "--help") == 0){
    		cerr << "all parameters are read form file setting.txt; details are online at wiki" << endl;
    		return 0;
    	}
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
    		cerr << "Deme: not implemented" << endl;
			cerr << "Universe: not implemented" << endl;
			cerr << "Wrapper: not implemented" << endl;
    		return 0;
    	}
    	if(strcmp( argv[1], "--version") == 0){
    		cerr << "Forsim v1.2.development" << endl;
    		cerr << "... on the way to be nice program" << endl;
    	}

    }
    
    Chromosome t1('A', 6);
    Individual George;
    Individual Luis('B', 3, 100, 1, 0.3, 1);

    return 0;
}
