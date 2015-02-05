#include <map>
#include <random>
static int RESOLUTION = 101; /*RESOLUTION is Ls in Baird 94; it has to work with one loci case*/

using namespace std;

//struct and functions
struct arrangeClass {
  bool operator() (int i,int j) { return (i<j);}
} arrangeObject;

int tossAcoin (){
  return rand() % 2;
}

// int recombPosition (){
//   return (rand() % (RESOLUTION-1))+1;
// }

int recombPosition (){
  return (rand() % RESOLUTION);
}

// fctions
int getResolution(){
	return RESOLUTION;
}

/**
 * @brief CHROMOSOME is class consiting of map of junctions on chromosome.
 * First junciton in every chromosome is 1:char where char is letter for begining of the chromosome.
 * The class functions are sorted to blocks by purpose.
 * @author Kamil S Jaron
 */

class Chromosome
{
	public:
/* INITIATION */
		Chromosome(){};
		Chromosome(char starting_char){chromosome[1] = starting_char;}; /*inic*/
		Chromosome(map <int, char> input_chrom){chromosome = input_chrom;}; /*inic*/
		~Chromosome(){chromosome.clear();}; /*destr*/

/* PLOTTING METHODS */
		void showChromosome(); /*writes all junctions*/
		void viewChromosome(); /*ASCII visualisation*/
		
/* COMPUTING METHODS */
		bool Acheck(); /*returns 1 if there are only A, 0 in other case*/
		bool Bcheck(); /*returns 1 if there are only B, 0 in other case*/
		int countB(); /*returns number of B loci in chromosome*/
		int getNumberOfJunctions(); /*returns number of Junctions in chromosome*/
		void getSizesOfBBlocks(vector<int>& sizes); /*fills vector of ints by sizes of B blocks in chromosome*/
		
/* COMUNICATION METHODS */
		static void setResolution(int res){RESOLUTION = res;};
		static int getResolution(){return RESOLUTION;}; /* return resolution of the chromosome */
		void clear(){chromosome.clear();}; /* method for deleting chromosome */
		char read(int i){return chromosome[i];}; /* returns value of junction */
		void write(int i, char l){chromosome[i] = l;}; /* makes new junction*/
		map<int, char>::iterator begin(){return chromosome.begin();};
		map<int, char>::iterator end(){return chromosome.end();};
		map<int, char>::iterator find(int i){return chromosome.find(i);};
		
  private:
		map <int, char> chromosome;
};

void Chromosome::showChromosome(){
	for(auto i=chromosome.begin(); i!=chromosome.end(); ++i){
		cout << i->first << ':' << i->second << '\n';
	}
}

void Chromosome::viewChromosome(){
	if(RESOLUTION < 101){
		cout << "Number of loci is too small for plotting" << endl;
	} else {
	vector<int> vals;
	vector<char> seq;
	unsigned int index = 0;
	int letter = 0;
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		vals.push_back(pos->first / ((double)RESOLUTION / 100));
		seq.push_back(pos->second);
	}
	if(vals.empty()){
		cout << "ERROR: non initiated chromosome is impossible to plot" << endl;
		return;
	}
	for(letter = 0; letter < 100; letter++){
		if(vals[index] == letter && index < vals.size()){
			index++;
		}
		cout << seq[index-1];
	}
	cout << endl;
	}
}

bool Chromosome::Acheck(){
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'A'){
			return 0;
		}
	}
	return 1;
}

bool Chromosome::Bcheck(){
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'B'){
			return 0;
		}
	}
	return 1;
}

int Chromosome::countB(){
	int sum = 0;
	char last_seq = 'A';
	int last_val = 0;
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sum += (pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sum += (RESOLUTION - last_val) + 1;
	}
	return sum;
}

int Chromosome::getNumberOfJunctions(){
	return chromosome.size() - 1;
}

void Chromosome::getSizesOfBBlocks(vector<int>& sizes){
	sizes.clear();
	char last_seq = 'A';
	int last_val = 0;
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sizes.push_back(pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sizes.push_back((RESOLUTION - last_val) + 1);
	}
}

