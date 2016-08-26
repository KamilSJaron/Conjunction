/*
World, (Universe previously) was a wrapping class for set of demes with given interactions.
Copyright (C) 2013-2016  Kamil S. Jaron

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
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>
#include <iomanip>

#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/Imigrant.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"
#include "../include/World.h"

using namespace std;

int getNumberOfDescendants(double fitness){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	double uniform = double(x % detailness) / (detailness-1);

	int result = 0;
	double q = exp(-fitness);
	double p = q;
	double roll = uniform;
	while(roll > q){
		result++;
		p = p * fitness / result;
		q = q + p;
	}
	return result;
}

World::World() {
	// USER
	// space
	dimension = -1;
	number_of_demes_l_r = -1;
	type_of_l_r_edges = "nothing";
	number_of_demes_u_d = -1;
	type_of_u_d_edges = "nothing";
	deme_size = -1;
	number_of_chromosomes = -1;
	lambda = -1;
	number_of_loci = -1;
	selection = -1;
	beta = -1;


	// INTERNAL
	edges_per_deme = -1;
	index_last_left = -1;
	index_next_left = -1;
	index_last_right = -1;
	index_next_right = -1;
}

void World::basicUnitCreator(char type, char init){
	int max_index = world.size();
	vector<int> new_indexes;
	int index;
// 	1D world definition
	if(dimension == 1){
		switch (type) {
			case 'b':
				index_next_left = 1;
				index_next_right = 2;
				index_last_left = 0;
				index_last_right = 0;
				new_indexes.clear();
				new_indexes.push_back(side_border(0,index_next_left));
				if(number_of_demes_l_r == 1){
					new_indexes.push_back(side_border(0,index_next_right));
				} else {
					new_indexes.push_back(index_next_right);
				}
				world[0] = new Deme(0,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
				break;
			case 'l':
				new_indexes.clear();
				new_indexes.push_back(max_index + 2);
				new_indexes.push_back(index_last_left);
				world[index_next_left] = new Deme(index_next_left,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
				index_last_left = index_next_left;
				index_next_left = max_index + 2;
				break;
			case 'r':
				new_indexes.clear();
				new_indexes.push_back(index_last_right);
				if(type_of_l_r_edges == "wrapping"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(0);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}
				if(type_of_l_r_edges == "reflexive"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(index_next_right);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}
				if(type_of_l_r_edges == "extending"){
					new_indexes.push_back(max_index + 2);
				}
				if(type_of_l_r_edges == "infinite"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(-8);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}

				world[index_next_right] = new Deme(index_next_right,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
				index_last_right = index_next_right;
				index_next_right = max_index + 2;
				break;

		}
		return;
	}

// 	2D world definition
	switch (type) {
		case 'b':
			index_next_left = number_of_demes_u_d;
			index_next_right = number_of_demes_u_d * 2;
			index_last_left = 0;
			index_last_right = 0;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(side_border(i,index_next_left + i));
				new_indexes.push_back(i + number_of_demes_u_d * 2);
				new_indexes.push_back(upper_border(i,max_index));
				new_indexes.push_back(lower_border(i,max_index));
				world[i] = new Deme(i,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
			}
			break;
		case 'l':
			index = index_next_left;
			index_next_left = max_index + 2 * number_of_demes_u_d;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(side_border(index + i,index_next_left + i));
				new_indexes.push_back(index_last_left + i);
				new_indexes.push_back(upper_border(index + i,index));
				new_indexes.push_back(lower_border(index + i,index));
				world[index + i] = new Deme(index + i,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
			}
			index_last_left = index;
			break;
		case 'r':
			index = index_next_right;
			index_next_right = max_index + 2 * number_of_demes_u_d;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(index_last_right + i);
				new_indexes.push_back(side_border(index + i,index_next_right + i));
				new_indexes.push_back(upper_border(index + i,index));
				new_indexes.push_back(lower_border(index + i,index));
				world[index + i] = new Deme(index + i,new_indexes,init, deme_size, selection, beta, number_of_chromosomes, number_of_loci, lambda);
			}
			index_last_right = index;
			break;
		default:
			cerr << "Error: I have no idea, what do you mean by... " << type << " I understand only 'b' basic, 'l' left and 'r' right, type of basic unit.. try it again please." << endl;
			break;
	}
	return;
}

void World::worldSlave(){
	if(number_of_demes_l_r == 1){
		basicUnitCreator('b', 'C');
		return;
	}
	if(number_of_demes_l_r == 2){
		basicUnitCreator('b', 'A');
		basicUnitCreator('r', 'B');
		return;
	}

	double midpoint = double(number_of_demes_l_r + 1) / 2;
	basicUnitCreator('b', 'A');

	for(double i = 2; i < number_of_demes_l_r; i++){
		if(i < midpoint){
			basicUnitCreator('r', 'A');
		} else {
			basicUnitCreator('r', 'B');
		}
	}
	basicUnitCreator('r', 'B');

	return;
}

  // // // // // // // //
 // COMPUTING METHODS //
// // // // // // // //

int World::migration(){
	const int demesize = deme_size;
	if(dimension == 0){
//		cerr << "Premigration Population size: " << zeroD_immigrant_pool.size() << endl;
		zeroD_immigrant_pool.reserve(zeroD_immigrant_pool.size() + demesize);
		for(int i = 0; i < demesize;i++){
			zeroD_immigrant_pool.push_back(Imigrant(number_of_chromosomes, number_of_loci, selection));
		}
//		cerr << "Postmigration Population size: " << zeroD_immigrant_pool.size() << endl;
		return 0;
	}
	if(world.empty()){
		cerr << "ERROR: Missing demes" << endl;
		return -1;
	}
	if(edges_per_deme == 0){
		return 0;
	}

	int index_last_left_fix = index_last_left;
	int index_last_right_fix = index_last_right;
	map<int, vector<Individual> > ImmigranBuffer;

	vector<int> neigbours;
	int MigInd = demesize / (2 * edges_per_deme );
	int deme_index;
	/*bufferVectorMap is container for all individuals imigrating to all demes*/
	for (map<int, Deme*>::const_iterator deme=world.begin(); deme!=world.end(); ++deme){
		neigbours = deme->second->getNeigbours();
		for(unsigned int j=0;j < neigbours.size();j++){
			deme_index = neigbours[j];
			if(deme_index == -8){
				continue;
			}
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[deme_index].push_back(deme->second->getIndividual(k));
			}
		}
	}

	for(map<int, vector<Individual> >::iterator buff=ImmigranBuffer.begin(); buff!=ImmigranBuffer.end(); ++buff){
		if(buff->first >= index_last_left_fix and buff->first < index_last_left_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[buff->first].push_back(Individual('A', number_of_chromosomes, number_of_loci, lambda));
			}
		}
		if(buff->first >= index_last_right_fix and buff->first < index_last_right_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[buff->first].push_back(Individual('B', number_of_chromosomes, number_of_loci, lambda));
			}
		}
		if(index_next_left <= buff->first and buff->first < index_next_left + number_of_demes_u_d){
			if(Acheck(buff->second)){
				continue;
			}
			basicUnitCreator('l', 'A');
		}
		if(index_next_right <= buff->first and buff->first < index_next_right + number_of_demes_u_d){
			if(Bcheck(buff->second)){
				continue;
			}
			basicUnitCreator('r', 'B');
		}
		world[buff->first]->integrateMigrantVector(buff->second);
	}
	return 0;
}

void World::set(int index,string type){
	int demesize = deme_size;
	vector<Individual> migBuffer;
	if(type == "pureA"){
		for(int i=0;i<demesize;i++){
			migBuffer.push_back(Individual('A', number_of_chromosomes, number_of_loci, lambda));
		}
		world[index]->integrateMigrantVector(migBuffer);
		return;
	}

	if(type == "pureB"){
		for(int i=0;i<demesize;i++){
			migBuffer.push_back(Individual('B', number_of_chromosomes, number_of_loci, lambda));
		}
		world[index]->integrateMigrantVector(migBuffer);
		return;
	}

	if(type == "hetero"){
		for(int i=0;i<demesize/4;i++){
			migBuffer.push_back(Individual('A', number_of_chromosomes, number_of_loci, lambda));
		}
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back(Individual('C', number_of_chromosomes, number_of_loci, lambda));
		}
		for(int i=0;i<demesize/4;i++){
			migBuffer.push_back(Individual('B', number_of_chromosomes, number_of_loci, lambda));
		}
		world[index]->integrateMigrantVector(migBuffer);
		return;
	}

	if(type == "halfAhalfhetero"){
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back(Individual('A', number_of_chromosomes, number_of_loci, lambda));
		}
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back(Individual('C', number_of_chromosomes, number_of_loci, lambda));
		}
		world[index]->integrateMigrantVector(migBuffer);
		return;
	}

	cerr << "ERROR: unknown parameter " << type << " of Universe.set()";
	return;
}

void World::globalBreeding(){
	if(dimension == 0){
		vector<Imigrant> new_generation;
		vector<Chromosome> gamete;
		new_generation.reserve(zeroD_immigrant_pool.size());
		gamete.reserve(number_of_chromosomes);
		double fitness;
		int num_of_desc;

//		cerr << "Breeding " << zeroD_immigrant_pool.size() << " immigrants" << endl;
		for(unsigned int index = 0; index < zeroD_immigrant_pool.size(); index++){
			fitness = zeroD_immigrant_pool[index].getFitness();
//			cerr << "Fitness: " << fitness << endl;
			num_of_desc = getNumberOfDescendants(fitness);
//			cerr << "Number of descendants: " << num_of_desc << endl;
			for(int i=0;i<num_of_desc;i++){
				zeroD_immigrant_pool[index].makeGamete(gamete);
				if(gameteAcheck(gamete)){
					continue;
				}
				new_generation.push_back( Imigrant(gamete, selection) );
			}
			num_of_desc = getNumberOfDescendants(fitness);
			for(int i=0;i<num_of_desc;i++){
				zeroD_immigrant_pool[index].makeGamete(gamete);
				if(gameteAcheck(gamete)){
					continue;
				}
				new_generation.push_back( Imigrant(gamete, selection) );
			}
		}
//		cerr << " New generation baby: " << new_generation.size() << endl;
		zeroD_immigrant_pool.clear(); // 1, this is incredibly stupid what I am doing here
		zeroD_immigrant_pool = new_generation; // 2, I should change pointers instead of copy-pasting
		double material = 0;
		int pop_size = zeroD_immigrant_pool.size();
		for(int i = 0;i < pop_size;i++){
			material += zeroD_immigrant_pool[i].getBprop();
		}
		cout << "Population size: " << pop_size << endl;
// 		cout << "Amount of material: " << material << endl;
		new_generation.clear(); // 3, in memory
		return;
	}

	vector<int> indexes;
	for (map<int, Deme*>::const_iterator i=world.begin(); i!=world.end(); ++i){
		indexes.push_back(i->first);
	}

	int index = 0;
	int i_size = indexes.size();

// 	#pragma omp parallel
// 	{
//
// 		{
// 		#ifdef _OPENMP
// 		int tnum = omp_get_num_threads();
// 		#else
// 		int tnum = 1;
// 		#endif
//
// 		cout << "Number of threads " << tnum << " "<< i_size << endl;
// 		}
//
// 	#pragma omp parallel for
		for(int i = 0; i < i_size; i++){
			index = indexes[i];
			world[index]->Breed();
		}
// 	}
	return;
}


  // // // // // // //
 // LOGICAL METHODS//
// // // // // // //

bool World::Acheck(vector<Individual>& buffer){
	for(int i = 0; (unsigned)i < buffer.size(); i++){
		if(buffer[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool World::Bcheck(vector<Individual>& buffer){
	for(int i = 0; (unsigned)i < buffer.size(); i++){
		if(buffer[i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool World::empty(){
	return (world.size() == 0);
}

  // // // // // // // //
 // PLOTTING // STATS //
// // // // // // // //

void World::listOfParameters() const{
	cerr << "***************" << endl
	<< "Size of World: " << world.size() << " Dim: " << dimension << " edges_per_deme: " << edges_per_deme << endl
	<< "Number of demes l/r: " << number_of_demes_l_r << " Number of demes u/d: " << number_of_demes_u_d << endl
	<< "Type of l/r edges: " << type_of_l_r_edges << " Type of u/d edges: " << type_of_u_d_edges << endl
	<< "Last left index: " << index_last_left << " Last right index: " << index_last_right << endl
	<< "Next left index: " << index_next_left << " Next right index: " << index_next_right << endl
	<< "***************" << endl;
	listOfNumericalParameters(std::cerr);
	cerr << "***************" << endl;
	return;
}

void World::listOfNumericalParameters(std::ostream& stream) const{
	stream << "# Selection: " << selection << endl
	<< "# Lambda: " << lambda << endl
	<< "# Beta: " << beta << endl
	<< "# Loci: " << number_of_loci << endl
	<< "# Chromosomes: " << number_of_chromosomes << endl
	<< "# Deme size: " << deme_size << endl;
	return;
}

void World::listOfDemes(){
	cerr << "of dimension: " << dimension << endl;
	if(dimension == 0){
		cerr << "Population of imigrants has " << zeroD_immigrant_pool.size() << endl;
	} else {
		cerr << "World of size " << world.size() << endl;
		cerr << "Number of demes up to down: " << number_of_demes_u_d << endl;
		cerr << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
		if(type_of_l_r_edges != "extending"){
			cerr << "Number of demes left to right: " << number_of_demes_l_r << endl;
		}
		cerr << "Type of borders left to right: " << type_of_l_r_edges << endl;
		cerr << "                 EDGE" << endl;
		cerr << setw(7) << right << "DEME " << setw(7) << left << " LEFT" << setw(6) << left << "RIGHT" << setw(5) << left << "UP" << setw(6) << left << "DOWN" << endl;
		for (map<int, Deme*>::const_iterator i=world.begin(); i!=world.end(); ++i){
			i->second->showDeme();
		}
	}
}

int World::summary(ostream& stream){
	if(dimension == 0){
		stream << "Selection: " << selection << endl;
		stream << "Recombination rate: " << lambda << endl;
		stream << "Theta: " << selection / lambda << endl;
		stream << "Number of Immigrants per generation: " << deme_size << endl;
	} else {
		int worlsize = world.size();
		cerr << "World of size " << worlsize << endl;
		stream << "       EDGE" << endl;
		stream << setw(7) << right << "DEME "
		<< setw(7) << left << " LEFT"
		<< setw(6) << left << "RIGHT";
		if(dimension == 2){
			stream << setw(6) << left << "UP"
			<< setw(6) << left << "DOWN";
		}
		stream << setw(12) << left << "meanf"
		<< setw(12) << left << "f(heter)"
		<< setw(12) << left << "meanHI"
		<< setw(12) << left << "var(HI)";
		if(number_of_loci * number_of_chromosomes > 1){
			stream << setw(12) << left << "var(p)"
			<< setw(12) << left << "LD";
		}

		if(number_of_loci * number_of_chromosomes <= 16){
			for(int ch = 0;ch < number_of_chromosomes;ch++){
				for(int l = 0; l < number_of_loci;l++){
					stream << left << "Ch" << ch+1 << "l" << l+1 << setw(7) << ' ';
				}
			}
		}
		stream << endl;
		for (map<int, Deme*>::const_iterator i=world.begin(); i!=world.end(); ++i){
	// 		if(i->first == 9){
			i->second->summary();
	// 			i->second->readAllGenotypes();
	// 		}
		}
	}
	return 0;
}

double World::getProportionOfHeterozygotes(int index){
	return world[index]->getProportionOfHeterozygotes();
}

double World::getProportionOfHomozygotes(int index, char type){
	return world[index]->getProportionOfHomozygotes(type);
}

void World::showOneDeme(int index){
	world[index]->showDeme();
}

int World::SaveTheUniverse(string type, string filename){
	ofstream ofile;
	vector<double> props;

	ofile.open(filename); // Opens file
	if (ofile.fail()){
		return 1;
	}

	if(dimension == 0){
		vector<int> blockSizes;
		for(unsigned int index = 0; index < zeroD_immigrant_pool.size(); index++){
			zeroD_immigrant_pool[index].getSizesOfBBlocks(blockSizes);
			for(unsigned int i = 0;i < blockSizes.size(); i++){
				ofile << blockSizes[i] / double(number_of_loci) << endl;
			}
			blockSizes.clear();
		}
		ofile.close();
		return 0;
	} else {
		int return_value = 0;
		if(type == "complete"){
			return_value = save_complete(ofile);
		}
		if(type == "summary"){
			return_value = summary(ofile);
		}
		if(type == "hybridIndices"){
			return_value = save_hybridIndices(ofile);
		}
		if(type == "hybridIndicesJunctions"){
			return_value = save_hybridIndicesJunctions(ofile);
		}
//		if(type == "raspberrypi"){
//			return save_raspberrypi(ofile);
//		}
		if(type == "blocks"){
			return_value = save_blocks(ofile);
		}

		ofile.close();
		return return_value;
	}

	cerr << "WARNING: The output was not saved" << endl;
	cerr << "         unknown saving format" << endl;
	return 1;
}

void World::getLD(){
	for (map<int, Deme*>::const_iterator i=world.begin(); i!=world.end(); ++i){
		cout << i->second->getLD() << '\t';
	}
	cout << endl;
}

  // // // // // // // //
 // PARAMETER HANDLING//
// // // // // // // //

void World::setHeight(int heig){
	number_of_demes_u_d = heig;
}

void World::setWidth(int width){
	number_of_demes_l_r = width;
}


void World::setLREdgesType(string ed_type){
	type_of_l_r_edges = ed_type;
}

void World::setUDEdgesType(string ed_type){
	type_of_u_d_edges = ed_type;
}

void World::setDimension(int dim){
	dimension = dim;
}

void World::setNumberOfEdges(int nue){
	edges_per_deme = nue;
}

void World::setSlectionBetaLambda(double s, double b, double l){
	selection = s;
	beta = b;
	lambda = l;
}

void World::setLociChromDeme(int l, int ch, int d){
	number_of_loci = l;
	number_of_chromosomes = ch;
	deme_size = d;
}

void World::restart(){
	if(dimension == 0){
		zeroD_immigrant_pool.clear();
	} else {
		clear();
		worldSlave();
	}
	cerr << "World is reset." << endl;
	return;
}

void World::clear(){
	for (map<int, Deme*>::const_iterator i=world.begin(); i!=world.end(); ++i){
		delete i->second;
	}
	world.clear();
	return;
}

  // // // // //
 //  PRIVATE //
// // // // //

int World::upper_border(int index, int max_index){
	if(type_of_u_d_edges == "reflexive"){
		if(index == max_index){
			return index;
		} else {
			return index - 1;
		}
	}
	if(type_of_u_d_edges == "wrapping"){
		if(index == max_index){
			return index + (number_of_demes_u_d-1);
		} else {
			return index - 1;
		}
	}
	cerr << "Error: The type of upper-down edges is not valid." << endl;
	return -1;
}

int World::lower_border(int index, int max_index){
	if(type_of_u_d_edges == "reflexive"){
		if(index == max_index + number_of_demes_u_d - 1){
			return index;
		} else {
			return index + 1;
		}
	}
	if(type_of_u_d_edges == "wrapping"){
		if(index == max_index + number_of_demes_u_d  - 1){
			return index - (number_of_demes_u_d-1);
		} else {
			return index + 1;
		}
	}
	cerr << "Error: The type of upper-down edges is not valid." << endl;
	return -1;
}

int World::side_border(int reflexive, int extending){
	if(type_of_l_r_edges == "reflexive"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			return reflexive;
		}
	}
	if(type_of_l_r_edges == "extending"){
		return extending;
	}
	if(type_of_l_r_edges == "wrapping"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			if(reflexive < number_of_demes_u_d){
				return number_of_demes_l_r * number_of_demes_u_d + reflexive % number_of_demes_u_d;
			} else {
				return reflexive % number_of_demes_u_d;
			}
		}
	}
	if(type_of_l_r_edges == "infinite"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			return -8;
		}
	}
	cerr << "Error: The type of left-right edges is not valid." << endl;
	return -1;
}

bool World::gameteAcheck(std::vector<Chromosome>& gamete){
	for(int i=0;i<number_of_chromosomes;i++){
		if(gamete[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

int World::save_complete(ofstream& ofile){
	cerr << "WARNING: experimental option save_complete " << endl;
	int index = index_last_left;
	vector<double> props;
	for(unsigned int i = 0; i < world.size(); i++){
		for(int y = 0; y < number_of_demes_u_d; y++){
			world[index+y]->getBproportions(props);
			save_line(ofile,index+y,props);
			world[index+y]->getJunctionNumbers(props);
			save_line(ofile,index+y,props);
			world[index+y]->getHeterozygoty(props);
			save_line(ofile,index+y,props);
		}
		if(index != index_last_right){
			index = world[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
	ofile.close();
	return 0;
}


int World::save_hybridIndices(ofstream& ofile){
	int index = index_last_left;
	vector<double> props;
	for(unsigned int i = 0; i < world.size(); i++){
		for(int y = 0; y < number_of_demes_u_d; y++){
			world[index+y]->getBproportions(props);
			save_line(ofile,index+y,props);
		}
		if(index != index_last_right){
			index = world[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
//	cerr << "The output was successfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}

int World::save_hybridIndicesJunctions(ofstream& ofile){
	int index = index_last_left;
	vector<double> props;
	for(unsigned int i = 0; i < world.size(); i++){
		for(int y = 0; y < number_of_demes_u_d; y++){
			world[index+y]->getBproportions(props);
			save_line(ofile,index+y,props);
			world[index+y]->getJunctionNumbers(props);
			save_line(ofile,index+y,props);
		}
		if(index != index_last_right){
			index = world[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
//	cerr << "The output was successfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}

//int World::save_raspberrypi(ofstream& ofile){
//
//	if(world.size() != 64){
//		cerr << "Wrong number of demes (" << world.size() << "), define 64 demes for raspberrypi file output \n";
//		return 1;
//	}
//
//	usleep(900);
//	cout << 'c' << endl;
//	usleep(100);
//
//	int index = index_last_left;
//	double hybridIndex = 0, LD = 0;
//	int R = 0, G = 0, B = 0;
//	cout << "m ";
//	for(unsigned int i = 0; i < world.size(); i++){
//		for(int y = 0; y < number_of_demes_u_d; y++){
//			hybridIndex = world[index+y]->getMeanBproportion();
//			LD = world[index+y]->getLD();
//			R = (int) (hybridIndex * 255 * 0.5);
//			G = (int) (abs(LD) * 4 * 255);
//// 			cerr << LD << ' ';
//			B = (int) ((1 - hybridIndex) * 255 * 0.5);
//			cout << R << ' ' << G << ' ' << B << ' ';
////			R G B 0 255 FF0, 00F
//		}
//		if(index != index_last_right){
//			index = world[index]->getNeigbours()[1];
//		} else {
//			break;
//		}
//	}
//	cout << endl;
//	ofile.close();
//	return 0;
//}

int World::save_blocks(ofstream& ofile){
	int index = index_last_left;
	vector<int> block_sizes;
	for(unsigned int i = 0; i < world.size(); i++){
		for(int y = 0; y < number_of_demes_u_d; y++){
			for(int ind_index = 0; ind_index < deme_size; ind_index++){
				world[index]->getSizesOfABlocks(block_sizes, ind_index);
				save_line(ofile,index+y,block_sizes);
				world[index]->getSizesOfBBlocks(block_sizes, ind_index);
				save_line(ofile,index+y,block_sizes);
			}
		}
		if(index != index_last_right){
			index = world[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
//	cerr << "The output was successfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}

int World::save_line(ofstream& ofile, int index, vector< double >& vec){
	ofile << index << '\t';
	for(unsigned int ind = 0; ind < vec.size(); ind++){
		ofile << vec[ind] << '\t';
	}
	ofile << endl;
	return 0;
}

int World::save_line(ofstream& ofile, int index, vector<int>& vec){
	ofile << index << '\t';
	for(unsigned int ind = 0; ind < vec.size(); ind++){
		ofile << vec[ind] << '\t';
	}
	ofile << endl;
	return 0;
}
