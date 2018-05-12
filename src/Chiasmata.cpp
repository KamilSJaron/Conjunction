/*
Implemenation of class Chiasmata.
Copyright (C) 2018  Kamil S. Jaron

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

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "../include/Chiasmata.h"

using namespace std;

/* INITIATION */

Chiasmata::Chiasmata(){ }

Chiasmata::Chiasmata(std::vector<int> input_chiasmata){
	chiasmata = input_chiasmata;
}

Chiasmata::~Chiasmata(){
	chiasmata.clear();
}

int Chiasmata::length() const{
	return chiasmata.size();
}

int Chiasmata::get(int index) const{
	if( index > length()){
		throw runtime_error("Problematic request on chiasmata.get(index), index bigger than number of chiasmata");
	}
	return chiasmata[index];
}

void Chiasmata::set(std::vector<int> input_chiasmata){
	chiasmata = input_chiasmata;
}

void Chiasmata::add(int chiasma){
	chiasmata.push_back(chiasma);
	sort(chiasmata.begin(), chiasmata.end());
}

std::string Chiasmata::collapse() const{
	if( length() == 0){
		return "~";
	}
	string collapsed = "";
	for(int chiasma = 0; chiasma < length(); chiasma++){
		if(collapsed == ""){
			collapsed = to_string(chiasmata[chiasma]);
		} else {
			collapsed = collapsed + "," + to_string(chiasmata[chiasma]);
		}
	}
	return collapsed;
}