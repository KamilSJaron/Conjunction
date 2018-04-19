#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../include/Chiasmata.h"

class Individual
{
	public:
/* DECLARATION */
		Individual();
		Individual(char origin, int input_ch, int input_loci, double input_lamda, int input_selected_loci);
		Individual(	std::vector<Chromosome>& gamete1,
					std::vector<Chromosome>& gamete2,
					double input_lamda, int input_selected_loci); /*init Individual by gametes on imput */
		Individual(	std::vector<Chromosome>& gamete1, std::vector<Chiasmata>& chaiasmata1,
					std::vector<Chromosome>& gamete2, std::vector<Chiasmata>& chaiasmata2,
					double input_lamda, int input_selected_loci,
					int input_birthdeme, int input_birthindex); /*init Individual by gametes on imput */
		~Individual(); /* destructor */

/* COMPUTIONG METHODS */
		void replace_chromozome(int set, int position,std::map <int, char> input_chrom, int size);
		int getChiasma();
		void makeGamete(std::vector<Chromosome>& gamete, std::vector<Chiasmata>& chiasmata);
		int getBcount() const;
		double getBprop() const;
		double getSelectedHybridIndex();
		double getHetProp();
		bool Acheck() const;
		bool Bcheck() const;
		std::map<int, char>::iterator getChromosomeBegining(int set, int chrom);
		int getNumberOfJunctions(int set, int chrom);
		int getNumberOfJunctions();
		void getSizesOfBBlocks(std::vector<int>& sizes);
		void getSizesOfABlocks(std::vector<int>& sizes);

/* ASCII PLOTTING METHODS */
		void readGenotype();

/* COMUNICATION */
		void setNumberOfChromosomes(int ch);
		void setLambda(double Rr);
		void setParents(int in_mum_birthdeme, int in_mum_birthindex, int in_dad_birthdeme, int in_dad_birthindex);
		int getNumberOfChromosomes() const;
		double getLambda() const;
		int getNumberOfLoci(int ch) const;
		int getNumberOfSelectedLoci() const;
		void getNumberOfLoci(std::vector<int>& ch) const;
		void getGenotype(std::vector<std::string>& hapl) const;
		void getChiasmata(std::vector<std::string>& rec) const;
		int getBirtheme() const;
		int getBirthindex() const;
		int getMumBirtheme() const;
		int getMumBirthindex() const;
		int getDadBirtheme() const;
		int getDadBirthindex() const;

	private:
	/*data*/
		std::vector<Chromosome> genome[2];
		std::vector<Chiasmata> chiasmata[2];
		int number_of_chromosomes, selected_loci;
		double lambda;
		int birthdeme, birthindex; // deme index and individual index where he has born
		int mum_birthdeme, mum_birthindex;
		int dad_birthdeme, dad_birthindex;

	/*inner functions*/
		int getOneChromeHetero(bool write, std::map<int, char>::const_iterator& pos, int chromosome, int last_pos);
		std::string collapseBlocks(std::vector<int>& blocks) const;
};

#endif
