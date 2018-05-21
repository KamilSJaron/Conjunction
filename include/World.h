/*
 * World.h
 *
 *  Created on: May 22, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_WORLD_H_
#define INCLUDE_WORLD_H_


#include "../include/Context.h"
#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/Imigrant.h"
#include "../include/Deme.h"

class World {
	public:
		World();

		void basicUnitCreator(char type, char init);
		void worldSlave();

	// 	computing functions
		int migration(); // int will be the errorcode
		void Breed(int index);
		void globalBreeding();
		int getNumberOfDescendants(double fitness); // for imigrants

	// 	testing functions vector<Chromosome>& gamete
		bool isPureA(std::vector<Individual>& buffer);
		bool isPureB(std::vector<Individual>& buffer);
		bool isEmpty();

	// 	plotting functions
		void listOfParameters() const;
		void listOfNumericalParameters(std::ostream& stream) const;
		void listOfDemes();
		int summary(std::ostream& stream);
		void showOneDeme(int index);
		int saveTheUniverse(std::string type, std::string filename);
		void getLD();

	// 	parameter changing functions
		void setHeight(int heig);
		void setWidth(int width);
		void setLREdgesType(std::string ed_type);
		void setUDEdgesType(std::string ed_type);
		void setDimension(int dim);
		void setNumberOfEdges(int nue);
		void setSlectionBetaLambda(double s, double b, double l);
		void setLociSelLoci(int l, int L);
		void setChromDeme(int ch, int d);

		void restart(); // clear the space, creates a new one (2 columns, rows defined by user)
		void clear();	// deletes all demes

	private:
	// 	inner methods
	// computing
		int upperBorder(int index, int max_index); // function returns index of upper neigbour for new demes
		int lowerBorder(int index, int max_index);
		int sideBorder(int reflexive, int extending);
		bool isGameteA(std::vector<Chromosome>& gamete);

	// saving methods
		int saveLinesPerIndividual(std::ofstream& ofile, std::string type);
		int saveLinesPerDeme(std::ostream& ofile, std::string type);
		int saveRaspberrypi(std::ostream& ofile);
		int saveBlocks(std::ofstream& ofile);
		int saveBacktrace(std::ofstream& ofile);

		void streamBlockSizesOf0DWorld(std::ostream& stream);

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
		int deme_size, number_of_chromosomes, number_of_loci, number_of_selected_loci;
		double selection, beta, lambda;

	//	internal
		int edges_per_deme; // defines migration and extension of hybrid zone
		int index_last_left, index_next_left; // variables handling dynamic extension of hybrid zone
		int index_last_right, index_next_right;
		
		Context context;
};

#endif /* INCLUDE_WORLD_H_ */
