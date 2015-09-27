#include "deme.h"

using namespace std;

static string NAMEofOUTPUTfile = "out";
static string TYPEofOUTPUTfile = "summary";

int getNumberOfDescendants(double fitness){
	int result = 0;
	double q = exp(-fitness);
	double p = q;
	double roll = uniform();
	while(roll > q){
		result++;
		p = p * fitness / result;
		q = q + p;
	}
	return result;
}

class Universe  
{
	public:
// 		declarations / decl fctions
		Universe(int dimension, int edges_per_deme, int number_of_demes_l_r,string edges_l_r, int number_of_demes_u_d, string edges_u_d);
		Universe();
		void basicUnitCreator(char type, char init);
		void worldSlave();
		
		
// 		void infSimulator(vector<Imigrant>& GogolBordello, int NUMofGEN);
		
// 		computing functions
		int migration(); // int will be the errorcode
		void Breed(int index);
		void set(int index, string type);
		void globalBreeding();
		
// 		testing functions vector<Chromosome>& gamete
		bool Acheck(vector<Individual>& buffer);
		bool Bcheck(vector<Individual>& buffer);
		bool empty(){return (space.size() == 0);};
		
// 		plotting functions
		void listOfParameters();
		void listOfDemes();
		void summary();
		double getProportionOfHeterozygotes(int index);
		double getProportionOfHomozygotes(int index, char type);
		void showOneDeme(int index);
		void viewOneDeme(int index);
		void plotHeadOfDeme(int i);
		void viewDemesOneByOne();
// 		int SaveTheUniverse(int order);
// 		int SaveTheUniverse(int order, string type);
		int SaveTheUniverse(string type);
		void getLD();
		
// 		parameter changing functions
		void setHeight(int heig);
		void setWidth(int width);
		void setLREdgesType(string ed_type);
		void setUDEdgesType(string ed_type);
		void setDimension(int dim);
		void setNumberOfEdges(int nue);
		void restart(); // crear the space, creates a new one (2 columns, rows defined by user)
		void clear();	// deletes all demes
	
	private:
// 		inner fctions
		int upper_border(int index, int max_index); // function returns index of upper neigbour for new demes
		int lower_border(int index, int max_index);
		int side_border(int reflexive, int extending);
		int save_complete(ofstream& ofile);
		int save_hybridIndices(ofstream& ofile);
		int save_hybridIndicesJunctions(ofstream& ofile);
		int save_summary(ofstream& ofile);
		int save_line(ofstream& ofile, int index, vector<double>& vec);
		
// 		variables
		map<int, Deme*> space; // container of Demes for non zero dimensional simation
		int dimension, edges_per_deme; // defines migration and extension of hybrid zone
		int number_of_demes_l_r, number_of_demes_u_d; // defines the size of the space
		string type_of_l_r_edges, type_of_u_d_edges; // defines the behavior of the l / r and u /d edges of HZ
		int index_last_left, index_next_left; // variables handling dynamic extension of hybrid zone
		int index_last_right, index_next_right;
		vector<Imigrant> zeroD_immigrant_pool; // container of individuals for 0 dimensional simualtion
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
// 	melo by to fungovat i z temahle nesmyslnyma hodnotama, abych si byl jisty, ze vdycky budou vsechny parametry nastaveny
// 	dimension = -1;
// 	edges_per_deme = 0;
	number_of_demes_l_r = 0;
// 	type_of_l_r_edges = "";
// 	number_of_demes_u_d = 0;
// 	type_of_u_d_edges = "";
}

// char init initiates demes as pure A, pure B, mixed or pure A/B with some incoming individuals
void Universe::basicUnitCreator(char type, char init){
	int max_index = space.size();
	vector<int> new_indexes;
	int index;
// 	1D world definition
	if(dimension == 1){
		switch (type) {
			case 'b':
				index_next_left = 1;
				index_next_right = 2;
				index_last_left = 0;
				index_last_right = 0;
				new_indexes.clear();
				new_indexes.push_back(side_border(0,index_next_left));
				if(number_of_demes_l_r == 1){
					new_indexes.push_back(side_border(0,index_next_right));
				} else {
					new_indexes.push_back(index_next_right);
				}
				space[0] = new Deme(0,new_indexes,init);
				break;
			case 'l':
				new_indexes.clear();
				new_indexes.push_back(max_index + 2);
				new_indexes.push_back(index_last_left);
				space[index_next_left] = new Deme(index_next_left,new_indexes,init);
				index_last_left = index_next_left;
				index_next_left = max_index + 2;
				break;
			case 'r':
				new_indexes.clear();
				new_indexes.push_back(index_last_right);
				if(type_of_l_r_edges == "wrapping"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(0);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}
				if(type_of_l_r_edges == "reflexive"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(index_next_right);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}
				if(type_of_l_r_edges == "extending"){
					new_indexes.push_back(max_index + 2);
				}
				if(type_of_l_r_edges == "infinite"){
					if(index_next_right == number_of_demes_l_r){
						new_indexes.push_back(-8);
					} else {
						new_indexes.push_back(max_index + 2);
					}
				}
				
				space[index_next_right] = new Deme(index_next_right,new_indexes,init);
				index_last_right = index_next_right;
				index_next_right = max_index + 2;
				break;
				
		}
		return;
	}
	
// 	2D world definition
	switch (type) {
		case 'b':
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
			cerr << "Error: I have no idea, what do you mean by... " << type << " I understand only 'b' basic, 'l' left and 'r' right, type of basic unit.. try it again please." << endl;
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
	if(type_of_u_d_edges == "wrapping"){
		if(index == max_index){
			return index + (number_of_demes_u_d-1);
		} else {
			return index - 1;
		}
	}
	cerr << "Error: The type of upper-down edges is not valid." << endl;
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
	if(type_of_u_d_edges == "wrapping"){
		if(index == max_index + number_of_demes_u_d  - 1){
			return index - (number_of_demes_u_d-1);
		} else {
			return index + 1;
		}
	}
	cerr << "Error: The type of upper-down edges is not valid." << endl;
	return -1;
}

int Universe::side_border(int reflexive, int extending){
	if(type_of_l_r_edges == "reflexive"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			return reflexive;
		}
	}
	if(type_of_l_r_edges == "extending"){
		return extending;
	}
	if(type_of_l_r_edges == "wrapping"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			if(reflexive < number_of_demes_u_d){
				return number_of_demes_l_r * number_of_demes_u_d + reflexive % number_of_demes_u_d;
			} else {
				return reflexive % number_of_demes_u_d;
			}
		}
	}
	if(type_of_l_r_edges == "infinite"){
		if(reflexive < number_of_demes_l_r * number_of_demes_u_d and reflexive > number_of_demes_u_d){
			return extending;
		} else {
			return -8;
		}
	}
	cerr << "Error: The type of left-right edges is not valid." << endl;
	return -1;
}

// int save_complete();
// 		int save_hybridInces();
// 		int save_hybridIncesJunctions();
// 		int save_summary();
int Universe::save_complete(ofstream& ofile){
	cerr << "WARNING: The output was not sucesfully saved to: " << NAMEofOUTPUTfile << endl;
	cerr << "Saving of complete record is not implemented yet!!!" << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}


int Universe::save_hybridIndices(ofstream& ofile){
	int index = index_last_left;
	vector<double> props;
	for(unsigned int i = 0; i < space.size(); i++){
		space[index]->getBproportions(props);
		save_line(ofile,index,props);
		if(index != index_last_right){
			index = space[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
	cerr << "The output was sucesfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}

int Universe::save_hybridIndicesJunctions(ofstream& ofile){
	int index = index_last_left;
	vector<double> props;
	for(unsigned int i = 0; i < space.size(); i++){
		space[index]->getBproportions(props);
		save_line(ofile,index,props);
// 		getSizesOfBBlocks neni dobra fce, musim napsat novou... Tohle je na test konstruktu.
		space[index]->getJunctionNumbers(props);
		save_line(ofile,index,props);
		if(index != index_last_right){
			index = space[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
	cerr << "The output was sucesfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}

int Universe::save_line(ofstream& ofile, int index, vector< double >& vec){
	ofile << index << '\t';
	for(unsigned int ind = 0; ind < vec.size(); ind++){
		ofile << vec[ind] << '\t';
	}
	ofile << endl;
	return 0;
}



int Universe::save_summary(ofstream& ofile){
	int worlsize = space.size();
	cerr << "World of size " << worlsize << endl;
	cerr << "of dimension: " << dimension << endl;
	cerr << "Number of demes up to down: " << number_of_demes_u_d << endl;
	cerr << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
	if(type_of_l_r_edges != "extending"){
		cerr << "Number of demes left to right: " << number_of_demes_l_r << endl;
	}
	cerr << "Type of borders left to right: " << type_of_l_r_edges << endl;
	ofile << "                 EDGE" << endl;
	ofile << setw(7) << right << "DEME " 
	<< setw(7) << left << " LEFT" 
	<< setw(6) << left << "RIGHT"; 
	if(dimension == 2){
		ofile << setw(6) << left << "UP" 
		<< setw(6) << left << "DOWN";
	}
	ofile << setw(12) << left << "mean f"
	<< setw(12) << left << "f(heter)"
	<< setw(12) << left << "meanHI" 
	<< setw(12) << left << "var(HI)";
	if(LOCI * NUMBERofCHROMOSOMES > 1){
		ofile << setw(12) << left << "var(p)" 
		<< setw(12) << left << "LD";
	}

	if(LOCI * NUMBERofCHROMOSOMES <= 16){
		for(int ch = 0;ch < NUMBERofCHROMOSOMES;ch++){
			for(int l = 0; l < LOCI;l++){
				ofile << left << "Ch" << ch+1 << "l" << l+1 << setw(7) << ' ';
			}
		}
	}
	ofile << endl;
	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
		i->second->summary(ofile);
	}

	cerr << "The output was sucesfully saved to: " << NAMEofOUTPUTfile << endl;
	ofile.close();
	return 0;
}


void Universe::worldSlave(){
	if(number_of_demes_l_r == 1){
		basicUnitCreator('b', 'C');
		return;
	}
	if(number_of_demes_l_r == 2){
		basicUnitCreator('b', 'A');
		basicUnitCreator('r', 'B');
		return;
	}
	
	double midpoint = double(number_of_demes_l_r + 1) / 2;
	basicUnitCreator('b', 'A');
	
	for(double i = 2; i < number_of_demes_l_r; i++){
		if(i < midpoint){
			basicUnitCreator('r', 'A');
		} else {
			basicUnitCreator('r', 'B');
		}
	}
	basicUnitCreator('r', 'B');
	
	return;	
}


int Universe::migration(){
	const int demesize = Deme::getDEMEsize();
	if(dimension == 0){
		zeroD_immigrant_pool.reserve(demesize);
		for(int i = 0; i < demesize;i++){
			zeroD_immigrant_pool.push_back(Imigrant('B'));
		}
// 		cerr << "GB size: " <<	GogolBordello.size() << endl;
		return 0;
	}
	if(space.empty()){
		cerr << "ERROR: Missing demes" << endl;
		return -1;
	}
	if(edges_per_deme == 0){
		return 0;
	}

	int index_last_left_fix = index_last_left;
	int index_last_right_fix = index_last_right;
	map<int, vector<Individual> > ImmigranBuffer;
	
	vector<int> neigbours;
	int MigInd = demesize / (2 * edges_per_deme );
	int deme_index;
	/*bufferVectorMap is container for all individuals imigrating to all demes*/
	for (map<int, Deme*>::const_iterator deme=space.begin(); deme!=space.end(); ++deme){
		neigbours = deme->second->getNeigbours();
		for(unsigned int j=0;j < neigbours.size();j++){
			deme_index = neigbours[j];
			if(deme_index == -8){
				continue;
			}
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[deme_index].push_back(deme->second->getIndividual(k));
			}
		}
	}

	for(map<int, vector<Individual> >::iterator buff=ImmigranBuffer.begin(); buff!=ImmigranBuffer.end(); ++buff){
		if(buff->first >= index_last_left_fix and buff->first < index_last_left_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[buff->first].push_back(Individual('A'));
			}
		}
		if(buff->first >= index_last_right_fix and buff->first < index_last_right_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				ImmigranBuffer[buff->first].push_back(Individual('B'));
			}
		}
		if(index_next_left <= buff->first and buff->first < index_next_left + number_of_demes_u_d){
			if(Acheck(buff->second)){
				continue;
			}
			basicUnitCreator('l', 'A');
		} 
		if(index_next_right <= buff->first and buff->first < index_next_right + number_of_demes_u_d){
			if(Bcheck(buff->second)){
				continue;
			}
			basicUnitCreator('r', 'B');
		}
		space[buff->first]->integrateMigrantVector(buff->second);
	}
	return 0;
}

void Universe::Breed(int index){
	space[index]->Breed();
}

void Universe::set(int index,string type){
	int demesize = Deme::getDEMEsize();
	vector<Individual> migBuffer;
	if(type == "pureA"){
		for(int i=0;i<demesize;i++){
			migBuffer.push_back('A');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "pureB"){
		for(int i=0;i<demesize;i++){
			migBuffer.push_back('B');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "hetero"){
		for(int i=0;i<demesize/4;i++){
			migBuffer.push_back('A');
		}
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back('C');
		}
		for(int i=0;i<demesize/4;i++){
			migBuffer.push_back('B');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "halfAhalfhetero"){
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back('A');
		}
		for(int i=0;i<demesize/2;i++){
			migBuffer.push_back('C');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	cerr << "ERROR: unknown parameter " << type << " of Universe.set()";
	return;
}


void Universe::globalBreeding(){
	if(dimension == 0){
		vector<Imigrant> new_generation;
		new_generation.reserve(zeroD_immigrant_pool.size());
		Imigrant desc;
		double fitness;
		int num_of_desc;
// 
		for(unsigned int index = 0; index < zeroD_immigrant_pool.size(); index++){
			fitness = zeroD_immigrant_pool[index].getFitness();
			num_of_desc = getNumberOfDescendants(fitness);
			for(int i=0;i<num_of_desc;i++){
				zeroD_immigrant_pool[index].makeGamete(desc);
				if(desc.Acheck()){
					continue;
				}
				new_generation.push_back(desc);
			}
			num_of_desc = getNumberOfDescendants(fitness);
			for(int i=0;i<num_of_desc;i++){
				zeroD_immigrant_pool[index].makeGamete(desc);
				if(desc.Acheck()){
					continue;
				}
				new_generation.push_back(desc);
			}
		}
		zeroD_immigrant_pool.clear();
		zeroD_immigrant_pool = new_generation;
		double material = 0;
		int pop_size = zeroD_immigrant_pool.size();
		for(int i = 0;i < pop_size;i++){
			material += zeroD_immigrant_pool[i].getBprop();
		}
		cout << "Population size: " << pop_size << endl;
// 		cout << "Amount of material: " << material << endl;
		new_generation.clear();
		return;
	}
	
	vector<int> indexes;
	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
		indexes.push_back(i->first);
	}
	
	int index = 0;
	int i_size = indexes.size();
	
// 	#pragma omp parallel
// 	{
// 
// 		{
// 		#ifdef _OPENMP
// 		int tnum = omp_get_num_threads();
// 		#else
// 		int tnum = 1;
// 		#endif
// 
// 		cout << "Number of threads " << tnum << " "<< i_size << endl;
// 		}
// 
// 	#pragma omp parallel for
		for(int i = 0; i < i_size; i++){
			index = indexes[i];
			space[index]->Breed();
		}
// 	}
	return;
}

bool Universe::Acheck(vector<Individual>& buffer){
	for(int i = 0; (unsigned)i < buffer.size(); i++){
		if(buffer[i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Universe::Bcheck(vector<Individual>& buffer){
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
		cerr << "It is not possible to change parameter height once, you create world";
	}
}

void Universe::setWidth(int width){
	if(space.size() == 0){
		number_of_demes_l_r = width;
	} else {
		cerr << "It is not possible to change parameter height once, you create world";
	}
}


void Universe::setLREdgesType(string ed_type){
	type_of_l_r_edges = ed_type;
}

void Universe::setUDEdgesType(string ed_type){
	type_of_u_d_edges = ed_type;
}

void Universe::setDimension(int dim){
	dimension = dim;
}

void Universe::setNumberOfEdges(int nue){
	edges_per_deme = nue;
}

void Universe::restart(){
	if(dimension == 0){
		zeroD_immigrant_pool.clear();
	} else {
		clear();
		worldSlave();
		cerr << "World is reset." << endl;
	}
	return;
}

void Universe::clear(){
	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
		delete i->second;
	}
	space.clear();
	return;
}


// int Universe::getIndex(int i){
// 	if((unsigned)i >= space.size()){
// 		return -1;
// 	} else {
// 		return space[i]->getDemeIndex();
// 	}
// }

  // // // // 
 //  PLOT //
// // // //

void Universe::listOfParameters(){
	cerr << "***************" << endl << "Size of World: " << space.size() << " Dim: " << dimension << " edges_per_deme: " << edges_per_deme << endl
	<< "Number of demes l/r: " << number_of_demes_l_r << " Number of demes u/d: " << number_of_demes_u_d << endl
	<< "Type of l/r edges: " << type_of_l_r_edges << " Type of u/d edges: " << type_of_u_d_edges << endl
	<< "Last left index: " << index_last_left << " Last right index: " << index_last_right << endl
	<< "Next left index: " << index_next_left << " Next right index: " << index_next_right << endl  << "***************" << endl;
	return;
}

void Universe::listOfDemes(){
	cerr << "of dimension: " << dimension << endl;
	if(dimension == 0){
		cerr << "Population of imigrants has " << zeroD_immigrant_pool.size() << endl;
	} else {
		cerr << "World of size " << space.size() << endl;
		cerr << "Number of demes up to down: " << number_of_demes_u_d << endl;
		cerr << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
		if(type_of_l_r_edges != "extending"){
			cerr << "Number of demes left to right: " << number_of_demes_l_r << endl;
		}
		cerr << "Type of borders left to right: " << type_of_l_r_edges << endl;
		cerr << "                 EDGE" << endl;
		cerr << setw(7) << right << "DEME " << setw(7) << left << " LEFT" << setw(6) << left << "RIGHT" << setw(5) << left << "UP" << setw(6) << left << "DOWN" << endl;
		for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
			i->second->showDeme();
		}
	}
}

void Universe::summary(){
	if(dimension == 0){
		cout << "Selection: " << SELECTIONpressure << endl;
		cout << "Recombination rate: " << RECOMBINATIONrate << endl;
		cout << "Theta: " << SELECTIONpressure / RECOMBINATIONrate << endl;
		cout << "Number of Imigrants per generation: " << DEMEsize << endl;
	} else {
		int worlsize = space.size();
		cerr << "World of size " << worlsize << endl;
		cerr << "of dimension: " << dimension << endl;
		cerr << "Number of demes up to down: " << number_of_demes_u_d << endl;
		cerr << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
		if(type_of_l_r_edges != "extending"){
			cerr << "Number of demes left to right: " << number_of_demes_l_r << endl;
		}
		cerr << "Type of borders left to right: " << type_of_l_r_edges << endl;
		cout << "                 EDGE" << endl;
		cout << setw(7) << right << "DEME " 
		<< setw(7) << left << " LEFT" 
		<< setw(6) << left << "RIGHT"; 
		if(dimension == 2){
			cout << setw(6) << left << "UP" 
			<< setw(6) << left << "DOWN";
		}
		cout << setw(12) << left << "meanf"
		<< setw(12) << left << "f(heter)"
		<< setw(12) << left << "meanHI" 
		<< setw(12) << left << "var(HI)";
		if(LOCI * NUMBERofCHROMOSOMES > 1){
			cout << setw(12) << left << "var(p)" 
			<< setw(12) << left << "LD";
		}
		
		if(LOCI * NUMBERofCHROMOSOMES <= 16){
			for(int ch = 0;ch < NUMBERofCHROMOSOMES;ch++){
				for(int l = 0; l < LOCI;l++){
					cout << left << "Ch" << ch+1 << "l" << l+1 << setw(7) << ' ';
				}
			}
		}
		cout << endl;
		for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
	// 		if(i->first == 9){
			i->second->summary();
	// 			i->second->readAllGenotypes();
	// 		}
		}
	}
}


double Universe::getProportionOfHeterozygotes(int index){
	return space[index]->getProportionOfHeterozygotes();
}

double Universe::getProportionOfHomozygotes(int index, char type){
	return space[index]->getProportionOfHomozygotes(type);
}

void Universe::showOneDeme(int index){
	space[index]->showDeme();
}

void Universe::viewOneDeme(int index){
	space[index]->viewDeme();
}

void Universe::plotHeadOfDeme(int index){
	space[index]->plotHeadOfDeme();
}

void Universe::viewDemesOneByOne(){
	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
		cout << "***** DEME " << i->first << " *****" << endl;
		i->second->viewDeme();
		cin.get();
	}
}

int Universe::SaveTheUniverse(string type){
	ofstream ofile;
	vector<double> props;
	
	ofile.open(NAMEofOUTPUTfile); // Opens file
	if (ofile.fail()){
		return 1;
	}  
	
	
	if(dimension == 0){
		vector<int> blockSizes;
		for(unsigned int index = 0; index < zeroD_immigrant_pool.size(); index++){
			zeroD_immigrant_pool[index].getSizesOfBBlocks(blockSizes);
			for(unsigned int i = 0;i < blockSizes.size(); i++){
				ofile << blockSizes[i] / double(LOCI) << endl;
			}
			blockSizes.clear();
		}
		ofile.close();
		return 0;
	} else {
		if(type == "complete"){
			return save_complete(ofile);
		}
		if(type == "summary"){
			return save_summary(ofile);
		}
		if(type == "hybridIndices"){
			return save_hybridIndices(ofile);
		}
		if(type == "hybridIndicesJunctions"){
			return save_hybridIndicesJunctions(ofile);
		}
	}

	cerr << "WARNING: The output was not saved" << endl;
	cerr << "         unknow saving format" << endl;
	return 1;
}

void Universe::getLD(){
	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
		cout << i->second->getLD() << '\t';
	}
	cout << endl;
}






