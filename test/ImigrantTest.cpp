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
  Imigrant Andrea('B', 5, 2000, 0.5, 1.53);

  std::vector<Chromosome> gamete;
  Andrea.makeGamete(gamete);

  Imigrant Amina(gamete, 0.5, 1.53);

  cerr << "Individual\tBprop\n";
  cerr << "Iakov\t" << Iakov.getBprop() << endl;
  cerr << "Andrea\t" << Andrea.getBprop() << endl;
  cerr << "Amina\t" << Amina.getBprop() << endl;

  std::vector<int> sizes;
  Amina.getSizesOfBBlocks(sizes);
  for(int i = 0; i < sizes.size(); i++){
    cerr << sizes[i] << " \n";
  }

  cerr << " Acheck: " << Amina.Acheck() << endl;
  cerr << " Bcheck: " << Amina.Bcheck() << endl;

  return 0;
}
