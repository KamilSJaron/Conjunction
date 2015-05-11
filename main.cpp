#include <iostream> // preprocessor directive
#include <cmath>
#include <string>  // strings
#include <cstring> 
#include <map>  // associative arrays
#include <algorithm> // sort
#include <fstream>
#include <new> // alocation of memory in heap
#include <vector>
#include <iomanip>
#include <time.h>
#include <omp.h>


#include "universe.h" // Class of individuals

static int SEEDtoRAND = 1; //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
static int NUMBERofGENERATIONS = 500; // #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/
static int NUMBERofSAVES = 1;
static int DELAY = 0;

using namespace std;

// list of fctions
void parameterSlave(string parameter, double value); // sends value to corresponding parameter
void parameterSlave(char parameter, double value); // sends value to corresponding parameter
void parameterSlave(string parameter, vector<double>& valvec, vector<double>& paravec, vector<char>& para); // sets parameter as vector of values
void probMAPslave(vector<double>& velvec); // saves parameter probabiliy map and ensure correction (length, normalization)
void seleMAPslave(vector<double>& velvec); // saves parameter probabiliy map and ensure correction (length, normalization)
int worldSlave(string& line, Universe* World); // creates the world according to description (prbably should be moved inside of the Class)
int standardworldSlave(int dim, string& typeOfLRedge,int width,string& typeOfUDedge, int height, Universe* World);
int setParameters(Universe* World, vector<double>& PARAvec1,vector<double>& PARAvec2,vector<double>& PARAvec3,vector<char>& PARAnames); // reads the setting file and parse it, setting parameters through slave functions
int testParameters(Universe* World); // function for mendelian tests of parameters (recombination, selection); will be deleted to full version
void const showVector(vector<double>& valvec); // prints the vector on error stream (confirmation of parameter setting)
void simulate(Universe* World, int save_pos); // simulation and saving the output to file_save_pos.dat
void simulate(Universe* World); // simulation with one save in file.dat or without

int main()
{
// 	this part sould not be edited anymore
	Universe World;
	vector<double> PARAvector1, PARAvector2, PARAvector3;
	vector<char> PARAnames;
	
	int check = setParameters(&World, PARAvector1, PARAvector2, PARAvector3,PARAnames);
	if(check != 0){
		cerr << "Exit: input file problem." << endl;
		return 1;
	}
	
	srand (SEEDtoRAND); // setting a seed
	setPoisSeed (SEEDtoRAND+1);
	
	clock_t t_total1, t_total2;
	int run = 0;
	int pos1 = 0, pos2 = 0, pos3 = 0, save_pos = 0; // indexing variables for vector variable handling togheder with the creation of filenames
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
		if(NUMBERofSAVES > 9){
			NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_0*");
		} else {
			NAMEofOUTPUTfile = NAMEofOUTPUTfile + string("_*");
		}
		save_pos = NAMEofOUTPUTfile.find('*');
	}
	NAMEofOUTPUTfile = NAMEofOUTPUTfile + string(".dat");
	cerr << "*** OUTPUT INFO ***" << endl;
	cerr << "Name of the file: " << NAMEofOUTPUTfile << endl;
	cerr << "Type of the file: " << TYPEofOUTPUTfile << endl;
	cerr << "Number of files: " << NUMBERofSAVES << endl;
	cerr << "*******************" << endl;
// 	this moster construction handles simulation for 0 to 3 defined vector variables
	if(PARAnames.size() >= 1){
		t_total1 = clock();
		for(unsigned int j = 0;j < PARAvector1.size();j++){
			if(NUMBERofSAVES > 9){
				NAMEofOUTPUTfile[save_pos - 1] = '0';
			}
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
					if(NUMBERofSAVES > 9){
						NAMEofOUTPUTfile[save_pos - 1] = '0';
					}
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
							if(NUMBERofSAVES > 9){
								NAMEofOUTPUTfile[save_pos - 1] = '0';
							}
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
							simulate(&World,save_pos); // simulate  edit for 0 dim case
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
		simulate(&World,save_pos);  //both simulate edit for 0 dim case
	}
// 	 	char filePattern[] = "../playground/pictXX.png";
	
	World.clear();
	return 0;
}



void parameterSlave(string parameter, double value){
	if(parameter == "LOCI"){
		Chromosome::setResolution(int(value));
		cerr << "Setting parameter LOCI to: " << value << endl;
		return;
	}
	if(parameter == "PROBABILITYmap"){
		cerr << "Warning: parameter PROBABILITYmap is expected in form..." << endl 
		<< "      [ v1, v2, v3, ... vn]" << endl 
		<< "      where n is LOCI + 1 and sumation of vi is 1 for i = 1 ... n" << endl;
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
	if(parameter == "DELAY"){
		DELAY = (int(value));
		cerr << "Setting parameter DELAY to: " << value << endl;
		return;
	}
	cerr << "Warning: unknown parameter: " << parameter << endl;
	return;
}

void parameterSlave(char parameter, double value){
	if(parameter == 'L'){
		Chromosome::setResolution(int(value));
		cerr << "Setting parameter LOCI to: " << value << endl;
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


void probMAPslave(vector<double>& velvec){
		if(!velvec.empty()){
			PROBABILITYmap = velvec;
			double sum_of_elems=0;
			if(PROBABILITYmap.size() != unsigned(LOCI + 1)){
				cerr << "Warning: Length of probability map do not match the number of Loci (parameter LOCI)" << endl;
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
			if(LOCI < 20){
				for(int i = 0; i < (LOCI - 1); i++){
					cerr << (1. / double(LOCI - 1)) << ", " ;
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
		return;
}

void seleMAPslave(vector<double>& velvec){
		SELECTIONmap = velvec;
		if(SELECTIONmap.size() != unsigned(LOCI)){
			cerr << "Warning: Length of selection map do not match the number of Loci (parameter LOCI)" << endl;
			SELECTIONmap.clear();
		}
		cerr << "Setting parameter SELECTIONmap to: [";
		for(unsigned int i = 0;i < SELECTIONmap.size()-1;i++){
			cerr << SELECTIONmap[i] << ", ";
		}
		cerr << SELECTIONmap[SELECTIONmap.size()-1] << "]" << endl;
		return;
}

void parameterSlave(string parameter, vector<double>& valvec, vector<double>& paravec, vector<char>& para){
	paravec = valvec;
	if(parameter == "PROBABILITYmap"){
		probMAPslave(valvec);
		return;
	}
	if(parameter == "SELECTIONmap"){
		seleMAPslave(valvec);
		return;
	}
	if(parameter == "LOCI"){
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
		Deme::setDEMEsize(int(paravec[0]));
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
				if(parameter.substr(0,16) == "TYPEofOUTPUTfile"){
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
			
			cout << "parameter: " << parameter << " to " << value << endl;
			
			if(!parameter.empty()){
				if(paravec.empty()){
//				string or value
					if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
						NAMEofOUTPUTfile = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,16) == "TYPEofOUTPUTfile"){
						TYPEofOUTPUTfile = number;
						number.clear();
						parameter.clear();
						continue;
					}
					if(parameter.substr(0,5) == "WORLD"){
						myfile.close();
						if(PROBABILITYmap.empty()){
							probMAPslave(paravec);
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
	if(World->empty()){
		cerr << "Error: the world was not defined" << endl;
		return -1;
	}
	
	return 0;
}

int worldSlave(string& line, Universe* World){
	int switcher = 0, n = 0;
	double arenan = 0;
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
					World->setHeight(n);
					if(n == 1){
						World->setDimension(1);
						World->setNumberOfEdges(2);
					} else {
						World->setDimension(2);
						World->setNumberOfEdges(4);
						World->setUDEdgesType("wrapping");
					}
					World->setLREdgesType("extending");
					World->basicUnitCreator('b', 'A');
					World->basicUnitCreator('r', 'B');
					cerr << "World is quick defined as " << n << " demes long hybrid zone." << endl;
					return 0;
				}
				if(type == "Arena"){
					arenan = stod( number );
					n = stoi( number );
					World->setHeight(n);
					World->setUDEdgesType("reflexive");
					if(n > 2){
						World->setLREdgesType("extending");
					} else {
						World->setLREdgesType("reflexive");
					}
					
					if(n == 1){
						World->setDimension(1);
						World->setWidth(1);
						World->setNumberOfEdges(2);
						World->basicUnitCreator('b', 'C');
						return 0;
					} else {
						World->setNumberOfEdges(4);
						World->setDimension(2);
					}
					
					if(n % 2 == 0){
						World->basicUnitCreator('b', 'A');
					} else {
						World->basicUnitCreator('b', 'C');
					}
					
					for(int i=0;i < (ceil(arenan / 2) - 2);i++){
						World->basicUnitCreator('l', 'A');
					}
					for(int i=0;i < floor(arenan / 2) - 1;i++){
						World->basicUnitCreator('r', 'B');
					}
					
					World->setUDEdgesType("reflexive");
					World->setLREdgesType("reflexive");
					if(n > 2){
						World->basicUnitCreator('l', 'A');
					}
					World->basicUnitCreator('r', 'B');
					
					cerr << "World is quick-defined as " << n << 'x' << n << " demes arena." << endl;
					return 0;
				}
				if(type == "InfInf"){
					cerr << "World is quick-defined as zero dimensional border of infinite popualtions" << endl;
					World->setDimension(0);
					return 8;
				}
				if(type == "LowMigrationBazykin"){
					World->setHeight(1);
					World->setDimension(2);
					World->setNumberOfEdges(4);
					World->setUDEdgesType("reflexive");
					World->setLREdgesType("extending");
					World->basicUnitCreator('b', 'A');
					World->basicUnitCreator('r', 'B');
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
					} else {
						stdHeight = stoi(number);
						return standardworldSlave(stdDim,stdLR,stdWidth,stdUD,stdHeight,World);
					}
					switcher = 13;
				}
			}
		}
	}
	
	return -1;
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
	int order = 0, check = 0, modulo = ceil((double)(NUMBERofGENERATIONS-DELAY-1) / NUMBERofSAVES);
	
	cerr << "Starting I world: " << endl;
	World->listOfDemes();
	t_sim1 = clock();
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World->migration();
		World->globalBreeding();
		t2=clock();
		cerr << "Generation: " << i + 1 << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
		if(((i % modulo)+1) == modulo and i != NUMBERofGENERATIONS - 1 and i >= DELAY){
			order++;
			if(order >= 10){
				NAMEofOUTPUTfile[save_pos-1] = '0' + char(order / 10 % 10);
				NAMEofOUTPUTfile[save_pos] = '0' + char(order % 10);
			} else {
				NAMEofOUTPUTfile[save_pos] = '0' + char(order);
			}
			check = World->SaveTheUniverse(TYPEofOUTPUTfile);
			if(check != 0){
				cerr << "Error in saving the output." << endl;
				return;
			}
		}
	}
	t_sim2 = clock();
	if(NUMBERofSAVES > 1){
		order++;
		if(order >= 10){
			NAMEofOUTPUTfile[save_pos-1] = '0' + char(order / 10 % 10);
			NAMEofOUTPUTfile[save_pos] = '0' + char(order % 10);
		} else {
			NAMEofOUTPUTfile[save_pos] = '0' + char(order);
		}
	}
	if(NUMBERofSAVES > 0){
		check = World->SaveTheUniverse(TYPEofOUTPUTfile);
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
	clock_t t1, t2;
	clock_t t_sim1, t_sim2;
	int check = 0;
	
	cerr << "Starting II world: " << endl;
	World->listOfDemes();
// 	World->summary();
	t_sim1 = clock();
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World->migration();
		World->globalBreeding();
		t2=clock();
		cerr << "Generation: " << i + 1 << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
	}
	t_sim2 = clock();
	if(NUMBERofSAVES == 1){
		check = World->SaveTheUniverse(TYPEofOUTPUTfile);
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

int standardworldSlave(int dim, string& typeOfLRedge, int width, string& typeOfUDedge, int height, Universe* World){
	if(dim < 0 or dim > 2){
		cerr << "ERROR: Dimension can not be higher than 2 or negative." << endl;
		return -1;
	}
	if(dim == 0){
		World->setDimension(0);
		return 8;
	}
	if(dim == 1){
		World->setDimension(1);
		World->setNumberOfEdges(2);
		World->setHeight(1);
	}
	
	if(dim == 2){
		World->setDimension(2);
		World->setNumberOfEdges(4);
		World->setHeight(height);
		World->setUDEdgesType(typeOfUDedge);
	}
	World->setWidth(width);
	
	if(width == 1){
		World->setLREdgesType(typeOfLRedge);
		World->basicUnitCreator('b', 'C');
		return 0;
	}
	if(width == 2){
		World->setLREdgesType(typeOfLRedge);
		World->basicUnitCreator('b', 'A');
		World->basicUnitCreator('r', 'B');
		return 0;
	}
	
	double midpoint = double(width + 1) / 2;
	World->setLREdgesType(typeOfLRedge);
	World->basicUnitCreator('b', 'A');

	for(double i = 2; i < width; i++){
		if(i < midpoint){
			World->basicUnitCreator('r', 'A');
		} else {
			World->basicUnitCreator('r', 'B');
		}
	}
	World->setLREdgesType(typeOfLRedge);
	World->basicUnitCreator('r', 'B');
	return 0;
}