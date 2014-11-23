#include <iostream> // preprocessor directive
#include <cmath>
#include <string>  // strings
#include <map>  // associative arrays
#include <algorithm> // sort
#include <fstream>
#include <new> // alocation of memory in heap
#include <iomanip>
#include <time.h>
#include<g2.h>
#include<g2_X11.h>

#include "universe.h" // Class of individuals

#define SEEDtoRAND 612380 //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
// #define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/

using namespace std;

template <class T> class auto_ptr{
	T* ptr;
	public:
		explicit auto_ptr(T* p = 0) : ptr(p) {}
		~auto_ptr(){delete ptr;}
		T& operator*(){return *ptr;}
		T* operator->(){return ptr;}

};

// v0.141020

int main()
{
	using namespace std; // std as default
	srand (SEEDtoRAND); // setting a seed
// 	Universe KamilWorld;
// 	clock_t t1,t2;
// // 	
// 	KamilWorld.basicUnitCreator('b', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// // 	KamilWorld.basicUnitCreator('l', 'A');
// 	
// // 	KamilWorld.globalBreeding();
// 	KamilWorld.listOfDemes();
// // 	KamilWorld.plotOneDeme(0);
// 	for(int i=0; i < 100;i++){
// 		t1=clock();
// 		KamilWorld.migration();
// 		KamilWorld.globalBreeding();
// 		t2=clock();
// 		cout << "Generation: " << i << " in ";
// 		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
// 	}
// 	KamilWorld.listOfDemes();
	Individual Eddie('C');
	Individual Luis(Eddie.makeGamete(),Eddie.makeGamete());
	
	Luis.readGenotype();
	Luis.plotGenotype();
	
	cin.get();
	return 0;
}












