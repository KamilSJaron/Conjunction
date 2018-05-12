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

#include "../include/Chromosome.h"
#include "../include/Imigrant.h"
#include "../include/RandomGenerators.h"

using namespace std;

Imigrant::Imigrant(int input_ch, int size, double input_lambda){
	lambda = input_lambda;
	number_of_chromosomes = input_ch;
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(Chromosome('B', size));
	}
}

Imigrant::Imigrant(char origin, int input_ch, int size, double input_lambda){
	lambda = input_lambda;
	number_of_chromosomes = input_ch;
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(Chromosome(origin, size));
	}
}

Imigrant::Imigrant(vector<Chromosome>& gamete, double input_lambda){
	lambda = input_lambda;
	number_of_chromosomes = gamete.size();
	genome.reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome.push_back(gamete[i]);
	}
}

Imigrant::~Imigrant(){
	genome.clear();
}

void Imigrant::makeGamete(std::vector<Chromosome>& gamete){
	gamete.clear();
	gamete.reserve(number_of_chromosomes);
	Chromosome CHtemp;
	int chiasmata = 0;
	for(int ch = 0; ch < number_of_chromosomes;ch++){
		chiasmata = poisson(lambda);
//		cerr << " CH: " << ch+1 << " will get " << chiasmata << " chiasmata.\n";
		genome[ch].makeRecombinant(CHtemp, chiasmata);
		gamete.push_back(CHtemp);
	}
}

int Imigrant::getNumberOfJunctions() const{
	int junctions = 0;
	for(int i=0;i<number_of_chromosomes;i++){
		junctions = genome[i].getNumberOfJunctions();
	}
	return junctions;
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

void Imigrant::getSizesOfBBlocks(vector<int>& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int ch = 0;ch < number_of_chromosomes;ch++){
		genome[ch].getSizesOfBBlocks(sizes);
	}
	return;
}

int Imigrant::getNumberOfBBlocks(){
	int number_of_blocks = 0, junctions = 0;
	for(int ch = 0;ch < number_of_chromosomes;ch++){
		junctions = genome[ch].getNumberOfJunctions();
		if(junctions % 2 == 1){
			number_of_blocks += (junctions + 1) / 2;
		} else {
			if(genome[ch].read(0) == 'A'){
				number_of_blocks += junctions / 2;
			} else {
				number_of_blocks += (junctions / 2) + 1;
			}
		}
	}
	return number_of_blocks;
}

bool Imigrant::isPureA() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[i].isPureA()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Imigrant::isPureB() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[i].isPureB()){
			continue;
		}
		return 0;
	}
	return 1;
}

int Imigrant::getLambda() const{
	return lambda;
}

void Imigrant::readGenotype(){
	for(int i=0;i<number_of_chromosomes;i++){
		cerr << "---Chromozome---number-" << i+1 << "---" << endl;
		genome[i].showChromosome();
	}
	cout << endl;
}
