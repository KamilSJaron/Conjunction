#include <iostream> // preprocessor directive
#include <cmath>
#include <string>  // strings
#include <map>  // associative arrays
#include <algorithm> // sort
#include <fstream>
#include <new> // alocation of memory in heap
#include <iomanip>
#include <time.h>

#include "universe.h" // Class of individuals

#define SEEDtoRAND 732125927 //seed only for rand() fction, values of the poisson distribution are always generated with same initial seed
#define NUM_OF_IMIGRANTS 100 /* number of complete heterozygotious imigrants per generation (assuming, that Pois(1)*500 would be very close to 500, cause computationally it is not worth the computational time)*/

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
// 	
// 	KamilWorld.setLREdgesType("reflexive");
// 	KamilWorld.basicUnitCreator('b', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// 	KamilWorld.basicUnitCreator('l', 'A');
// 	KamilWorld.basicUnitCreator('r', 'B');
// 	KamilWorld.listOfDemes();

// 	KamilWorld.migration();
// 	KamilWorld.plotOneDeme(0);
// 	for(int i=0; i < 3;i++){
// 		t1=clock();
// 		cout << "Generation: " << i << " in ";
// 		KamilWorld.migration();
// 		KamilWorld.globalNaiveBreeding();
// 		t2=clock();
// 		cout << ((float)t2 - (float)t1) / CLOCKS_PER_SEC << endl;
// 	}
	
// 	KamilWorld.listOfDemes();	
// 	KamilWorld.plotDemesOneByOne();
// 	Individual Eddie('A');
// 	Individual Luis('B');
// 	vector<Chromosome> gamete1;
// 	vector<Chromosome> gamete2;
// 	gamete1 = Eddie.makeGamete();
// 	gamete2 = Luis.makeGamete();
// 	Individual Eddie_junior(gamete1,gamete2);
// 	gamete1 = Eddie_junior.makeGamete();
// 	gamete2 = Eddie_junior.makeGamete();
// 	Individual Luis_junior(gamete1,gamete2);
	
// 	cout << "***EDDIE***" << endl;
// 	Eddie.readGenotype();
// 	cout << Eddie.getFitness() << endl;
// 	
// 	cout << "***EDDIE JUNIOR***" << endl;
// 	Eddie_junior.readGenotype();
// 	cout << Eddie_junior.getFitness() << endl;

// 	cout << "***LUIS JUNIOR***" << endl;
// 	Luis_junior.readGenotype();
// 	cout << Luis_junior.getFitness() << endl;
	
// 	Chromosome chrom_check('B');
// 	cout << chrom_check.countB() << endl;
	cout << "myVec <- c(";
	double Bcount = 0;
	for(int i = 0;i < 1000;i++){
		cout << (1 - (SELECTIONpressure * pow( 4 * Bcount * (1 - Bcount),BETA))) << ", ";
		Bcount += 0.001;
	}
	cout << 1 - (SELECTIONpressure * pow( 4 * 1 * (1 - 1),BETA));
	cout << ")" << endl;
	return 0;
}












