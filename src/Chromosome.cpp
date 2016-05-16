#include <iostream>
#include <map>
#include <vector>

#include "../include/Chromosome.h"
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

void Chromosome::viewChromosome() const{
	vector<int> vals;
	vector<char> seq;
	unsigned int index = 0;
	int letter = 0;
	if(loci < 101){
		for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
			vals.push_back(pos->first);
			seq.push_back(pos->second);
		}
		if(vals.empty()){
			cerr << "ERROR: non initiated chromosome is impossible to plot" << endl;
			return;
		}
		for(letter = 0; letter < loci; letter++){
			if(vals[index] == letter && index < vals.size()){
				index++;
			}
			cout << seq[index-1];
		}	
		cout << endl;
	} else {
		for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
			vals.push_back(pos->first / ((double)loci / 100));
			seq.push_back(pos->second);
		}
		if(vals.empty()){
			cerr << "ERROR: non initiated chromosome is impossible to plot" << endl;
			return;
		}
		for(letter = 0; letter < 100; letter++){
			if(vals[index] == letter && index < vals.size()){
				index++;
			}
			cout << seq[index-1];
		}
		cout << endl;
	}
}


/* COMUNICATION METHODS */ 
bool Chromosome::Acheck() const{
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'A'){
			return 0;
		}
	}
	return 1;
}

bool Chromosome::Bcheck() const{
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

void Chromosome::makeRecombinant(Chromosome& chromNew, int numberOfChaisma){
// 	starts_by 0 home, 1 alien
	int starts_by = tossAcoin();
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
		recombination.push_back(recombPosition());
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

int Chromosome::tossAcoin(){
	return rand() % 2;
}

int Chromosome::recombPosition(){
	int roll = rand();
	if(loci == 1){
		return 0;
	}
	return (roll % (loci-1)) + 1;
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















