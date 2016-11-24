#ifndef IMIGRANT_H
#define IMIGRANT_H


class Imigrant
{
	public:
		/* DECLARATION */
		Imigrant(int input_ch, int size, double input_lambda);
		/* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Imigrant(char origin, int input_ch, int size, double input_lambda);
		/*init Individual by gametes on imput */
		Imigrant(std::vector<Chromosome>& gamete, double input_lambda);
		~Imigrant(); /* destructor */

		/* COMPUTIONG METHODS */
		int getChiasma();
		void makeGamete(std::vector<Chromosome>& gamete);
		void getSizesOfBBlocks(std::vector<int>& sizes);

		/* STATS */
		int getNumberOfBBlocks();
		int getNumberOfJunctions() const;
		double getBprop() const;
		bool Acheck() const;
		bool Bcheck() const;

		/* GETTERS */
		int getLambda() const;

		/* ASCII PLOTTING METHODS */
		void readGenotype();

	private:
		std::vector<Chromosome> genome;
		int number_of_chromosomes;
		double lambda;
};


#endif
