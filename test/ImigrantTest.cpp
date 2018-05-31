int testImigrant(){
	Context context = createTestContext(); 
	/* COMPUTIONG METHODS */
//	int getChiasma();
//	void makeGamete(std::vector<Chromosome>& gamete); //this method should be rewriten to accept vector of chromosomes instead of pointer to imigrant
//	double getFitness();
//	double getBprop() const;
//	void getSizesOfBBlocks(std::vector<int>& sizes);
//	bool isPureA() const;
//	bool isPureB() const;
//	int getSelectionPressure() const;
	Imigrant Iakov(context, 5, 2000, 1.53);
	Imigrant Andrea(context, 'B', 5, 2000, 1);

//	for(int i = 0; i < 10000; i++){
//		std::cout << Andrea.getChiasma() << std::endl;
//	}

	std::vector<Chromosome> gamete;
	Andrea.makeGamete(gamete);

	Imigrant Amina(context, gamete, 1.53);

//	std::cerr << "Individual\tBprop\n";
//	std::cerr << "Iakov\t" << Iakov.getBprop() << std::endl;
//	std::cerr << "Andrea\t" << Andrea.getBprop() << std::endl;
//	std::cerr << "Amina\t" << Amina.getBprop() << std::endl;
//
//	std::vector<int> sizes;
//	Amina.getSizesOfBBlocks(sizes);
//	for(int i = 0; i < sizes.size(); i++){
//		std::cerr << sizes[i] << " \n";
//	}
//
//	Amina.readGenotype();
//
//	std::cerr << " isPureA: " << Amina.isPureA() << std::endl;
//	std::cerr << " isPureB: " << Amina.isPureB() << std::endl;

	return 0;
}
