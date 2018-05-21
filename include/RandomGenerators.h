/*
 * RandomGenrators.h
 *
 *  Created on: Jan 12, 2017
 *      Author: KamilSJaron
 */

#ifndef INCLUDE_RANDOM_GENERATOR_H_
#define INCLUDE_RANDOM_GENERATOR_H_

class RandomGenerator
{
	public :
		int tossAcoin();
		int recombPosition(int loci);
		double uniform();
		int poisson(double lambda);
};

#endif /* INCLUDE_RANDOM_GENERATOR_H_ */
