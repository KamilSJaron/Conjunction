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

static int SEEDtoRAND; //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
static int NUMBERofGENERATIONS; // #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/
static int TEST = 0; // temp

using namespace std;

// list of fctions
void parameterSlave(string parameter, double value);
int worldSlave(string line, Universe* World);
int setParameters(Universe* World);
int testParameters(Universe* World);

int main()
{
// 	this part sould not be edited anymore
	Universe World;
	int check = setParameters(&World);
	if(check == 1){
		cerr << "Exit: input file problem." << endl;
		return 1;
	}
	srand (SEEDtoRAND); // setting a seed

// 	ready for deletion, when more testing wont be needed
	if(TEST != 0){
		testParameters(&World);
	} else {
// 	this part yes
	clock_t t1,t2,t_sim1,t_sim2;
<<<<<<< HEAD
// 	int modulo = max(int(round(double(NUMBERofGENERATIONS) / 8)),5), j = 1;
	vector<double> Svector{0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95};
	NAMEofOUTPUTfile = "bazi_s_gradient_0X.dat";
	int Xpos = NAMEofOUTPUTfile.find('X'), ten_stop = 0;
	
	for(unsigned int j = 0;j < Svector.size();j++){
		cerr << "RUN: " << j+1 << endl;
		cerr << "SELECTIONpressure: " << Svector[j] << endl;
		parameterSlave("SELECTIONpressure", Svector[j]);
		if(j < unsigned(ten_stop + 10)){
			NAMEofOUTPUTfile[Xpos] = char(j + '0' - ten_stop);
		} else {
			ten_stop += 10;
			NAMEofOUTPUTfile[Xpos-1] = char(NAMEofOUTPUTfile[Xpos-1] + 1);
			NAMEofOUTPUTfile[Xpos] = char(j + '0' - ten_stop);
		}
		cerr << NAMEofOUTPUTfile << endl;
		World.restart();
		cerr << "Starting world: " << endl;
		World.listOfDemes();
		t_sim1 = clock();
		
		for(int i=0; i < NUMBERofGENERATIONS;i++){
			t1=clock();
			World.migration();
			World.globalBreeding();
			t2=clock();
			cerr << "Generation: " << i << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
	// 		if(((i % modulo)-9) == 0){
	// 			check = World.SaveTheUniverse(j);
	// 			if(check != 0){
	// 				cerr << "Error in saving the output." << endl;
	// 				return 1;
	// 			}
	// 			j++;
	// 		}
			
		}
// 		World.SaveTheUniverse("average");
		t_sim2 = clock();
		cerr << "FINISHING SIMULATION " << j+1 << " IN " << ((float)t_sim2 - (float)t_sim1) / CLOCKS_PER_SEC << endl;
		cerr << "Ending world: " << endl;
		World.summary();
=======
	int modulo = max(int(round(double(NUMBERofGENERATIONS) / 8)),5), j = 1;
	
	t_sim1 = clock();
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World.migration();
		World.globalBreeding();
		t2=clock();
		cerr << "Generation: " << i << " done in " << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
		if(((i % modulo)-9) == 0){
			check = World.SaveTheUniverse(j);
			if(check != 0){
				cerr << "Error in saving the output." << endl;
				return 1;
			}
			j++;
		}
	}
	World.SaveTheUniverse(9);
	t_sim2 = clock();
	cerr << "FINISHING SIMULATION IN " << ((float)t_sim2 - (float)t_sim1) / CLOCKS_PER_SEC << endl;
	cerr << "Ending world: " << endl;
	World.listOfDemes();
>>>>>>> 050aeddbe0bf09684242a7e78f2b99521e2f56d1

	//  	char filePattern[] = "../playground/pictXX.png";
		}
	}
	return 0;
}



void parameterSlave(string parameter, double value){
	if(parameter == "RESOLUTION"){
		Chromosome::setResolution(int(value));
		cerr << "Setting parameter RESOLUTION to: " << value << endl;
		return;
	}
	if(parameter == "PROBABILITYmap"){
		double sum_of_elems=0;
		
		if(!PROBABILITYmap.empty()){
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
			for(int i = 0; i < (RESOLUTION - 1); i++){
				cerr << (1. / double(RESOLUTION - 1)) << ", " ;
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
	if(parameter == "TEST"){
		TEST = value;
		cerr << "Setting parameter TEST to: " << value << endl;
	return;
	}
	cerr << "Warning: unknown parameter: " << parameter << endl;
	return;
}

int setParameters(Universe* World){
	int switcher;
	double value;
	string line, parameter, number;
	ifstream myfile ("setting.txt");
	if (myfile.is_open()){
		while ( getline (myfile,line) ){
			if(line.empty()){
				continue;
			}
			switcher = 1; //dafualt state of switcher - reading parameters name
			for(unsigned int i = 0;i < line.size();i++){
				if(line[i] == '#'){
					break;
				}
				if(line[i] == '='){
					switcher = 2; // symbol == switches to reading values
					continue;
				}
				if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
					number += line[i];
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
					if((line[i] == ']' or line[i] == ',' or line[i] == ';') and parameter == "PROBABILITYmap"){
						PROBABILITYmap.push_back(stod(number));
						number.clear();
					}
				}
			}
			if(!parameter.empty()){
				if(parameter == "PROBABILITYmap"){
					parameterSlave(parameter,0);
					number.clear();
					parameter.clear();
					continue;
				}
				if(parameter.substr(0,16) == "NAMEofOUTPUTfile"){
					NAMEofOUTPUTfile = number;
					number.clear();
					parameter.clear();
					continue;
				}
				if(parameter.substr(0,5) == "WORLD"){
					myfile.close();
					if(PROBABILITYmap.empty()){
						parameterSlave("PROBABILITYmap",0);
					}
					return worldSlave(line, World);
				} else {
					value = stod(number);
					parameterSlave(parameter,value);
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







