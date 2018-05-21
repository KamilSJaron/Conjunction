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
	Context context;

	if(!ch1.isPureA()){
		cerr << "UNEXPECTED ch(A, 50).isPureA is " << ch1.isPureA() << endl;
		return 1;
	}

	if(ch1.isPureB()){
		cerr << "UNEXPECTED ch(A, 50).isPureB is " << ch1.isPureB() << endl;
		return 1;
	}

	if(ch1.countB() != 0){
		cerr << "UNEXPECTED ch(A, 50).Bcount is " << ch1.countB() << endl;
	}

	if(ch2.countB() != 50){
		cerr << "UNEXPECTED ch(B, 50).Bcount is " << ch2.countB() << endl;
	}

	if(ch1.getNumberOfJunctions() != 0){
				cerr << "UNEXPECTED ch(A, 50).getNumberOfJunctions is "
				<< ch1.getNumberOfJunctions() << endl;
	}

	int toss = context.random.tossAcoin();
	if(!(toss == 0 or toss == 1)){
		cerr << "tossAcoin function is flawed." << endl;
	}

	for(int i = 0; i < 1000; i++){
		Chromosome ch3('A', rand());
		int loci = ch3.getResolution();
		if(context.random.recombPosition(loci) >= loci){
			cerr << "recombPosition generated position larger that the size of the chromosome."
				<< endl;
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
		cerr << "ERROR: recombination of two pure A chromosome resulted in chromosome with junctions." << endl;
		return 1;
	}

	ch2.makeRecombinant(ch3, 5, context);

	if(ch3.getNumberOfJunctions() < 4){
		cerr << "WARNING: very unlike scenario in random generation of junctions" << endl;
		cerr << "Rerun tests (different random seed will be used), if this message occurs again"
		<< " something is wrong, please do not ignore it and report the issue." << endl;
		return 2;
	}

	Chromosome ch4('A',10);
	ch4.write(2,'B');
	ch4.write(5,'A');
	ch4.write(6,'B');
	vector<int> blocks;
	ch4.getSizesOfBBlocks(blocks);

	if(blocks[0] != 3 or blocks[1] != 4){
		cerr << "ERROR in method reporting sizes of B blocks: getSizesOfBBlocks" << endl;
		return 1;
	}

	blocks.clear();
	ch4.getSizesOfABlocks(blocks);

	if(blocks[0] != 2 or blocks[1] != 1){
		cerr << "ERROR in method reporting sizes of A blocks: getSizesOfABlocks" << endl;
		return 1;
	}

/* Writing testing for plotting functions is not worth the energy spend on it, for verification,
following code can be uncommented and used for manual testing */
// 	ch1.makeRecombinant(ch3, 5);
// 	cerr << "CH 1" << endl;
// 	ch1.showChromosome();
// 	cerr << "CH REC" << endl;
// 	ch3.showChromosome();
// 	ch2.makeRecombinant(ch3, 5);
// 	cerr << "CH 3" << endl << endl;
// 	ch3.showChromosome();
// 	cerr << endl;
// 	ch3.viewChromosome();
//
// 	ch3.setResolution(200);
// 	cerr << endl;
// 	ch3.viewChromosome();
// 	cerr << "CH REC" << endl;
// 	ch3.showChromosome();
//		void makeRecombinant(Chromosome& chromNew,int numberOfChaisma);

	return 0;
}
