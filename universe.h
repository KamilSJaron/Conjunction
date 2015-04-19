#include "deme.h"

using namespace std;

static string NAMEofOUTPUTfile = "out";

int getNumberOfDescendants(double fitness){
	poisson_distribution<int> pois(fitness);
	return pois(generator);
}

class Universe  
{
	public:
// 		declarations / decl fctions
		Universe(int dimension, int edges_per_deme, int number_of_demes_l_r,string edges_l_r, int number_of_demes_u_d, string edges_u_d);
		Universe();
		void basicUnitCreator(char type, char init);
// 		void infSimulator(vector<Imigrant>& GogolBordello, int NUMofGEN);
		
// 		computing functions
		int migration(); // int will be the errorcode
		void globalNaiveBreeding();
		void Breed(int index);
		void set(int index, string type);
		void globalBreeding();
		
// 		testing functions vector<Chromosome>& gamete
		bool Acheck(vector<Individual>& buffer);
		bool Bcheck(vector<Individual>& buffer);
		
// 		plotting functions
		void listOfParameters();
		void listOfDemes();
		void summary();
		double getProportionOfHeterozygotes(int index);
		double getProportionOfHomozygotes(int index, char type);
		void showOneDeme(int index);
		void viewOneDeme(int index);
		void plotOneDeme(int index);
		void plotOneDeme(int index, const char*  fileName);
		void plotHeadOfDeme(int i);
		void viewDemesOneByOne();
		void plotDemesOneByOne();
		void plotDemesOneByOne(char fileNamepattern[]);
		int SaveTheUniverse(int order);
		int SaveTheUniverse(int order, string type);
		int SaveTheUniverse(string type);
		
// 		parameter changing functions
		void setHeight(int heig);
		void setWidth(int width);
		void setLREdgesType(string ed_type);
		void setUDEdgesType(string ed_type);
		void setDimension(int dim);
		void setNumberOfEdges(int nue);
		void restart();
		void clear();
		
		int getNumOfDemesInColumn(){return number_of_demes_u_d;};
		int getSpaceSize(){return space.size();};
		int getIndex(int i);
	
	private:
// 		inner fctions
		int upper_border(int index, int max_index);
		int lower_border(int index, int max_index);
		int side_border(int reflexive, int extending);
// 		variables
		map<int, Deme*> space;
		int dimension, edges_per_deme;
		int number_of_demes_l_r, number_of_demes_u_d;
		string type_of_l_r_edges, type_of_u_d_edges;
		int index_last_left, index_next_left;
		int index_last_right, index_next_right;
		vector<Imigrant> GogolBordello;
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
// 	dimension = 2;
// 	edges_per_deme = 4;
// 	number_of_demes_l_r = 6;
// 	type_of_l_r_edges = "extending";
// 	number_of_demes_u_d = 1;
// 	type_of_u_d_edges = "reflexive";
}

// I have to add char init to initiate demes as pure A, pure B, mixed or pure A/B with some incoming individuals
void Universe::basicUnitCreator(char type, char init){
	int max_index = space.size();
// 	cerr << "Space size: " << max_index << endl;
	vector<int> new_indexes;
	int index;
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
				new_indexes.push_back(max_index + 2);
				space[index_next_right] = new Deme(index_next_right,new_indexes,init);
				index_last_right = index_next_right;
				index_next_right = max_index + 2;
				break;
		}
		return;
	}
	
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
			cerr << "I have no idea, what do you mean by... " << type << " I understand only 'b' basic, 'l' left and 'r' right, type of basic unit.. try it again please." << endl;
			break;
	}
	return;
}

// void Universe::infSimulator(vector<Imigrant>& GogolBordello, int NUMofGEN){
// 	for(int generation = 1; generation < NUMofGEN; generation ++){
// // 		migration
// 		for(int i = 0; i < DEMEsize;i++){
// 			GogolBordello.push_back(Imigrant('B'));
// 		}
// // 		selection & reproduction
// 		
// 		
// 	}
// // 	create zero dimensional space
// // 	one specialized deme with redefined migration and breeding?
// }


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
	if(type_of_u_d_edges == "wrap"){
		if(index == max_index){
			return index + (number_of_demes_u_d-1);
		} else {
			return index - 1;
		}
	}
	cerr << "The type of upper-down edges is not valid." << endl;
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
	if(type_of_u_d_edges == "wrap"){
		if(index == max_index + number_of_demes_u_d  - 1){
			return index - (number_of_demes_u_d-1);
		} else {
			return index + 1;
		}
	}
	cerr << "The type of upper-down edges is not valid." << endl;
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
	cerr << "The type of left-right edges is not valid." << endl;
	return -1;
}


int Universe::migration(){
	if(dimension == 0){
		GogolBordello.reserve(DEMEsize);
		for(int i = 0; i < DEMEsize;i++){
			GogolBordello.push_back(Imigrant('B'));
		}
// 		cerr << "GB size: " <<	GogolBordello.size() << endl;
		return 0;
	}
	if(space.empty()){
		cerr << "ERROR: Missing demes" << endl;
		return -1;
	}

	int index_last_left_fix = index_last_left;
	int index_last_right_fix = index_last_right;
	map<int, vector<Individual>> bufferVectorMap;
	
	vector<int> neigbours;
	int MigInd = Deme::getDEMEsize() / (2 * edges_per_deme );
	int deme_index;
	/*bufferVectorMap is container for all individuals imigrating to all demes*/
	for (auto deme=space.begin(); deme!=space.end(); ++deme){
// 		cerr << i << endl;
// 		cerr << "lets transfer individuals of DEME " << i << endl;
		neigbours = deme->second->getNeigbours();
		
// 		following code should be deleted afterwards, slowing down the program (testing)
// 		if(neigbours.size() != (unsigned)edges_per_deme){
// 			cerr << "ERROR: More neigbours than edges: " << neigbours.size() << " != " << edges_per_deme << endl;
// 			return -1;
// 		}

		for(unsigned int j=0;j < neigbours.size();j++){
			deme_index = neigbours[j];
			for(int k=0;k < MigInd; k++){
				bufferVectorMap[deme_index].push_back(deme->second->getIndividual(k));
// 				cerr << "from " << world[i]->getDemeIndex() << " to " << deme_index << endl;
// 				world[i]->getIndividual(k).plotGenotype();
			}
		}
	}

	for(auto buff=bufferVectorMap.begin(); buff!=bufferVectorMap.end(); ++buff){
		if(buff->first >= index_last_left_fix and buff->first < index_last_left_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				bufferVectorMap[buff->first].push_back(Individual('A'));
			}
		}
		if(buff->first >= index_last_right_fix and buff->first < index_last_right_fix + number_of_demes_u_d){
			for(int k=0;k < MigInd; k++){
				bufferVectorMap[buff->first].push_back(Individual('B'));
			}
		}
		
// 		cerr << "Buffer " << buff->first << " has size " << buff->second.size() << endl;
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
// 		cout << "Integating to deme " << buff->first << " " << buff->second.size() << " individuals" << endl;
		space[buff->first]->integrateMigrantVector(buff->second);
	}
	return 0;
}

void Universe::globalNaiveBreeding(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		i->second->quickBreed();
	}
}

void Universe::Breed(int index){
	space[index]->Breed();
}

void Universe::set(int index,string type){
	vector<Individual> migBuffer;
	if(type == "pureA"){
		for(int i=0;i<DEMEsize;i++){
			migBuffer.push_back('A');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "pureB"){
		for(int i=0;i<DEMEsize;i++){
			migBuffer.push_back('B');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "hetero"){
		for(int i=0;i<DEMEsize/4;i++){
			migBuffer.push_back('A');
		}
		for(int i=0;i<DEMEsize/2;i++){
			migBuffer.push_back('C');
		}
		for(int i=0;i<DEMEsize/4;i++){
			migBuffer.push_back('B');
		}
		space[index]->integrateMigrantVector(migBuffer);
		return;
	}
	
	if(type == "halfAhalfhetero"){
		for(int i=0;i<DEMEsize/2;i++){
			migBuffer.push_back('A');
		}
		for(int i=0;i<DEMEsize/2;i++){
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
		Imigrant desc;
		double fitness;
		int num_of_desc;
// 
		for(unsigned int index = 0; index < GogolBordello.size(); index++){
			fitness = GogolBordello[index].getFitness();
			num_of_desc = getNumberOfDescendants(fitness);
			for(int i=0;i<num_of_desc;i++){
				GogolBordello[index].makeGamete(desc);
				if(desc.Acheck()){
					continue;
				}
				new_generation.push_back(desc);
			}
			num_of_desc = getNumberOfDescendants(fitness);
			for(int i=0;i<num_of_desc;i++){
				GogolBordello[index].makeGamete(desc);
				if(desc.Acheck()){
					continue;
				}
				new_generation.push_back(desc);
			}
		}
		GogolBordello.clear();
		GogolBordello = new_generation;
		new_generation.clear();
		return;
	}
	
	vector<int> indexes;
	for (auto i=space.begin(); i!=space.end(); ++i){
		indexes.push_back(i->first);
	}
	
	int index = 0;
	int i_size = indexes.size();
	
// 	#pragma omp parallel for //even worse than before...
	for(int i = 0; i < i_size; i++){
		index = indexes[i];
		space[index]->Breed();
	}
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
		GogolBordello.clear();
	} else {
		for (auto i=space.begin(); i!=space.end(); ++i){
			delete i->second;
		}
		space.clear();
		basicUnitCreator('b', 'A');
		basicUnitCreator('r', 'B');
		cerr << "World is reset." << endl;
	}
	return;
}

void Universe::clear(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		delete i->second;
	}
	space.clear();
	return;
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

void Universe::listOfParameters(){
	cerr << "***************" << endl << "Size of World: " << space.size() << " Dim: " << dimension << " edges_per_deme: " << edges_per_deme << endl
	<< "Number of demes l/r: " << number_of_demes_l_r << " Number of demes u/d: " << number_of_demes_u_d << endl
	<< "Type of l/r edges: " << type_of_l_r_edges << " Type of u/d edges: " << type_of_u_d_edges << endl
	<< "Last left index: " << index_last_left << " Last right index: " << index_last_right << endl
	<< "Next left index: " << index_next_left << " Next right index: " << index_next_right << endl  << "***************" << endl;
	return;
}

void Universe::listOfDemes(){
	if(dimension == 0){
		cerr << "Population of imigrants has " << GogolBordello.size() << endl;
	} else {
		cerr << "World of size " << space.size() << endl;
	}
	cerr << "of dimension: " << dimension << endl;
	cerr << "Number of demes up to down: " << number_of_demes_u_d << endl;
	cerr << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
	if(type_of_l_r_edges != "extending"){
		cerr << "Number of demes left to right: " << number_of_demes_l_r << endl;
	}
	cerr << "Type of borders left to right: " << type_of_l_r_edges << endl;
	cerr << "                 EDGE" << endl;
	cerr << setw(7) << right << "DEME " << setw(7) << left << " LEFT" << setw(6) << left << "RIGHT" << setw(5) << left << "UP" << setw(6) << left << "DOWN" << endl;
	for (auto i=space.begin(); i!=space.end(); ++i){
		i->second->showDeme();
	}
}

void Universe::summary(){
	int worlsize = space.size();
	cout << "World of size " << worlsize << endl;
	cout << "of dimension: " << dimension << endl;
	cout << "Number of demes up to down: " << number_of_demes_u_d << endl;
	cout << "Type of borders top and bottom: " << type_of_u_d_edges << endl;
	if(type_of_l_r_edges != "extending"){
		cout << "Number of demes left to right: " << number_of_demes_l_r << endl;
	}
	cout << "Type of borders left to right: " << type_of_l_r_edges << endl;
	cout << "                 EDGE" << endl;
	cout << setw(7) << right << "DEME " 
	<< setw(7) << left << " LEFT" 
	<< setw(6) << left << "RIGHT" 
	<< setw(5) << left << "UP" 
	<< setw(6) << left << "DOWN" 
	<< setw(12) << left << "mean f"
	<< setw(12) << left << "f(heter)"
	<< setw(12) << left << "meanHI" 
	<< setw(12) << left << "var(HI)" 
	<< setw(12) << left << "var(p)" 
	<< setw(12) << left << "LD";
	if(RESOLUTION * NUMBERofCHROMOSOMES == 2){
		cout << setw(40) << right << "genotypes: AAAA AAAB AABB ABBB BBBB" << endl;
	} else {
		if(RESOLUTION * NUMBERofCHROMOSOMES == 1){
			cout << setw(10) << "genotypes: " << right << setw(30) << "             AA       AB       BB" << endl;
		} else {
			cout << endl;
		}
	}
	for (auto i=space.begin(); i!=space.end(); ++i){
// 		if(i->first == 9){
		i->second->summary();
// 			i->second->readAllGenotypes();
// 		}
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

void Universe::plotOneDeme(int index){
	space[index]->plotDeme();
}

void Universe::plotOneDeme(int index, const char* fileName){
	space[index]->plotDeme(fileName);
}

void Universe::plotHeadOfDeme(int index){
	space[index]->plotHeadOfDeme();
}

void Universe::viewDemesOneByOne(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		cout << "***** DEME " << i->first << " *****" << endl;
		i->second->viewDeme();
		cin.get();
	}
}

void Universe::plotDemesOneByOne(){
	for (auto i=space.begin(); i!=space.end(); ++i){
		cout << "***** DEME " << i->first << " *****" << endl;
		i->second->plotDeme();
		cin.get();
	}
}

void Universe::plotDemesOneByOne(char fileNamepattern[]){
	int len = strlen(fileNamepattern);
  static char* newName = fileNamepattern;
	char decimal = '0', unit = '0';
/*potebuju nejak z paterny udelat specificky nazev ktery budu moct poslat jako argument do plotovaci fce*/
/* Mám pocit, že to bude fungovat jen pro 20 nik */ 
	for (auto i=space.begin(); i!=space.end(); ++i){
		if(i->first > 10){
			decimal = '1';
			if(i->first > 20){
				decimal = '2';
				if(i->first == 30){
					cerr << "WARNING: Too much pictures. Only first 30 are plotted.";
					break;
				}
			}
		}
		cout << "Unit: " << i->first % 10 << ' '; 
		unit = char('0' + i->first % 10);

		fileNamepattern[len - 6] = decimal;
		fileNamepattern[len - 5] = unit;
		cout << "***** DEME " << i->first << " *****" << endl;
		i->second->plotDeme(newName);
		cin.get();
	}
}

int Universe::SaveTheUniverse(int order){
	string fn = NAMEofOUTPUTfile;
	if(fn.find('X') > 50){
		cerr << "Warrning, the 'X' in the filename is missing." << endl; // udelat to tak, aby v pripade, ze tam neni X, aby to ulozilo jen nakonci.
		cerr << "            current name is "<< NAMEofOUTPUTfile << endl;
	}
	fn[fn.find('X')] = char(order+'0');
	
	int index = index_last_left;
	ofstream ofile;
	vector<double> props;
	
	ofile.open(fn); // Opens file
	if (ofile.fail()){
		return 1;
	}  
	
	for(unsigned int i = 0; i < space.size(); i++){
		props = space[index]->getBproportions();
		ofile << index << '\t';
		for(unsigned int ind = 0; ind < props.size(); ind++){
			ofile << props[ind] << '\t';
		}
		ofile << endl;
		if(index != index_last_right){
// 			cerr << index << ' ';
// 			cerr << space[index]->getNeigbours()[0] << ' ';
			index = space[index]->getNeigbours()[1];
		} else {
			break;
		}
	}
	
	ofile.close();
	
// 	index_last_left = space[index_last_left]->zsgetNeigbours()[2];
	cerr << "The output was sucesfully saved to: " << fn << endl;
	return 0;
}

int Universe::SaveTheUniverse(int order, string type){
	string fn = NAMEofOUTPUTfile;
	if(fn.find('X') > 50){
		cerr << "Warrning, the 'X' in the filename is missing." << endl; // udelat to tak, aby v pripade, ze tam neni X, aby to ulozilo jen nakonci.
		cerr << "            current name is "<< NAMEofOUTPUTfile << endl;
	}
	fn[fn.find('X')] = char(order+'0');
	
	int index = index_last_left;
	ofstream ofile;
	vector<double> props;
	
	ofile.open(fn); // Opens file
	if (ofile.fail()){
		return 1;
	}  
	
	if(type == "complete"){
		for(unsigned int i = 0; i < space.size(); i++){
			props = space[index]->getBproportions();
			ofile << index << '\t';
			for(unsigned int ind = 0; ind < props.size(); ind++){
				ofile << props[ind] << '\t';
			}
			ofile << endl;
			if(index != index_last_right){
				index = space[index]->getNeigbours()[1];
			} else {
				break;
			}
		}
		cerr << "The output was sucesfully saved to: " << fn << endl;
		ofile.close();
		return 0;
	}
	if(type == "average"){
		double avrg;
		for(unsigned int i = 0; i < space.size(); i++){
			props = space[index]->getBproportions();
			ofile << index << '\t';
			for(unsigned int ind = 0; ind < props.size(); ind++){
				avrg += props[ind];
			}
			avrg = avrg / DEMEsize;
			ofile << avrg << endl;
			if(index != index_last_right){
				index = space[index]->getNeigbours()[1];
			} else {
				break;
			}
		}
		cerr << "The output was sucesfully saved to: " << fn << endl;
		ofile.close();
		return 0;
	}

	cerr << "WARNING: The output was not saved" << endl;
	cerr << "         unknow saving format" << endl;
	return 1;
}

int Universe::SaveTheUniverse(string type){
	int index = index_last_left;
	ofstream ofile;
	vector<double> props;
	
	ofile.open(NAMEofOUTPUTfile); // Opens file
	if (ofile.fail()){
		return 1;
	}  
	
	if(dimension == 0){
		for(unsigned int ch = 0;ch < GogolBordello.size();ch++){
// 			GogolBordello[ch].getBprop();
			vector<int> IndBlocks;
			GogolBordello[index].getSizesOfBBlocks(IndBlocks);
			for(vector<int>::iterator it = IndBlocks.begin(); it != IndBlocks.end(); ++it) {
				ofile << *it / double(RESOLUTION) << endl;
			}
// 			ofile << ch << '\t';
// 			for(unsigned int ind = 0; ind < props.size(); ind++){
// 					ofile << props[ind] << '\t';
// 			}
// 			ofile << endl;
		}
	} else {
		if(type == "complete"){
			for(unsigned int i = 0; i < space.size(); i++){
				props = space[index]->getBproportions();
				ofile << index << '\t';
				for(unsigned int ind = 0; ind < props.size(); ind++){
					ofile << props[ind] << '\t';
				}
				ofile << endl;
				if(index != index_last_right){
					index = space[index]->getNeigbours()[1];
				} else {
					break;
				}
			}
	}
		cerr << "The output was sucesfully saved to: " << NAMEofOUTPUTfile << endl;
		ofile.close();
		return 0;
	}
	if(type == "average"){
		double avrg;
		for(unsigned int i = 0; i < space.size(); i++){
			props = space[index]->getBproportions();
			ofile << index << '\t';
			for(unsigned int ind = 0; ind < props.size(); ind++){
				avrg += props[ind];
			}
			avrg = avrg / DEMEsize;
			ofile << avrg << endl;
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

	cerr << "WARNING: The output was not saved" << endl;
	cerr << "         unknow saving format" << endl;
	return 1;
}








