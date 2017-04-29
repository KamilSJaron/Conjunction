// Deme test

// the unnecessery constructors should be deleted.

int testDeme(){
    int myints[] = {1,2};
    std::vector<int> neigb (myints, myints + sizeof(myints) / sizeof(int) );

// index, neigb, init, size, selection, beta, in_ch, in_loc, in_lambda
    Deme Hell(0, neigb, 'C', 10000, 0.8, 1, 1, 16, 16, 10);

  //  cout << "*****S*****" << endl;
  //  Hell.streamSummary(cout);
  //  cout << "*****FG*****" << endl;
  //  Hell.readGenotypeFrequencies();
  //  cout << endl;

  //  vector<double> fv;
  //  Hell.getFitnessVector(fv);
   //
  //  for(unsigned int i = 0; i < fv.size(); i++){
  //    cout << " D " << fv[i] << " D ";
  //  }
  //  cout << " Mean fitness: " << Hell.getMeanFitness() << endl;

   Hell.Breed();

  //  cout << "*****S*****" << endl;
  //  Hell.streamSummary(cout);
  //  cout << "*****FG*****" << endl;
  //  Hell.readGenotypeFrequencies();
  //  cout << endl;

  //  Hell.getFitnessVector(fv);
  //  for(unsigned int i = 0; i < fv.size(); i++){
  //    cout << " C " << fv[i] << " C ";
  //  }

  // cout << " Mean fitness: " << Hell.getMeanFitness() << endl;

    Hell.Breed();

  //  cout << "*****S*****" << endl;
  //  Hell.streamSummary(cout);
  //  cout << "*****FG*****" << endl;
  //  Hell.readGenotypeFrequencies();
  //  cout << endl;

  if(Hell.getProportionOfHomozygotes('A') > 0.2 or Hell.getProportionOfHomozygotes('A') < 0.15 ){
    cerr << "Warnig: Selection has unexpected behavior" << endl;
  }

  //  Hell.getFitnessVector(fv);
  //  for(unsigned int i = 0; i < fv.size(); i++){
  //    cout << " X " << fv[i] << " X ";
  //  }

//   cout << " Mean fitness: " << Hell.getMeanFitness() << endl;

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
