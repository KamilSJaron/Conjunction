#include "deme.h"
// v0.141107

using namespace std;

// deme creator will have these arguments:
// int dimension = 0 - 2 (popr 3, N, but 2 is working so far)
// int edge_per_deme = 2 - F
// string edges_l_r = ('reflexive','absorbing','extending','warping','population_border')
// string edges_u_d = ('reflexive','absorbing','extending','warping','population_border')
// number_of_demes_l_r = 0 - Q
// number_of_demes_u_d = 0 - W

class Universe  
{
	public:
// 		declarations / decl fctions
		Universe(int dimension, int edges_per_deme, int number_of_demes_l_r,string edges_l_r, int number_of_demes_u_d, string edges_u_d);
		Universe();
		void basicUnitCreator(char type, char init);
		
// 		computing functions
		int migration(); // int will be the errorcode
		void globalNaiveBreeding();
		void globalBreeding();
		bool Acheck(vector<Individual> buffer);
		bool Bcheck(vector<Individual> buffer);
		
// 		plotting functions
		void listOfDemes();
		void showOneDeme(int index);
		void viewOneDeme(int index);
		void plotOneDeme(int index);
		void plotOneDeme(int index, string fileName);
		void plotHeadOfDeme(int i);
		void plotDemesOneByOne();
		
// 		parameter changing functions
		void setHeight(int heig);
		void setLREdgesType(string ed_type);
		
		int getNumOfDemesInColumn(){return number_of_demes_u_d;};
		int getSpaceSize(){return space.size();};
		int getIndex(int i);
	
	private:
// 		inner fctions
		int upper_border(int index, int max_index);
		int lower_border(int index, int max_index);
		int side_border(int reflexive, int extending);
// 		variables
// 		vector<Deme*> world;
		map<int, Deme*> space;
				
		int dimension, edges_per_deme;
		int number_of_demes_l_r, number_of_demes_u_d;
		string type_of_l_r_edges, type_of_u_d_edges;
		int index_last_left, index_next_left;
		int index_last_right, index_next_right;
};

Universe::Universe(int dim, int ed_per_deme, int num_of_demes_l_r,string ed_l_r, int num_of_demes_u_d, string edges_u_d){
	dimension = dim;
	edges_per_deme = ed_per_deme;
	number_of_demes_l_r = num_of_demes_l_r;
	type_of_l_r_edges = ed_l_r;
	number_of_demes_u_d = num_of_demes_u_d;
	type_of_u_d_edges = edges_u_d;
}

Universe::Universe(){
	dimension = 2;
	edges_per_deme = 4;
	number_of_demes_l_r = 6;
	type_of_l_r_edges = "extending";
	number_of_demes_u_d = 1;
	type_of_u_d_edges = "reflexive";
}

// I have to add char init to initiate demes as pure A, pure B, mixed or pure A/B with some incoming individuals
void Universe::basicUnitCreator(char type, char init){
	int max_index = space.size();
// 	cout << "Space size: " << max_index << endl;
	vector<int> new_indexes;
	int index;
	switch (type) {
		case 'b':
// 			case of creating all new indexes
			index_next_left = number_of_demes_u_d;
			index_next_right = number_of_demes_u_d * 2;
			index_last_left = 0;
			index_last_right = 0;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(side_border(i,index_next_left + i));
				new_indexes.push_back(i + number_of_demes_u_d * 2);
				new_indexes.push_back(upper_border(i,max_index));
				new_indexes.push_back(lower_border(i,max_index));
				space[i] = new Deme(i,new_indexes,init);
			}
			break;
		case 'l':
			index = index_next_left;
			index_next_left = max_index + 2 * number_of_demes_u_d;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(side_border(index + i,index_next_left + i));
				new_indexes.push_back(index_last_left + i);
				new_indexes.push_back(upper_border(index + i,index));
				new_indexes.push_back(lower_border(index + i,index));
				space[index + i] = new Deme(index + i,new_indexes,init);
			}
			index_last_left = index;
			break;
		case 'r':
			index = index_next_right;
			index_next_right = max_index + 2 * number_of_demes_u_d;
			for(int i=0;i<number_of_demes_u_d;i++){
				new_indexes.clear();
				new_indexes.push_back(index_last_right + i);
				new_indexes.push_back(side_border(index + i,index_next_right + i));
				new_indexes.push_back(upper_border(index + i,index));
				new_indexes.push_back(lower_border(index + i,index));
				space[index + i] = new Deme(index + i,new_indexes,init);
			}
			index_last_right = index;
			break;
		default:
			cout << "I have no idea, what do you mean by... " << type << " I understand only 'b' basic, 'l' left and 'r' right, type of basic unit.. tr it again please." << endl;
			break;
	}
	return;
}

int Universe::upper_border(int index, int max_index){
	if(type_of_u_d_edges == "reflexive"){
		if(index == max_index){
			return index;
		} else {
			return index - 1;
		}
	}
	if(type_of_u_d_edges == "extending"){
		if(index == max_index){
			return index + number_of_demes_u_d;
		} else {
			return index - 1;
		}
	}
	if(type_of_u_d_edges == "population_border"){
// 		lets say, that population_border is marked by 666 label
		if(index == max_index){
			return 666;
		} else {
			return index - 1;
		}
	}
	cout << "The type of upper-down edges is not valid." << endl;
	return -1;
}

int Universe::lower_border(int index, int max_index){
	if(type_of_u_d_edges == "reflexive"){
		if(index == max_index + number_of_demes_u_d - 1){
			return index;
		} else {
			return index + 1;
		}
	}
	if(type_of_u_d_edges == "extending"){
		if(index == max_index + number_of_demes_u_d  - 1){
			return max_index;
		} else {
			return index + 1;
		}
	}
	if(type_of_u_d_edges == "population_border"){
// 		lets say, that population_border is marked by 666 label
		if(index == max_index + number_of_demes_u_d  - 1){
			return 666;
		} else {
			return index + 1;
		}
	}
	cout << "The type of upper-down edges is not valid." << endl;
	return -1;
}

int Universe::side_border(int reflexive, int extending){
	if(type_of_l_r_edges == "reflexive"){
		return reflexive;
	}
	if(type_of_l_r_edges == "extending"){
		return extending;
// 		index_next_left + i
	}
	if(type_of_l_r_edges == "population_border"){
// 		lets say, that population_border is marked by 666 label
		return 666;
	}
	cout << "The type of left-right edges is not valid." << endl;
	return -1;
}


int Universe::migration(){
	if(space.empty()){
		cout << "Missing demes" << endl;
		return -1;
	}
	map<int, vector<Individual>> bufferVectorMap;
	
	vector<int> neigbours;
	int MigInd = (DEMEsize) / (2 * edges_per_deme );
	int deme_index;
	
	for (auto i=space.begin(); i!=space.end(); ++i){
// 		cout << i << endl;
// 		cout << "lets transfer individuals of DEME " << i << endl;
		neigbours = i->second->getNeigbours();
// 		i->second->plotDeme();
//     i->second->permutation();
// 		i->second->plotDeme();
// 		
		for(int j=0;(unsigned)j < neigbours.size();j++){
			deme_index = neigbours[j];
			for(int k=0;k < MigInd; k++){
				bufferVectorMap[deme_index].push_back(i->second->getIndividual(k));
// 				cout << "from " << world[i]->getDemeIndex() << " to " << deme_index << endl;
// 				world[i]->getIndividual(k).plotGenotype();
			}
		}
	}

	for(auto i=bufferVectorMap.begin(); i!=bufferVectorMap.end(); ++i){
// 		cout << i->first << endl;
// 			if(there are only native individuals){
// 				continue;
// 			} else {
		if(index_next_left == i->first){
			if(Acheck(i->second)){
				continue;
			}
			basicUnitCreator('l', 'A');
		} 
		if(index_next_right == i->first){
			if(Bcheck(i->second)){
				continue;
			}
			basicUnitCreator('r', 'B');
		}
// 		}
		space[i->first]->integrateMigrantVector(i->second);
	}
	return 0;
}

void Universe::globalNaiveBreeding(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		i->second->quickBreed();
	}
}

void Universe::globalBreeding(){
	vector<int> indexes;
	for (auto i=space.begin(); i!=space.end(); ++i){
		indexes.push_back(i->first);
	}
	
	int index = 0;
	int i_size = indexes.size();
	
// 	#pragma omp parallel for
	for(int i = 0; i < i_size; i++){
		index = indexes[i];
		space[index]->Breed();
	}
}

bool Universe::Acheck(vector< Individual > buffer){
	for(int i = 0; (unsigned)i < buffer.size(); i++){
		if(buffer[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Universe::Bcheck(vector< Individual > buffer){
	for(int i = 0; (unsigned)i < buffer.size(); i++){
		if(buffer[i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

// // // // // // // // // // // // // //
// // // // setting functions // // // //
// // // // // // // // // // // // // //

void Universe::setHeight(int heig){
	if(space.size() == 0){
		number_of_demes_u_d = heig;
	} else {
		cout << "It is not possible to change parameter height once, you create world";
	}
}

void Universe::setLREdgesType(string ed_type){
	type_of_l_r_edges = ed_type;
}

int Universe::getIndex(int i){
	if((unsigned)i >= space.size()){
		return -1;
	} else {
		return space[i]->getDemeIndex();
	}
}

  // // // // 
 //  PLOT //
// // // //

void Universe::listOfDemes(){
	int worlsize = space.size();
	cout << "World of size " << worlsize << endl;
	cout << "Number of demes up to down: " << number_of_demes_u_d << endl;
	cout << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
	if(type_of_l_r_edges != "extending"){
		cout << "Number of demes left to right: " << number_of_demes_l_r << endl;
	}
	cout << "Type of borders left to right: " << type_of_l_r_edges << endl;
	cout << "                 EDGE" << endl;
	cout << setw(7) << right << "DEME " << setw(7) << left << " LEFT" << setw(6) << left << "RIGHT" << setw(5) << left << "UP" << setw(6) << left << "DOWN" << endl;
	for (auto i=space.begin(); i!=space.end(); ++i){
		i->second->showDeme();
	}
}

void Universe::showOneDeme(int index){
	space[index]->showDeme();
}

void Universe::viewOneDeme(int index){
	space[index]->viewDeme();
}

void Universe::plotOneDeme(int index){
	space[index]->plotDeme();
}

void Universe::plotOneDeme(int index, string fileName){
	space[index]->plotDeme(fileName);
}

void Universe::plotHeadOfDeme(int index){
	space[index]->plotHeadOfDeme();
}

void Universe::plotDemesOneByOne(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		cout << "***** DEME " << i->first << " *****" << endl;
		i->second->viewDeme();
		cin.get();
	}
}












