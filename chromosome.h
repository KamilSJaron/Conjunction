using namespace std;

static int LOCI = 1; /*LOCI is Ls in Baird 94; it has to work with one loci case*/
static vector<double> PROBABILITYmap;

//struct and functions
struct arrangeClass {
  bool operator() (int i,int j) { return (i<j);}
} arrangeObject;

int tossAcoin (){
  return rand() % 2;
}

int recombPosition (){
	int roll = rand();
	if(PROBABILITYmap.empty()){
		if(LOCI == 1){
			return 0;
		} else {
			return (roll % (LOCI-1)) + 1;
		}
	}
	for(unsigned int i=0;i < PROBABILITYmap.size();i++){
		if(PROBABILITYmap[i] < roll){
			return i;
		}
	}
	return PROBABILITYmap.size();
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
		Chromosome(char starting_char){chromosome[0] = starting_char;}; /*inic*/
		Chromosome(map <int, char> input_chrom){chromosome = input_chrom;}; /*inic*/
		~Chromosome(){chromosome.clear();}; /*destr*/

/* PLOTTING METHODS */
		void showChromosome() const; /*writes all junctions*/
		void viewChromosome() const; /*ASCII visualisation*/
		
/* COMPUTING METHODS */
		bool Acheck() const; /*returns 1 if there are only A, 0 in other case*/
		bool Bcheck() const; /*returns 1 if there are only B, 0 in other case*/
		int countB() const; /*returns number of B loci in chromosome*/
		int getNumberOfJunctions() const; /*returns number of Junctions in chromosome*/
		void getSizesOfBBlocks(vector<int>& sizes); /*fills vector of ints by sizes of B blocks in chromosome*/
		void makeRecombinant(Chromosome& chromNew,int numberOfChaisma);
// 		bool CorCheck();
		
/* COMUNICATION METHODS */
		static void setResolution(int res){LOCI = res;};
		static int getResolution(){return LOCI;}; /* return resolution of the chromosome */
		void clear(){chromosome.clear();}; /* method for deleting chromosome */
		char read(int i){return chromosome[i];}; /* returns value of junction */
		void write(int i, char l); /* makes new junction*/
		map<int, char>::iterator begin(){return chromosome.begin();};
		map<int, char>::iterator end(){return chromosome.end();};
		map<int, char>::iterator find(int i){return chromosome.find(i);};
		
  protected:
		map <int, char> chromosome;
};

void Chromosome::showChromosome() const{
	for(map<int, char>::const_iterator i=chromosome.begin(); i!=chromosome.end(); ++i){
		cout << i->first << ':' << i->second << '\n';
	}
}

void Chromosome::viewChromosome() const{
	vector<int> vals;
	vector<char> seq;
	unsigned int index = 0;
	int letter = 0;
	if(LOCI < 101){
		for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
			vals.push_back(pos->first / (double)LOCI);
			seq.push_back(pos->second);
		}
		if(vals.empty()){
			cerr << "ERROR: non initiated chromosome is impossible to plot" << endl;
			return;
		}
		for(letter = 0; letter < LOCI; letter++){
			if(vals[index] == letter && index < vals.size()){
				index++;
			}
			cout << seq[index-1];
		}
		cout << endl;
	} else {
		for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
			vals.push_back(pos->first / ((double)LOCI / 100));
			seq.push_back(pos->second);
		}
		if(vals.empty()){
			cerr << "ERROR: non initiated chromosome is impossible to plot" << endl;
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

bool Chromosome::Acheck() const{
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'A'){
			return 0;
		}
	}
	return 1;
}

bool Chromosome::Bcheck() const{
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'B'){
			return 0;
		}
	}
	return 1;
}

int Chromosome::countB() const{
	int sum = 0;
	char last_seq = 'A';
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sum += (pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sum += (LOCI - last_val);
	}
	return sum;
}

int Chromosome::getNumberOfJunctions() const{
	return chromosome.size() - 1;
}

void Chromosome::getSizesOfBBlocks(vector<int>& sizes){
	char last_seq = 'A';
	int last_val = 0;
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sizes.push_back(pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sizes.push_back(LOCI - last_val);
	}
}

void Chromosome::write(int i, char l){
	chromosome[i] = l;
}

void Chromosome::makeRecombinant(Chromosome& chromNew, int numberOfChaisma){
// 	starts_by 0 home, 1 alien
	int starts_by = tossAcoin();
	vector<int> recombination;
	chromNew.clear();
	chromNew.write(0,'A');
	
// 	this condition can be deleted if I will handle the numberOfChaisma=0 on some upper level
	if(numberOfChaisma == 0){
		if(starts_by == 0){
			return;
		}
		chromNew = Chromosome(chromosome);
		return;
	}
	
	int index, lastposition = 0;
	
	for(index=0;index<numberOfChaisma;index++){
		recombination.push_back(recombPosition());
	}
	
	sort(recombination.begin(), recombination.end(), arrangeObject);
	index = 0;
	
	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		while(recombination[index] <= pos->first && index < numberOfChaisma){
			if (chromosome[lastposition] != 'A'){
				if (starts_by == 1){
					chromNew.write(recombination[index],'A');
				} else {
					chromNew.write(recombination[index],'B');
				}
			}
			starts_by = abs(starts_by - 1);
			index++;
		}
		if(starts_by == 1){
			chromNew.write(pos->first,pos->second);
		}
		lastposition = pos->first;
	}
	
	while(index < numberOfChaisma){
			if (chromosome[lastposition] != 'A'){
				if (starts_by == 1){
					chromNew.write(recombination[index],'A');
				} else {
					chromNew.write(recombination[index],'B');
				}
			}
			starts_by = abs(starts_by - 1);
			index++;
	}
	return;
}