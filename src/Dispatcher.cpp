/*
 * Dispatcher.cpp
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

#include "../include/SimulationSetting.h"
#include "../include/Dispatcher.h"

using namespace std;

Dispatcher::Dispatcher(string filename) {
	ifstream setting_file(filename);

	parseSetting(setting_file);

	setting_file.close();
}

int Dispatcher::parseSetting(ifstream& myfile){
	double value;
	vector<double> paravec;
	string switcher;
	string line, parameter, number;
	string file_name_patten, type_of_file;
	simulations.reserve(1);

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			if(line.empty()){
				continue;
			}
			switcher = "read_name"; //dafualt state of switcher - reading parameters name
			for(unsigned int i = 0;i < line.size();i++){
				if(line[i] == '#'){ // # is comment symbol
					break;
				}
				if(line[i] == '='){
					switcher = "read_value"; // symbol = switches to reading values
					continue;
				}
				if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
					if(line[i] == '.' or line[i] == '~' or line[i] == '*' or line[i] == '/' or line[i] == '\\'){
						cerr << "Error: invalid name of the output file: symbols '.' '*' '~' and '/' are not allowed \n";
						return 1;
					}
					if(!isspace(line[i])){
						number += line[i];
					}
					continue;
				}
				if(parameter.substr(0,16) == "TYPEofOUTPUTfile"){
					if(!isspace(line[i])){
						number += line[i];
					}
					continue;
				}
				if(switcher == "read_name"){
					if(isalpha(line[i])){
						parameter.push_back(line[i]);
					}
				} else {
					if(isdigit(line[i]) or line[i] == '.'){
						number += line[i];
					}
					if((line[i] == ']' or line[i] == ',' or line[i] == ';')){
						paravec.push_back(stod(number));
						number.clear();
					}
				}
			}

			if(!parameter.empty()){
				if(paravec.empty()){
//				string or value
					if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
						file_name_patten = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,16) == "TYPEofOUTPUTfile"){
						type_of_file = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,5) == "world"){
						return parseWorldDefinition(line);
					} else {
// 					value
						value = stod(number);
						parameterSave(parameter,value);
					}
				} else {
// 				vector
					parameterSave(parameter,paravec);

					number.clear();
					parameter.clear();
					paravec.clear();
					continue;
				}
			}

//			clear for next line to read
			number.clear();
			parameter.clear();
		}

	}
	return 0;
}

int Dispatcher::parseWorldDefinition(string& myfile){
	return 1;
}

void Dispatcher::parameterSave(std::string& parameter, double value){
	int num_of_sims = simulations.capacity();

	for(int sim_index = 0; sim_index < num_of_sims; sim_index++){
		if(parameter == "LOCI"){
			simulations[sim_index].loci = int(value);
			continue;
		}
		if(parameter == "NUMBERofCHROMOSOMES"){
			simulations[sim_index].chromosomes = int(value);
			continue;
		}
		if(parameter == "LAMBDA"){
			simulations[sim_index].lamda = value;
			continue;
		}
		if(parameter == "SELECTIONpressure"){
			simulations[sim_index].selection = value;
			continue;
		}
		if(parameter == "BETA"){
			simulations[sim_index].beta = value;
			continue;
		}
		if(parameter == "DEMEsize"){
			simulations[sim_index].deme_size = int(value);
			continue;
		}
		if(parameter == "SEED"){
			simulations[sim_index].seed = int(value);
			continue;
		}
		if(parameter == "NUMBERofGENERATIONS"){
			simulations[sim_index].generations = int(value);
			return;
		}
		if(parameter == "NUMBERofSAVES"){
			simulations[sim_index].saves = int(value);
			continue;
		}
		if(parameter == "DELAY"){
			simulations[sim_index].delay = int(value);
			continue;
		}
		cerr << "Warning: unknown parameter: " << parameter << endl;
	}
	return;
}

void Dispatcher::parameterSave(string& parameter, vector<double>& value){
	int num_of_sims = simulations.capacity();
	int size_of_vec = value.size();

	vector<SimulationSetting> orig_setting = simulations;
	vector<SimulationSetting> run_to_add;
	run_to_add.reserve(num_of_sims);

	printSimulationSetting();

	simulations.reserve(num_of_sims * size_of_vec);

	for(int val_index = 0; val_index < size_of_vec; val_index++){
		run_to_add = orig_setting;
		for(int sim_index = 0; sim_index < num_of_sims; sim_index++){
			if(parameter == "LOCI"){
				run_to_add[sim_index].loci = int(value[val_index]);
				continue;
			}
			if(parameter == "NUMBERofCHROMOSOMES"){
				run_to_add[sim_index].chromosomes = int(value[val_index]);
				continue;
			}
			if(parameter == "LAMBDA"){
				run_to_add[sim_index].lamda = value[val_index];
				continue;
			}
			if(parameter == "SELECTIONpressure"){
				run_to_add[sim_index].selection = value[val_index];
				continue;
			}
			if(parameter == "BETA"){
				run_to_add[sim_index].beta = value[val_index];
				continue;
			}
			if(parameter == "DEMEsize"){
				run_to_add[sim_index].deme_size = int(value[val_index]);
				continue;
			}
			if(parameter == "SEED"){
				run_to_add[sim_index].seed = int(value[val_index]);
				continue;
			}
			if(parameter == "NUMBERofGENERATIONS"){
				run_to_add[sim_index].generations = int(value[val_index]);
				return;
			}
			if(parameter == "NUMBERofSAVES"){
				run_to_add[sim_index].saves = int(value[val_index]);
				continue;
			}
			if(parameter == "DELAY"){
				run_to_add[sim_index].delay = int(value[val_index]);
				continue;
			}
		}

		simulations.insert(simulations.end(), run_to_add.begin(), run_to_add.end());
	}

	cerr << "Warning: Unknown parameter variable " << parameter << endl;
	return;
}

void Dispatcher::printSimulationSetting(){
	int num_of_sims = simulations.capacity();

	cerr << setw(15) << " loci "
			<< setw(15) << " chromosomes "
			<< setw(15) << " deme_size "
			<< setw(15) << " generations "
			<< setw(15) << " lamda "
			<< setw(15) << " selection "
			<< setw(15) << " beta "
			<< endl;

	for(int val_index = 0; val_index < num_of_sims; val_index++){
		cerr << setw(15) << simulations[val_index].loci << ' ';
		cerr << setw(15) << simulations[val_index].chromosomes << ' ';
		cerr << setw(15) << simulations[val_index].deme_size << ' ';
		cerr << setw(15) << simulations[val_index].generations << ' ';
		cerr << setw(15) << simulations[val_index].lamda << ' ';
		cerr << setw(15) << simulations[val_index].selection << ' ';
		cerr << setw(15) << simulations[val_index].beta << endl;
	}

	return;
}
