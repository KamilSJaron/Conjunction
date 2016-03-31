/*
 * SelectionModel.cpp
 *
 *  Created on: Mar 31, 2016
 *      Author: kjaron
 */

#include <cmath>

#include "../include/SelectionModel.h"

SelectionModel::SelectionModel() {
	selection_pressure = 0.5;
	beta = 1;
}

SelectionModel::SelectionModel(double in_selection, double in_beta) {
	selection_pressure = in_selection;
	beta = in_beta;
}

void SelectionModel::setSelectionPressure(double Sp){
	selection_pressure = Sp;
}

void SelectionModel::setBeta(double in_beta){
	beta = in_beta;
}

double SelectionModel::getFitness(double het){
	return 1 - (selection_pressure * pow( 4 * het * (1 - het),beta));
}
