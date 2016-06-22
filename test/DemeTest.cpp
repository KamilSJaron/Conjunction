// Deme test

// the unnecessery constructors should be deleted.

int testDeme(){
    int myints[] = {1,2};
    std::vector<int> neigb (myints, myints + sizeof(myints) / sizeof(int) );

    Deme Hell(0, neigb, 'C', 4, 0.8, 1, 1, 100, 1);

    cout << "*****SD*****" << endl;
    Hell.showDeme();
    cout << "*****S*****" << endl;
    Hell.summary();

    Hell.Breed();

    cout << "*****SD*****" << endl;
    Hell.showDeme();
    cout << "*****S*****" << endl;
    Hell.summary();

    Hell.Breed();

    cout << "*****SD*****" << endl;
    Hell.showDeme();
    cout << "*****S*****" << endl;
    Hell.summary();


//		std::vector<int> getNeigbours();
//		Individual getIndividual(int index);
//		int getDemeIndex();
//		void setDemeSize(int size);
//		int getDemeSize();
//
//// computing functions
//		void Breed();
//		void permutation(); // permutation function
//		void integrateMigrantVector(std::vector<Individual>& migBuffer);
//// 		vector<double> getBproportions() const; // not used
//		void getBproportions(std::vector<double>& props) const;
//		void getJunctionNumbers(std::vector<double>& juncs) const;
//		void getHeterozygoty(std::vector<double>& heterozs);
//		double getMeanBproportion() const;
//		double getProportionOfHomozygotes(char population) const;
//		double getProportionOfHeterozygotes() const;
//		void getFitnessVector(std::vector<double>& fitnessVector);
//		double getMeanFitness();
//		double getVARhi() const;
//		double getVARp();
//		void getps(std::vector<double>& ps, int ch);
//		double getLD();
//		double getLD(double z,double varz,double varp);
//
//// plotting functions
//		void showDeme();
//		void summary();
//		void summary(std::ofstream& ofile);
//		void readAllGenotypes();
//		void readGenotypeFrequencies();

    return 0;
}

