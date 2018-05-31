// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)

#ifndef DEME_H
#define DEME_H

#include <vector>

#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Context.h"

class Deme
{
	public:
// constructor / destructors
		Deme(const Context& context, int ind, std::vector<int> neigb, char init, int size, double sel, double beta, int in_ch, int in_loc, int in_sel_loci, double in_lambda, int in_x, int in_y);
		~Deme();

// communication functions
		std::vector<int> getNeigbours();
		Individual getIndividual(int index);
		int getDemeIndex();
		void setDemeSize(int size);
		int getDemeSize();
		int getX();
		int getY();

// computing functions
		void Breed();
		void integrateMigrantVector(std::vector<Individual>& migBuffer);
// 		std::vector<double> getBproportions() const; // not used
		void getBproportions(std::vector<double>& props) const;
		//void getJunctionNumbers(std::vector<double>& juncs) const;
		//void getHeterozygoty(std::vector<double>& heterozs);
		double getMeanBproportion() const;
		//void getSizesOfBBlocks(std::vector<int>& sizes, int ind);
		//void getSizesOfABlocks(std::vector<int>& sizes, int ind);
		double getProportionOfHomozygotes(char population) const;
		double getProportionOfHeterozygotes() const;
		void getFitnessVector(std::vector<double>& fitnessVector);
		double getMeanFitness();
		double getVARhi() const;
		double getVARp();
		void getps(std::vector<double>& ps, int ch);
		double getLD();
		double getLD(double z,double varz,double varp);

// plotting functions
		void showDeme();
		void streamSummary(std::ostream& stream);
		void streamBlocks(std::ostream& stream);
		void streamChiasmata(std::ostream& stream);
		void streamHIs(std::ostream& stream) const;
		void streamJunctions(std::ostream& stream) const;
		void streamHeterozygosity(std::ostream& stream) const;


		void readAllGenotypes();
		void readGenotypeFrequencies();

	private:
		const Context& context;

		int pickAnIndividual();
		static int sum(std::vector<bool>& ve);
		template<typename T>
		int streamLine(std::ostream& stream, std::vector<T>& vec) const;
		double roundForPrint(double number) const;

		Individual *deme; // farme of individuals
		int index; // index of deme in universe
		int x, y;
		int deme_size; // size of deme
		std::vector<int> neigbours; // indices of neigbours
		SelectionModel selection_model;
};

#endif
