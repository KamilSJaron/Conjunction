#include <iostream>
#include <map>
#include <vector>
#include <cmath>

#include "../include/Chromosome.h"
#include "../include/Individual.h"

using namespace std;

int tossAcoin (){
  return rand() % 2;
}

double uniform(){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	return double(x % detailness) / detailness;
}

/* DECLARATION */

Individual::Individual(){
	number_of_chromosomes = 1;
	lambda = 0;

	genome[0].reserve(number_of_chromosomes);
	genome[1].reserve(number_of_chromosomes);
	for(int i=0;i<number_of_chromosomes;i++){
		genome[0].push_back(Chromosome('A',1));
		genome[1].push_back(Chromosome('B',1));
	}
}

Individual::Individual(char origin){
	number_of_chromosomes = 1;
	lambda = 0;

	genome[0].reserve(number_of_chromosomes);
	genome[1].reserve(number_of_chromosomes);
	if(origin == 'A' or origin == 'B'){
		for(int i=0;i<number_of_chromosomes;i++){
			genome[0].push_back(Chromosome(origin, 1));
			genome[1].push_back(Chromosome(origin, 1));
		}
	} else {
		for(int i=0;i<number_of_chromosomes;i++){
			genome[0].push_back(Chromosome('A', 1));
			genome[1].push_back(Chromosome('B', 1));
		}
	}
}

Individual::Individual(	char origin, int input_ch, int input_loci, 
						double input_lamda){
	number_of_chromosomes = input_ch;
	lambda = input_lamda;

	genome[0].reserve(number_of_chromosomes);
	genome[1].reserve(number_of_chromosomes);
	if(origin == 'A' or origin == 'B'){
		for(int i=0;i<number_of_chromosomes;i++){
			genome[0].push_back(Chromosome(origin, input_loci));
			genome[1].push_back(Chromosome(origin, input_loci));
		}
	} else {
		for(int i=0;i<number_of_chromosomes;i++){
			genome[0].push_back(Chromosome('A', input_loci));
			genome[1].push_back(Chromosome('B', input_loci));
		}
	}
}

Individual::Individual(	vector<Chromosome>& gamete1, 
						vector<Chromosome>& gamete2, 
						double input_lamda){
	number_of_chromosomes = gamete1.size();
	lambda = input_lamda;
	
	genome[0].reserve(number_of_chromosomes);
	genome[1].reserve(number_of_chromosomes);
	int i; 
	for(i=0;i<number_of_chromosomes;i++){
		genome[0].push_back(gamete1[i]);
		genome[1].push_back(gamete2[i]);
	}
}

Individual::~Individual(){
	genome[0].clear();genome[1].clear();
}

/* COMPUTIONG METHODS */

void Individual::replace_chromozome(int set, int position, map <int, char>  input_chrom, int size){
	genome[set][position] = Chromosome(input_chrom, size);
}

int Individual::getChiasma(){
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

void Individual::makeGamete(vector<Chromosome>& gamete){
	gamete.clear(); // variable for new gamete
	gamete.reserve(number_of_chromosomes);
	vector<int> chiasmas; // vector of randomes chismas
	Chromosome recombinant_ch; // temp chromosome
	char last_material_s1, last_material_s2;
	int rec_pos, numberOfChaisma, starts_by;

/* for every chromosome... */
	for(int i=0;i<number_of_chromosomes;i++){
/* syntax genome[set][chromosome] */
		numberOfChaisma = getChiasma();
		starts_by = tossAcoin();
		
/* no chiasma mean inheritance of whole one parent chromosome */
		if(numberOfChaisma == 0){
			gamete.push_back(genome[starts_by][i]);
			continue;
		}
		
/* inicialization / restart of variables */
		map<int, char>::const_iterator pos1=genome[0][i].begin();
		map<int, char>::const_iterator pos2=genome[1][i].begin();
		last_material_s1 = genome[0][i].read(0);
		last_material_s2 = genome[1][i].read(0);
		int last_roll = -1;
		int loci = genome[0][i].getResolution();
		
		chiasmas.clear();
		recombinant_ch.clear();
		recombinant_ch.setResolution(loci);
/* roll the chiasmas positions */
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = genome[0][i].recombPosition();
			chiasmas.push_back(rec_pos);
		}
		sort(chiasmas.begin(), chiasmas.end());
		
		for(int index=0;index<numberOfChaisma;index++){
			if(last_roll == chiasmas[index]){
				chiasmas.erase (chiasmas.begin()+index,chiasmas.begin()+index+1);
				index -= 2;
				numberOfChaisma -= 2;
				last_roll = -1;
			} else {
				last_roll = chiasmas[index];
			}
		}
		if(chiasmas[0] != 0){
			recombinant_ch.write(0,genome[starts_by][i].read(0));
		} else {
			recombinant_ch.write(0,genome[(starts_by + 1) % 2][i].read(0));
		}
		pos1++;
		pos2++;
		
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = chiasmas[index];
			if(rec_pos == 0){
				starts_by = (starts_by + 1) % 2;
				continue;
			}
			if(rec_pos == loci){
				continue;
			}
			
			if(starts_by==0){
				while(pos1->first < rec_pos and pos1->first != genome[0][i].end()->first){
					recombinant_ch.write(pos1->first,pos1->second);
					last_material_s1 = pos1->second;
					pos1++;
				}
				while(pos2->first <= rec_pos and pos2->first != genome[1][i].end()->first){
					last_material_s2 = pos2->second;
					pos2++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
					recombinant_ch.write(chiasmas[index],last_material_s2);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(chiasmas[index],last_material_s2);
				}
			} else {
				while(pos2->first < rec_pos and pos2->first != genome[1][i].end()->first){
					recombinant_ch.write(pos2->first,pos2->second);
					last_material_s2 = pos2->second;
					pos2++;
				}
				while(pos1->first <= rec_pos and pos1->first != genome[0][i].end()->first){
					last_material_s1 = pos1->second;
					pos1++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
					recombinant_ch.write(chiasmas[index],last_material_s1);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(chiasmas[index],last_material_s1);
				}
			}
			starts_by = (starts_by + 1) % 2;
		}
		if(starts_by==0){
			while(pos1->first != genome[0][i].end()->first){
				recombinant_ch.write(pos1->first,pos1->second);
				last_material_s1 = pos1->second;
				pos1++;
			}
			while(pos2->first != genome[1][i].end()->first){
				last_material_s2 = pos2->second;
				pos2++;
			}
		} else {
			while(pos2->first != genome[1][i].end()->first){
				recombinant_ch.write(pos2->first,pos2->second);
				last_material_s2 = pos2->second;
				pos2++;
			}
			while(pos1->first != genome[0][i].end()->first){
				last_material_s1 = pos1->second;
				pos1++;
			}
		}
		gamete.push_back(recombinant_ch);
	}
}

int Individual::getBcount() const{
	int count = 0;
	for(int i=0;i<number_of_chromosomes;i++){
		count += genome[0][i].countB();
		count += genome[1][i].countB();
	}
	return count;
}

double Individual::getBprop() const{
	int loci = genome[0][0].getResolution();
	double prop = 0;
	for(int i=0;i<number_of_chromosomes;i++){
		prop += genome[0][i].countB();
		prop += genome[1][i].countB();
	}
	prop = prop / (loci*2*number_of_chromosomes);
	return prop;
}

double Individual::getHetProp(){
	bool write;
	long number_of_het_loci = 0;
	int last_pos = 0;
	int loci = genome[0][0].getResolution();
	
	map<int, char>::const_iterator pos1, pos2;
	
	for(int i=0;i<number_of_chromosomes;i++){
		pos1=genome[0][i].begin(); pos2=genome[1][i].begin();
		last_pos = 0;
		write = !(pos1->second == pos2->second);
		pos1++; pos2++;
		if((pos1 == genome[0][i].end()) & (pos2 == genome[1][i].end())){
			number_of_het_loci += loci * write;
			continue;
		}
		
		if(pos1 == genome[0][i].end()){
			number_of_het_loci += getOneChromeHetero(write, pos2, i, 0);
			continue;
		}
		
		if(pos2 == genome[1][i].end()){
			number_of_het_loci += getOneChromeHetero(write, pos1, i, 0);
			continue;
		}
		
		while(pos1 != genome[0][i].end() and pos2 != genome[1][i].end()){
			if(pos1->first < pos2->first){
				last_pos = pos1->first;
				pos1++;
				write = !write;
				continue;
			}
			
			if(pos1->first > pos2->first){
				number_of_het_loci += (pos2->first - last_pos) * write;
				last_pos = pos2->first;
				pos1++;
				write = !write;
				continue;
			}
			
			if(pos1->first == pos2->first){
				number_of_het_loci += (pos1->first - last_pos) * write;
				last_pos = pos2->first;
				pos1++; pos2++;
				continue;
			}
			
			cerr << "WARNING: Heterozygotisity counting problem (junc level)!";
		}
		
		if((pos1 == genome[0][i].end()) & (pos2 == genome[1][i].end())){
			number_of_het_loci += (loci - last_pos) * write;
			continue;
		}
		
		if(pos1 == genome[0][i].end()){
			number_of_het_loci += getOneChromeHetero(write, pos2, i, last_pos);
			continue;
		}
		
		if(pos2 == genome[1][i].end()){
			number_of_het_loci += getOneChromeHetero(write, pos1, i, last_pos);
			continue;
		}
		cerr << "WARNING: Heterozygotisity counting problem (ch level)!";
	}
	return ((double)number_of_het_loci / (loci * number_of_chromosomes));
}

bool Individual::Acheck() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[0][i].Acheck() and genome[1][i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Individual::Bcheck() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[0][i].Bcheck() and genome[1][i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

map< int, char >::iterator Individual::getChromosomeBegining(int set, int chrom){
	return genome[set][chrom].begin();
}

int Individual::getNumberOfJunctions(int set, int chrom){
	return genome[set][chrom].getNumberOfJunctions();
}

int Individual::getNumberOfJunctions(){
	int sum = 0;
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < number_of_chromosomes; chrom++){
			sum += genome[set][chrom].getNumberOfJunctions();
		}
	}
	return sum;
}

void Individual::readGenotype(){
	for(int i=0;i<number_of_chromosomes;i++){
		cout << "---Chromozome---set-1---number-" << i+1 << "---" << endl;
		genome[0][i].showChromosome();
		cout << "---Chromozome---set-2---number-" << i+1 << "---" << endl;
		genome[1][i].showChromosome();
	}
	cout << endl;
}

/* COMUNICATION */
void Individual::setNumberOfChromosomes(int ch){
	number_of_chromosomes = ch;
}

void Individual::setLambda(double Rr){
	lambda = Rr;
}

int Individual::getNumberOfChromosomes() const{
	return number_of_chromosomes;
}

double Individual::getLambda() const{
	return lambda;
}

int Individual::getNumberOfLoci(int ch) const{
	return genome[0][ch].getResolution();
}

/* PRIVATE */

int Individual::getOneChromeHetero(bool write, map<int, char>::const_iterator& pos, int chromosome, int last_pos){
	int number_of_het_loci = 0;
	int loci = genome[0][0].getResolution();
	
	while((pos != genome[0][chromosome].end()) and (pos != genome[1][chromosome].end())){
		number_of_het_loci += (pos->first - last_pos) * write;
		last_pos = pos->first;
		pos++;
		write = !write;
	}
	number_of_het_loci += (loci - last_pos) * write;
	
	return number_of_het_loci;
}
















