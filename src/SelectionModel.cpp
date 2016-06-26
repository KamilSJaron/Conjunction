/*
Selection model emplyed by deme for computing fiteness of individuals.
Copyright (C) 2016  Kamil S. Jaron

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cmath>

#include "../include/SelectionModel.h"

SelectionModel::SelectionModel() {
	selection_pressure = -1;
	beta = -1;
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
