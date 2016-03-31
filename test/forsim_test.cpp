/* In general it's good to include also the header of the current .c,
   to avoid repeating the prototypes */
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#include "forsim_test.h"
#include "../include/chromosome.h"
#include "../include/individual.h"

int testChromosome(){
	Chromosome ch1('A',50);
	Chromosome ch2('B',50);
	// second type of inicitation
	
	if(!ch1.Acheck()){
		cerr << "UNEXPECTED ch(A, 50).Acheck is " << ch1.Bcheck() << endl; 
		return 1;
	}

	if(ch1.Bcheck()){
		cerr << "UNEXPECTED ch(A, 50).Bcheck is " << ch1.Bcheck() << endl; 
		return 1;
	}

	if(ch1.countB() != 0){
		cerr << "UNEXPECTED ch(A, 50).Bcount is " << ch1.countB() << endl; 
	}
	
	if(ch2.countB() != 50){
		cerr << "UNEXPECTED ch(B, 50).Bcount is " << ch2.countB() << endl; 
	}
	
	if(ch1.getNumberOfJunctions() != 0){
				cerr << "UNEXPECTED ch(A, 50).getNumberOfJunctions is " 
				<< ch1.getNumberOfJunctions() << endl; 
	}

	int toss = ch1.tossAcoin();
	if(!(toss == 0 or toss == 1)){
		cerr << "tossAcoin function is flawed." << endl;
	}
	
	for(int i = 0; i < 1000; i++){
		Chromosome ch3('A', rand());
		if(ch3.recombPosition() >= ch3.getResolution()){
			cerr << "recombPosition generated position larger that the size of the chromosome."
				<< endl;
			return 1;
		}
	}
	
	Chromosome ch3('X',5);
	ch3.setResolution(666);
	if(ch3.getResolution() != 666){
		return 1;
	}
	
	ch3.setResolution(100);
	ch3.write(10,'B');
	
	if(ch3.read(0) != 'X' or ch3.read(10) != 'B'){
		return 1;
	}

	ch1.makeRecombinant(ch3, 5);
	
	if(ch3.getNumberOfJunctions() != 0){
		cerr << "ERROR: recombination of two pure A chromosome resulted is chromosome with junctions." << endl;
		return 1;
	}
	
	ch2.makeRecombinant(ch3, 5);

	if(ch3.getNumberOfJunctions() < 4){
		cerr << "WARNING: very unlike scenario in random generation of junctions" << endl;
		cerr << "Change seed for random number and rerun tests, if this message occurs again"
		<< " something is wrong, please do not ignore it and report the issue." << endl;
		return 2;
	}

/* Writing testing for plotting functions is not worth the energy spend on it, for verification, 
following code can be uncommented and used for manual testing */
// 	ch1.makeRecombinant(ch3, 5);
// 	cerr << "CH 1" << endl;	
// 	ch1.showChromosome();
// 	cerr << "CH REC" << endl;
// 	ch3.showChromosome();
// 	ch2.makeRecombinant(ch3, 5);
// 	cerr << "CH 3" << endl << endl;
// 	ch3.showChromosome();
// 	cerr << endl;
// 	ch3.viewChromosome();
// 	
// 	ch3.setResolution(200);
// 	cerr << endl;
// 	ch3.viewChromosome();
// 	cerr << "CH REC" << endl;
// 	ch3.showChromosome();
//		void makeRecombinant(Chromosome& chromNew,int numberOfChaisma);

	return 0;
}

int testIndividual(){
	Individual Lili;
	Individual Hugo('C');
    Individual Igor('B', 3, 50, 2.3, 0.3, 4);
// 		Individual(	vector<Chromosome>& gamete1, 
// 					vector<Chromosome>& gamete2, 
// 					double input_lamda,
// 					double input_sp,
// 					double input_beta);
//		void replace_chromozome(int set, int position,map <int, char> input_chrom, int size);
	
	double sumCh = 0.0;
	for(int i = 0; i < 10000; i++){
		sumCh += Igor.getChiasma();
	}
	sumCh = sumCh / 10000;
	
	if(sumCh > 2.5 or sumCh < 2.1){
		cerr << "WARNING: unexpected behavior of generator of Chiasmas, " << endl;
		cerr << "Change seed for random number and rerun tests, if this message occurs again"
		<< " something is wrong, please do not ignore it and report the issue." << endl;
		return 2;
	}

// 		void makeGamete(vector<Chromosome>& gamete);
// 		double getFitness();
// 		int getBcount() const;
// 		double getBprop() const;
// 		double getHetProp();
// 		bool Acheck() const;
// 		bool Bcheck() const;
// 		map<int, char>::iterator getChromosomeBegining(int set, int chrom);
// 		int getNumberOfJunctions(int set, int chrom);
// 		int getNumberOfJunctions();
// 		
// /* ASCII PLOTTING METHODS */
// 		void readGenotype();
// 		void viewGenotype();
// 		
// /* COMUNICATION */
// 		void setNumberOfChromosomes(int ch);
// 		void setLambda(double Rr);
// 		void setSelectionPressure(double Sp);
// 		void setBeta(double in_beta);
// 			
// 	private:
// 	/*data*/
// 		vector<Chromosome> genome[2];
// 		int number_of_chromosomes;
// 		double lambda;
// 		double selection_pressure;
// 		double beta;
// 		
// 	/*inner functions*/
// 		int getOneChromeHetero(bool write, map<int, char>::const_iterator& pos, int chromosome, int last_pos);
	return 0;
}
