/*
 * WorldTest.cpp
 *
 *  Created on: May 22, 2016
 *      Author: kjaron
 */


int testWorld(){

	World world;

	world.listOfParameters();

	world.setHeight(2);
	world.setWidth(2);
	world.setLREdgesType("reflexive");
	world.setUDEdgesType("reflexive");
	world.setDimension(2);
	world.setNumberOfEdges(4);

	world.setSlectionBetaLambda(0.1, 1, 1.5);
	world.setLociChromDeme(16, 4, 64);

	world.listOfParameters();

	world.listOfDemes();

	world.basicUnitCreator('b', 'A');
	world.basicUnitCreator('l', 'B');

//	world.worldSlave();

	world.listOfDemes();

	return 0;
}
