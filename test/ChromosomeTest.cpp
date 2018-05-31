/*
 * ChromosomeTest.cpp
 *
 *  Created on: May 16, 2016
 *      Author: kjaron
 */

int testChromosome(){
	Chromosome ch1('A',50);
	Chromosome ch2('B',50);
	// second type of inicitation
	Context context = createTestContext();

	if(!ch1.isPureA()){
		std::cerr << "UNEXPECTED ch(A, 50).isPureA is " << ch1.isPureA() << std::endl;
		return 1;
	}

	if(ch1.isPureB()){
		std::cerr << "UNEXPECTED ch(A, 50).isPureB is " << ch1.isPureB() << std::endl;
		return 1;
	}

	if(ch1.countB() != 0){
		std::cerr << "UNEXPECTED ch(A, 50).Bcount is " << ch1.countB() << std::endl;
	}

	if(ch2.countB() != 50){
		std::cerr << "UNEXPECTED ch(B, 50).Bcount is " << ch2.countB() << std::endl;
	}

	if(ch1.getNumberOfJunctions() != 0){
		std::cerr << "UNEXPECTED ch(A, 50).getNumberOfJunctions is "
			<< ch1.getNumberOfJunctions() << std::endl;
	}

	int toss = context.random.tossAcoin();
	if(!(toss == 0 or toss == 1)){
		std::cerr << "tossAcoin function is flawed." << std::endl;
	}

	for(int i = 0; i < 1000; i++){
		Chromosome ch3('A', rand());
		int loci = ch3.getResolution();
		if(context.random.recombPosition(loci) >= loci){
			std::cerr << "recombPosition generated position larger that the size of the chromosome."
				<< std::endl;
			return 1;
		}
	}

	Chromosome ch3('X',5);
	ch3.setResolution(666);
	if(ch3.getResolution() != 666){
		return 1;
	}

	ch3.setResolution(100);
	ch3.write(10,'B');

	if(ch3.read(0) != 'X' or ch3.read(10) != 'B'){
		return 1;
	}

	ch1.makeRecombinant(ch3, 5, context);

	if(ch3.getNumberOfJunctions() != 0){
		std::cerr << "ERROR: recombination of two pure A chromosome resulted in chromosome with junctions." << std::endl;
		return 1;
	}

	ch2.makeRecombinant(ch3, 5, context);

	if(ch3.getNumberOfJunctions() < 4){
		std::cerr << "WARNING: very unlike scenario in random generation of junctions" << std::endl;
		std::cerr << "Rerun tests (different random seed will be used), if this message occurs again"
			<< " something is wrong, please do not ignore it and report the issue." << std::endl;
		return 2;
	}

	Chromosome ch4('A',10);
	ch4.write(2,'B');
	ch4.write(5,'A');
	ch4.write(6,'B');
	std::vector<int> blocks;
	ch4.getSizesOfBBlocks(blocks);

	if(blocks[0] != 3 or blocks[1] != 4){
		std::cerr << "ERROR in method reporting sizes of B blocks: getSizesOfBBlocks" << std::endl;
		return 1;
	}

	blocks.clear();
	ch4.getSizesOfABlocks(blocks);

	if(blocks[0] != 2 or blocks[1] != 1){
		std::cerr << "ERROR in method reporting sizes of A blocks: getSizesOfABlocks" << std::endl;
		return 1;
	}

/* Writing testing for plotting functions is not worth the energy spend on it, for verification,
following code can be uncommented and used for manual testing */
// 	ch1.makeRecombinant(ch3, 5);
// 	std::cerr << "CH 1" << std::endl;
// 	ch1.showChromosome();
// 	std::cerr << "CH REC" << std::endl;
// 	ch3.showChromosome();
// 	ch2.makeRecombinant(ch3, 5);
// 	std::cerr << "CH 3" << std::endl << std::endl;
// 	ch3.showChromosome();
// 	std::cerr << std::endl;
// 	ch3.viewChromosome();
//
// 	ch3.setResolution(200);
// 	std::cerr << std::endl;
// 	ch3.viewChromosome();
// 	std::cerr << "CH REC" << std::endl;
// 	ch3.showChromosome();
//		void makeRecombinant(Chromosome& chromNew,int numberOfChaisma);

	return 0;
}
