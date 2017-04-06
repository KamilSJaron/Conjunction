/*
 * World.h
 *
 *  Created on: May 22, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_WORLD_H_
#define INCLUDE_WORLD_H_

class World {
	public:
		World();

		void basicUnitCreator(char type, char init);
		void worldSlave();

	// 	computing functions
		int migration(); // int will be the errorcode
		void Breed(int index);
		void set(int index, std::string type);
		void globalBreeding();
		int getNumberOfDescendants(double fitness); // for imigrants

	// 	testing functions vector<Chromosome>& gamete
		bool Acheck(std::vector<Individual>& buffer);
		bool Bcheck(std::vector<Individual>& buffer);
		bool empty();

	// 	plotting functions
		void listOfParameters() const;
		void listOfNumericalParameters(std::ostream& stream) const;
		void listOfDemes();
		int summary(std::ostream& stream);
		double getProportionOfHeterozygotes(int index);
		double getProportionOfHomozygotes(int index, char type);
		void showOneDeme(int index);
		int SaveTheUniverse(std::string type, std::string filename);
		void getLD();

	// 	parameter changing functions
		void setHeight(int heig);
		void setWidth(int width);
		void setLREdgesType(std::string ed_type);
		void setUDEdgesType(std::string ed_type);
		void setDimension(int dim);
		void setNumberOfEdges(int nue);
		void setSlectionBetaLambda(double s, double b, double l);
		void setLociChromDeme(int l, int ch, int d);
		void restart(); // clear the space, creates a new one (2 columns, rows defined by user)
		void clear();	// deletes all demes

	private:
	// 	inner methods
	// computing
		int upperBorder(int index, int max_index); // function returns index of upper neigbour for new demes
		int lowerBorder(int index, int max_index);
		int sideBorder(int reflexive, int extending);
		bool gameteAcheck(std::vector<Chromosome>& gamete);

	// saving methods
		int saveLinesPerIndividual(std::ofstream& ofile, std::string type);
		int saveLinesPerDeme(std::ostream& ofile, std::string type);
		int saveBlocks(std::ofstream& ofile);

		void streamBlockSizesOf0DWorld(std::ostream& stream);
		template<typename T>
		int save_line(std::ostream& stream, int index, std::vector<T>& vec) const;

	// stats
		double getMaterial() const;
		int getTotalNumberOfBBlocks();
		double getMeanFitness() const;
	// 	variables
	//	containers
		std::map<int, Deme*> world; // container of Demes for non zero dimensional simation
		std::vector<Imigrant> zeroD_immigrant_pool; // container of individuals for 0 dimensional simualtion

	//	parameters:	world definition
		int number_of_demes_l_r, number_of_demes_u_d, dimension; // defines the size of the space
		std::string type_of_l_r_edges, type_of_u_d_edges; // defines the behavior of the l / r and u /d edges of HZ

	//	parameters:	deme, individual, chromosome setting
		int deme_size, number_of_chromosomes, number_of_loci;
		double selection, beta, lambda;

	//	internal
		int edges_per_deme; // defines migration and extension of hybrid zone
		int index_last_left, index_next_left; // variables handling dynamic extension of hybrid zone
		int index_last_right, index_next_right;
};

#endif /* INCLUDE_WORLD_H_ */
