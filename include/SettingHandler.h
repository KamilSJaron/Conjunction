/*
 * Dispatcher.h
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

class SettingHandler{
	public:
		SettingHandler(std::string filename);

		SimulationSetting getSimualtionSetting(int index) const;
		int getNumberOfSimulations() const;

		// testing
		bool checkParameters();
		void fillDefault();

		// printing functions
		void printParameters() const;
		void printWorld() const;
		void printParameterCounts() const;
		//	void plotAsciiWorld();

	private:
		int parseSetting(std::ifstream& myfile);
		int parseWorldDefinition(std::string& line);
		void parameterSave(std::string& parameter, double value);
		void parameterSave(std::string& parameter, std::vector<double>& value);
		void updateNumberOfSimulations();
		template<typename T>
		int printVectorValue(unsigned int index, std::vector<T> val_vec) const;
		char setParameterOfSetting(SimulationSetting& mySetting, std::string parameter, int index) const;
		void adjustFileName(std::string& name, char par, int total, int refactorised_index) const;

		// simulation parameters

		std::vector<int> loci, chrom, deme, gen;
		std::vector<double> sel, beta, lambda;

		// capture output parameters
		std::string file_name_patten, type_of_save;
		std::vector<int> seed, delay, saves;

		// world setting

		int up_down_demes, left_right_demes, dimension, edges_per_deme;
		std::string type_of_updown_edges, type_of_leftright_edges;

		// internals
		int number_of_simulations, replicates;
		std::vector<std::string> parameters_in_order;
		std::vector<int> parameters_numbers;
};

#endif /* SRC_DISPATCHER_H_ */
