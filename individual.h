#include "chromosome.h"
#include <g2.h>
static int NUMBERofCHROMOSOMES = 20; /* mouse have 19+1, but now I am reproducing 84 results */
static double RECOMBINATIONrate = 1;
static double SELECTIONpressure = 0.5;
static double BETA = 1;


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
/* DECLARATION */
		Individual();
		Individual(char origin); /* init Individual as 'A', 'B' or pure "AB" heterozygot*/
		Individual(vector<Chromosome>& gamete1, vector<Chromosome>& gamete2); /*init Individual by gametes on imput */
		~Individual(){genome[0].clear();genome[1].clear();}; /* destructor */
		
/* COMPUTIONG METHODS */
		void makeGamete(vector<Chromosome>& gamete); //
		void replace_chromozome(int set, int position,map <int, char> input_chrom);
		double getFitness();
		double getBprop();
		bool Acheck();
		bool Bcheck();
		
/* ASCII PLOTTING METHODS */
		void readGenotype();
		void viewGenotype();
		
/* GRAPHICS */
		void plotGenotype();
		void plotGenotype(int dev,int line,int height, int width, int demesize);
		
/* COMUNICATION */
		static void setNumberOfChromosomes(int ch){NUMBERofCHROMOSOMES = ch;};
		static void setRECOMBINATIONrate(double Rr){RECOMBINATIONrate = Rr;};
		static void setSELECTIONpressure(double Sp){SELECTIONpressure = Sp;};
		static void setBETA(double beta){BETA = beta;};
	
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

Individual::Individual(vector<Chromosome>& gamete1, vector<Chromosome>& gamete2){
	int i; 
	for(i=0;i<NUMBERofCHROMOSOMES;i++){
		genome[0].push_back(gamete1[i]);
		genome[1].push_back(gamete2[i]);
	}
}

void Individual::replace_chromozome(int set, int position, map <int, char>  input_chrom){
	genome[set][position] = input_chrom;
}

void Individual::makeGamete(vector<Chromosome>& gamete){
// iniciatiaztion of variables
	gamete.clear();
	vector<int> recombination;
	Chromosome recombinant_ch;
	char last_material_s1, last_material_s2;
	int rec_pos, numberOfChaisma, starts_by;

/* for every chromosome... */
// 	cout << NUMBERofCHROMOSOMES << endl;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
/* it is very important to understand syntax genome[set][chromosome] */
		numberOfChaisma = getChiasma(RECOMBINATIONrate);
// 		cout << numberOfChaisma << ' ';
		starts_by = tossAcoin();
		
/* no chiasma mean inheritance of whole one parent chromosome */
		if(numberOfChaisma == 0){
			gamete.push_back(genome[starts_by][i]);
			continue;
		}
		
/* inicialization / restart of variables */
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
		sort(recombination.begin(), recombination.end(), arrangeObject);
		
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
}

double Individual::getFitness(){
	double Bcount = 0;
	double fitness = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		Bcount += genome[0][i].countB();
		Bcount += genome[1][i].countB();
	}
	Bcount = Bcount / (getResolution()*2*NUMBERofCHROMOSOMES); /* relative B count*/
	fitness = 1 - (SELECTIONpressure * pow( 4 * Bcount * (1 - Bcount),BETA));
// 	cout << fitness << ' ';
	return fitness;
}

double Individual::getBprop(){
	double prop = 0;
	for(int i=0;i<NUMBERofCHROMOSOMES;i++){
		prop += genome[0][i].countB();
		prop += genome[1][i].countB();
	}
	prop = prop / (getResolution()*2*NUMBERofCHROMOSOMES);
	return prop;
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
	int colA = 3, colB = 25, colH = 7;
	int dev = 0, x1 = 1, x2 = 1;
	int height = 10 * NUMBERofCHROMOSOMES;
	int width = 512;
	dev = g2_open_X11(width,height);
	g2_set_line_width(dev,10);
// // 	A = modra populace; B = zluta
	for(int i=0; i < NUMBERofCHROMOSOMES; i++){
		auto pos1=genome[0][i].begin();
		auto pos2=genome[1][i].begin();
		last_material_s1 = pos1->second;
		last_material_s2 = pos2->second;
		pos1++;
		pos2++;
		x1 = 1;
		x2 = 1;
		while(pos1->first != genome[0][i].end()->first){
// 			cout << "COMPARING:" << pos1->first << " " << pos2->first << "\n";
			if(pos1->first <= pos2->first){
				if(x1 < x2){
					x1 = pos1->first;
				} else {
					x2 = pos1->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,colA);
					} else {
						g2_pen(dev,colB);
					}
				} else {
					g2_pen(dev,colH);
				}
// 				cout << x1 << ' ' << x2 << endl;
				g2_line(dev,((double)x1 / RESOLUTION)*width,
								(height-(i*10))-5,
								((double)x2 / RESOLUTION)*width,
								(height-(i*10))-5);
				
				last_material_s1 = pos1->second;
				pos1++;
				continue;
			}
			if(pos2->first == genome[1][i].end()->first){
				break;
			}
			
			while((pos2->first <= pos1->first) and pos2->first != genome[1][i].end()->first){
				if(x1 < x2){
					x1 = pos2->first;
				} else {
					x2 = pos2->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,colA);
					} else {
						g2_pen(dev,colB);
					}
				} else {
					g2_pen(dev,colH);
				}
// 				cout << x1 << ' ' << x2 << endl;
				g2_line(dev,((double)x1 / RESOLUTION)*width,
								(height-(i*10))-5,
								((double)x2 / RESOLUTION)*width,
								(height-(i*10))-5);
				last_material_s2 = pos2->second;
				pos2++;
			}
		}
		
		while(pos2->first != genome[1][i].end()->first){
			if(x1 < x2){
				x1 = pos2->first;
			} else {
				x2 = pos2->first;
			}
			if(last_material_s1 == last_material_s2){
				if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
				} else {
					g2_pen(dev,colB);
				}
			} else {
				g2_pen(dev,colH);
			}
// 			cout << x1 << ' ' << x2 << endl;
			g2_line(dev,((double)x1 / RESOLUTION)*width,
							(height-(i*10))-5,
							((double)x2 / RESOLUTION)*width,
							(height-(i*10))-5);
			last_material_s2 = pos2->second;
			pos2++;
		}
		
		while(pos1->first != genome[0][i].end()->first){
			if(x1 < x2){
				x1 = pos1->first;
			} else {
				x2 = pos1->first;
			}
			if(last_material_s1 == last_material_s2){
				if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
				} else {
					g2_pen(dev,colB);
				}
			} else {
				g2_pen(dev,colH);
			}
// 			cout << x1 << ' ' << x2 << endl;
			g2_line(dev,((double)x1 / RESOLUTION)*width,
							(height-(i*10))-5,
							((double)x2 / RESOLUTION)*width,
							(height-(i*10))-5);
			last_material_s1 = pos1->second;
			pos1++;
		}
// 		cout << pos1->first << " " << genome[0][i].end()->first << endl;
// 		cout << pos2->first << " " << genome[1][i].end()->first << endl;
		
		if(x1 < x2){
				x1 = RESOLUTION;
			} else {
				x2 = RESOLUTION;
		}
		if(last_material_s1 == last_material_s2){
			if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
			} else {
					g2_pen(dev,colB);
			}
		} else {
			g2_pen(dev,colH);
		}
		g2_line(dev,((double)x1 / RESOLUTION)*width,
						(height-(i*10))-5,
						((double)x2 / RESOLUTION)*width,
						(height-(i*10))-5);
	}
	return;
}

void Individual::plotGenotype(int dev,int line,int height, int width, int demesize){
	char last_material_s1, last_material_s2;
	int x_buf_1 = 1, x_buf_2 = 1;
	int x1 = 1, x2 = 1, y = 1;
	int colA = 3, colB = 25, colH = 7;
	g2_set_line_width(dev,height);
// // 	A = modra populace; B = zluta
	for(int i=0; i < NUMBERofCHROMOSOMES; i++){
		auto pos1=genome[0][i].begin();
		auto pos2=genome[1][i].begin();
		last_material_s1 = pos1->second;
		last_material_s2 = pos2->second;
		pos1++;
		pos2++;
		x_buf_1 = 1;
		x_buf_2 = 1;
		while(pos1->first != genome[0][i].end()->first){
			if(pos1->first <= pos2->first){
				if(x_buf_1 < x_buf_2){
					x_buf_1 = pos1->first;
				} else {
					x_buf_2 = pos1->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,colA);
					} else {
						g2_pen(dev,colB);
					}
				} else {
					g2_pen(dev,colH);
				}
				x1 = (((double)x_buf_1 / RESOLUTION)*width) + (i * width);
				y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
				x2 = (((double)x_buf_2 / RESOLUTION)*width) + (i * width);
				g2_line(dev,x1,y,x2,y);
				
				last_material_s1 = pos1->second;
				pos1++;
				continue;
			}
			if(pos2->first == genome[1][i].end()->first){
				break;
			}
			
			while((pos2->first <= pos1->first) and pos2->first != genome[1][i].end()->first){
				if(x_buf_1 < x_buf_2){
					x_buf_1 = pos2->first;
				} else {
					x_buf_2 = pos2->first;
				}
				if(last_material_s1 == last_material_s2){
					if(last_material_s1 == 'A'){
						g2_pen(dev,colA);
					} else {
						g2_pen(dev,colB);
					}
				} else {
					g2_pen(dev,colH);
				}
				x1 = (((double)x_buf_1 / RESOLUTION)*width) + (i * width);
				y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
				x2 = (((double)x_buf_2 / RESOLUTION)*width) + (i * width);
				g2_line(dev,x1,y,x2,y);
				
				last_material_s2 = pos2->second;
				pos2++;
			}
		}
		
		while(pos2->first != genome[1][i].end()->first){
			if(x_buf_1 < x_buf_2){
				x_buf_1 = pos2->first;
			} else {
				x_buf_2 = pos2->first;
			}
			if(last_material_s1 == last_material_s2){
				if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
				} else {
					g2_pen(dev,colB);
				}
			} else {
				g2_pen(dev,colH);
			}
			x1 = (((double)x_buf_1 / RESOLUTION)*width) + (i * width);
			y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
			x2 = (((double)x_buf_2 / RESOLUTION)*width) + (i * width);
			g2_line(dev,x1,y,x2,y);
				
			last_material_s2 = pos2->second;
			pos2++;
		}
		
		while(pos1->first != genome[0][i].end()->first){
			if(x_buf_1 < x_buf_2){
				x_buf_1 = pos1->first;
			} else {
				x_buf_2 = pos1->first;
			}
			if(last_material_s1 == last_material_s2){
				if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
				} else {
					g2_pen(dev,colB);
				}
			} else {
				g2_pen(dev,colH);
			}
			x1 = (((double)x_buf_1 / RESOLUTION)*width) + (i * width);
			y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
			x2 = (((double)x_buf_2 / RESOLUTION)*width) + (i * width);
			g2_line(dev,x1,y,x2,y);
			
			last_material_s1 = pos1->second;
			pos1++;
		}
		
		if(x_buf_1 < x_buf_2){
				x_buf_1 = RESOLUTION;
			} else {
				x_buf_2 = RESOLUTION;
		}
		
		if(last_material_s1 == last_material_s2){
			if(last_material_s1 == 'A'){
					g2_pen(dev,colA);
			} else {
					g2_pen(dev,colB);
			}
		} else {
			g2_pen(dev,colH);
		}
		x1 = (((double)x_buf_1 / RESOLUTION)*width) + (i * width);
		y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
		x2 = (((double)x_buf_2 / RESOLUTION)*width) + (i * width);
		g2_line(dev,x1,y,x2,y);
	}
	return;
}








