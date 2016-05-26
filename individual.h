#include "chromosome.h"
static int NUMBERofCHROMOSOMES = 1;
static double RECOMBINATIONrate = 1;
static double SELECTIONpressure = 0.5;
static double BETA = 1;
static vector<double> SELECTIONmap;

// fctions
int getNumberOfChromosomes(){
	return NUMBERofCHROMOSOMES;
}

// uniform
double uniform(){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	return double(x % detailness) / detailness;
}

int getChiasma(){
	int result = 0;
	double q = exp(-RECOMBINATIONrate);
	double p = q;
	double roll = uniform();
	while(roll > q){
		result++;
		p = p * RECOMBINATIONrate / result;
		q = q + p;
	}
	return result;
}

class Individual  
{
	public:
/* DECLARATION */
		Individual();
		Individual(char origin); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Individual(vector<Chromosome>& gamete1, vector<Chromosome>& gamete2); /*init Individual by gametes on imput */
		~Individual(){genome[0].clear();genome[1].clear();}; /* destructor */
		
/* COMPUTIONG METHODS */
		void makeGamete(vector<Chromosome>& gamete); //
		void replace_chromozome(int set, int position,map <int, char> input_chrom);
		double getFitness();
		int getBcount() const;
		double getBprop() const;
		double getHetProp();
		bool Acheck() const;
		bool Bcheck() const;
		map<int, char>::iterator getChromosomeBegining(int set, int chrom);
		int getNumberOfJunctions(int set, int chrom);
		int getNumberOfJunctions();
		void getSizesOfBBlocks(std::vector<int>& sizes);
		void getSizesOfABlocks(std::vector<int>& sizes);

		
/* ASCII PLOTTING METHODS */
		void readGenotype();
		void viewGenotype();
		
/* COMUNICATION */
		static void setNumberOfChromosomes(int ch){NUMBERofCHROMOSOMES = ch;};
		static void setLAMBDA(double Rr){RECOMBINATIONrate = Rr;};
		static void setSELECTIONpressure(double Sp){SELECTIONpressure = Sp;};
		static void setBETA(double beta){BETA = beta;};
	
	private:
		vector<Chromosome> genome[2];
		int getOneChromeHetero(bool write, map<int, char>::const_iterator& pos, int chromosome, int last_pos);
};

Individual::Individual(){
	genome[0].reserve(NUMBERofCHROMOSOMES);
	genome[1].reserve(NUMBERofCHROMOSOMES);
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		genome[0].push_back(Chromosome('A'));
		genome[1].push_back(Chromosome('B'));
	}
}

Individual::Individual(char origin){
	genome[0].reserve(NUMBERofCHROMOSOMES);
	genome[1].reserve(NUMBERofCHROMOSOMES);
	if(origin == 'A' or origin == 'B'){
		for(int i=0;i<NUMBERofCHROMOSOMES;i++){
			genome[0].push_back(Chromosome(origin));
			genome[1].push_back(Chromosome(origin));
		}
	} else {
		for(int i=0;i<NUMBERofCHROMOSOMES;i++){
			genome[0].push_back('A');
			genome[1].push_back('B');
		}
	}
}

Individual::Individual(vector<Chromosome>& gamete1, vector<Chromosome>& gamete2){
	genome[0].reserve(NUMBERofCHROMOSOMES);
	genome[1].reserve(NUMBERofCHROMOSOMES);
	int i; 
	for(i=0;i<NUMBERofCHROMOSOMES;i++){
		genome[0].push_back(gamete1[i]);
		genome[1].push_back(gamete2[i]);
	}
}

void Individual::replace_chromozome(int set, int position, map <int, char>  input_chrom){
	genome[set][position] = input_chrom;
}

void Individual::makeGamete(vector<Chromosome>& gamete){
	gamete.clear(); // variable for new gamete
	gamete.reserve(NUMBERofCHROMOSOMES);
	vector<int> chiasmas; // vector of randomes chismas
	Chromosome recombinant_ch; // temp chromosome
	char last_material_s1, last_material_s2;
	int rec_pos, numberOfChaisma, starts_by;

/* for every chromosome... */
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
/* syntax genome[set][chromosome] */
		numberOfChaisma = getChiasma();
		starts_by = tossAcoin();
		
/* no chiasma mean inheritance of whole one parent chromosome */
		if(numberOfChaisma == 0){
			gamete.push_back(genome[starts_by][i]);
			continue;
		}
		
/* inicialization / restart of variables */
		map<int, char>::const_iterator pos1=genome[0][i].begin();
		map<int, char>::const_iterator pos2=genome[1][i].begin();
		last_material_s1 = genome[0][i].read(0);
		last_material_s2 = genome[1][i].read(0);
		int last_roll = -1;
		chiasmas.clear();
		recombinant_ch.clear();
// roll the chiasmas positions
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = recombPosition();
			chiasmas.push_back(rec_pos);
		}
		sort(chiasmas.begin(), chiasmas.end(), arrangeObject);
		
		for(int index=0;index<numberOfChaisma;index++){
			if(last_roll == chiasmas[index]){
				chiasmas.erase (chiasmas.begin()+index,chiasmas.begin()+index+1);
				index -= 2;
				numberOfChaisma -= 2;
				last_roll = -1;
			} else {
				last_roll = chiasmas[index];
			}
		}
		if(chiasmas[0] != 0){
			recombinant_ch.write(0,genome[starts_by][i].read(0));
		} else {
			recombinant_ch.write(0,genome[(starts_by + 1) % 2][i].read(0));
		}
		pos1++;
		pos2++;
		
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = chiasmas[index];
			if(rec_pos == 0){
				starts_by = (starts_by + 1) % 2;
				continue;
			}
			if(rec_pos == LOCI){
				continue;
			}
			
			if(starts_by==0){
				while(pos1->first < rec_pos and pos1->first != genome[0][i].end()->first){
					recombinant_ch.write(pos1->first,pos1->second);
					last_material_s1 = pos1->second;
					pos1++;
				}
				while(pos2->first <= rec_pos and pos2->first != genome[1][i].end()->first){
					last_material_s2 = pos2->second;
					pos2++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
					recombinant_ch.write(chiasmas[index],last_material_s2);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(chiasmas[index],last_material_s2);
				}
			} else {
				while(pos2->first < rec_pos and pos2->first != genome[1][i].end()->first){
					recombinant_ch.write(pos2->first,pos2->second);
					last_material_s2 = pos2->second;
					pos2++;
				}
				while(pos1->first <= rec_pos and pos1->first != genome[0][i].end()->first){
					last_material_s1 = pos1->second;
					pos1++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
					recombinant_ch.write(chiasmas[index],last_material_s1);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
					recombinant_ch.write(chiasmas[index],last_material_s1);
				}
			}
			starts_by = (starts_by + 1) % 2;
		}
		if(starts_by==0){
			while(pos1->first != genome[0][i].end()->first){
				recombinant_ch.write(pos1->first,pos1->second);
				last_material_s1 = pos1->second;
				pos1++;
			}
			while(pos2->first != genome[1][i].end()->first){
				last_material_s2 = pos2->second;
				pos2++;
			}
		} else {
			while(pos2->first != genome[1][i].end()->first){
				recombinant_ch.write(pos2->first,pos2->second);
				last_material_s2 = pos2->second;
				pos2++;
			}
			while(pos1->first != genome[0][i].end()->first){
				last_material_s1 = pos1->second;
				pos1++;
			}
		}
		gamete.push_back(recombinant_ch);
	}
}

double Individual::getFitness(){
	double Bcount = 0;
	double fitness = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		Bcount += genome[0][i].countB();
		Bcount += genome[1][i].countB();
	}
	Bcount = Bcount / (LOCI*2*NUMBERofCHROMOSOMES); /* relative B count*/
	fitness = 1 - (SELECTIONpressure * pow( 4 * Bcount * (1 - Bcount),BETA));
	return fitness;
}

double Individual::getBprop() const{
	double prop = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		prop += genome[0][i].countB();
		prop += genome[1][i].countB();
	}
	prop = prop / (LOCI*2*NUMBERofCHROMOSOMES);
	return prop;
}

double Individual::getHetProp(){
	bool write;
	long numberOFhetLOCI = 0;
	int last_pos = 0;
	map<int, char>::const_iterator pos1, pos2;
	
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		pos1=genome[0][i].begin(); pos2=genome[1][i].begin();
		last_pos = 0;
		write = !(pos1->second == pos2->second);
		pos1++; pos2++;
		if((pos1 == genome[0][i].end()) & (pos2 == genome[1][i].end())){
			numberOFhetLOCI += LOCI * write;
			continue;
		}
		
		if(pos1 == genome[0][i].end()){
			numberOFhetLOCI += getOneChromeHetero(write, pos2, i, 0);
			continue;
		}
		
		if(pos2 == genome[1][i].end()){
			numberOFhetLOCI += getOneChromeHetero(write, pos1, i, 0);
			continue;
		}
		
		while(pos1 != genome[0][i].end() and pos2 != genome[1][i].end()){
			if(pos1->first < pos2->first){
				last_pos = pos1->first;
				pos1++;
				write = !write;
				continue;
			}
			
			if(pos1->first > pos2->first){
				numberOFhetLOCI += (pos2->first - last_pos) * write;
				last_pos = pos2->first;
				pos1++;
				write = !write;
				continue;
			}
			
			if(pos1->first == pos2->first){
				numberOFhetLOCI += (pos1->first - last_pos) * write;
				last_pos = pos2->first;
				pos1++; pos2++;
				continue;
			}
			
			cerr << "WARNING: Heterozygotisity counting problem (junc level)!";
		}
		
		if((pos1 == genome[0][i].end()) & (pos2 == genome[1][i].end())){
			numberOFhetLOCI += (LOCI - last_pos) * write;
			continue;
		}
		
		if(pos1 == genome[0][i].end()){
			numberOFhetLOCI += getOneChromeHetero(write, pos2, i, last_pos);
			continue;
		}
		
		if(pos2 == genome[1][i].end()){
			numberOFhetLOCI += getOneChromeHetero(write, pos1, i, last_pos);
			continue;
		}
		cerr << "WARNING: Heterozygotisity counting problem (ch level)!";
	}
	return ((double)numberOFhetLOCI / (LOCI * NUMBERofCHROMOSOMES));
}


int Individual::getBcount() const{
	int count = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		count += genome[0][i].countB();
		count += genome[1][i].countB();
	}
	return count;
}


bool Individual::Acheck() const{
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[0][i].Acheck() and genome[1][i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Individual::Bcheck() const{
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[0][i].Bcheck() and genome[1][i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

map< int, char >::iterator Individual::getChromosomeBegining(int set, int chrom){
	return genome[set][chrom].begin();
}

int Individual::getNumberOfJunctions(int set, int chrom){
	return genome[set][chrom].getNumberOfJunctions();
}

int Individual::getNumberOfJunctions(){
	int sum = 0;
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < NUMBERofCHROMOSOMES; chrom++){
			sum += genome[set][chrom].getNumberOfJunctions();
		}
	}
	return sum;
}

void Individual::getSizesOfBBlocks(std::vector<int>& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < NUMBERofCHROMOSOMES; chrom++){
			genome[set][chrom].getSizesOfBBlocks(sizes);
		}
	}
	return;
}

void Individual::getSizesOfABlocks(std::vector<int>& sizes){
	sizes.clear();
	sizes.reserve(500);
	for(int set = 0; set < 2; set++){
		for(int chrom = 0; chrom < NUMBERofCHROMOSOMES; chrom++){
			genome[set][chrom].getSizesOfABlocks(sizes);
		}
	}
	return;
}

void Individual::readGenotype(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		cout << "---Chromozome---set-1---number-" << i+1 << "---" << endl;
		genome[0][i].showChromosome();
		cout << "---Chromozome---set-2---number-" << i+1 << "---" << endl;
		genome[1][i].showChromosome();
	}
	cout << endl;
}

void Individual::viewGenotype(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		cout << "---Chromozome---set-1---number-" << i+1 << "---" << endl;
		genome[0][i].viewChromosome();
		cout << "---Chromozome---set-2---number-" << i+1 << "---" << endl;
		genome[1][i].viewChromosome();
	}
	cout << endl;
}

class Imigrant  
{
	public:
/* DECLARATION */
		Imigrant();
		Imigrant(char origin); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Imigrant(vector<Chromosome>& gamete); /*init Individual by gametes on imput */
		~Imigrant(){genome.clear();}; /* destructor */
		
// /* COMPUTIONG METHODS */
		void makeGamete(Imigrant& descendant); //
		double getFitness();
		double getBprop() const;
		void getSizesOfBBlocks(vector<int>& sizes);
		bool Acheck() const;
		bool Bcheck() const;
	
	private:
		vector<Chromosome> genome;
};

Imigrant::Imigrant(){
	genome.reserve(NUMBERofCHROMOSOMES);
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		genome.push_back(Chromosome('B'));
	}
}

Imigrant::Imigrant(char origin){
	genome.reserve(NUMBERofCHROMOSOMES);
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		genome.push_back(Chromosome(origin));
	}
}

Imigrant::Imigrant(vector<Chromosome>& gamete){
	genome.reserve(NUMBERofCHROMOSOMES);
	int i; 
	for(i=0;i<NUMBERofCHROMOSOMES;i++){
		genome.push_back(gamete[i]);
	}
}


double Imigrant::getBprop() const{
	double prop = 0;
	for(int ch = 0;ch < NUMBERofCHROMOSOMES;ch++){
		prop += genome[ch].countB();
	}
	prop = prop / (NUMBERofCHROMOSOMES * LOCI);
	return prop;
}

void Imigrant::getSizesOfBBlocks(vector< int >& sizes){
	sizes.clear();
	for(int ch = 0;ch < NUMBERofCHROMOSOMES;ch++){
		genome[ch].getSizesOfBBlocks(sizes);
	}
	return;
}


double Imigrant::getFitness(){
	return 1 - (SELECTIONpressure * getBprop());
}

bool Imigrant::Acheck() const{
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Imigrant::Bcheck() const{
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

void Imigrant::makeGamete(Imigrant& descendant){
	vector<Chromosome> gamete;
	gamete.reserve(NUMBERofCHROMOSOMES);
	Chromosome CHtemp;
	for(int ch = 0; ch < NUMBERofCHROMOSOMES;ch++){
		genome[ch].makeRecombinant(CHtemp, getChiasma());
		gamete.push_back(CHtemp);
	}
	descendant = Imigrant(gamete);
}

int Individual::getOneChromeHetero(bool write, map<int, char>::const_iterator& pos, int chromosome, int last_pos){
	int numberOFhetLOCI = 0;
	
	while((pos != genome[0][chromosome].end()) and (pos != genome[1][chromosome].end())){
		numberOFhetLOCI += (pos->first - last_pos) * write;
		last_pos = pos->first;
		pos++;
		write = !write;
	}
	numberOFhetLOCI += (LOCI - last_pos) * write;
	
	return numberOFhetLOCI;
}
