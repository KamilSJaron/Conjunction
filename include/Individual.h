#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class Individual  
{
	public:
/* DECLARATION */
		Individual();
		Individual(char origin); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Individual(char origin, int input_ch, int input_loci, double input_lamda);
		Individual(	std::vector<Chromosome>& gamete1,
					std::vector<Chromosome>& gamete2,
					double input_lamda); /*init Individual by gametes on imput */
		~Individual(); /* destructor */
		
/* COMPUTIONG METHODS */
		void replace_chromozome(int set, int position,std::map <int, char> input_chrom, int size);
		int getChiasma();
		void makeGamete(std::vector<Chromosome>& gamete);
		int getBcount() const;
		double getBprop() const;
		double getHetProp();
		bool Acheck() const;
		bool Bcheck() const;
		std::map<int, char>::iterator getChromosomeBegining(int set, int chrom);
		int getNumberOfJunctions(int set, int chrom);
		int getNumberOfJunctions();
		
/* ASCII PLOTTING METHODS */
		void readGenotype();
		
/* COMUNICATION */
		void setNumberOfChromosomes(int ch);
		void setLambda(double Rr);
		int getNumberOfChromosomes() const;
		double getLambda() const;
		int getNumberOfLoci(int ch) const;
			
	private:
	/*data*/
		std::vector<Chromosome> genome[2];
		int number_of_chromosomes;
		double lambda;
		
	/*inner functions*/
		int getOneChromeHetero(bool write, std::map<int, char>::const_iterator& pos, int chromosome, int last_pos);
};

#endif
