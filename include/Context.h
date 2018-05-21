/* 
 * File:   context.h
 * Author: izar
 *
 * Created on May 15, 2018, 10:24 PM
 */

#ifndef INCLUDE_CONTEXT_H
#define	INCLUDE_CONTEXT_H

#include "../include/RandomGenerators.h"

class Context {

private:
	RandomGenerator _random;

public:
	Context();
	Context(const Context& orig);
	virtual ~Context();
	
	
	
	RandomGenerator& random;

};

#endif	/* INCLUDE_CONTEXT_H */

