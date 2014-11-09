#include <map>
#include <random>
const int RESOLUTION = 101; /*RESOLUTION is Ls in Baird 94; it has to work with one loci case*/
// v0.141107

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
 * @brief CHROMOSOME is the lowest unit
 * 
 * @author Kamil S Jaron
 */
class Chromosome
{
	public:
		Chromosome(){}; /*inic*/
		Chromosome(char starting_char){chromosome[1] = starting_char;}; /*inic*/
		Chromosome(map <int, char> input_chrom){chromosome = input_chrom;}; /*inic*/
		~Chromosome(){chromosome.clear();}; /*destr*/
		void showChromosome(); /*writes all junctions*/
		/**
		 * @brief This method writes down all junctions of the chromosome
		 * 
		 * @return void
		 */
		void plotChromosome(); /*ASCII visualisation*/
		bool Acheck(); /*returns 1 if there are only A, 0 in other case*/
		int countB(); /*returns number of B loci in chromosome*/
		vector<int> getSizesOfBBlocks(); /*returns vector of sizes of B blocks in chromosome*/
		int getNumberOfJunctions(); /*returns number of Junctions in chromosome*/
		int getResolution(){return RESOLUTION;}; /* return resolution of the chromosome */
		void clear(){chromosome.clear();};
		char read(int i){return chromosome[i];};
		void write(int i, char l){chromosome[i] = l;};
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

void Chromosome::plotChromosome(){
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
		if(pos->second == 'B'){
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

vector<int> Chromosome::getSizesOfBBlocks(){
	vector<int> sizes;
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
	return sizes;
}

