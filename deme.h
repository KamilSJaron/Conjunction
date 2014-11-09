#include "individual.h"

const int DEMEsize = 8;
// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)
// v0.141107

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
		Deme(){};
// 		Deme(int ind){index = ind;};
// 		Deme(vector<int> neigb){neigbours = neigb;};
		Deme(int ind, char init);
		Deme(int ind, vector<int> neigb){index = ind;neigbours = neigb;};
		Deme(int ind, vector<int> neigb, char init);
// 		~Deme(){delete deme;}; // ????
		
// communication functions
		vector<int> getNeigbours(){return neigbours;};
		Individual getIndividual(int index){return deme[index];}
		int getDemeIndex(){return index;};
		Deme(const vector<int>&);
		
// computing functions
		void quickBreed();
		void getFitnessVector(vector<double>& fitnessVector);
		
		
		void permutation(); // permutation function
		void integrateVectorMigrants(vector<Individual>& migBuffer);
		
// plotting functions
		void showDeme();
		void plotDeme();
		void plotHeadOfDeme();
	
	private:
		void swap(int ind1, int ind2);
		
		Individual *deme = new Individual[DEMEsize];
		int index;
		vector<int> neigbours;
};

  // // // // // // // // // // // // // //
 // constructor/destructors functions / //
// // // // // // // // // // // // // //


Deme::Deme(int ind, vector<int> neigb, char init){
	index = ind;
	neigbours = neigb;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< DEMEsize){
			deme[i] = tempB;
			i++;
		}
	}
	
}

Deme::Deme(int ind, char init){
	index = ind;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<DEMEsize;i++){
			deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (DEMEsize / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< DEMEsize){
			deme[i] = tempB;
			i++;
		}
	}
}

  // // // // // // // // // // // // // //
 // // // // computing functions / // // /
// // // // // // // // // // // // // //

void Deme::quickBreed(){
	Individual metademe[DEMEsize];
	for(int i=0;i<DEMEsize;i++){
		metademe[i] = Individual(deme[pickAnIndividual()].makeGamete(),deme[pickAnIndividual()].makeGamete());
	}
	for(int i=0;i<DEMEsize;i++){
		deme[i] = metademe[i];
	}
}

void Deme::permutation(){
	int j = 0;
  for(int i = 0;i<DEMEsize;i++){
		j = rand() % (DEMEsize - i);
		j += i;
		swap(i, j);
	}
}

void Deme::integrateVectorMigrants(vector<Individual>& migBuffer){
	int i = 0;
	while((unsigned)i < migBuffer.size()){
		deme[i] = migBuffer[i];
		i++;
// 		migBuffer[i].plotGenotype();
	}
	return;
}

void Deme::getFitnessVector(vector<double>& fitnessVector){
	double sum = 0;
	for(int i;i < DEMEsize;i++){
		sum =+ deme[i].getFitness();
		fitnessVector.push_back(sum);
	}
}
		
		
  // // // // // // // // // // // // // //
 // // // // plotting functions / // // //
// // // // // // // // // // // // // //

void Deme::showDeme(){
	int neigbsize = neigbours.size();
	cout << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cout << setw(5) << left << neigbours[i] << " ";
	}
	cout << endl;
}

void Deme::plotDeme(){
	for(int i=0;i<DEMEsize;i++){
		cout << "Individual: " << i+1 << endl;
		deme[i].plotGenotype();
	}
}

void Deme::plotHeadOfDeme(){
	cout << "Individual: 0" << endl;
	deme[0].plotGenotype();
	cout << "Individual: 1" << endl;
	deme[1].plotGenotype();
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