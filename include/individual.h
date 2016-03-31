#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

using namespace std;

class Individual  
{
	public:
/* DECLARATION */
		Individual();
		Individual(char origin); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Individual(char origin, int input_ch, int input_loci, double input_lamda);
		Individual(	vector<Chromosome>& gamete1, 
					vector<Chromosome>& gamete2, 
					double input_lamda); /*init Individual by gametes on imput */
		~Individual(); /* destructor */
		
/* COMPUTIONG METHODS */
		void replace_chromozome(int set, int position,map <int, char> input_chrom, int size);
		int getChiasma();
		void makeGamete(vector<Chromosome>& gamete);
		int getBcount() const;
		double getBprop() const;
		double getHetProp();
		bool Acheck() const;
		bool Bcheck() const;
		map<int, char>::iterator getChromosomeBegining(int set, int chrom);
		int getNumberOfJunctions(int set, int chrom);
		int getNumberOfJunctions();
		
/* ASCII PLOTTING METHODS */
		void readGenotype();
		void viewGenotype();
		
/* COMUNICATION */
		void setNumberOfChromosomes(int ch);
		void setLambda(double Rr);
			
	private:
	/*data*/
		vector<Chromosome> genome[2];
		int number_of_chromosomes;
		double lambda;
		
	/*inner functions*/
		int getOneChromeHetero(bool write, map<int, char>::const_iterator& pos, int chromosome, int last_pos);
};

#endif
