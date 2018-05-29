/*
 * a unit of
 * conjunction: simulator of secondary contact using Fisher's representation of genome admixture
 * handling simulation itself and providing API usable by any other wrapper
Copyright (C) 2016  Kamil S. Jaron

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
#include <string>
#include <math.h>
#include <cmath>

#include "../include/SimulationSetting.h"
#include "../include/Simulation.h"

Simulation::Simulation(SimulationSetting& simulation_setting) :
	world{simulation_setting}
{
	setWorld(simulation_setting);
}

int Simulation::simulate(){
	int save_pos = -1;

	if(file_type != "backtrace"){
		if(saves >= 10){
			file_name = file_name + "_0*";
		} else {
			if(saves > 1 and saves < 10){
				file_name = file_name + "_*";
			}
		}
		save_pos = file_name.find('*');
	}

	file_name = file_name + ".tsv";

	if(saves > 0){ // no saves means no saves
		if(file_name[0] != '_' and file_name[0] != '.'){ // non specified name means no saves
			std::cerr << "##### OUTPUT  INFO #####\n";
			std::cerr << "# Name of the file(s): " << file_name << "\n";
			std::cerr << "# Type of the file(s): " << file_type << "\n";
			std::cerr << "#       Files to save: " << saves << "\n";
			std::cerr << "########################\n";
		}
	}

	clock_t t_total1, t_total2;
	t_total1 = clock();
	clock_t t1, t2;

	int order = 0, check = 0;
	// variable modulo contain a modulo for generations to produce an output (check line 93)
	int modulo = ceil((double)(generations-delay-1) / saves);
	if(modulo == 0){
		modulo = 1;
	}
	// modulo only for non-zero saves
	// std::cout << modulo << std::endl;
	std::cerr << "###### PARAMETERS ######\n";
	world.listOfNumericalParameters(std::cerr);
	std::cerr << "########################\n";
	world.restart();

	for(int i=0; i < generations;i++){
		t1=clock();
		// std::cerr << "Migration: " << i + 1 << std::endl;
		world.migration();
		// std::cerr << "Breeding: " << i + 1 << std::endl;
		world.globalBreeding();
		t2=clock();
		std::cerr << "Generation: " << i + 1 << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << "\n";
		if((((i - delay) % modulo)+1) == modulo and (i - delay < generations - modulo or i+1 == generations)){
			order++;
			check = saveWorld(order, save_pos);
			if(check != 0){
				std::cerr << "Error in saving the output.\n";
				return 1;
			}
		}
	}

// make the final save after simulation only if it was not done yet (in cases when number of generations divided by number of saves are not giving intiger)
	if((((generations - 1 - delay) % modulo)+1) != modulo){
		order++;
		check = saveWorld(order, save_pos);
		if(check != 0){
			std::cerr << "Error in saving the output." << std::endl;
			return 1;
		}
	}

	t_total2 = clock();
	std::cerr << "Total time of simulation: " << ((float)t_total2 - (float)t_total1) / CLOCKS_PER_SEC << std::endl;
	world.clear();

	return 0;
}

void Simulation::setWorld(SimulationSetting& simulation_setting){

	saves = simulation_setting.saves;
	delay = simulation_setting.delay;
	generations = simulation_setting.generations;
	file_name = simulation_setting.file_to_save;
	file_type = simulation_setting.type_of_save;
}

int Simulation::saveWorld(int order, int save_pos){
	// always print summary to std out
	if(file_type != "raspberrypi"){
		world.summary(std::cout);
		if(saves >= 1 || file_type == "backtrace"){
			std::cerr << "Saving output to: " << file_name << std::endl;
		}
	}

	if (file_type == "backtrace"){
		return world.saveTheUniverse(file_type, file_name);
	}
	// only if number of saves and name of outfile are specified
	if(saves >= 1 and file_name[0] != '.' and file_name[0] != '_'){
		if(order >= 10){
			file_name[save_pos-1] = '0' + char(order / 10 % 10);
			file_name[save_pos] = '0' + char(order % 10);
		} else {
			file_name[save_pos] = '0' + char(order);
		}
		return world.saveTheUniverse(file_type, file_name);
	}
	return 0;
}
