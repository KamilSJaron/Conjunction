/*
Deme is set of admixing individuals, picking of parents is relative to ther fitness.
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
#include <iomanip>
#include <fstream>

#include "../include/Chromosome.h"
#include "../include/Individual.h"
#include "../include/SelectionModel.h"
#include "../include/Deme.h"

using namespace std;

// fctions
double selectionRand(){
		return (double)rand() / RAND_MAX;
}


/* DECLARATION */

// // // // // // // // // // // // // //
// constructor/destructors functions / //
// // // // // // // // // // // // // //

Deme::Deme(int ind, double sel, double beta){
	index = ind;
	deme_size = 0;
	deme = new Individual[deme_size];

	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}


Deme::Deme(std::vector<int> neigb, double sel, double beta){
	neigbours = neigb;
	index = 0;
	deme_size = 0;
	deme = new Individual[deme_size];
	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}


Deme::Deme(int ind, vector<int> neigb, int size, double sel, double beta){
	index = ind;
	neigbours = neigb;
	deme_size = size;
	deme = new Individual[deme_size];
	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}

Deme::Deme(int ind, vector<int> neigb, char init, int size, double sel, double beta){
	deme_size = size;
	deme = new Individual[deme_size];
	index = ind;
	neigbours = neigb;
	if(init == 'A' or init == 'B'){
		Individual temp(init);
		for(int i=0;i<deme_size;i++){
			deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (deme_size / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< deme_size){
			deme[i] = tempB;
			i++;
		}
	}
	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}

Deme::Deme(int ind, char init, int size, double sel, double beta){
	deme_size = size;
	deme = new Individual[deme_size];
	index = ind;
		if(init == 'A' or init == 'B'){
			Individual temp(init);
			for(int i=0;i<deme_size;i++){
				deme[i] = temp;
		}
	} else {
		Individual tempA('A');
		Individual tempB('B');
		int i = 0;
		while(i< (deme_size / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< deme_size){
			deme[i] = tempB;
			i++;
		}
	}
	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}

Deme::Deme(int ind, std::vector<int> neigb, char init, int size, double sel, double beta, int in_ch, int in_loc, double in_lambda){
	deme_size = size;
	deme = new Individual[deme_size];
	index = ind;
		if(init == 'A' or init == 'B'){
			Individual temp(init, in_ch, in_loc, in_lambda);
			for(int i=0;i<deme_size;i++){
				deme[i] = temp;
		}
	} else {
		Individual tempA('A', in_ch, in_loc, in_lambda);
		Individual tempB('B', in_ch, in_loc, in_lambda);
		int i = 0;
		while(i< (deme_size / 2)){
			deme[i] = tempA;
			i++;
		}
		while(i< deme_size){
			deme[i] = tempB;
			i++;
		}
	}
	selection_model.setSelectionPressure(sel);
	selection_model.setBeta(beta);
}


Deme::~Deme(){
	delete[] deme;
}

// // // // // // // // // // // // // // //
// // // // comunication functions / // // /
// // // // // // // // // // // // // // //

std::vector<int> Deme::getNeigbours(){
	return neigbours;
}

Individual Deme::getIndividual(int ind){
	return deme[ind];
}

int Deme::getDemeIndex(){
	return index;
}

void Deme::setDemeSize(int size){
	deme_size = size;
}

int Deme::getDemeSize(){
	return deme_size;
};

// // // // // // // // // // // // // //
// // // // computing functions / // // /
// // // // // // // // // // // // // //

void Deme::Breed(){
	double lambda = deme[0].getLambda();
	vector<double> fitnessVector;
	vector<Chromosome> gamete1, gamete2;
	getFitnessVector(fitnessVector);
	double RandMax = fitnessVector[deme_size-1];
	double roll;
	map<double, int> parentPick;
	map<double, int>::iterator it;
	vector<int> mothers(deme_size);
	vector<int> fathers(deme_size);

	for(int i=0;i < deme_size*2;i++){
		roll = (selectionRand() * RandMax);
		it = parentPick.find(roll);
		while(it != parentPick.end()){ //rolling twice same number will overwrite the first one, but this event is so rare, that solution is to just roll again without any bias
			roll = (selectionRand() * RandMax);
			it = parentPick.find(roll);
		}
		parentPick[roll] = i;
	}

	int i = 0;
	map<double, int>::iterator pos=parentPick.begin();

	while(pos!=parentPick.end()){

		if(fitnessVector[i] >= pos->first){
			if(pos->second >= deme_size){
				mothers[pos->second - deme_size] = i;
			} else {
				fathers[pos->second] = i;
			}
			++pos;
		} else {
			i++;
		}
	}

	Individual *metademe = new Individual[deme_size];
	for(int i=0;i<deme_size;i++){
		deme[mothers[i]].makeGamete(gamete1);
		deme[fathers[i]].makeGamete(gamete2);
		metademe[i] = Individual(gamete1,gamete2,lambda);
	}

	for(int i=0;i<deme_size;i++){
		deme[i] = metademe[i];
	}
	delete[] metademe;
}

void Deme::permutation(){
	int j = 0;
for(int i = 0;i<deme_size;i++){
		j = rand() % (deme_size - i);
		j += i;
		swap(i, j);
	}
}

void Deme::integrateMigrantVector(vector<Individual>& migBuffer){
	unsigned int i = 0;
	while(i < migBuffer.size()){
		deme[i] = migBuffer[i];
		i++;
	}
	return;
}


// vector<double>
void Deme::getBproportions(vector<double>& props) const{
	props.clear();
	props.reserve(deme_size);
	for(int i = 0;i < deme_size;i++){
		props.push_back(deme[i].getBprop());
	}
	return;
}

void Deme::getJunctionNumbers(vector< double >& juncs) const{
	juncs.clear();
	juncs.reserve(deme_size);
	for(int i = 0;i < deme_size;i++){
		juncs.push_back(deme[i].getNumberOfJunctions());
	}
	return;
}


void Deme::getHeterozygoty(vector< double >& heterozs){
	heterozs.clear();
	heterozs.reserve(deme_size);
	for(int i = 0;i < deme_size;i++){
		heterozs.push_back(deme[i].getHetProp());
	}
	return;
}

double Deme::getMeanBproportion() const{
	double props = 0;
	for(int i = 0;i < deme_size;i++){
		props += deme[i].getBprop();
	}
	return (props / deme_size);
}


double Deme::getProportionOfHomozygotes(char population) const{
	double proportion = 0;
	if(population == 'A'){
		for(int i = 0;i < deme_size;i++){
			proportion += (deme[i].Acheck());
		}
		proportion = proportion / deme_size;
		return proportion;
	}
	if(population == 'B'){
		for(int i = 0;i < deme_size;i++){
			proportion += (deme[i].Bcheck());
		}
		proportion = proportion / deme_size;
		return proportion;
	}
	return 0;
}


double Deme::getProportionOfHeterozygotes() const{
	double proportion = 0;
	for(int i = 0;i < deme_size;i++){
		proportion += (deme[i].Acheck());
		proportion += (deme[i].Bcheck());
	}
	proportion = 1 - (proportion / deme_size);
	return proportion;
}


void Deme::getFitnessVector(vector<double> &fitnessVector){
	double sum = 0, read_fitness = 0;
	fitnessVector.reserve(deme_size);
	for(int i = 0;i < deme_size;i++){
		read_fitness = selection_model.getFitness(deme[i].getHetProp());
		sum += read_fitness;
		fitnessVector.push_back(sum);
	}
	return;
}

// Computes average fitness in the deme
double Deme::getMeanFitness(){
	double sum = 0, read_fitness = 0, read_heterozygocity = 0;
	for(int i = 0;i < deme_size;i++){
		read_heterozygocity = deme[i].getHetProp();
		read_fitness = selection_model.getFitness(read_heterozygocity);
		sum += read_fitness;
	}
	return sum / deme_size;
}


double Deme::getVARhi() const{
	double varz = 0, z = getMeanBproportion();
	for(int i = 0; i < deme_size; i++){
		varz += pow(z - deme[i].getBprop(),2);
	}
	varz = varz / deme_size;
	return varz;
}

double Deme::getVARp(){
	// number of chromosomes / loci is called form first individual of deme, and its first chromosome
	int number_chromosomes = deme[0].getNumberOfChromosomes(), number_loci = deme[0].getNumberOfLoci(0);
	double p = 0, varp = 0, pmean = getMeanBproportion();
	vector<double> ps;
	for(int ch = 0; ch < number_chromosomes; ch++){
		getps(ps,ch);
		for(unsigned int locus = 0;locus < ps.size();locus++){
			p = ps[locus];
			varp += (pmean - p) * (pmean - p);
		}
		ps.clear();
	}
	varp = varp / (number_loci*number_chromosomes);
	return varp;
}

void Deme::getps(vector<double>& ps, int ch){
	double p = 0;
	int TotalCHnum = deme_size * 2; //total number of chromosomes in deme (which one is computed using parameter ch)
	int number_loci = deme[0].getNumberOfLoci(0);
	vector<bool> states;	// 0 'A', 1 'B'
	vector<map<int, char>::iterator> chroms; //vector with chromosome junctions
	vector<int> ch_sizes; //vector with number of chromosome junctions

	chroms.reserve(TotalCHnum);
	ch_sizes.reserve(TotalCHnum);
	states.reserve(TotalCHnum);
	ps.reserve(number_loci);

	for(int i = 0;i < deme_size;i++){
		if(deme[i].getChromosomeBegining(0,ch)->second == 'A'){
			states.push_back(0);
		} else {
			states.push_back(1);
		}
		if(deme[i].getChromosomeBegining(1,ch)->second == 'A'){
			states.push_back(0);
		} else {
			states.push_back(1);
		}
		chroms.push_back(deme[i].getChromosomeBegining(0,ch));
		chroms.push_back(deme[i].getChromosomeBegining(1,ch));
		ch_sizes.push_back(deme[i].getNumberOfJunctions(0,ch));
		ch_sizes.push_back(deme[i].getNumberOfJunctions(1,ch));
	}
	for(int i = 0;i < number_loci;i++){
// 		I have to update states every iteration
		for(int j = 0;j < TotalCHnum;j++){
			if(chroms[j]->first == i){
				if(chroms[j]->second == 'A'){
					states[j] = 0;
				} else {
					states[j] = 1;
				}
				if(ch_sizes[j] != 0){
					chroms[j]++;
					ch_sizes[j] -= 1;
				}
			}
		}
		p = sum(states) / (double)(deme_size * 2);
		ps.push_back(p);
	}
}



double Deme::getLD(){
	double LD = 0, varz = getVARhi(), z = getMeanBproportion(), varp = getVARp(), n = deme[0].getNumberOfLoci(0); //LOCI

	LD = (varz - ((1 / (2 * n)) * ((z * (1 - z)) - varp))) / (0.5 * (1 - (1 / n))) ;

	return LD;
}

double Deme::getLD(double z,double varz,double varp){
	double LD = 0, n = deme[0].getNumberOfLoci(0);

	LD = (varz - ((1 / (2 * n)) * ((z * (1 - z)) - varp))) / (0.5 * (1 - (1 / n))) ;

	return LD;
}



// // // // // // // // // // // // // //
// // // // plotting functions / // // //
// // // // // // // // // // // // // //

void Deme::showDeme(){
	int neigbsize = neigbours.size();
	cerr << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cerr << setw(5) << left << neigbours[i] << " ";
	}
	cerr << endl;
}

void Deme::summary(){
	int number_chromosomes = deme[0].getNumberOfChromosomes(), number_loci = deme[0].getNumberOfLoci(0);
	double z = getMeanBproportion();
	double varz = getVARhi();
	double varp = getVARp();
	int neigbsize = neigbours.size();
	cout << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		cout << setw(5) << left << neigbours[i] << " ";
	}
	cout << setw(12) << left << ((round(getMeanFitness() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(getProportionOfHeterozygotes() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(z * 1000000)) / 1000000)
	<< setw(12) << left << ((round(varz * 1000000)) / 1000000);
	if(number_loci * number_chromosomes > 1){
		cout	<< setw(12) << left << ((round(varp * 1000000)) / 1000000)
		<< setw(12) << left << ((round(getLD(z,varz,varp) * 1000000)) / 1000000);
	}
	if((number_loci * number_chromosomes) <= 16){
		vector<double> ps;
		for(int ch = 0;ch < number_chromosomes; ch++){
			ps.clear();
			getps(ps,ch);
			for(unsigned int l = 0; l < ps.size();l++){
				cout << setw(12) << left << ((round(ps[l] * 10000)) / 10000);
			}
		}
	}
	cout << endl;
}


void Deme::summary(ofstream& ofile){
	int number_chromosomes = deme[0].getNumberOfChromosomes(), number_loci = deme[0].getNumberOfLoci(0);
	double z = getMeanBproportion();
	double varz = getVARhi();
	double varp = getVARp();
	int neigbsize = neigbours.size();
	ofile << setw(5) << right << index << ":  ";
	for(int i = 0; i < neigbsize; i++){
		ofile << setw(5) << left << neigbours[i] << " ";
	}
	ofile << setw(12) << left << ((round(getMeanFitness() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(getProportionOfHeterozygotes() * 1000000)) / 1000000)
	<< setw(12) << left << ((round(z * 1000000)) / 1000000)
	<< setw(12) << left << ((round(varz * 1000000)) / 1000000);
	if(number_loci * number_chromosomes > 1){
		ofile	<< setw(12) << left << ((round(varp * 1000000)) / 1000000)
		<< setw(12) << left << ((round(getLD(z,varz,varp) * 1000000)) / 1000000);
	}
	if((number_loci * number_chromosomes) <= 16){
		vector<double> ps;
		for(int ch = 0;ch < number_chromosomes; ch++){
			ps.clear();
			getps(ps,ch);
			for(unsigned int l = 0; l < ps.size();l++){
				ofile << setw(12) << left << ((round(ps[l] * 10000)) / 10000);
			}
		}
	}
	ofile << endl;
}

void Deme::readAllGenotypes(){
	for(int i=0;i<deme_size;i++){
		cerr << "Individual: " << i << " B proportion: " << deme[i].getBprop() << endl;
		deme[i].readGenotype();
	}
}


void Deme::readGenotypeFrequencies(){
	int number_chromosomes = deme[0].getNumberOfChromosomes(), number_loci = deme[0].getNumberOfLoci(0);
	vector<double> freqs;
	freqs.reserve(number_chromosomes*number_loci*2 + 1);
	for(int i=0;i < number_chromosomes*number_loci*2 + 1;i++){
		freqs.push_back(0);
	}
	for(int i=0;i<deme_size;i++){
		freqs[deme[i].getBcount()]++;
	}
	for(int i=0;i < number_chromosomes*number_loci*2 + 1;i++){
		cout << ((round((freqs[i] / deme_size) * 10000)) / 10000) << ' ';
	}
}

 // // // // //
//  PRIVATE //
// // // // //

void Deme::swap(int ind1, int ind2){
	Individual tempInd;
	tempInd = deme[ind2];
	deme[ind2] = deme[ind1];
	deme[ind1] = tempInd;
}

int Deme::pickAnIndividual(){
		return rand() % deme_size;
}

int Deme::sum(vector< bool >& ve){
	int sum = 0;
	for(unsigned int i = 0; i < ve.size();i++){
		sum += ve[i];
	}
	return sum;
}
