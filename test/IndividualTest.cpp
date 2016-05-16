/*
 * IndividualTest.cpp
 *
 *  Created on: May 16, 2016
 *      Author: kjaron
 */

int testIndividual(){
	Individual Lili;
	Individual Hugo('C');
    Individual Igor('C', 3, 50, 2.3);
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

	vector<Chromosome> gamete;
	Igor.makeGamete(gamete);

	for(int i=0; i < Igor.getNumberOfChromosomes(); i++){
		gamete[0].showChromosome();
	}
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



