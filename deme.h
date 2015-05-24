#include "individual.h"

static int DEMEsize = -1;
// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)

using namespace std;

// fctions
double selectionRand(){
		return (double)rand() / RAND_MAX;
}

int pickAnIndividual(){
		return rand() % DEMEsize;
}

class Deme  
{
	public:
// constructor / destructors
// 		Deme(){};
// 		Deme(int ind){index = ind;};
// 		Deme(vector<int> neigb){neigbours = neigb;};
		Deme(int ind, char init);
		Deme(int ind, vector<int> neigb){deme = new Individual[DEMEsize];index = ind;neigbours = neigb;};
		Deme(int ind, vector<int> neigb, char init);
		~Deme(){delete[] deme;}; // ????
		
// communication functions
		vector<int> getNeigbours(){return neigbours;};
		Individual getIndividual(int index){return deme[index];}
		int getDemeIndex(){return index;};
		Deme(const vector<int>&);
		static void setDEMEsize(int demesize){DEMEsize = demesize;};
		static int getDEMEsize(){return DEMEsize;};
		
// computing functions
		void Breed();
		void getFitnessVector(vector<double>& fitnessVector);
		void permutation(); // permutation function
		void integrateMigrantVector(vector<Individual>& migBuffer);
		vector<double> getBproportions() const; // not used
		double getMeanBproportion() const;
		double getProportionOfHomozygotes(char population) const;
		double getProportionOfHeterozygotes() const;
		double getMeanFitness() const;
		double getVARhi() const;
		double getVARp();
		void getps(vector<double>& ps, int ch);
		double getLD();
		double getLD(double z,double varz,double varp);
		
// plotting functions
		void showDeme();
		void summary();
		void summary(ofstream& ofile);
		void viewDeme();
		void readAllGenotypes();
		void readGenotypeFrequencies();
		void plotHeadOfDeme();
	
	private:
		void swap(int ind1, int ind2);
		static int sum(vector<bool>& ve);
// 		static int SIZE;
		
		Individual *deme;
// 		vector<Individual> niche;
		int index;
		vector<int> neigbours;
};

  // // // // // // // // // // // // // //
 // constructor/destructors functions / //
// // // // // // // // // // // // // //


Deme::Deme(int ind, vector<int> neigb, char init){
// 	niche.reserve(DEMEsize);
	deme = new Individual[DEMEsize];
	index = ind;
	neigbours = neigb;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
// 			niche.push_back(temp);
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
// 			niche.push_back(tempA);
			deme[i] = tempA;
			i++;
		}
		while(i< DEMEsize){
// 			niche.push_back(tempB);
			deme[i] = tempB;
			i++;
		}
	}
	
}

Deme::Deme(int ind, char init){
	deme = new Individual[DEMEsize];
// 	niche.reserve(DEMEsize);
	index = ind;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
// 			niche.push_back(temp);
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
			deme[i] = tempA;
// 			niche.push_back(tempA);
			i++;
		}
		while(i< DEMEsize){
			deme[i] = tempB;
// 			niche.push_back(tempA);
			i++;
		}
	}
}

  // // // // // // // // // // // // // //
 // // // // computing functions / // // /
// // // // // // // // // // // // // //

void Deme::Breed(){
	vector<double> fitnessVector;
	vector<Chromosome> gamete1, gamete2;
	getFitnessVector(fitnessVector);
	double RandMax = fitnessVector[DEMEsize-1];
	double roll;
	map<double, int> parentPick;
	map<double, int>::iterator it;
	vector<int> mothers(DEMEsize);
	vector<int> fathers(DEMEsize);
	
	for(int i=0;i < DEMEsize*2;i++){
		roll = (selectionRand() * RandMax);
		it = parentPick.find(roll);
		while(it != parentPick.end()){ //rolling twice same number will overwrite the first one, but this event is so rare, that solution is to just roll again without any bias
			roll = (selectionRand() * RandMax);
			it = parentPick.find(roll);
		}
		parentPick[roll] = i;
	}
	
	int i = 0;
	map<double, int>::iterator pos=parentPick.begin();
	
	while(pos!=parentPick.end()){
		
		if(fitnessVector[i] >= pos->first){
			if(pos->second >= DEMEsize){
				mothers[pos->second - DEMEsize] = i;
			} else {
				fathers[pos->second] = i;
			}
			++pos;
		} else {
			i++;
		}
	}
	
	Individual *metademe = new Individual[DEMEsize];
	for(int i=0;i<DEMEsize;i++){
		deme[mothers[i]].makeGamete(gamete1);
		deme[fathers[i]].makeGamete(gamete2);
		metademe[i] = Individual(gamete1,gamete2);
	}
	
	for(int i=0;i<DEMEsize;i++){
		deme[i] = metademe[i];
	}
	delete[] metademe;
}

void Deme::permutation(){
	int j = 0;
  for(int i = 0;i<DEMEsize;i++){
		j = rand() % (DEMEsize - i);
		j += i;
		swap(i, j);
	}
}

void Deme::integrateMigrantVector(vector<Individual>& migBuffer){
	unsigned int i = 0;
	while(i < migBuffer.size()){
		deme[i] = migBuffer[i];
		i++;
	}
	return;
}

vector<double> Deme::getBproportions() const{
	vector<double> props;
	props.reserve(DEMEsize);
	for(int i = 0;i < DEMEsize;i++){
		props.push_back(deme[i].getBprop());
	}
	return props;
}

double Deme::getMeanBproportion() const{
	double props = 0;
	for(int i = 0;i < DEMEsize;i++){
		props += deme[i].getBprop();
	}
	return (props / DEMEsize);
}

double Deme::getProportionOfHomozygotes(char population) const{
	double proportion = 0;
	if(population == 'A'){
		for(int i = 0;i < DEMEsize;i++){
			proportion += (deme[i].Acheck());
		}
		proportion = proportion / DEMEsize;
		return proportion;
	}
	if(population == 'B'){
		for(int i = 0;i < DEMEsize;i++){
			proportion += (deme[i].Bcheck());
		}
		proportion = proportion / DEMEsize;
		return proportion;
	}
	return 0;
}

double Deme::getProportionOfHeterozygotes() const{
	double proportion = 0;
	for(int i = 0;i < DEMEsize;i++){
		proportion += (deme[i].Acheck());
		proportion += (deme[i].Bcheck());
	}
	proportion = 1 - (proportion / DEMEsize);
	return proportion;
}


void Deme::getFitnessVector(vector<double> &fitnessVector){
	double sum = 0;
	fitnessVector.reserve(DEMEsize);
	for(int i = 0;i < DEMEsize;i++){
		sum += deme[i].getFitness();
		fitnessVector.push_back(sum);
	}
	return;
}
		
/* Computes average fitness in the deme*/
double Deme::getMeanFitness() const{
	double sum = 0;
	for(int i = 0;i < DEMEsize;i++){
		sum += deme[i].getFitness();
	}
	return sum / DEMEsize;
}

double Deme::getVARhi() const{
	double varz = 0, z = getMeanBproportion();
	for(int i = 0; i < DEMEsize; i++){
		varz += pow(z - deme[i].getBprop(),2);
	}
	varz = varz / DEMEsize;
	return varz;
}

double Deme::getVARp(){
	double p = 0, varp = 0, pmean = getMeanBproportion();
	vector<double> ps;
	for(int ch = 0; ch < NUMBERofCHROMOSOMES; ch++){
		getps(ps,ch);
		for(unsigned int locus = 0;locus < ps.size();locus++){
			p = ps[locus];
			varp += (pmean - p) * (pmean - p);
		}
		ps.clear();
	}
	varp = varp / (LOCI*NUMBERofCHROMOSOMES);
	return varp;
}

void Deme::getps(vector<double>& ps, int ch){
	double p = 0;
	int TotalCHnum = DEMEsize * 2; //total number of chromosomes in deme
	vector<bool> states;	// 0 'A', 1 'B'
	vector<map<int, char>::iterator> chroms; //vector with chromosome junctions
	vector<int> ch_sizes; //vector with number of chromosome junctions
	
	chroms.reserve(TotalCHnum);
	ch_sizes.reserve(TotalCHnum);
	states.reserve(TotalCHnum);
	ps.reserve(LOCI);
	
	for(int i = 0;i < DEMEsize;i++){
		if(deme[i].getChromosomeBegining(0,ch)->second == 'A'){
			states.push_back(0);
		} else {
			states.push_back(1);
		}
		if(deme[i].getChromosomeBegining(1,ch)->second == 'A'){
			states.push_back(0);
		} else {
			states.push_back(1);
		}
		chroms.push_back(deme[i].getChromosomeBegining(0,ch));
		chroms.push_back(deme[i].getChromosomeBegining(1,ch));
		ch_sizes.push_back(deme[i].getNumberOfJunctions(0,ch));
		ch_sizes.push_back(deme[i].getNumberOfJunctions(1,ch));
	}
	for(int i = 0;i < LOCI;i++){
// 		I have to update states every iteration
		for(int j = 0;j < TotalCHnum;j++){
			if(chroms[j]->first == i){
				if(chroms[j]->second == 'A'){
					states[j] = 0;
				} else {
					states[j] = 1;
				}
				if(ch_sizes[j] != 0){
					chroms[j]++;
					ch_sizes[j] -= 1;
				}
			}
		}
		p = sum(states) / (double)(DEMEsize * 2);
		ps.push_back(p);
	}
}



double Deme::getLD(){
	double LD = 0, varz = getVARhi(), z = getMeanBproportion(), varp = getVARp(), n = LOCI;
		
	LD = (varz - ((1 / (2 * n)) * ((z * (1 - z)) - varp))) / (0.5 * (1 - (1 / n))) ;
	
	return LD;
}

double Deme::getLD(double z,double varz,double varp){
	double LD = 0, n = LOCI;
		
	LD = (varz - ((1 / (2 * n)) * ((z * (1 - z)) - varp))) / (0.5 * (1 - (1 / n))) ;
	
	return LD;
}
		
  // // // // // // // // // // // // // //
 // // // // plotting functions / // // //
// // // // // // // // // // // // // //

void Deme::showDeme(){
	int neigbsize = neigbours.size();
	cerr << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cerr << setw(5) << left << neigbours[i] << " ";
	}
	cerr << endl;
}

void Deme::summary(){
	double z = getMeanBproportion();
	double varz = getVARhi();
	double varp = getVARp();
	int neigbsize = neigbours.size();
	cout << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cout << setw(5) << left << neigbours[i] << " ";
	}
	cout << setw(12) << left << ((round(getMeanFitness() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(getProportionOfHeterozygotes() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(z * 1000000)) / 1000000)
	<< setw(12) << left << ((round(varz * 1000000)) / 1000000);
	if(LOCI * NUMBERofCHROMOSOMES > 1){
		cout	<< setw(12) << left << ((round(varp * 1000000)) / 1000000)
		<< setw(12) << left << ((round(getLD(z,varz,varp) * 1000000)) / 1000000);
	} 
	if((LOCI * NUMBERofCHROMOSOMES) <= 16){
		vector<double> ps;
		for(int ch = 0;ch < NUMBERofCHROMOSOMES; ch++){
			ps.clear();
			getps(ps,ch);
			for(unsigned int l = 0; l < ps.size();l++){
				cout << setw(12) << left << ((round(ps[l] * 10000)) / 10000);
			}
		}
	}
	cout << endl;
}

void Deme::summary(ofstream& ofile){
	double z = getMeanBproportion();
	double varz = getVARhi();
	double varp = getVARp();
	int neigbsize = neigbours.size();
	ofile << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		ofile << setw(5) << left << neigbours[i] << " ";
	}
	ofile << setw(12) << left << ((round(getMeanFitness() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(getProportionOfHeterozygotes() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(z * 1000000)) / 1000000)
	<< setw(12) << left << ((round(varz * 1000000)) / 1000000);
	if(LOCI * NUMBERofCHROMOSOMES > 1){
		ofile	<< setw(12) << left << ((round(varp * 1000000)) / 1000000)
		<< setw(12) << left << ((round(getLD(z,varz,varp) * 1000000)) / 1000000);
	} 
	if((LOCI * NUMBERofCHROMOSOMES) <= 16){
		vector<double> ps;
		for(int ch = 0;ch < NUMBERofCHROMOSOMES; ch++){
			ps.clear();
			getps(ps,ch);
			for(unsigned int l = 0; l < ps.size();l++){
				ofile << setw(12) << left << ((round(ps[l] * 10000)) / 10000);
			}
		}
	}
	ofile << endl;
}


void Deme::viewDeme(){
	for(int i=0;i<DEMEsize;i++){
		cout << "Individual: " << i+1 << endl;
		deme[i].viewGenotype();
	}
}

void Deme::readAllGenotypes(){
	for(int i=0;i<DEMEsize;i++){
		cerr << "Individual: " << i << " B proportion: " << deme[i].getBprop() << endl;
		deme[i].readGenotype();
	}
}

void Deme::readGenotypeFrequencies(){
	vector<double> freqs;
	freqs.reserve(NUMBERofCHROMOSOMES*LOCI*2 + 1);
	for(int i=0;i < NUMBERofCHROMOSOMES*LOCI*2 + 1;i++){
		freqs.push_back(0);
	}	
	for(int i=0;i<DEMEsize;i++){
		freqs[deme[i].getBcount()]++;
	}
	for(int i=0;i < NUMBERofCHROMOSOMES*LOCI*2 + 1;i++){
		cout << ((round((freqs[i] / DEMEsize) * 10000)) / 10000) << ' ';
	}
}


void Deme::plotHeadOfDeme(){
	cout << "Individual: 0" << endl;
	deme[0].viewGenotype();
	cout << "Individual: 1" << endl;
	deme[1].viewGenotype();
}


   // // // // //
  //  PRIVATE //
 // // // // //
 
void Deme::swap(int ind1, int ind2){
	Individual tempInd;
	tempInd = deme[ind2];
	deme[ind2] = deme[ind1];
	deme[ind1] = tempInd;
}

int Deme::sum(vector< bool >& ve){
	int sum = 0; 
	for(unsigned int i = 0; i < ve.size();i++){
		sum += ve[i];
	}
	return sum;
}
