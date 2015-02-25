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

using namespace std;

// list of fctions
void parameterSlave(string parameter, double value);
int worldSlave(string line, Universe* World);
int setParameters(Universe* World);
int testParameters(Universe* World);

template <class T> class auto_ptr;

int main()
{
// 	this part sould not be edited anymore
	Universe World;
	int check = setParameters(&World);
	if(check == 1){
		cout << "Exit: input file problem." << endl;
		return 1;
	}
	srand (SEEDtoRAND); // setting a seed
	cout << "Starting world: " << endl;
// 	World.listOfDemes();
// 	World.listOfParameters();
	testParameters(&World);
// 	this part yes
// 	clock_t t1,t2,t_sim1,t_sim2;
// 	int modulo = max(int(round(double(NUMBERofGENERATIONS) / 9)),5), j = 1;
// 	
// 	t_sim1 = clock();
// 	for(int i=0; i < NUMBERofGENERATIONS;i++){
// 		t1=clock();
// 		cout << "Generation: " << i;
// 		World.migration();
// // 		cout << "Migration: done\nBreeding:procesing\n" << endl;
// 		World.globalBreeding();
// // 		cout << "Breeding: done" << endl;
// 		t2=clock();
// 		cout  << " done in ";
// 		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
// 		if(((i % modulo)-9) == 0){
// 			check = World.SaveTheUniverse(j);
// 			if(check != 0){
// 				cout << "Error in saving the output." << endl;
// 				return 1;
// 			}
// 			j++;
// 		}
// 	}
// 	World.SaveTheUniverse(0);
// 	t_sim2 = clock();
// 	cout << "FINISHING SIMULATION IN " << ((float)t_sim2 - (float)t_sim1) / CLOCKS_PER_SEC << endl;
// 	cout << "Ending world: " << endl;
// 	World.listOfDemes();

//  	char filePattern[] = "../playground/pictXX.png";
//  	World.plotDemesOneByOne(filePattern);

	return 0;
}


template <class T> class auto_ptr{
	T* ptr;
	public:
		explicit auto_ptr(T* p = 0) : ptr(p) {}
		~auto_ptr(){delete ptr;}
		T& operator*(){return *ptr;}
		T* operator->(){return ptr;}

};

void parameterSlave(string parameter, double value){
	if(parameter == "RESOLUTION"){
		Chromosome::setResolution(int(value));
		cout << "Setting parameter RESOLUTION to: " << value << endl;
		return;
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		Individual::setNumberOfChromosomes(int(value));
		cout << "Setting parameter NUMBERofCHROMOSOMES to: " << value << endl;
		return;
	}
	if(parameter == "RECOMBINATIONrate"){
		Individual::setRECOMBINATIONrate(value);
		cout << "Setting parameter RECOMBINATIONrate to: " << value << endl;
		return;
	}
	if(parameter == "SELECTIONpressure"){
		Individual::setSELECTIONpressure(value);
		cout << "Setting parameter SELECTIONpressure to: " << value << endl;
		return;
	}
	if(parameter == "BETA"){
		Individual::setBETA(value);
		cout << "Setting parameter BETA to: " << value << endl;
		return;
	}
	if(parameter == "DEMEsize"){
		Deme::setDEMEsize(int(value));
		cout << "Setting parameter DEMEsize to: " << value << endl;
		return;
	}
	if(parameter == "SEED"){
		SEEDtoRAND = value;
		cout << "Setting parameter SEED to: " << value << endl;
		return;
	}
	if(parameter == "NUMBERofGENERATIONS"){
		NUMBERofGENERATIONS = value;
		cout << "Setting parameter NUMBERofGENERATIONS to: " << value << endl;
		return;
	}
	cout << "Warning: unknown parameter: " << parameter << endl;
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
			switcher = 1;
			for(unsigned int i = 0;i < line.size();i++){
				if(line[i] == '#'){
					break;
				}
				if(line[i] == '='){
					switcher = 2;
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
				}
			}
			if(!parameter.empty()){
				if(parameter.substr(0,5) == "WORLD"){
					myfile.close();
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
		cout << "Error: Cannot open file 'setting.txt'!\n"; return 1;
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
					cout << "ERROR: the " << type << " world description is not defined (yet)." << endl;
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
					cout << "World is quick defined as " << n << " demes long hybrid zone." << endl;
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
					cout << "World is quick-defined as " << n << 'x' << n << " demes arena." << endl;
					return 0;
				}
				cout << "Error: Unknown pre-defined world " << type << endl;
				return 1;
			}
		}
	}
	return 0;
}

int testParameters(Universe* World){
	World->set(0,"halfAhalfhetero");
	for(int i=0;i < NUMBERofGENERATIONS;i++){
		cout << "Genertion " << i << endl; 
		cout << "Homozygotes A: " << World->getProportionOfHomozygotes(0,'A') << endl;
		cout << "Heterozygotes: "<< World->getProportionOfHeterozygotes(0) << endl;
		cout << "Homozygotes B: "<< World->getProportionOfHomozygotes(0,'B') << endl;
		World->Breed(0);
	}
	
	return 0;
}







