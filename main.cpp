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

static int SEEDtoRAND = 150206; //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
static int NUMBERofGENERATIONS = 500;
// #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/

using namespace std;

// list of fctions
int setParameters(Universe* World);
void parameterSlave(string parameter, double value);
int worldSlave(string line, Universe* World);
template <class T> class auto_ptr;

int main()
{
// 	this part sould not be edited anymore
	srand (SEEDtoRAND); // setting a seed
	Universe World;
	int check = setParameters(&World);
	if(check == 1){
		cout << "Exit: input file problem." << endl;
		return 1;
	}
	World.listOfDemes();
// 	this part yes
	clock_t t1,t2;
	for(int i=0; i < NUMBERofGENERATIONS;i++){
		t1=clock();
		World.migration();
		World.globalBreeding();
		t2=clock();
		cout << "Generation: " << i << " in ";
		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
	}
// 	KamilWorld.listOfDemes();
// 	char filePattern[] = "../pictures/pictXX.png";
// 	KamilWorld.plotDemesOneByOne(filePattern);

// 	vector<Chromosome> gamete1;
// 	vector<Chromosome> gamete2;
// 	Individual EddieP1('A');
// 	Individual EddieP2('B');
// 	EddieP1.makeGamete(gamete1);
// 	EddieP2.makeGamete(gamete2);
// 	Individual Eddie(gamete1,gamete2);
// 	Eddie.makeGamete(gamete1);
// 	Eddie.makeGamete(gamete2);
// 	Individual Luis(gamete1,gamete2);
// 	Luis.makeGamete(gamete1);
// 	Luis.makeGamete(gamete2);
// 	Individual Luis_jr(gamete1,gamete2);
// // 
// 	EddieP1.plotGenotype();
// 	EddieP2.plotGenotype();
// 	Eddie.plotGenotype();
// 	Luis.plotGenotype();
// 	Luis_jr.plotGenotype();
// 	cin.get();
// 	
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
					World->setUDEdgesType("wrap");
					World->basicUnitCreator('b', 'A');
					World->basicUnitCreator('r', 'B');
					cout << "World is quick defined as " << n << " demes long hybrid zone." << endl;
					return 0;
				}
				if(type == "Arena"){
					World->setHeight(n);
					World->setUDEdgesType("reflexive");
					World->setLREdgesType("reflexive");
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










