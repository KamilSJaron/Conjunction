/*
Class Setting Handler is reading setting file as it is described in master thesis of KamilSJaron
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
#include <vector>
#include <string>
#include <iomanip>

#include "../include/SimulationSetting.h"
#include "../include/SettingHandler.h"

using namespace std;

SettingHandler::SettingHandler(string filename) {
	ifstream setting_file(filename);

	parseSetting(setting_file);
	updateNumberOfSimulations();

	setting_file.close();
}

SimulationSetting SettingHandler::getSimualtionSetting(int simulation_index){
	SimulationSetting mySetting;
	int number_of_parsed_simulations = 1;
	int refactorised_index = 0;
	char parsed_parameter;

	for(unsigned int parameter_index = 0; parameter_index < parameters_in_order.size(); parameter_index++){
		refactorised_index = simulation_index % (number_of_parsed_simulations * parameters_numbers[parameter_index]);
		refactorised_index = (refactorised_index - (refactorised_index % number_of_parsed_simulations)) / number_of_parsed_simulations;

		parsed_parameter = setPatameterOfSetting(mySetting, parameters_in_order[parameter_index], refactorised_index);

		if(parameters_numbers[parameter_index] > 1){
			file_name_patten = file_name_patten + parsed_parameter + char(parameter_index);
		}

		number_of_parsed_simulations = number_of_parsed_simulations * parameters_numbers[parameter_index];
	}

	mySetting.file_to_save = file_name_patten;
	mySetting.type_of_save = type_of_save;

	mySetting.dimension = dimension;
	mySetting.up_down_demes = up_down_demes;
	mySetting.left_right_demes = left_right_demes;
	mySetting.edges_per_deme = edges_per_deme;

	mySetting.type_of_updown_edges = type_of_updown_edges;
	mySetting.type_of_leftright_edges = type_of_leftright_edges;


	return mySetting;
};

int SettingHandler::getNumberOfSimulations() const{
	return number_of_simulations;
}

void SettingHandler::printParameters() const{
	int checker = 0;
	cerr << setw(15) << "loci"
			<< setw(15) << "chromosomes"
			<< setw(15) << "deme_size"
			<< setw(15) << "generations"
			<< setw(15) << "lamda"
			<< setw(15) << "selection"
			<< setw(15) << "beta"
			<< setw(15) << "seed"
			<< setw(15) << "delay"
			<< setw(15) << "saves"
			<< endl;

	for(int val_index = 0; val_index < number_of_simulations; val_index++){
		cerr << setw(15);
		checker += printVectorValue(val_index, loci);
		cerr << setw(15);
		checker += printVectorValue(val_index, chrom);
		cerr << setw(15);
		checker += printVectorValue(val_index, deme);
		cerr << setw(15);
		checker += printVectorValue(val_index, gen);
		cerr << setw(15);
		checker += printVectorValue(val_index, lambda);
		cerr << setw(15);
		checker += printVectorValue(val_index, sel);
		cerr << setw(15);
		checker += printVectorValue(val_index, beta);
		cerr << setw(15);
		checker += printVectorValue(val_index, seed);
		cerr << setw(15);
		checker += printVectorValue(val_index, delay);
		cerr << setw(15);
		checker += printVectorValue(val_index, saves);
		cerr << endl;
		if(checker == 0){
			return;
		} else {
			checker = 0;
		}
	}

	return;
}

void SettingHandler::printParameterCounts() const{
	for(int i = 0; i < parameters_in_order.size(); i++){
		cerr << setw(15) << parameters_in_order[i] << ' ';
	}
	cerr << endl;
	for(int i = 0; i < parameters_numbers.size(); i++){
		cerr << setw(15) << parameters_numbers[i] << ' ';
	}
	cerr << endl;
	return;
}

  // // // // //
 // PRIVATE  //
// // // // //

int SettingHandler::parseSetting(ifstream& myfile){
	double value;
	vector<double> paravec;
	string switcher;
	string line, parameter, number;

	if (myfile.is_open()){
		while ( getline (myfile,line) ){

			// extract parameter name and its value from line

			if(line.empty()){
				continue;
			}
			switcher = "read_name"; //dafualt state of switcher for every line is to read parameters name
			for(unsigned int i = 0;i < line.size();i++){
				if(line[i] == '#'){ // # is comment symbol
					break;
				}
				if(line[i] == ' '){
					continue;
				}
				if(line[i] == '='){
					switcher = "read_value"; // symbol = switches to reading values
					continue;
				}
				if(parameter.substr(0,16) == "NAMEofOUTPUTfile" or parameter.substr(0,16) == "TYPEofOUTPUTfile"){
					if(line[i] == '.' or line[i] == '~' or line[i] == '*' or line[i] == '/' or line[i] == '\\'){
						cerr << "Error: Symbols '.' '*' '~' and '/' are not allowed in any parameter value. \n";
						return 1;
					}
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

			// save value to appropriate variable

			if(!parameter.empty()){
				if(paravec.empty()){
//					string (file name, file type, world) or value
					if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
						file_name_patten = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,16) == "TYPEofOUTPUTfile"){
						type_of_save = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,5) == "WORLD"){
						return parseWorldDefinition(line);
					} else {
// 					if single value was read
						parameters_in_order.push_back(parameter);
						parameters_numbers.push_back(1);

						value = stod(number);
						parameterSave(parameter,value);
					}
				} else {
// 					if vector vector was read
					parameters_in_order.push_back(parameter);
					parameters_numbers.push_back(paravec.size());

					parameterSave(parameter,paravec);
				}
//				prepare for next line
				number.clear();
				parameter.clear();
				paravec.clear();
				continue;
			}
		}
	}
	return 0;
}

int SettingHandler::parseWorldDefinition(string& line){
	int switcher = 0, n = 0;
// 	double arenan = 0;
	string type, number;
	int stdHeight = 0, stdWidth = -1, stdDim = 0;
	string stdLR, stdUD;
	for(unsigned int i = 0;i < line.size();i++){
		if(line[i] == '-'){
			switcher = 1;
		}
		if(switcher == 1){
			if(isalpha(line[i])){
				type.push_back(line[i]);
				continue;
			} else {
				if(line[i] == '='){
					if(type == "quick"){
						type.clear();
						switcher = 2;
						continue;
					}
					if(type == "standard"){
						type.clear();
						stdLR.clear();
						stdUD.clear();
						switcher = 12;
						continue;
					}
// 					if(type == "complete"){
// 						switcher = 22;
// 						continue;
// 					}
					cerr << "ERROR: the " << type << " world description is not defined (yet)." << endl;
					return 1;
				}
			}
		}
		if(switcher == 2){
			if(line[i] == '\''){
				switcher = 3;
				continue;
			}
		}
		if(switcher == 3){
			if(line[i] == '\''){
				switcher = 4;
				continue;
			}
			type.push_back(line[i]);
			continue;
		}
		if(switcher == 4){
			if(isdigit(line[i])){
				switcher = 5;
				number.push_back((line[i]));
				continue;
			}
		}
		if(switcher == 5){
			if(isdigit(line[i])){
				number.push_back((line[i]));
			} else {
				if(type == "HybridZone"){
					n = stoi( number );
					left_right_demes = 2;
					up_down_demes = n;
					if(n == 1){
						dimension = 1;
						edges_per_deme = 2;
					} else {
						dimension = 2;
						edges_per_deme = 4;
						type_of_updown_edges = "wrapping";
					}
					type_of_leftright_edges = "extending";
					cerr << "World is quick defined as " << n << " demes long hybrid zone." << endl;
					return 0;
				}
				if(type == "Arena"){
					n = stoi(number);
					up_down_demes = n;
					left_right_demes = n;
					type_of_updown_edges = "reflexive";
					type_of_leftright_edges = "reflexive";
					if(n > 2){
						dimension = 2;
						edges_per_deme = 4;
					} else {
						dimension = 1;
						edges_per_deme = 0;
					}
					cerr << "World is quick-defined as " << n << 'x' << n << " demes arena." << endl;
					return 0;
				}
				if(type == "InfInf"){
					cerr << "World is quick-defined as zero dimensional border of infinite popualtions" << endl;
					dimension = 0;
					return 0;
				}
				if(type == "LowMigrationBazykin"){
					type_of_updown_edges = 1;
					type_of_leftright_edges = 2;
					dimension = 2;
					edges_per_deme = 4;
					type_of_updown_edges = "reflexive";
					type_of_leftright_edges = "extending";
					cerr << "World is quick defined as " << n << " demes long hybrid zone." << endl;
					return 0;
				}
				cerr << "Error: Unknown pre-defined world " << type << endl;
				return 1;
			}
		}
		if(switcher == 12){
			if(isdigit(line[i])){
				switcher = 13;
				number.push_back((line[i]));
				stdDim = stoi(number);
				number.clear();
				continue;
			}
		}
		if(switcher == 13){
			if(line[i] == '\''){
				switcher = 14;
				continue;
			}
		}
		if(switcher == 14){
			if(line[i] == '\''){
				if(stdLR.empty()){
					stdLR = type;
				} else {
					stdUD = type;
				}
				type.clear();
				switcher = 15;
				continue;
			}
			type.push_back(line[i]);
			continue;
		}
		if(switcher == 15){
			if(isdigit(line[i])){
				number.push_back((line[i]));
				continue;
			} else {
				if(number.empty()){
					continue;
				} else {
					if(stdWidth == -1){
						stdWidth = stoi(number);
						number.clear();
					} else {
						stdHeight = stoi(number);
						dimension = stdDim;
						if(stdDim == 1){
							edges_per_deme = 2;
						} else {
							edges_per_deme = 4;
						}
						up_down_demes = stdHeight;
						left_right_demes = stdWidth;
						type_of_updown_edges = stdUD;
						type_of_leftright_edges = stdLR;
						return 0;
					}
					switcher = 13;
				}
			}
		}
	}

	return -1;
}

void SettingHandler::parameterSave(std::string& parameter, double value){
	if(parameter == "LOCI"){
		loci.push_back(int(value));
		return;
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		chrom.push_back(int(value));
		return;
	}
	if(parameter == "LAMBDA"){
		lambda.push_back(value);
		return;
	}
	if(parameter == "SELECTIONpressure"){
		sel.push_back(value);
		return;
	}
	if(parameter == "BETA"){
		beta.push_back(value);
		return;
	}
	if(parameter == "DEMEsize"){
		deme.push_back(int(value));
		return;
	}
	if(parameter == "SEED"){
		seed.push_back(int(value));
		return;
	}
	if(parameter == "NUMBERofGENERATIONS"){
		gen.push_back(int(value));
		return;
	}
	if(parameter == "NUMBERofSAVES"){
		saves.push_back(int(value));
		return;
	}
	if(parameter == "DELAY"){
		delay.push_back(int(value));
		return;
	}

	cerr << "Warning: unknown parameter: " << parameter << endl;
	return;
}

void SettingHandler::parameterSave(string& parameter, vector<double>& value){
	int size_of_vec = value.size();

	for(int val_index = 0; val_index < size_of_vec; val_index++){
		parameterSave(parameter, value[val_index]);
	}

	return;
}

void SettingHandler::updateNumberOfSimulations(){
	number_of_simulations =
			loci.size() *
			chrom.size() *
			deme.size() *
			gen.size() *
			sel.size() *
			beta.size() *
			lambda.size() *
			seed.size() *
			delay.size() *
			saves.size();
	return;
}

template<typename T>
int SettingHandler::printVectorValue(int index, vector<T> val_vec) const{
	if(index < val_vec.size()){
		cerr << val_vec[index];
		if(index + 1 < val_vec.size()){
			return 1;
		} else {
			return 0;
		}
	} else {
		cerr << ' ';
		return 0;
	}
}

char SettingHandler::setPatameterOfSetting(SimulationSetting& mySetting, std::string parameter, int index){
	if(parameter == "LOCI"){
		mySetting.loci = loci[index];
		return 'l';
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		mySetting.chromosomes = chrom[index];
		return 'c';
	}
	if(parameter == "LAMBDA"){
		mySetting.lambda = lambda[index];
		return 'r';
	}
	if(parameter == "SELECTIONpressure"){
		mySetting.selection = sel[index];
		return 's';
	}
	if(parameter == "BETA"){
		mySetting.beta = beta[index];
		return 'b';
	}
	if(parameter == "DEMEsize"){
		mySetting.deme_size = deme[index];
		return 'd';
	}
	if(parameter == "SEED"){
		mySetting.seed = seed[index];
		return 'n';
	}
	if(parameter == "NUMBERofGENERATIONS"){
		mySetting.generations = gen[index];
		return 'G';
	}
	if(parameter == "NUMBERofSAVES"){
		mySetting.saves = saves[index];
		return 'S';
	}
	if(parameter == "DELAY"){
		mySetting.delay = delay[index];
		return 'D';
	}
	return '?';
}
