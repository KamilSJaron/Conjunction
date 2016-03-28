#ifndef IMIGRANT_H
#define IMIGRANT_H

using namespace std;

class Imigrant  
{
	public:
/* DECLARATION */
		Imigrant(int input_ch, int size, int input_sp);
		Imigrant(char origin, int input_ch, int size, int input_sp); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Imigrant(vector<Chromosome>& gamete, int input_sp); /*init Individual by gametes on imput */
		~Imigrant(){genome.clear();}; /* destructor */
		
// /* COMPUTIONG METHODS */
		int getChiasma();
		void makeGamete(Imigrant& descendant); //
		double getFitness();
		double getBprop() const;
		void getSizesOfBBlocks(vector<int>& sizes);
		bool Acheck() const;
		bool Bcheck() const;
		int getSelectionPressure() const;
	
	private:
		vector<Chromosome> genome;
		int number_of_chromosomes;
		int selection_pressure;
};


#endif
