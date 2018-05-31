/*
Class Individual contain table of homologous chromosomes. Can created a gemetes (vectors of chromosomes).
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
#include <algorithm>

#include "../include/Individual.h"
#include "../include/RandomGenerators.h"

/* DECLARATION */

Individual::Individual() : context{nullptr} {
	number_of_chromosomes = -1;
	lambda = -1;
	selected_loci = -1;
}

Individual::Individual( const Context *context,
		char origin, int input_ch, int input_loci,
		double input_lamda, int input_selected_loci,
		std::tuple<int, int, int> ind_birthplace )
	: context{context}
{
	number_of_chromosomes = input_ch;
	lambda = input_lamda;
	selected_loci = input_selected_loci;

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
	birthplace = ind_birthplace;
}

Individual::Individual( const Context *context,
		std::vector<Chromosome>& gamete1, std::vector<Chiasmata>& chaiasmata1,
		std::vector<Chromosome>& gamete2, std::vector<Chiasmata>& chaiasmata2,
		double input_lamda, int input_selected_loci,
		std::tuple<int, int, int> ind_birthplace)
	: context{context}
{
	number_of_chromosomes = gamete1.size();
	selected_loci = input_selected_loci;
	lambda = input_lamda;
	birthplace = ind_birthplace;

	genome[0].reserve(number_of_chromosomes);
	genome[1].reserve(number_of_chromosomes);
	chiasmata[0].reserve(number_of_chromosomes);
	chiasmata[1].reserve(number_of_chromosomes);

	for(int i=0; i<number_of_chromosomes; i++){
		genome[0].push_back(gamete1[i]);
		genome[1].push_back(gamete2[i]);
		chiasmata[0].push_back(chaiasmata1[i]);
		chiasmata[1].push_back(chaiasmata2[i]);
	}
}

Individual::~Individual(){
	genome[0].clear();genome[1].clear();
}

/* COMPUTIONG METHODS */

void Individual::replace_chromozome(int set, int position, std::map <int, char>  input_chrom, int size){
	genome[set][position] = Chromosome(input_chrom, size);
}

void Individual::makeGamete(std::vector<Chromosome>& gamete, std::vector<Chiasmata>& chiasmata){
	gamete.clear(); // variable for new gamete
	chiasmata.clear();
	gamete.reserve(number_of_chromosomes);
	chiasmata.reserve(number_of_chromosomes);
	std::vector<int> local_chiasmata; // vector of randomes chismas
	Chromosome recombinant_ch; // temp chromosome
	char last_material_s1, last_material_s2;
	int rec_pos, numberOfChaisma, starts_by;

/* for every chromosome... */
	for(int i=0;i<number_of_chromosomes;i++){
/* syntax genome[set][chromosome] */
//		std::cout << genome[0][i].getResolution() << ' ';
//		if(genome[0][i].getResolution() == 1){
//			genome[3][i].getResolution();
//		}
		int loci = genome[0][i].getResolution();
		numberOfChaisma = context->random.poisson(lambda);
		starts_by = context->random.tossAcoin();

/* no chiasma mean inheritance of whole one parent chromosome */
		if(numberOfChaisma == 0){
			chiasmata.push_back(Chiasmata());
			gamete.push_back(genome[starts_by][i]);
			continue;
		}

/* inicialization / restart of variables */
		std::map<int, char>::const_iterator pos1=genome[0][i].begin();
		std::map<int, char>::const_iterator pos2=genome[1][i].begin();
		last_material_s1 = genome[0][i].read(0);
		last_material_s2 = genome[1][i].read(0);
		int last_roll = -1;

		local_chiasmata.clear();
		recombinant_ch.clear();
		recombinant_ch.setResolution(loci);
/* make sure that we start by 0 and do a crossover if not */
		if(starts_by == 1){
			local_chiasmata.push_back(0);
			starts_by = 0;
		}
/* roll the chiasmata positions */
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = context->random.recombPosition(loci);
			local_chiasmata.push_back(rec_pos);
		}
		sort(local_chiasmata.begin(), local_chiasmata.end());
		// construct that removes double recombination events on the same spot
		for(int index=0;index<numberOfChaisma;index++){
			if(last_roll == local_chiasmata[index]){
				local_chiasmata.erase (local_chiasmata.begin()+index,local_chiasmata.begin()+index+1);
				index -= 2;
				numberOfChaisma -= 2;
				last_roll = -1;
			} else {
				last_roll = local_chiasmata[index];
			}
		}

		chiasmata.push_back(Chiasmata(local_chiasmata));

// I think that given the code above we could replace starts_by by 0 in both expressions bellow
		if(local_chiasmata[0] != 0){
			recombinant_ch.write(0,genome[starts_by][i].read(0));
		} else {
			recombinant_ch.write(0,genome[(starts_by + 1) % 2][i].read(0));
		}
		pos1++;
		pos2++;

		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = local_chiasmata[index];
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
					recombinant_ch.write(local_chiasmata[index],last_material_s2);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(local_chiasmata[index],last_material_s2);
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
					recombinant_ch.write(local_chiasmata[index],last_material_s1);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(local_chiasmata[index],last_material_s1);
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
//	std::cout << prop << " / (" << loci << " * 2 * " << number_of_chromosomes << " = ";
	prop = prop / (loci*2*number_of_chromosomes);
//	std::cout << prop << std::endl;
	return prop;
}

double Individual::getSelectedHybridIndex(){
	//TODO add constrain on selected loci ( (loci - selected) % (selected - 1) == 0)
	int loci = genome[0][0].getResolution();
	int neutural_block_size = 1 + ((loci - selected_loci) / (selected_loci - 1));
	double prop = 0;
	// std::cerr << "Block size : " << neutural_block_size << std::endl;

	std::map<int, char>::const_iterator pos, next_pos;
	for (int i=0; i<number_of_chromosomes; i++){
		for (int ploidy = 0; ploidy < 2; ploidy++){
			// std::cerr << "ploidy : " << ploidy << " chromosome : " << i << std::endl;

			pos = genome[ploidy][i].begin();
			next_pos = genome[ploidy][i].begin();
			next_pos++;
			if(pos->second == 'B'){
				prop++;
			}
			while (next_pos != genome[ploidy][i].end()){
				// std::cerr << pos->second << " block : " << pos->first << " to " << next_pos->first << std::endl;
				if(pos->second == 'B'){
					// std::cerr << "adding "
					// 	<< (((next_pos->first - 1) / neutural_block_size) -
					// 	    ((pos->first - 1) / neutural_block_size))
					// 	<< " to prop" << std::endl;
					prop += ((next_pos->first - 1) / neutural_block_size) -
							((pos->first - 1) / neutural_block_size);
				}
				pos = next_pos;
				next_pos++;
			}
			if(pos->second == 'B'){
				// std::cerr << pos->second << " block : " << pos->first << " to " << loci << std::endl;
				// std::cerr << "adding "
				// 	<< (((loci - 1) / neutural_block_size) - ((pos->first - 1) / neutural_block_size))
				// 	<< " to prop"  << std::endl;
				prop += ((loci - 1) / neutural_block_size) -
						((pos->first - 1) / neutural_block_size);
			}
		}
	}
	// std::cerr << "B count: " << prop << std::endl;
	prop = prop / (2 * selected_loci * number_of_chromosomes);
	// std::cerr << "selected hybrid index : " << prop << std::endl;
	return prop;
}

double Individual::getHetProp(){
	bool write;
	long number_of_het_loci = 0;
	int last_pos = 0;
	int loci = genome[0][0].getResolution();

	std::map<int, char>::const_iterator pos1, pos2;

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

			std::cerr << "WARNING: Heterozygotisity counting problem (junc level)!";
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
		std::cerr << "WARNING: Heterozygotisity counting problem (ch level)!\n";
	}
	return ((double)number_of_het_loci / (loci * number_of_chromosomes));
}

bool Individual::isPureA() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[0][i].isPureA() and genome[1][i].isPureA()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Individual::isPureB() const{
	for(int i=0;i<number_of_chromosomes;i++){
		if(genome[0][i].isPureB() and genome[1][i].isPureB()){
			continue;
		}
		return 0;
	}
	return 1;
}

std::map< int, char >::iterator Individual::getChromosomeBegining(int set, int chrom){
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

void Individual::getSizesOfBBlocks(std::vector<int>& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < number_of_chromosomes; chrom++){
			genome[set][chrom].getSizesOfBBlocks(sizes);
		}
	}
	return;
}

void Individual::getSizesOfABlocks(std::vector<int>& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < number_of_chromosomes; chrom++){
			genome[set][chrom].getSizesOfABlocks(sizes);
		}
	}
	return;
}

/* ASCII PLOTTING METHODS */

void Individual::readGenotype(){
	for(int i=0;i<number_of_chromosomes;i++){
		std::cout << "---Chromozome---set-1---number-" << i+1 << "---" << std::endl;
		genome[0][i].showChromosome();
		std::cout << "---Chromozome---set-2---number-" << i+1 << "---" << std::endl;
		genome[1][i].showChromosome();
	}
	std::cout << std::endl;
}

/* COMUNICATION */
void Individual::setNumberOfChromosomes(int ch){
	number_of_chromosomes = ch;
}

void Individual::setLambda(double Rr){
	lambda = Rr;
}

void Individual::setParents(std::tuple<int,int,int> in_mum, std::tuple<int,int,int> in_dad){
	mum = in_mum;
	dad = in_dad;
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

int Individual::getNumberOfSelectedLoci() const{
	return selected_loci;
}

void Individual::getNumberOfLoci(std::vector<int>& ch) const{
	ch.clear();
	ch.reserve(number_of_chromosomes);
	for(int i = 0; i < number_of_chromosomes; i++){
		ch.push_back(genome[0][i].getResolution());
	}
}

void Individual::getGenotype(std::vector<std::string>& hapl) const{
	std::vector<int> blocks;
	hapl.clear();
	hapl.reserve(number_of_chromosomes*2);
	for(int chrom = 0; chrom < number_of_chromosomes; chrom++){
		for(int ploidy = 0; ploidy < 2; ploidy++){
			// TO CHANGE
			blocks.clear();
			genome[ploidy][chrom].getSizesOfBlocks(blocks);
			hapl.push_back(collapseBlocks(blocks));
		}
	}
}

void Individual::getChiasmata(std::vector<std::string>& rec) const{
	rec.clear();
	rec.reserve(number_of_chromosomes*2);
	for(int chrom = 0; chrom < number_of_chromosomes; chrom++){
		for(int ploidy = 0; ploidy < 2; ploidy++){
			rec.push_back(chiasmata[ploidy][chrom].collapse());
		}
	}
}

std::tuple<int,int,int> Individual::getBirthplace() const {
	return birthplace;
}

std::tuple<int,int,int> Individual::getMum() const {
	return mum;
}

std::tuple<int,int,int> Individual::getDad() const {
	return dad;
}
/* PRIVATE */

int Individual::getOneChromeHetero(bool write, std::map<int, char>::const_iterator& pos, int chromosome, int last_pos){
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

std::string Individual::collapseBlocks(std::vector<int>& blocks) const{
	std::string collapsed = "";
	for(unsigned int block = 0; block < blocks.size(); block++){
		if(collapsed == ""){
			collapsed = std::to_string(blocks[block]);
		} else {
			collapsed = collapsed + "," + std::to_string(blocks[block]);
		}
	}
	return collapsed;
}