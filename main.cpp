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

static int SEEDtoRAND = 376090; //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
// #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/

using namespace std;

template <class T> class auto_ptr{
	T* ptr;
	public:
		explicit auto_ptr(T* p = 0) : ptr(p) {}
		~auto_ptr(){delete ptr;}
		T& operator*(){return *ptr;}
		T* operator->(){return ptr;}

};

void parameterSlave(string parameter, double value){
	cout << "Processing: " << parameter << " = " << value << endl;
	if(parameter == "RESOLUTION"){
		Chromosome::setResolution(int(value));
		return;
	}
	if(parameter == "NUMBERofCHROMOSOMES"){
		Individual::setNumberOfChromosomes(int(value));
		return;
	}
	if(parameter == "RECOMBINATIONrate"){
		Individual::setRECOMBINATIONrate(value);
		return;
	}
	if(parameter == "SELECTIONpressure"){
		Individual::setSELECTIONpressure(value);
		return;
	}
	if(parameter == "BETA"){
		Individual::setBETA(value);
		return;
	}
	if(parameter == "DEMEsize"){
		Deme::setDEMEsize(int(value));
		return;
	}
	cout << "Warning: unknown parameter" << parameter << endl;
	return;
}

int setParameters(){
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
					parameter.push_back(line[i]);
				} else {
					if(isdigit(line[i]) or line[i] == '.'){
						number += line[i];
					}
				}
			}
			if(!parameter.empty()){
				value = stod(number);
				parameterSlave(parameter,value);
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

int main()
{
// 	this part sould not be edited anymore
	srand (SEEDtoRAND); // setting a seed
	int check = setParameters();
	if(check == 1){
		cout << "Exit: input file problem.";
		return 1;
	}
	
// 	this part yes
	Universe KamilWorld;
// 	clock_t t1,t2;
	
// 	KamilWorld.basicUnitCreator('b', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// 	KamilWorld.basicUnitCreator('l', 'A');
// 	
// 	KamilWorld.globalBreeding();
// 	KamilWorld.listOfDemes();
// 	KamilWorld.plotOneDeme(0);
// 	for(int i=0; i < 3;i++){
// 		t1=clock();
// 		KamilWorld.migration();
// 		KamilWorld.globalBreeding();
// 		t2=clock();
// 		cout << "Generation: " << i << " in ";
// 		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
// 	}
// 	KamilWorld.listOfDemes();
// 	char filePattern[] = "../pictures/pictXX.png";
// 	KamilWorld.plotDemesOneByOne(filePattern);
// 	Chromosome::setResolution(512);
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












