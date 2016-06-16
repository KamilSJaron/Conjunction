/*
conjunction: simulator of secondary contact using Fisher's representation of genome admixture
Copyright (C) 2013-2016  Kamil S. Jaron

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

#include <iostream>
#include <map>
#include <vector>
#include <cmath>

#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"

using namespace std;

int main(int argc, char **argv)
{
    if(argc > 1){
    	if(strcmp( argv[1], "--help") == 0){
    		cerr << "all parameters are read form file setting.txt; details are online at wiki" << endl;
    		return 0;
    	}
    	if(strcmp( argv[1], "--version") == 0){
    		cerr << "Forsim v1.2.development" << endl;
    		cerr << "... on the way to be a nice program" << endl;
    	}

    }
    

    return 0;
}
