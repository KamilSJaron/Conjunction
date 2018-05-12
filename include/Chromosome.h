#ifndef CHROMOSOME_H
#define CHROMOSOME_H

class Chromosome
{
	public :
/* INITIATION */
		Chromosome(); /* blank chromosome is used for recombination*/
		Chromosome(char starting_char, int size);
		Chromosome(std::map <int, char> input_chrom, int size);
		~Chromosome();

/* PLOTTING METHODS */
		void showChromosome() const; /*writes all junctions*/

/* COMUNICATION METHODS */
		bool isPureA() const; /*returns 1 if there are only A, 0 in other case*/
		bool isPureB() const; /*returns 1 if there are only B, 0 in other case*/
		int countB() const; /*returns number of B loci in chromosome*/
		int countB(int selectedB) const; /*returns number of B loci in chromosome*/
		int getNumberOfJunctions() const; /*returns number of Junctions in chromosome*/
		void getSizesOfBBlocks(std::vector<int>& sizes); /*fills vector of ints by sizes of B blocks in chromosome*/
		void getSizesOfABlocks(std::vector<int>& sizes);
		void getSizesOfBlocks(std::vector<int>& sizes) const;
		void makeRecombinant(Chromosome& chromNew,int numberOfChaisma);

/* COMUNICATION METHODS */
		void setResolution(int res);
		int getResolution() const; /* return resolution of the chromosome */
		void clear(); /* method for deleting chromosome */
		char read(int i); /* returns value of junction */
		void write(int i, char l); /* makes new junction*/
		std::map<int, char>::iterator begin();
		std::map<int, char>::iterator end();
		std::map<int, char>::iterator find(int i);

	protected:
		std::map <int, char> chromosome;
		int loci;
};

#endif
