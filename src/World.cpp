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
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include <cmath>

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
