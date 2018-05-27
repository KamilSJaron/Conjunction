/*
 * WorldTest.cpp
 *
 *  Created on: May 22, 2016
 *      Author: kjaron
 */


SimulationSetting createSimulationSetting () {
	SimulationSetting setting;
	setting.up_down_demes = 2;
	setting.left_right_demes = 2;
	setting.type_of_leftright_edges = "reflexive";
	setting.type_of_updown_edges = "reflexive";
	setting.dimension = 2;
	setting.edges_per_deme = 4;
	
	setting.selection = 0.1;
	setting.beta = 1;
	setting.lambda = 1.5;

	setting.selected_loci = 16;
	setting.loci = 16;

	setting.chromosomes = 4;
	setting.deme_size = 64;

	setting.seed = RANDOM_SEED;
	return setting;
}


int testWorld(){

	Context context = createTestContext();
	World world(createSimulationSetting());

	int number_descendants = 0;
	for(int i = 0; i < 10000; i++){
		number_descendants += context.random.poisson(1);
	}
	if(number_descendants < 9000 or number_descendants > 11000){
		cerr << "Warning: unxepected behavious of generator of descendants\n";
		cerr << "10000 rolls with fitness 1 resulted in " << number_descendants
		<< " descendants\n";
	}

//	world.listOfParameters();

//	world.listOfDemes();

	world.basicUnitCreator('b', 'A');
	world.basicUnitCreator('l', 'B');

//	world.worldSlave();

//	world.listOfDemes();

	return 0;
}
