#include <iostream> // preprocessor directive
#include <cmath>
#include <string>  // strings
#include <cstring> 
#include <map>  // associative arrays
#include <algorithm> // sort
#include <fstream>
#include <new> // alocation of memory in heap
#include <regex>
#include <iomanip>
#include <time.h>
#include<g2.h>
#include<g2_X11.h>
#include<g2_PS.h>
#include<g2_gd.h>

#include "universe.h" // Class of individuals

static int SEEDtoRAND = 1; //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
static int NUMBERofGENERATIONS = 500; // #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/
static int NUMBERofSAVES = 1;

using namespace std;

// list of fctions
void parameterSlave(string parameter, double value);
void parameterSlave(char parameter, double value);
void parameterSlave(string parameter, vector<double>& valvec, vector<double>& paravec, vector<char>& para);
void probMAPslave(string parameter, vector<double>& velvec);
int worldSlave(string line, Universe* World);
int setParameters(Universe* World, vector<double>& PARAvec1,vector<double>& PARAvec2,vector<double>& PARAvec3,vector<char>& PARAnames);
int testParameters(Universe* World);
void const showVector(vector<double>& valvec);
void simulate(Universe* World, int save_pos);
void simulate(Universe* World);

int main()
{
// 	this part sould not be edited anymore
	Universe World;
	vector<double> PARAvector1, PARAvector2, PARAvector3;
	vector<char> PARAnames;
	
	int check = setParameters(&World, PARAvector1, PARAvector2, PARAvector3,PARAnames);
	if(check == 1){
		cerr << "Exit: input file problem." << endl;
		return 1;
	}
	
	srand (SEEDtoRAND); // setting a seed
	
	clock_t t_total1, t_total2;
	int run = 0;
	int pos1 = 0, pos2 = 0, pos3 = 0, save_pos = 0;
	int ten_stop1 = 0, ten_stop2 = 0, ten_stop3 = 0;
	
	if(PARAnames.size() >= 1){
		NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_") + PARAnames[0] + string("_0~");
		pos1 = NAMEofOUTPUTfile.find('~');
	}
	if(PARAnames.size() >= 2){
		NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_") + PARAnames[1] + string("_0/");
		pos2 = NAMEofOUTPUTfile.find('/');
	}
	if(PARAnames.size() >= 3){
		NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_") + PARAnames[2] + string("_0\\");
		pos3 = NAMEofOUTPUTfile.find('\\');
	}
	if(NUMBERofSAVES > 1){
		NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_*");
		save_pos = NAMEofOUTPUTfile.find('*');
	}
	NAMEofOUTPUTfile = NAMEofOUTPUTfile + string(".dat");
	
	if(PARAnames.size() >= 1){
		t_total1 = clock();
		for(unsigned int j = 0;j < PARAvector1.size();j++){
// 			setting for next simulation
			parameterSlave(PARAnames[0],PARAvector1[j]);
			if(j < unsigned(ten_stop1 + 10)){
				NAMEofOUTPUTfile[pos1] = char(j + '0' - ten_stop1);
			} else {
				ten_stop1 += 10;
				NAMEofOUTPUTfile[pos1-1] = char(NAMEofOUTPUTfile[pos1-1] + 1);
				NAMEofOUTPUTfile[pos1] = char(j + '0' - ten_stop1);
			}
			
			if(PARAnames.size() >= 2){
				for(unsigned int k = 0;k < PARAvector2.size();k++){
					parameterSlave(PARAnames[1],PARAvector2[k]);
					if(k < unsigned(ten_stop2 + 10)){
						NAMEofOUTPUTfile[pos2] = char(k + '0' - ten_stop2);
					} else {
						ten_stop2 += 10;
						NAMEofOUTPUTfile[pos2-1] = char(NAMEofOUTPUTfile[pos2-1] + 1);
						NAMEofOUTPUTfile[pos2] = char(k + '0' - ten_stop2);
					}
					
					if(PARAnames.size() == 3){
						for(unsigned int l = 0;l < PARAvector3.size();l++){
							parameterSlave(PARAnames[2],PARAvector3[l]);
							if(l < unsigned(ten_stop3 + 10)){
								NAMEofOUTPUTfile[pos3] = char(l + '0' - ten_stop3);
							} else {
								ten_stop3 += 10;
								NAMEofOUTPUTfile[pos3-1] = char(NAMEofOUTPUTfile[pos3-1] + 1);
								NAMEofOUTPUTfile[pos3] = char(l + '0' - ten_stop3);
							}
							run++;
							cerr << "RUN : " << run << endl;
							World.restart();
							simulate(&World,save_pos);
						}
					} else {
						run++;
						cerr << "RUN : " << run << endl;
						World.restart();
						simulate(&World,save_pos);
					}
				}
			} else {
				run++;
				cerr << "RUN : " << run << endl;
				World.restart();
				simulate(&World,save_pos);
			}
		}
		t_total2 = clock();
		cerr << "TOTAL TIME OF THE SIMULATION IS " << ((float)t_total2 - (float)t_total1) / CLOCKS_PER_SEC << endl;
	} else {
		simulate(&World);
	}
// 	 	char filePattern[] = "../playground/pictXX.png";
	
	return 0;
}



void parameterSlave(string parameter, double value){
	if(parameter == "RESOLUTION"){
		Chromosome::setResolution(int(value));
		cerr << "Setting parameter RESOLUTION to: " << value << endl;
		return;
	}
	if(parameter == "PROBABILITYmap"){
		cerr << "Warning: parameter PROBABILITYmap is expected in form..." << endl 
		<< "      [ v1, v2, v3, ... vn]" << endl 
		<< "      where n is RESOLUTION + 1 and sumation of vi is 1 for i = 1 ... n" << endl;
		return;
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		Individual::setNumberOfChromosomes(int(value));
		cerr << "Setting parameter NUMBERofCHROMOSOMES to: " << value << endl;
		return;
	}
	if(parameter == "RECOMBINATIONrate"){
		Individual::setRECOMBINATIONrate(value);
		cerr << "Setting parameter RECOMBINATIONrate to: " << value << endl;
		return;
	}
	if(parameter == "SELECTIONpressure"){
		Individual::setSELECTIONpressure(value);
		cerr << "Setting parameter SELECTIONpressure to: " << value << endl;
		return;
	}
	if(parameter == "BETA"){
		Individual::setBETA(value);
		cerr << "Setting parameter BETA to: " << value << endl;
		return;
	}
	if(parameter == "DEMEsize"){
		Deme::setDEMEsize(int(value));
		cerr << "Setting parameter DEMEsize to: " << value << endl;
		return;
	}
	if(parameter == "SEED"){
		SEEDtoRAND = value;
		cerr << "Setting parameter SEED to: " << value << endl;
		return;
	}
	if(parameter == "NUMBERofGENERATIONS"){
		NUMBERofGENERATIONS = value;
		cerr << "Setting parameter NUMBERofGENERATIONS to: " << value << endl;
		return;
	}
	if(parameter == "NUMBERofSAVES"){
		NUMBERofSAVES = (int(value));
		cerr << "Setting parameter NUMBERofSAVES to: " << value << endl;
		return;
	}
	cerr << "Warning: unknown parameter: " << parameter << endl;
	return;
}

void parameterSlave(char parameter, double value){
	if(parameter == 'L'){
		Chromosome::setResolution(int(value));
		cerr << "Setting parameter RESOLUTION to: " << value << endl;
		return;
	}
	if(parameter == 'C'){
		Individual::setNumberOfChromosomes(int(value));
		cerr << "Setting parameter NUMBERofCHROMOSOMES to: " << value << endl;
		return;
	}
	if(parameter == 'r'){
		Individual::setRECOMBINATIONrate(value);
		cerr << "Setting parameter RECOMBINATIONrate to: " << value << endl;
		return;
	}
	if(parameter == 's'){
		Individual::setSELECTIONpressure(value);
		cerr << "Setting parameter SELECTIONpressure to: " << value << endl;
		return;
	}
	if(parameter == 'b'){
		Individual::setBETA(value);
		cerr << "Setting parameter BETA to: " << value << endl;
		return;
	}
	if(parameter == 'D'){
		Deme::setDEMEsize(int(value));
		cerr << "Setting parameter DEMEsize to: " << value << endl;
		return;
	}
	cerr << "Warning: unknown parameter: " << parameter << endl;
	return;
}


void probMAPslave(string parameter, vector<double>& velvec){
		if(!velvec.empty()){
			PROBABILITYmap = velvec;
			double sum_of_elems=0;
			if(PROBABILITYmap.size() != unsigned(RESOLUTION + 1)){
				cerr << "Warning: Length of probability map do not match the number of Loci (parameter RESOLUTION)" << endl;
				cerr << "        The default PROBABILITYmap will be used..." << endl;
				PROBABILITYmap.clear();
			} else {
				for(vector<double>::iterator j=PROBABILITYmap.begin();j!=PROBABILITYmap.end();++j){
					sum_of_elems += *j;
				}
				if(sum_of_elems != 1){
					for(unsigned int i=0;i<PROBABILITYmap.size();i++){
						PROBABILITYmap[i] = PROBABILITYmap[i] / sum_of_elems;
					}
					cerr << "Warning: the PROBABILITYmap is not equal to 1" << endl;
					cerr << "        The PROBABILITYmap was normalized..." << endl;
				}
			}
		} else {
			cerr << "Warning: PROBABILITYmap was not defined" << endl;
			cerr << "        The default PROBABILITYmap will be used..." << endl;
		}
		
		cerr << "Setting parameter PROBABILITYmap to: [";
		
		if(PROBABILITYmap.empty() == true){
			cerr << "0, ";
			if(RESOLUTION < 20){
				for(int i = 0; i < (RESOLUTION - 1); i++){
					cerr << (1. / double(RESOLUTION - 1)) << ", " ;
				}
			} else {
				cerr << "1 / (L - 1) , ... , 1 / (L - 1), ";
			}
			cerr << "0]" << endl;
		} else {
			double sum = 0;
			for(unsigned int i = 0;i < PROBABILITYmap.size()-1;i++){
				cerr << PROBABILITYmap[i] << ", ";
				sum += PROBABILITYmap[i];
				PROBABILITYmap[i] = sum;
			}
			cerr << PROBABILITYmap[PROBABILITYmap.size()-1] << "]" << endl;
			sum += PROBABILITYmap[PROBABILITYmap.size()-1];
			PROBABILITYmap[PROBABILITYmap.size()-1] = sum;
		}
		cerr << "Warning: unknown setting of PROBABILITYmap." << endl;
		return;
}

void parameterSlave(string parameter, vector<double>& valvec, vector<double>& paravec, vector<char>& para){
	paravec = valvec;
	if(parameter == "PROBABILITYmap"){
		probMAPslave(parameter, valvec);
		return;
	}
	if(parameter == "RESOLUTION"){
		para.push_back('L');
		cerr << para.size() << ". vector variable resolution (L) is set to ";
		showVector(paravec);
		return;
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		para.push_back('C');
		cerr << para.size() << ". vector variable number of chromosomes (C) is set to ";
		showVector(paravec);
		return;
	}
	if(parameter == "RECOMBINATIONrate"){
		para.push_back('r');
		cerr << para.size() << ". vector variable recombination rate (r) is set to ";
		showVector(paravec);
		return;
	}
	if(parameter == "SELECTIONpressure"){
		para.push_back('s');
		cerr << para.size() << ". vector variable is selection (s) is set to ";
		showVector(paravec);
		return;
	}
	if(parameter == "BETA"){
		para.push_back('b');
		cerr << para.size() << ". vector variable is beta (b) is set to ";
		showVector(paravec);
		return;
	}
	if(parameter == "DEMEsize"){
		para.push_back('D');
		cerr << para.size() << ". vector variable is deme size (D) is set to ";
		showVector(paravec);
		return;
	}
	cerr << "Warning: Unknown parameter variable " << parameter << endl;
	return;
}

int setParameters(Universe* World, vector<double>& PARAvec1,vector<double>& PARAvec2,vector<double>& PARAvec3,vector<char>& PARAnames){
	int switcher;
	double value;
	vector<double> paravec;
	string line, parameter, number;
	ifstream myfile ("setting.txt");
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			if(line.empty()){
				continue;
			}
			switcher = 1; //dafualt state of switcher - reading parameters name
			for(unsigned int i = 0;i < line.size();i++){
				if(line[i] == '#'){ // # is comment symbol
					break;
				}
				if(line[i] == '='){
					switcher = 2; // symbol = switches to reading values
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
				if(switcher == 1){
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
						NAMEofOUTPUTfile = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,5) == "WORLD"){
						myfile.close();
						if(PROBABILITYmap.empty()){
							probMAPslave("PROBABILITYmap",paravec);
						}
						return worldSlave(line, World);
					} else {
// 					value
						value = stod(number);
						parameterSlave(parameter,value);
					}
				} else {
// 				vector
					switch (PARAnames.size()) {
					case 0:
						parameterSlave(parameter,paravec,PARAvec1,PARAnames);
						break;
					case 1:
						parameterSlave(parameter,paravec,PARAvec2,PARAnames);
						break;
					case 2:
						parameterSlave(parameter,paravec,PARAvec3,PARAnames);
						break;
					default:
						cerr << "Too much vector variables. Only three are allowed." << endl;
						break;
					}
					number.clear();
					parameter.clear();
					paravec.clear();
					continue;
				}
			}
			number.clear();
			parameter.clear();
		}
		myfile.close();
	} else {
		cerr << "Error: Cannot open file 'setting.txt'!\n"; return 1;
		return 1;
	}
	
	return 0;
}

int worldSlave(string line, Universe* World){
	int switcher = 0, n = 0;
	string type, number;
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
// 					if(type == "standard"){
// 						switcher = 12;
// 						continue;
// 					}
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
				istringstream(number) >> n;
				if(type == "HybridZone"){
					World->setHeight(n);
					if(n == 1){
						World->setDimension(1);
						World->setNumberOfEdges(2);
					} else {
						World->setDimension(2);
						World->setNumberOfEdges(4);
						World->setUDEdgesType("wrap");
					}
					World->basicUnitCreator('b', 'A');
					World->basicUnitCreator('r', 'B');
					cerr << "World is quick defined as " << n << " demes long hybrid zone." << endl;
					return 0;
				}
				if(type == "Arena"){
					World->setHeight(n);
					World->setUDEdgesType("reflexive");
					World->setLREdgesType("reflexive");
					if(n == 1){
						World->setDimension(1);
						World->setWidth(1); 
					}
					if(n % 2 == 0){
						World->basicUnitCreator('b', 'A');
					} else {
						World->basicUnitCreator('b', 'C');
					}
					for(int i=0;i < (n / 2) - 1;i++){
						World->basicUnitCreator('l', 'A');
					}
					for(int i=0;i < (n / 2);i++){
						World->basicUnitCreator('r', 'B');
					}
					cerr << "World is quick-defined as " << n << 'x' << n << " demes arena." << endl;
					return 0;
				}
				if(type == "InfInf"){
					cerr << "World is quick-defined as zero dimensional border of infinite popualtions" << endl;
					World->infCreator();
					return 0;
				}
				cerr << "Error: Unknown pre-defined world " << type << endl;
				return 1;
			}
		}
	}
	return 0;
}

int testParameters(Universe* World){
	World->set(0,"halfAhalfhetero");
	for(int i=0;i < NUMBERofGENERATIONS;i++){
		cerr << "Genertion " << i << endl; 
		cerr << "Homozygotes A: " << World->getProportionOfHomozygotes(0,'A') << endl;
		cerr << "Heterozygotes: "<< World->getProportionOfHeterozygotes(0) << endl;
		cerr << "Homozygotes B: "<< World->getProportionOfHomozygotes(0,'B') << endl;
		World->Breed(0);
	}
	
	return 0;
}

const void showVector(vector< double >& valvec){
	if(!valvec.empty()){
		cerr << "[";
		for(unsigned int j = 0;j < (valvec.size() - 1);j++){
			cerr << valvec[j] << ", ";
		}
		cerr << valvec[valvec.size()-1] << "]\n";
	} else {
		cerr << "The vector is empty\n";
	}
}

void simulate(Universe* World, int save_pos){
	clock_t t1, t2;
	clock_t t_sim1, t_sim2;
	int order = 0, check = 0, modulo = ceil((double)NUMBERofGENERATIONS / NUMBERofSAVES);
	
	cerr << "Starting world: " << endl;
	World->listOfDemes();
	t_sim1 = clock();
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World->migration();
		World->globalBreeding();
		t2=clock();
		cerr << "Generation: " << i << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
		if(((i % modulo)+1) == modulo and i != NUMBERofGENERATIONS - 1){
			order++;
			NAMEofOUTPUTfile[save_pos] = '0' + char(order);
			check = World->SaveTheUniverse("complete");
			if(check != 0){
				cerr << "Error in saving the output." << endl;
				return;
			}
		}
	}
	t_sim2 = clock();
	if(NUMBERofSAVES > 1){
		order++;
		NAMEofOUTPUTfile[save_pos] = '0' + char(order);
	}
	if(NUMBERofSAVES > 0){
		check = World->SaveTheUniverse("complete");
		if(check != 0){
			cerr << "Error in saving the output." << endl;
			return;
		}
	}
	cerr << "FINISHING SIMULATION IN " << ((float)t_sim2 - (float)t_sim1) / CLOCKS_PER_SEC << endl;
	cerr << "Ending world: " << endl;
	World->listOfDemes();
	World->summary();
}

void simulate(Universe* World){
	clock_t t1, t2, t_sim1, t_sim2;
	int check = 0;
	
	cerr << "Starting world: " << endl;
	World->listOfDemes();
	t_sim1 = clock();
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World->migration();
		World->globalBreeding();
		t2=clock();
		cerr << "Generation: " << i << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
	}
	t_sim2 = clock();
	if(NUMBERofSAVES == 1){
		check = World->SaveTheUniverse("complete");
		if(check != 0){
			cerr << "Error in saving the output." << endl;
			return;
		}
	}
	cerr << "FINISHING SIMULATION IN " << ((float)t_sim2 - (float)t_sim1) / CLOCKS_PER_SEC << endl;
	cerr << "Ending world: " << endl;
	World->listOfDemes();
	World->summary();
}