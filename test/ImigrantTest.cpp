int GNoD(double fitness){
	int x = RAND_MAX;
	int detailness = 100000;
	while(x >= RAND_MAX - (RAND_MAX % detailness)){
		x = rand();
	}
	double uniform = double(x % detailness) / (detailness-1);

	int result = 0;
	double q = exp(-fitness);
	double p = q;
	double roll = uniform;
	while(roll > q){
		result++;
		p = p * fitness / result;
		q = q + p;
	}
	return result;
}

int testImigrant(){
// // /* COMPUTIONG METHODS */
//   int getChiasma();
//   void makeGamete(std::vector<Chromosome>& gamete); //this method should be rewriten to accept vector of chromosomes instead of pointer to imigrant
//   double getFitness();
//   double getBprop() const;
//   void getSizesOfBBlocks(std::vector<int>& sizes);
//   bool Acheck() const;
//   bool Bcheck() const;
//   int getSelectionPressure() const;

  Imigrant Iakov(5, 2000, 0.5, 1.53);
  Imigrant Andrea('B', 5, 2000, 0.5, 1);

  // for(int i = 0; i < 10000; i++){
  //   cout << Andrea.getChiasma() << endl;
  // }

  std::vector<Chromosome> gamete;
  Andrea.makeGamete(gamete);

  Imigrant Amina(gamete, 0.5, 1.53);

  // cerr << "Individual\tBprop\n";
  // cerr << "Iakov\t" << Iakov.getBprop() << endl;
  // cerr << "Andrea\t" << Andrea.getBprop() << endl;
  // cerr << "Amina\t" << Amina.getBprop() << endl;
  //
  // std::vector<int> sizes;
  // Amina.getSizesOfBBlocks(sizes);
  // for(int i = 0; i < sizes.size(); i++){
  //   cerr << sizes[i] << " \n";
  // }
  //
  // Amina.readGenotype();
  //
  // cerr << " Acheck: " << Amina.Acheck() << endl;
  // cerr << " Bcheck: " << Amina.Bcheck() << endl;

  vector<Imigrant> current_generation;
  vector<Imigrant> new_generation;
  gamete.clear();

  int demesize = 15, ch = 1, size = 100, num_of_desc = 0;
  double lambda = 1.53, sp = 0.5, fitness = 0;

  for(int generation = 0; generation < 100; generation++){
    for(int i = 0; i < demesize; i++){
      current_generation.push_back(Imigrant(ch, size, sp, lambda));
    }

    for(unsigned int index = 0; index < current_generation.size(); index++){
      fitness = current_generation[index].getFitness();
  //			cerr << "Fitness: " << fitness << endl;
      num_of_desc = GNoD(fitness);
  //			cerr << "Number of descendants: " << num_of_desc << endl;
      for(int i=0;i<num_of_desc;i++){
        current_generation[index].makeGamete(gamete);
        if(gamete[0].Acheck()){
          continue;
        }
        new_generation.push_back( Imigrant(gamete, sp, lambda) );
      }
      num_of_desc = GNoD(fitness);
      for(int i=0;i<num_of_desc;i++){
        current_generation[index].makeGamete(gamete);
        if(gamete[0].Acheck()){
          continue;
        }
        new_generation.push_back( Imigrant(gamete, sp, lambda) );
      }
    }

    current_generation.clear();
    current_generation.swap(new_generation);
    new_generation.clear();
    cerr << current_generation.size() << ' ' << new_generation.size() << endl;
  }


  return 0;
}
