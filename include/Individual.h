#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "../include/Chiasmata.h"

class Individual
{
	public:
/* DECLARATION */
		Individual();
		Individual(char origin, int input_ch, int input_loci, double input_lamda, int input_selected_loci, std::tuple<int, int, int> ind_birthplace);
		Individual(	std::vector<Chromosome>& gamete1,
					std::vector<Chromosome>& gamete2,
					double input_lamda, int input_selected_loci); /*init Individual by gametes on imput */
		Individual(	std::vector<Chromosome>& gamete1, std::vector<Chiasmata>& chaiasmata1,
					std::vector<Chromosome>& gamete2, std::vector<Chiasmata>& chaiasmata2,
					double input_lamda, int input_selected_loci,
					std::tuple<int, int, int> ind_birthplace); /*init Individual by gametes on imput */
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
		void setParents(std::tuple<int,int,int> in_mum, std::tuple<int,int,int> in_dad);
		int getNumberOfChromosomes() const;
		double getLambda() const;
		int getNumberOfLoci(int ch) const;
		int getNumberOfSelectedLoci() const;
		void getNumberOfLoci(std::vector<int>& ch) const;
		void getGenotype(std::vector<std::string>& hapl) const;
		void getChiasmata(std::vector<std::string>& rec) const;
		std::tuple<int,int,int> getBirthplace() const;
		std::tuple<int,int,int> getMum() const;
		std::tuple<int,int,int> getDad() const;

	private:
	/*data*/
		std::vector<Chromosome> genome[2];
		std::vector<Chiasmata> chiasmata[2];
		int number_of_chromosomes, selected_loci;
		double lambda;
		// cartesian x, y of deme indeces and index of ind within the deme
		std::tuple<int,int,int> birthplace, mum, dad;

	/*inner functions*/
		int getOneChromeHetero(bool write, std::map<int, char>::const_iterator& pos, int chromosome, int last_pos);
		std::string collapseBlocks(std::vector<int>& blocks) const;
};

#endif
