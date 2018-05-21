/*
Implemenation of class Chromosome. Contating a map between junctions and origin of genetic materials.
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
#include <algorithm>

#include "../include/Chromosome.h"
#include "../include/RandomGenerators.h"

using namespace std;

/* INITIATION */

Chromosome::Chromosome(){
	loci = 0;
}

Chromosome::Chromosome(char starting_char, int size){
	chromosome[0] = starting_char;
	loci = size;
}

Chromosome::Chromosome(map <int, char> input_chrom, int size){
	chromosome = input_chrom;
	loci = size;
}

Chromosome::~Chromosome(){
	chromosome.clear();
}

/* PLOTTING METHODS */
void Chromosome::showChromosome() const{
	for(map<int, char>::const_iterator i=chromosome.begin(); i!=chromosome.end(); ++i){
		cout << i->first << ':' << i->second << '\n';
	}
}

/* COMUNICATION METHODS */
bool Chromosome::isPureA() const{
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'A'){
			return 0;
		}
	}
	return 1;
}

bool Chromosome::isPureB() const{
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'B'){
			return 0;
		}
	}
	return 1;
}

int Chromosome::countB() const{
	int sum = 0;
	char last_seq = 'A';
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sum += (pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sum += (loci - last_val);
	}
	return sum;
}

int Chromosome::getNumberOfJunctions() const{
	return chromosome.size() - 1;
}

void Chromosome::getSizesOfBBlocks(vector<int>& sizes){
	char last_seq = 'A';
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sizes.push_back(pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sizes.push_back(loci - last_val);
	}
}

void Chromosome::getSizesOfABlocks(vector<int>& sizes){
	char last_seq = 'B';
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'A'){
			sizes.push_back(pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'A'){
			sizes.push_back(loci - last_val);
	}
}

void Chromosome::getSizesOfBlocks(vector<int>& sizes) const{
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->first != 0){
			sizes.push_back(pos->first - last_val);
		} else if (pos->second == 'B') {
			sizes.push_back(0);
		}
		last_val = pos->first;
	}
	sizes.push_back(loci - last_val);
}

void Chromosome::makeRecombinant(Chromosome& chromNew, int numberOfChaisma, const Context &context){
// 	starts_by 0 home, 1 alien
	int starts_by = context.random.tossAcoin();
	vector<int> recombination;
	chromNew.clear();
	chromNew.setResolution(loci);
	chromNew.write(0,'A');

// 	this condition can be deleted if I will handle the numberOfChaisma=0 on some upper level
	if(numberOfChaisma == 0){
		if(starts_by == 0){
			return;
		}
		chromNew = Chromosome(chromosome, loci);
		return;
	}

	int index, lastposition = 0;

	for(index=0;index<numberOfChaisma;index++){
		recombination.push_back(context.random.recombPosition(loci));
	}

	sort(recombination.begin(), recombination.end());

	/*for(index=0;index<numberOfChaisma;index++){
		cout << recombination[index] << ' ';
	}
	cout << endl;*/

	index = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		while(recombination[index] <= pos->first && index < numberOfChaisma){
			if (chromosome[lastposition] != 'A'){
				if (starts_by == 1){
					chromNew.write(recombination[index],'A');
				} else {
					chromNew.write(recombination[index],'B');
				}
			}
			starts_by = abs(starts_by - 1);
			index++;
		}
		if(starts_by == 1){
			chromNew.write(pos->first,pos->second);
		}
		lastposition = pos->first;
	}

	while(index < numberOfChaisma){
		if (chromosome[lastposition] != 'A'){
			if (starts_by == 1){
				chromNew.write(recombination[index],'A');
			} else {
				chromNew.write(recombination[index],'B');
			}
		}
		starts_by = abs(starts_by - 1);
		index++;
	}
	return;
}

/* COMUNICATION METHODS */
void Chromosome::setResolution(int res){
	loci = res;
}

int Chromosome::getResolution() const{
	return loci;
} /* return resolution of the chromosome */

void Chromosome::clear(){
	chromosome.clear();
	loci = 0;
} /* method for deleting chromosome */

char Chromosome::read(int i){
	return chromosome[i];
} /* returns value of junction */

void Chromosome::write(int i, char l){
	chromosome[i] = l;
}

std::map<int, char>::iterator Chromosome::begin(){
	return chromosome.begin();
}

std::map<int, char>::iterator Chromosome::end(){
	return chromosome.end();
}

std::map<int, char>::iterator Chromosome::find(int i){
	return chromosome.find(i);
}
