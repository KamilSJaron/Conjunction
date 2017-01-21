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

using namespace std;

// alternative, modulo bias is negligible if there is any
int tossAcoin (){
  return rand() % 2;
}

// discrete uniform (integers between 0 and ceil - 1)
int recombPosition(int loci){
  int roll = rand();
  if(loci == 1){
    return 0;
  }
  return (roll % (loci-1)) + 1;
}

// continuus unifrom; corrected for modulo bias
double uniform(){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	return double(x % detailness) / detailness;
}

// Poisson
int poisson(double lambda){
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
