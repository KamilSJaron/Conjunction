#include "chromosome.h"
#include <g2.h>
const int NUMBERofCHROMOSOMES = 1; /* mouse have 19+1, but now I am reproducing 84 results */
const double RECOMBINATIONrate = 4;
const double SELECTIONpressure = 0.5;
const double BETA = 1;

using namespace std;

// fctions
int getNumberOfChromosomes(){
	return NUMBERofCHROMOSOMES;
}

default_random_engine generator;

int getChiasma(double R){
	poisson_distribution<int> pois(R);
	return pois(generator);
}

class Individual  
{
	public:
// 		decl
		Individual();
		Individual(char origin);
		Individual(vector<Chromosome> gamete1, vector<Chromosome> gamete2);
		~Individual(){genome[0].clear();genome[1].clear();};
		
// 		computing
		vector<Chromosome> makeGamete(); //prepsat tak aby vector dostal pointrem
		void replace_chromozome(int set, int position,map <int, char> input_chrom);
		double getFitness();
		bool Acheck();
		bool Bcheck();
		
// 		plotting
		void readGenotype();
		void viewGenotype();
		
		/* GRAPHICS */
		void plotGenotype();
		
// 		static int totalVectCount; //???
	
	private:
		vector<Chromosome> genome[2];
};

Individual::Individual(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		genome[0].push_back(Chromosome());
		genome[1].push_back(Chromosome());
	}
}

Individual::Individual(char origin){
	if(origin == 'A' or origin == 'B'){
		for(int i=0;i<NUMBERofCHROMOSOMES;i++){
			genome[0].push_back(Chromosome(origin));
			genome[1].push_back(Chromosome(origin));
		}
	} else {
		for(int i=0;i<NUMBERofCHROMOSOMES;i++){
			genome[0].push_back('A');
			genome[1].push_back('B');
		}
	}
}

Individual::Individual(vector<Chromosome> gamete1, vector<Chromosome> gamete2){
	int i; 
	for(i=0;i<NUMBERofCHROMOSOMES;i++){
		genome[0].push_back(gamete1[i]);
		genome[1].push_back(gamete2[i]);
	}
}

void Individual::replace_chromozome(int set, int position, map <int, char>  input_chrom){
	genome[set][position] = input_chrom;
}

vector<Chromosome> Individual::makeGamete(){
// iniciatiaztion of variables
	vector<int> recombination;
	Chromosome recombinant_ch;
	vector<Chromosome> gamete;
	char last_material_s1, last_material_s2;
	int rec_pos, numberOfChaisma, starts_by;

// for every chromosome...
// 	cout << NUMBERofCHROMOSOMES << endl;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
// it is very important to understand syntax genome[set][chromosome]
		numberOfChaisma = getChiasma(RECOMBINATIONrate);
		starts_by = tossAcoin();
		
// 		cout << "numberOfChaisma " << numberOfChaisma << " starts_by " << starts_by << endl; 
// no chiasma mean inheritance of whole one parent chromosome
		if(numberOfChaisma == 0){
			gamete.push_back(genome[starts_by][i]);
			continue;
		}
		
// inicialization / restart of variables
		auto pos1=genome[0][i].begin();
		auto pos2=genome[1][i].begin();
		last_material_s1 = genome[0][i].read(1);
		last_material_s2 = genome[1][i].read(1);
		recombination.clear();
		recombinant_ch.clear();
// roll the recombination positions
		for(int index=0;index<numberOfChaisma;index++){
			recombination.push_back(recombPosition());
		}
		sort (recombination.begin(), recombination.end(), arrangeObject);
		
// 		cout << "Rolled: ";
// 		for(int index=0;index<numberOfChaisma;index++){
// 			cout << recombination[index] << ' ';
// 		}
// 		cout << endl;
		

		if(recombination[0] != 0){
// 			cout << "Writing " << 1 << ' ' << genome[starts_by][i].read(1) << endl;
			recombinant_ch.write(1,genome[starts_by][i].read(1));
		} else {
// 			cout << "Writing " << 1 << ' ' << genome[(starts_by + 1) % 2][i].read(1) << endl;
			recombinant_ch.write(1,genome[(starts_by + 1) % 2][i].read(1));
		}
		pos1++;
		pos2++;
		
		for(int index=0;index<numberOfChaisma;index++){
			rec_pos = recombination[index];
			if(rec_pos == 0){
				starts_by = (starts_by + 1) % 2;
				continue;
			}
			
			if(starts_by==0){
				while(pos1->first < rec_pos and pos1->first != genome[0][i].end()->first){
// 					cout << "wRiting " << pos1->first << ' ' << pos1->second << endl;
					recombinant_ch.write(pos1->first,pos1->second);
					last_material_s1 = pos1->second;
					pos1++;
				}
				while(pos2->first <= rec_pos and pos2->first != genome[1][i].end()->first){
					last_material_s2 = pos2->second;
					pos2++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
// 					cout << "wrIting " << recombination[index] << ' ' << last_material_s2 << endl;
					recombinant_ch.write(recombination[index],last_material_s2);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
// 					cout << "wriTing " << recombination[index] << ' ' << last_material_s2 << endl;
					recombinant_ch.write(recombination[index],last_material_s2);
				}
			} else {
				while(pos2->first < rec_pos and pos2->first != genome[1][i].end()->first){
// 					cout << "writIng " << pos2->first << ' ' << pos2->second << endl;
					recombinant_ch.write(pos2->first,pos2->second);
					last_material_s2 = pos2->second;
					pos2++;
				}
				while(pos1->first <= rec_pos and pos1->first != genome[0][i].end()->first){
					last_material_s1 = pos1->second;
					pos1++;
				}
				if(last_material_s1 == 'A' and last_material_s2 == 'B'){
// 					cout << "writiNg " << recombination[index] << ' ' << last_material_s1 << endl;
					recombinant_ch.write(recombination[index],last_material_s1);
				}
				if(last_material_s1 == 'B' and last_material_s2 == 'A'){
// 					cout << "writinG " << recombination[index] << ' ' << last_material_s1 << endl;
					recombinant_ch.write(recombination[index],last_material_s1);
				}
			}
			starts_by = (starts_by + 1) % 2;
		}
	if(starts_by==0){
		while(pos1->first != genome[0][i].end()->first){
// 			cout << "writing " << pos1->first << ' ' << pos1->second << endl;
			recombinant_ch.write(pos1->first,pos1->second);
			last_material_s1 = pos1->second;
			pos1++;
		}
		while(pos2->first != genome[1][i].end()->first){
			last_material_s2 = pos2->second;
			pos2++;
		}
	} else {
		while(pos2->first != genome[1][i].end()->first){
// 			cout << "writing " << pos2->first << ' ' << pos2->second << endl;
			recombinant_ch.write(pos2->first,pos2->second);
			last_material_s2 = pos2->second;
			pos2++;
		}
		while(pos1->first != genome[0][i].end()->first){
			last_material_s1 = pos1->second;
			pos1++;
		}
	}

// add recombined gamete to vector
		gamete.push_back(recombinant_ch);
	}
	return gamete;
}

double Individual::getFitness(){
	double Bcount = 0;
	double fitness = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		Bcount += genome[0][i].countB();
		Bcount += genome[1][i].countB();
	}
	Bcount = Bcount / (getResolution()*2); /* relative B count*/
	fitness = 1 - (SELECTIONpressure * pow( 4 * Bcount * (1 - Bcount),BETA));
	return fitness;
}

bool Individual::Acheck(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[0][i].Acheck() and genome[1][i].Acheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

bool Individual::Bcheck(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		if(genome[0][i].Bcheck() and genome[1][i].Bcheck()){
			continue;
		}
		return 0;
	}
	return 1;
}

void Individual::readGenotype(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		cout << "---Chromozome---set-1---number-" << i+1 << "---" << endl;
		genome[0][i].showChromosome();
		cout << "---Chromozome---set-2---number-" << i+1 << "---" << endl;
		genome[1][i].showChromosome();
	}
	cout << endl;
}

void Individual::viewGenotype(){
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		cout << "---Chromozome---set-1---number-" << i+1 << "---" << endl;
		genome[0][i].viewChromosome();
		cout << "---Chromozome---set-2---number-" << i+1 << "---" << endl;
		genome[1][i].viewChromosome();
	}
	cout << endl;
}

void Individual::plotGenotype(){
	char last_material_s1, last_material_s2;
	int dev = 0, x1 = 1, x2 = 1;
	dev = g2_open_X11(512,512);
	g2_set_line_width(dev,1);
// 	A = modra populace; B = zluta
	for(int i=0; i < NUMBERofCHROMOSOMES; i++){
		auto pos1=genome[0][i].begin();
		pos1++;
		auto pos2=genome[1][i].begin();
		pos2++;
		last_material_s1 = genome[0][i].read(1);
		last_material_s2 = genome[1][i].read(1);
		while(pos1->first != genome[0][i].end()->first){
			cout << pos1->first << " " << pos2->first << endl;
			if(pos1->first <= pos2->first){
				if(x1 < x2){
					x1 = pos1->first;
				} else {
					x2 = pos1->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,3);
					} else {
						g2_pen(dev,25);
					}
				} else {
					g2_pen(dev,7);
				}
				g2_line(dev,((double)x1 / RESOLUTION)*512,512-i,((double)x2 / RESOLUTION)*512,512-i);
				
				last_material_s1 = pos1->second;
				pos1++;
				continue;
			}
			
			while(pos2->first <= pos1->first){
				if(x1 < x2){
					x1 = pos2->first;
				} else {
					x2 = pos2->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,3);
					} else {
						g2_pen(dev,25);
					}
				} else {
					g2_pen(dev,7);
				}
				g2_line(dev,((double)x1 / RESOLUTION)*512,512-i,((double)x2 / RESOLUTION)*512,512-i);
				last_material_s2 = pos2->second;
				pos2++;
			}	
		}
		while(pos2->first <= pos1->first){
			if(x1 < x2){
				x1 = pos2->first;
			} else {
				x2 = pos2->first;
			}
			if(last_material_s1 == last_material_s2){
				if(last_material_s1 == 'A'){
					g2_pen(dev,3);
				} else {
					g2_pen(dev,25);
				}
			} else {
				g2_pen(dev,7);
			}
			g2_line(dev,((double)x1 / RESOLUTION)*512,512-i,((double)x2 / RESOLUTION)*512,512-i);
			last_material_s2 = pos2->second;
			pos2++;
		}
		
		if(x1 < x2){
				x1 = RESOLUTION;
			} else {
				x2 = RESOLUTION;
		}
		if(last_material_s1 == last_material_s2){
			if(last_material_s1 == 'A'){
					g2_pen(dev,3);
			} else {
					g2_pen(dev,25);
			}
		} else {
			g2_pen(dev,7);
		}
		g2_line(dev,((double)x1 / RESOLUTION)*512,512-i,((double)x2 / RESOLUTION)*512,512-i);
	}
}










