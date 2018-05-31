/*
 * Core.hpp
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_SIMULATION_H_
#define INCLUDE_SIMULATION_H_

#include <string>

#include "../include/World.h"
#include "../include/SimulationSetting.h"

class Simulation {
	public:
		Simulation(SimulationSetting& simulation_setting);
		int simulate();

	private:
		void setWorld(SimulationSetting& simulation_setting);
		int saveWorld(int order, int save_pos);

		std::string file_name, file_type;
		int generations, delay, saves;
		World world;
};

#endif /* INCLUDE_SIMULATION_H_ */
