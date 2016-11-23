/*
 * SelectionModel.h
 *
 *  Created on: Mar 31, 2016
 *      Author: kjaron
 */

#ifndef INCLUDE_SELECTIONMODEL_H_
#define INCLUDE_SELECTIONMODEL_H_

class SelectionModel {
public:
	SelectionModel();
	SelectionModel(double in_selection, double in_beta);
	void setSelectionPressure(double Sp);
	void setBeta(double in_beta);
	double getFitness(double hybrid_index);

private:
	double selection_pressure;
	double beta;
};

#endif /* INCLUDE_SELECTIONMODEL_H_ */
