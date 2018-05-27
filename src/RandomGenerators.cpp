/*
Random numer generators
Copyright (C) 2017  Kamil S. Jaron

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <cmath>

#include "../include/RandomGenerators.h"

using namespace std;

RandomGenerator::RandomGenerator(int seed) {
	cerr << "The SEED for random numbers is set to: " << seed << endl;
	srand(seed);
}


// alternative, modulo bias is negligible if there is any
int RandomGenerator::tossAcoin (){
	return rand() % 2;
}

// discrete uniform (integers between 0 and ceil - 1)
int RandomGenerator::recombPosition(int loci){
	if(loci == 1){
		return 0;
	}
	// this construction handles modulo bias; (of rand do uniform 0, RAND_MAX
	// overhang of size RAND_MAX % loci would cause small disproportion of smaller
	// values, not crutial for small numbers of locu, but very more important for big numbers
	// similar construction is used in function bellow
	int roll = RAND_MAX;
	while(roll >= RAND_MAX - (RAND_MAX % loci)){
		roll = rand();
	}
	return (roll % (loci-1)) + 1;
}

// continuus unifrom; corrected for modulo bias
double RandomGenerator::uniform(){
	int roll = RAND_MAX;
	int detailness = 1000000;
	while(roll >= RAND_MAX - (RAND_MAX % detailness)){
		roll = rand();
	}
	return double(roll % detailness) / detailness;
}

// Poisson (by transformation from uniform)
int RandomGenerator::poisson(double lambda){
	int result = 0;
	double q = exp(-lambda);
	double p = q;
	double roll = uniform();
	while(roll > q){
		result++;
		p = p * lambda / result;
		q = q + p;
	}
	return result;
}
