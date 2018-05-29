/*
 * SimulationSetting.h
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_SIMSET_H_
#define INCLUDE_SIMSET_H_

#include <string>

struct SimulationSetting{
	int loci, selected_loci, chromosomes, deme_size, generations;
	double lambda, selection, beta;

	int saves, delay, seed;
	std::string file_to_save, type_of_save;

	int up_down_demes, left_right_demes, dimension, edges_per_deme;
	std::string type_of_updown_edges, type_of_leftright_edges;
};


#endif /* INCLUDE_SIMSET_H_ */
