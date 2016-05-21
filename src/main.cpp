#include <iostream>
#include <map>
#include <vector>
#include <cmath>

#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc > 1){
    	if(strcmp( argv[1], "--help") == 0){
    		cerr << "all parameters are read form file setting.txt; details are online at wiki" << endl;
    		return 0;
    	}
    	if(strcmp( argv[1], "--version") == 0){
    		cerr << "Forsim v1.2.development" << endl;
    		cerr << "... on the way to be a nice program" << endl;
    	}

    }
    
    Chromosome t1('A', 6);
    Individual George('C', 1, 50, 3);
    Individual Luis('B', 3, 100, 1);

    George.readGenotype();
	vector<Chromosome> gamete1, gamete2;
    George.makeGamete(gamete1);
    George.makeGamete(gamete2);
	Luis = Individual(gamete1,gamete2,0.4);
	Luis.readGenotype();

    George.makeGamete(gamete1);
    George.makeGamete(gamete2);
	Luis = Individual(gamete1,gamete2,0.4);
	Luis.readGenotype();


    George.makeGamete(gamete1);
    George.makeGamete(gamete2);
	Luis = Individual(gamete1,gamete2,0.4);
	Luis.readGenotype();

//    int myints[] = {1,2,3,4};
//    std::vector<int> neigb (myints, myints + sizeof(myints) / sizeof(int) );
//
//    Deme Hell(0, neigb, 'C', 4, 0.8, 1, 1, 100, 1);;
//
//    Hell.showDeme();
//    Hell.summary();
//    Hell.viewDeme();
//
//    Hell.Breed();
//
//    Hell.showDeme();
//    Hell.summary();
//    Hell.viewDeme();
//
//    Hell.Breed();
//
//    Hell.showDeme();
//    Hell.summary();
//    Hell.viewDeme();

    return 0;
}
