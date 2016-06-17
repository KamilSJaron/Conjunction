/*
Special case of Individual, with only hapoid represenation, second set is assumed to be pure A.
Copyright (C) 2014-2016  Kamil S. Jaron

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
#include <map>
#include <vector>
#include <cmath>

#include "../include/Chromosome.h"
#include "../include/Imigrant.h"

using namespace std;

//imigrants have no lambda right now
#define LAMBDA 1

double uniform_imig(){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	return double(x % detailness) / (detailness-1);
}

Imigrant::Imigrant(int input_ch, int size, int input_sp){
	selection_pressure = input_sp;
	number_of_chromosomes = input_ch;
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(Chromosome('B', size));
	}
}

Imigrant::Imigrant(char origin, int input_ch, int size, int input_sp){
	selection_pressure = input_sp;
	number_of_chromosomes = input_ch;
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(Chromosome(origin, size));
	}
}

Imigrant::Imigrant(vector<Chromosome>& gamete, int input_sp){
	selection_pressure = input_sp;
	number_of_chromosomes = gamete.size();
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(gamete[i]);
	}
}

int Imigrant::getChiasma(){
	int result = 0;
	double q = exp(-LAMBDA);
	double p = q;
	double roll = uniform_imig();
	while(roll > q){
		result++;
		p = p * LAMBDA / result;
		q = q + p;
	}
	return result;
}

double Imigrant::getBprop() const{
	int loci = genome[0].getResolution();
	double prop = 0;
	for(int ch = 0;ch < number_of_chromosomes;ch++){
		prop += genome[ch].countB();
	}
	prop = prop / (number_of_chromosomes * loci);
	return prop;
}

void Imigrant::getSizesOfBBlocks(vector< int >& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int ch = 0;ch < number_of_chromosomes;ch++){
		genome[ch].getSizesOfBBlocks(sizes);
	}
	return;
}


double Imigrant::getFitness(){
	return 1 - (selection_pressure * getBprop());
}

bool Imigrant::Acheck() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Imigrant::Bcheck() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

void Imigrant::makeGamete(std::vector<Chromosome>& gamete){
//	gamete.clear();
	gamete.reserve(number_of_chromosomes);
	Chromosome CHtemp;
	for(int ch = 0; ch < number_of_chromosomes;ch++){
		genome[ch].makeRecombinant(CHtemp, getChiasma());
		gamete.push_back(CHtemp);
	}
}

int Imigrant::getSelectionPressure() const{
	return selection_pressure;
}
