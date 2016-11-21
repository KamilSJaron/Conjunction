/*
 * WorldTest.cpp
 *
 *  Created on: May 22, 2016
 *      Author: kjaron
 */


int testWorld(){

	World world;

//	world.listOfParameters();
	world.setHeight(2);
	world.setWidth(2);
	world.setLREdgesType("reflexive");
	world.setUDEdgesType("reflexive");
	world.setDimension(2);
	world.setNumberOfEdges(4);

	int number_descendants = 0;
	for(int i = 0; i < 10000; i++){
		number_descendants += world.getNumberOfDescendants(1);
	}
	if(number_descendants < 9000 or number_descendants > 11000){
		cerr << "Warning: unxepected behavious of generator of descendants\n";
		cerr << "10000 rolls with fitness 1 resulted in " << number_descendants
		<< " descendants\n";
	}

	world.setSlectionBetaLambda(0.1, 1, 1.5);
	world.setLociChromDeme(16, 4, 64);

//	world.listOfParameters();

//	world.listOfDemes();

	world.basicUnitCreator('b', 'A');
	world.basicUnitCreator('l', 'B');

//	world.worldSlave();

//	world.listOfDemes();

	return 0;
}
