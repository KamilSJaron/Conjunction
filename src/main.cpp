/*
conjunction: simulator of secondary contact using Fisher's representation of genome admixture
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
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../include/Chromosome.h"
#include "../include/Imigrant.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"
#include "../include/World.h"
#include "../include/SimulationSetting.h"
#include "../include/SettingHandler.h"
#include "../include/Core.h"


using namespace std;

int main(int argc, char **argv)
{
	string setting_file = "setting.txt";

	if(argc > 1){
		if(strcmp( argv[1], "--help") == 0){
			cerr << "all parameters are read form file setting.txt; details are online at wiki" << endl;
			return 0;
		}
		if(strcmp( argv[1], "--version") == 0){
			cerr << "Conjunction v1.2.development" << endl;
			cerr << "... on the way to be a nice program" << endl;
			return 0;
		}

		setting_file = argv[1];

	}

	cerr << "Loading ... " << setting_file << endl;

	SettingHandler all_setting(setting_file);
	SimulationSetting one_sim_setting;
	Core sim(one_sim_setting);

	cout << "Performing: "<< all_setting.getNumberOfSimulations() << " simulations\n";
	all_setting.printParameters();
	all_setting.printParameterCounts();

	for(int sim_index = 0; sim_index < all_setting.getNumberOfSimulations(); sim_index++){
		one_sim_setting = all_setting.getSimualtionSetting(sim_index);
		sim = Core(one_sim_setting);
//		sim.simulate();
	}


	return 0;
}


