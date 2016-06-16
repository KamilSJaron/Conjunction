#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <string.h>

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
    

    return 0;
}
