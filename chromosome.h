using namespace std;

static int RESOLUTION = 1; /*RESOLUTION is Ls in Baird 94; it has to work with one loci case*/
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
		if(RESOLUTION == 1){
			return 0;
		} else {
			return (roll % (RESOLUTION-1)) + 1;
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
		bool CorCheck();
		
/* COMUNICATION METHODS */
		static void setResolution(int res){RESOLUTION = res;};
		static int getResolution(){return RESOLUTION;}; /* return resolution of the chromosome */
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
	for(auto i=chromosome.begin(); i!=chromosome.end(); ++i){
		cout << i->first << ':' << i->second << '\n';
	}
}

void Chromosome::viewChromosome() const{
	if(RESOLUTION < 101){
		cerr << "Number of loci is too small for plotting" << endl;
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
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(pos->second != 'A'){
			return 0;
		}
	}
	return 1;
}

bool Chromosome::Bcheck() const{
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
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
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_seq == 'B'){
			sum += (pos->first - last_val);
		}
		last_seq = pos->second;
		last_val = pos->first;
	}
	if(last_seq == 'B'){
			sum += (RESOLUTION - last_val);
	}
	return sum;
}

int Chromosome::getNumberOfJunctions() const{
	return chromosome.size() - 1;
}

void Chromosome::getSizesOfBBlocks(vector<int>& sizes){
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

void Chromosome::write(int i, char l){
// 	if(i >= RESOLUTION){
// 		cerr << "WARNING: trying to make junction out of RESOLUTION of chromosome." << endl;
// 		return;
// 	}
// 	if(l != 'A' and l != 'B'){
// 		cerr << "WARNING: trying to write junction followed by genetic information of unknown origin." << endl;
// 		return;
// 	}
// + podminka, ze spoj jeste neexistuje	
// + podminka, ze minuly spoj je jiny
	chromosome[i] = l;
}

void Chromosome::makeRecombinant(Chromosome& chromNew, int numberOfChaisma){
// 	starts_by 0 home, 1 alien
	int starts_by = tossAcoin();
	vector<int> recombination;
	chromNew.clear();
	chromNew.write(0,'A');
// 	cout << "Toss: " << starts_by << " numberOfChaisma: " << numberOfChaisma;

	
// 	this condition can be deleted if I will handle the numberOfChaisma=0 on some upper level
	if(numberOfChaisma == 0){
// 		cout << endl;
		if(starts_by == 0){
			return;
		}
		chromNew = Chromosome(chromosome);
		return;
	}
	
	
	int index, lastposition = 0;
	  //inicialization, in case of starting B it will be overwrited
	
// 	cout << " Rec. positions:";
	for(index=0;index<numberOfChaisma;index++){ //this loop is not needed to be ordered
		recombination.push_back(recombPosition());
// 		cout << recombination[index] << ", ";
	}
// 	cout << endl;
	
	sort(recombination.begin(), recombination.end(), arrangeObject);
	index = 0;
	
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
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

bool Chromosome::CorCheck(){
	char last_char = 'x';
	for(auto pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
		if(last_char == pos->second){
			return 1;
		}
		last_char = pos->second;
	}
	return 0;
}
