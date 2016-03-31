/*
 * SelectionModel.cpp
 *
 *  Created on: Mar 31, 2016
 *      Author: kjaron
 */

#include "../include/SelectionModel.h"

SelectionModel::SelectionModel() {
	selection_pressure = 0.5;
	beta = 1;
}

SelectionModel::SelectionModel(double in_selection, double in_beta) {
	selection_pressure = in_selection;
	beta = in_beta;
}
