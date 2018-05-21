/* 
 * File:   context.cpp
 * Author: izar
 * 
 * Created on May 15, 2018, 10:24 PM
 */

#include "../include/Context.h"

Context::Context() : _random{}, random{_random} {
}

Context::Context(const Context& orig) : _random{orig._random}, random{_random} {
}

Context::~Context() {
}

