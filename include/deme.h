// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)

// fctions
/*double selectionRand(){
		return (double)rand() / RAND_MAX;
}

int pickAnIndividual(){
		return rand() % DEMEsize;
}*/

class Deme  
{
	public:
// constructor / destructors
// 		Deme(){};
// 		Deme(int ind){index = ind;};
// 		Deme(vector<int> neigb){neigbours = neigb;};
		Deme(int ind, char init, int deme_s);
//		Deme(int ind, vector<int> neigb){deme = new Individual[DEMEsize];index = ind;neigbours = neigb;};
//		Deme(int ind, vector<int> neigb, char init);
//		~Deme(){delete[] deme;}; // ????
		
// communication functions
/*		vector<int> getNeigbours(){return neigbours;};
		Individual getIndividual(int index){return deme[index];}
		int getDemeIndex(){return index;};
		Deme(const vector<int>&);
		static void setDEMEsize(int demesize){DEMEsize = demesize;};
		static int getDEMEsize(){return DEMEsize;};*/
		
// computing functions
/*		void Breed();
		void getFitnessVector(vector<double>& fitnessVector);
		void permutation(); // permutation function
		void integrateMigrantVector(vector<Individual>& migBuffer);
// 		vector<double> getBproportions() const; // not used
		void getBproportions(vector<double>& props) const;
		void getJunctionNumbers(vector<double>& juncs) const;
		void getHeterozygoty(vector<double>& heterozs);
		double getMeanBproportion() const;
		double getProportionOfHomozygotes(char population) const;
		double getProportionOfHeterozygotes() const;
		double getMeanFitness() const;
		double getVARhi() const;
		double getVARp();
		void getps(vector<double>& ps, int ch);
		double getLD();
		double getLD(double z,double varz,double varp);*/
		
// plotting functions
/*		void showDeme();
		void summary();
		void summary(ofstream& ofile);
		void viewDeme();
		void readAllGenotypes();
		void readGenotypeFrequencies();
		void plotHeadOfDeme();*/
	
	private:
//		void swap(int ind1, int ind2);
//		static int sum(vector<bool>& ve);
// 		static int SIZE;
		
		Individual *deme;
// 		vector<Individual> niche;
		int index;
		int deme_size;
		std::vector<int> neigbours;
};

