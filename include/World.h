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
//		void worldSlave();
//
//
//// 		void infSimulator(vector<Imigrant>& GogolBordello, int NUMofGEN);
//
//// 		computing functions
//		int migration(); // int will be the errorcode
//		void Breed(int index);
//		void set(int index, string type);
//		void globalBreeding();
//
//// 		testing functions vector<Chromosome>& gamete
//		bool Acheck(vector<Individual>& buffer);
//		bool Bcheck(vector<Individual>& buffer);
//		bool empty(){return (world.size() == 0);};
//
//// 		plotting functions
//		void listOfParameters();
//		void listOfDemes();
//		void summary();
//		double getProportionOfHeterozygotes(int index);
//		double getProportionOfHomozygotes(int index, char type);
//		void showOneDeme(int index);
//		void viewOneDeme(int index);
//		void plotHeadOfDeme(int i);
//		void viewDemesOneByOne();
//// 		int SaveTheUniverse(int order);
//// 		int SaveTheUniverse(int order, string type);
//		int SaveTheUniverse(string type);
//		void getLD();
//
//// 		parameter changing functions
//		void setHeight(int heig);
//		void setWidth(int width);
//		void setLREdgesType(string ed_type);
//		void setUDEdgesType(string ed_type);
//		void setDimension(int dim);
//		void setNumberOfEdges(int nue);
//		void restart(); // crear the space, creates a new one (2 columns, rows defined by user)
//		void clear();	// deletes all demes
//
//	private:
// 		inner fctions
		int upper_border(int index, int max_index); // function returns index of upper neigbour for new demes
		int lower_border(int index, int max_index);
		int side_border(int reflexive, int extending);
//		int save_complete(ofstream& ofile);
//		int save_hybridIndices(ofstream& ofile);
//		int save_hybridIndicesJunctions(ofstream& ofile);
//		int save_summary(ofstream& ofile);
//		int save_line(ofstream& ofile, int index, vector<double>& vec);

// 		variables
		std::map<int, Deme*> world; // container of Demes for non zero dimensional simation
		int dimension, edges_per_deme; // defines migration and extension of hybrid zone
		int number_of_demes_l_r, number_of_demes_u_d; // defines the size of the space
		std::string type_of_l_r_edges, type_of_u_d_edges; // defines the behavior of the l / r and u /d edges of HZ
		int index_last_left, index_next_left; // variables handling dynamic extension of hybrid zone
		int index_last_right, index_next_right;
		std::vector<Imigrant> zeroD_immigrant_pool; // container of individuals for 0 dimensional simualtion
		int deme_size, number_of_chromosomes, number_of_loci;
		double selection, beta, lambda;
};

#endif /* INCLUDE_WORLD_H_ */
