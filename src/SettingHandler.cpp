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

	if (setting_file) {
		parseSetting(setting_file);
		fillDefault();
	} else {
		throw runtime_error("Can not open setting file: " + filename + "; It does not exist or you do not have premssion to read it.");
	}

	setting_file.close();

	checkParameters();
}

/* 	getSimualtionSetting is a lazy loader of settings, all the parameters are read from the vector parameters_in_order
	TODO make an explicit test for having all the values defined
	TODO (i.e. length of parameters_in_order == number of parameters for every simulation).
*/
SimulationSetting SettingHandler::getSimualtionSetting(int simulation_index) const{
	SimulationSetting mySetting;
	int number_of_parsed_simulations = 1; // 1 * parameters_numbers[i] for all i
	int refactorised_index = 0; // is an index of parameter order given a simulation index
	char parsed_parameter; // is a label of parameter used for extension of an output file name
	string file_to_save = file_name_patten; // not to overwrite pattern
//	cerr << "file_name_patten " << file_name_patten << endl;

	for(unsigned int parameter_index = 0; parameter_index < parameters_in_order.size(); parameter_index++){
		// cerr	<< "parameters_in_order " << parameters_in_order[parameter_index] << '\t'
		// 		<< "parameters_numbers " << parameters_numbers[parameter_index] << endl;
		refactorised_index = simulation_index % (number_of_parsed_simulations * parameters_numbers[parameter_index]);
		refactorised_index = (refactorised_index - (refactorised_index % number_of_parsed_simulations)) / number_of_parsed_simulations;

		if(parameters_in_order[parameter_index] == "SEED"){
			continue;
		} else {
			parsed_parameter = setParameterOfSetting(mySetting, parameters_in_order[parameter_index], refactorised_index);
		}

		adjustFileName(file_to_save, parsed_parameter, parameters_numbers[parameter_index], refactorised_index);

		number_of_parsed_simulations = number_of_parsed_simulations * parameters_numbers[parameter_index];
	}

// simulation will have defined seed even if it was not defined by user
//	cerr << " Seed of simulation " << simulation_index << " is set to " << seed[simulation_index] << "\n";
	mySetting.seed = seed[simulation_index];

	if(replicates > 1){
		// number_of_simulations / replicates corresponds to size of replicated block
		// simulation_index / size of block (integer division) give index of replicate
		refactorised_index = (simulation_index / (number_of_simulations / replicates));
		adjustFileName(file_to_save, 'n', replicates, refactorised_index);
	}

	// cerr << "File to save: " << file_to_save << endl;

	mySetting.file_to_save = file_to_save;
	mySetting.type_of_save = type_of_save;

	mySetting.dimension = dimension;
	mySetting.up_down_demes = up_down_demes;
	mySetting.left_right_demes = left_right_demes;
	mySetting.edges_per_deme = edges_per_deme;

	mySetting.type_of_updown_edges = type_of_updown_edges;
	mySetting.type_of_leftright_edges = type_of_leftright_edges;

	return mySetting;
}

int SettingHandler::getNumberOfSimulations() const{
	return number_of_simulations;
}

void SettingHandler::printParameters() const{
	int checker = 0;
	cerr << setw(15) << "loci"
		  << setw(15) << "selected_loci"
			<< setw(15) << "chromosomes"
			<< setw(15) << "deme_size"
			<< setw(15) << "generations"
			<< setw(15) << "lamda"
			<< setw(15) << "selection"
			<< setw(15) << "beta"
			<< setw(15) << "seed"
			<< setw(15) << "delay"
			<< setw(15) << "replicates"
			<< setw(15) << "saves"
			<< endl;

	for(int val_index = 0; val_index < number_of_simulations; val_index++){
		cerr << setw(15);
		checker += printVectorValue(val_index, loci);
		cerr << setw(15);
		checker += printVectorValue(val_index, selected_loci);
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
		if(val_index == 0){ cerr << replicates;}
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

void SettingHandler::printWorld() const{
	cerr << "##### WORLD SETTING #####\n"
	<< "Dimension: " << dimension << endl;
	if(dimension != 0){
		cerr << "COLUMNS: " << left_right_demes << endl
		<< "ROWS: " << up_down_demes << endl
		<< "UD DOWN BORDER: " << type_of_updown_edges << endl
		<< "LEFT RIGHT BORDER: " << type_of_leftright_edges << endl;
	}
	return;
}

void SettingHandler::printParameterCounts() const{
	for(unsigned int i = 0; i < parameters_in_order.size(); i++){
		cerr << setw(15) << parameters_in_order[i] << ' ';
	}
	cerr << endl;
	for(unsigned int i = 0; i < parameters_numbers.size(); i++){
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
	replicates = 1;

	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			// cout << "reading: " << line << endl;
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
						throw runtime_error("Symbols '.' '*' '~' and '/' are not allowed in any parameter value.");
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
						if(number.empty()){
							continue;
						}
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
						if(number.empty()){
							//cerr << "Warning, failed to read value of parameter " << parameter << endl;
							number.clear();
							parameter.clear();
							paravec.clear();
							continue;
						}

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
					throw runtime_error("ERROR: the " + type + " world description is not defined (yet?)");
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
				if(i+1 < line.size()){
					continue;
				}
			}
		}
		if(switcher == 5){
			if(isdigit(line[i]) and i+1 < line.size()){
				number.push_back((line[i]));
				continue;
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
					return 0;
				}
				if(type == "InfInf"){
					dimension = 0;
					return 0;
				}
				if(type == "LowMigrationBazykin"){
					up_down_demes = 1;
					left_right_demes = 2;
					dimension = 2;
					edges_per_deme = 4;
					type_of_updown_edges = "reflexive";
					type_of_leftright_edges = "extending";
					return 0;
				}
				cerr << "Error: Unknown pre-defined world " << type << endl;
				throw "missing informaiton in setting!";
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
				if(i+1 < line.size()){
					continue;
				}
			}
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
	throw runtime_error("ERROR: unknown problem during loading setting file (contact us please)");
}

void SettingHandler::parameterSave(std::string& parameter, double value){
	if(parameter == "LOCI"){
		loci.push_back(int(value));
		return;
	}
	if(parameter == "SELECTEDloci"){
		selected_loci.push_back(int(value));
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
	if(parameter == "REPLICATES"){
		replicates = int(value);
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
//	cerr << "REPLICATES: " << replicates << endl;
	number_of_simulations =
			loci.size() *
			selected_loci.size() *
			chrom.size() *
			deme.size() *
			gen.size() *
			sel.size() *
			beta.size() *
			lambda.size() *
			delay.size() *
			saves.size() *
			replicates;
	return;
}

template<typename T>
int SettingHandler::printVectorValue(unsigned int index, vector<T> val_vec) const{
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

char SettingHandler::setParameterOfSetting(SimulationSetting& mySetting, std::string parameter, int index) const{
	if(parameter == "LOCI"){
		mySetting.loci = loci[index];
		return 'l';
	}
	if(parameter == "SELECTEDloci"){
		if(selected_loci[0] == 0){
			mySetting.selected_loci = loci[index];
		} else {
			mySetting.selected_loci = selected_loci[index];
		}
		return 'L';
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

void SettingHandler::adjustFileName(string& name, char par, int total, int refactorised_index) const{
	if(total > 1){
		if(total > 9){
			name = name + '_' + par + '?' + '!';
			name[name.find('?')] = '0' + char((refactorised_index + 1) / 10);
			refactorised_index = refactorised_index - (((refactorised_index + 1) / 10) * 10);
		} else {
			name = name + '_' + par + '!';
		}
		name[name.find('!')] = '1' + char(refactorised_index);
	}
}

void SettingHandler::checkParameters(){

//	if(parameters_in_order.size() < 9){
//		cerr << " Error: non specified parameters. \n Expected: \n Found: ";
//
//		for(unsigned int i = 0; i < parameters_in_order.size(); i++){
//			cerr << parameters_in_order[i] << " ";
//		}
//		cerr << endl;
//	}

	// Number of generations / saves / delay

	for(unsigned int i = 0; i < gen.size(); i++){
		if(gen[i] < 0){
			throw runtime_error("Negative NUMBERofGENERATIONS.");
		}
		for(unsigned int j = 0; j < saves.size(); j++){
			if(saves[j] == 0){
				continue;
			}
			for(unsigned int k = 0; k < delay.size(); k++){
				if(saves[j] > (gen[i] - delay[k])){
					throw runtime_error("The number of saves (" + to_string(saves[j]) +
										") is greater than number of generations after delay (" +
										to_string(gen[i] - delay[k]) + ")." );
				}
			}
		}
	}

	// saving type
	bool correct_type = 1;

	if(!file_name_patten.empty()){
		vector<string> types{"complete", "summary", "hybridIndices", "hybridIndicesJunctions", "blocks", "raspberrypi", "backtrace"};
		for(unsigned int i = 0; i < types.size(); i++){
			if(type_of_save == types[i]){
				correct_type = !correct_type;
				break;
			}
		}

		if(correct_type > 0){
			throw runtime_error("Type of save is invalid: " + type_of_save);
		}
	}

	for(unsigned int i = 0; i < saves.size(); i++){
		if(saves[i] > 0){
			if(file_name_patten.empty()){
				cerr << "The name of output file was not specified. The only output will be printed strandard stream.\n";
			}
		}
	}

	// basic parameters

	if(deme.size() == 0){
		throw runtime_error("The DEMEsize was not set.");
	}
//	for(unsigned int i = 0; i < deme.size(); i++){
//		if(deme[i] < 4){
//			cerr << "Strange deme size \n"
//		}
//	}

	if(lambda.size() == 0){
		throw runtime_error("The LAMBDA ( mean number of crossovers per chromosome per generation) was not set.");
	}
//	for(unsigned int i = 0; i < lambda.size(); i++){
//		if(lambda[i] == -1){
//
//			return 1;
//		}
//	}

	if(loci.size() == 0){
		throw runtime_error("The number of loci per chromosome was not set.");
	}

//	for(unsigned int i = 0; i < loci.size(); i++){
//		if(loci[i] == -1){
//
//			return 1;
//		}
//	}

	int neutral_block_size = 0;
	for(unsigned int i = 0; i < selected_loci.size(); i++){
		if(selected_loci[i] == 0){
			if(selected_loci.size() > 1){
				throw runtime_error("If selected loci parameter is set to 0 it is expected to be only one value only\nIs selected loci are set to 0, all the loci are selected.");
			}
		} else {
			for(unsigned int j = 0; j < loci.size(); j++){
				neutral_block_size = (loci[j] - selected_loci[i]) % (selected_loci[i] - 1);
				if(neutral_block_size != 0){
					cerr 	<< "Values of loci (" << loci[j]
							<< ") and selected loci (" << selected_loci[i]
							<< ") are incompatible" << endl;
					cerr	<< "\t(loci - selected_loci) % (selected_loci - 1)" << endl;
					cerr	<< "\thas to be equal to 0, but in this case..." << endl;
					cerr	<< "\t(" << loci[j] << " - " << selected_loci[i] << ") % (" << selected_loci[i] << " - 1) = " << neutral_block_size << endl;
					throw runtime_error("yeah, it's bit messy!");
				}
			}
		}
	}

	if(sel.size() == 0){
		throw runtime_error("The selection pressure was not set.");
	}

//	for(unsigned int i = 0; i < sel.size(); i++){
//		if(sel[i] == -1){
//			return 1;
//		}
//	}

	if(beta.size() == 0){
		throw runtime_error("The BETA (epistatic interaction between loci) was not set.");
	}
	for(unsigned int i = 0; i < beta.size(); i++){
		if(beta[i] < 0.0001 or beta[i] > 32){
			cerr << "Warning: BETA is out of range of reasonable values: " << beta[i] << endl;
		}
	}

	if((unsigned)number_of_simulations != seed.size()){
		cerr << "The number of seeds is not equal to number of simulation\n";
		cerr << "Possible SEED: none (seeds will be random generated using time)\n";
		cerr << "               one value (other seeds will be generated using defined seed)\n";
		cerr << "               vector of values (has to have length equal to total number of simulations)\n";
		throw runtime_error("SEED problem.");
	}

	//cerr << "Checking World definition\n";
	if(dimension == -1){
		throw runtime_error("Dimension was not set.");
	}

	if(dimension >= 3){
		throw runtime_error("Dimension was not set correctly.");
	}

	if(left_right_demes < 1 and dimension > 0){
		throw runtime_error("The width of world was not set.");
	}

	if(up_down_demes < 1 and dimension > 0){
		throw runtime_error("The height of world was not set.");
	}

	if(dimension > 0){
		vector<string> edges {"extending","wrapping","infinite","reflexive"};
		if(type_of_leftright_edges.empty()){
			throw runtime_error("The type of left right edges of world was not set.");
		} else {
			correct_type = 1;
			for(unsigned int i = 0; i < edges.size(); i++){
				if(type_of_leftright_edges == edges[i] and dimension > 0){
					correct_type = !correct_type;
					break;
				}
			}
			if(correct_type){
				throw runtime_error("Type of left-right edges is invalid: " + type_of_leftright_edges);
			}
		}

		if(type_of_updown_edges.empty()){
			if(dimension == 2){
				throw runtime_error("The type of up-down edges of 2D world was not set.");
			}
		} else {
			correct_type = 1;
			for(unsigned int i = 1; i < edges.size(); i++){
				if(type_of_updown_edges == edges[i]){
					correct_type = !correct_type;
					break;
				}
			}
			if(correct_type){
				throw runtime_error("Type of UpDown edges is invalid: " + type_of_updown_edges);
			}
		}
	}

	return;
}

void SettingHandler::fillDefault(){

	if(delay.empty()){
		delay.push_back(0);
		parameters_in_order.push_back("DELAY");
		parameters_numbers.push_back(1);
	}

	if(selected_loci.empty()){
		cerr << "Assuming all loci under selection.\n";
		selected_loci.push_back(0);
		parameters_in_order.push_back("SELECTEDloci");
		parameters_numbers.push_back(1);
	}

	if(beta.empty()){
		cerr << "Assuming no epistatic interaction (BETA = 1).\n";
		beta.push_back(1);
		parameters_in_order.push_back("BETA");
		parameters_numbers.push_back(1);
	}

	updateNumberOfSimulations();
//	cerr << "Numbe of sims: " << number_of_simulations << endl;

	// seeds are computed for every simulations,
	// so all parameters has to be set before seed will.
	if(seed.empty()){
		// add number of simulations seeds
		//	cerr << "empty seed" << endl;
		srand ( time(NULL) );
		for(int i=0; i < number_of_simulations; i++){
			seed.push_back(rand() % 1000000);
		}
	} else {
			//cerr << "non-empty seed" << endl;
			if(seed.size() == 1){
				// add number of simulations - 1 new seeds (the first one was defined already)
				for(int i=0; i < (number_of_simulations-1); i++){
					seed.push_back(rand() % 1000000);
					//cerr << "added one seed to have " << seed.size() << endl;
				}
			}
	}

	return;
}
