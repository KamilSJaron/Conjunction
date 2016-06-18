/*
 * Dispatcher.h
 *
 *  Created on: Jun 17, 2016
 *      Author: kjaron
 */

#ifndef SRC_DISPATCHER_H_
#define SRC_DISPATCHER_H_

class Dispatcher {
public:
	Dispatcher(std::string filename);
//	 ~Dispatcher(){ delete simulations; };

	void printSimulationSetting();

private:
	int parseSetting(std::ifstream& myfile);
	int parseWorldDefinition(std::string& line);
	void parameterSave(std::string& parameter, double value);
	void parameterSave(std::string& parameter, std::vector<double>& value);

	std::vector<SimulationSetting> simulations;
};

#endif /* SRC_DISPATCHER_H_ */
