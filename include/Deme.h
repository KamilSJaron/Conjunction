// comment demesize in manual, it should be divideable by 2*number_of_edges
// comment demesize in manual, it should be power of 3 for easy square plotting (64, 216, 512, 1000, 1728, 4096)

class Deme  
{
	public:
// constructor / destructors
// 		Deme(){};
 		Deme(int ind, double sel, double beta);
 		Deme(std::vector<int> neigb, double sel, double beta);
		Deme(int ind, char init, int size, double sel, double beta);
		Deme(int ind, std::vector<int> neigb, int size, double sel, double beta);
		Deme(int ind, std::vector<int> neigb, char init, int size, double sel, double beta);
		Deme(int ind, std::vector<int> neigb, char init, int size, double sel, double beta, int in_ch, int in_loc, double in_lambda);
		~Deme(); // ????
		
// communication functions
		std::vector<int> getNeigbours();
		Individual getIndividual(int index);
		int getDemeIndex();
		void setDemeSize(int size);
		int getDemeSize();

// computing functions
		void Breed();
		void permutation(); // permutation function
		void integrateMigrantVector(std::vector<Individual>& migBuffer);
// 		vector<double> getBproportions() const; // not used
		void getBproportions(std::vector<double>& props) const;
		void getJunctionNumbers(std::vector<double>& juncs) const;
		void getHeterozygoty(std::vector<double>& heterozs);
		double getMeanBproportion() const;
		void getSizesOfBBlocks(std::vector<int>& sizes, int ind);
		void getSizesOfABlocks(std::vector<int>& sizes, int ind);
		double getProportionOfHomozygotes(char population) const;
		double getProportionOfHeterozygotes() const;
		void getFitnessVector(std::vector<double>& fitnessVector);
		double getMeanFitness();
		double getVARhi() const;
		double getVARp();
		void getps(std::vector<double>& ps, int ch);
		double getLD();
		double getLD(double z,double varz,double varp);

// plotting functions
		void showDeme();
		void summary();
		void summary(std::ofstream& ofile);
		void readAllGenotypes();
		void readGenotypeFrequencies();
	
	private:
		void swap(int ind1, int ind2);
		int pickAnIndividual();
		static int sum(std::vector<bool>& ve);

		
		Individual *deme; // farme of individuals
		int index; // index of deme in universe
		int deme_size; // size of deme
		std::vector<int> neigbours; // indices of neigbours
		SelectionModel selection_model;
};

