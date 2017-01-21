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

#include "../include/Chromosome.h"
#include "../include/Imigrant.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"
#include "../include/World.h"
#include "../include/SimulationSetting.h"
#include "../include/SettingHandler.h"
#include "../include/Simulation.h"
#include "../include/gitversion.h"

using namespace std;

int main(int argc, char **argv)
{
	string setting_file = "setting.txt";

	if(argc > 1){
		if(strcmp( argv[1], "--help") == 0 or strcmp( argv[1], "-h") == 0){
			cout << "all parameters are read form setting file (specified by argument or file setting.txt by default); details are online at wiki" << endl;
			cout << "\t conjunction [--version] [--help] [setting_file.txt]" << endl;
			return 0;
		}
		if(strcmp( argv[1], "--version") == 0 or strcmp( argv[1], "-v") == 0){
			cout << "Conjunction v1.2.development" << endl;
			cout << "\tcommit: " << GITVERSION << endl;
			return 0;
		}

		setting_file = argv[1];

	}

	cerr << "Running Conjunction" << endl;
	cerr << "Commit: " << GITVERSION << endl;
	cerr << "Loading ... " << setting_file << endl;

	SettingHandler all_setting(setting_file);
	SimulationSetting one_sim_setting;
	Simulation *sim;

	cerr << "Performing: "<< all_setting.getNumberOfSimulations() << " simulations\n";
	all_setting.printWorld();

	for(int sim_index = 0; sim_index < all_setting.getNumberOfSimulations(); sim_index++){
		cerr << "########################" << endl;
		cerr << "##### SIMULATION " << sim_index+1 << " #####" << endl;
		cerr << "########################" << endl;
		one_sim_setting = all_setting.getSimualtionSetting(sim_index);
		sim = new Simulation(one_sim_setting);
		if(sim->simulate() != 0){
			cerr << " A problem during simulation " << sim_index << " has occurred \n";
			return 1;
		}
		delete sim;
	}


	return 0;
}
