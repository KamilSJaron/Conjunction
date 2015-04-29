// CHROMOSOME
// bool Chromosome::CorCheck(){
// 	char last_char = 'x';
// 	for(map<int, char>::const_iterator pos=chromosome.begin(); pos!=chromosome.end(); ++pos){
// 		if(last_char == pos->second){
// 			return 1;
// 		}
// 		last_char = pos->second;
// 	}
// 	return 0;
// }

// INDIVIDUAL

//from getFitness fction
// 	cerr << "B count " << Bcount << " fitness" << fitness << endl;
// 	cerr << fitness << ' ';
// 	if(fitness < 0.5 or fitness > 1){
// 		cerr << "WARNING: The finess is " << fitness << endl;
// 	}


/* GRAPHICS */
// 		void plotGenotype();
// 		void plotGenotype(int dev,int line,int height, int width, int demesize);
// void Individual::plotGenotype(){
// 	char last_material_s1, last_material_s2;
// 	int colA = 3, colB = 25, colH = 7;
// 	int dev = 0, x1 = 1, x2 = 1;
// 	int height = 10 * NUMBERofCHROMOSOMES;
// 	int width = 512;
// 	dev = g2_open_X11(width,height);
// 	g2_set_line_width(dev,10);
// // // 	A = modra populace; B = zluta
// 	for(int i=0; i < NUMBERofCHROMOSOMES; i++){
// 		map<int, char>::const_iterator pos1=genome[0][i].begin();
// 		map<int, char>::const_iterator pos2=genome[1][i].begin();
// 		last_material_s1 = pos1->second;
// 		last_material_s2 = pos2->second;
// 		pos1++;
// 		pos2++;
// 		x1 = 1;
// 		x2 = 1;
// 		while(pos1->first != genome[0][i].end()->first){
// // 			cerr << "COMPARING:" << pos1->first << " " << pos2->first << "\n";
// 			if(pos1->first <= pos2->first){
// 				if(x1 < x2){
// 					x1 = pos1->first;
// 				} else {
// 					x2 = pos1->first;
// 				}
// 				if(last_material_s1 == last_material_s2){
// 					if(last_material_s1 == 'A'){
// 						g2_pen(dev,colA);
// 					} else {
// 						g2_pen(dev,colB);
// 					}
// 				} else {
// 					g2_pen(dev,colH);
// 				}
// // 				cerr << x1 << ' ' << x2 << endl;
// 				g2_line(dev,((double)x1 / LOCI)*width,
// 								(height-(i*10))-5,
// 								((double)x2 / LOCI)*width,
// 								(height-(i*10))-5);
// 				
// 				last_material_s1 = pos1->second;
// 				pos1++;
// 				continue;
// 			}
// 			if(pos2->first == genome[1][i].end()->first){
// 				break;
// 			}
// 			
// 			while((pos2->first <= pos1->first) and pos2->first != genome[1][i].end()->first){
// 				if(x1 < x2){
// 					x1 = pos2->first;
// 				} else {
// 					x2 = pos2->first;
// 				}
// 				if(last_material_s1 == last_material_s2){
// 					if(last_material_s1 == 'A'){
// 						g2_pen(dev,colA);
// 					} else {
// 						g2_pen(dev,colB);
// 					}
// 				} else {
// 					g2_pen(dev,colH);
// 				}
// // 				cerr << x1 << ' ' << x2 << endl;
// 				g2_line(dev,((double)x1 / LOCI)*width,
// 								(height-(i*10))-5,
// 								((double)x2 / LOCI)*width,
// 								(height-(i*10))-5);
// 				last_material_s2 = pos2->second;
// 				pos2++;
// 			}
// 		}
// 		
// 		while(pos2->first != genome[1][i].end()->first){
// 			if(x1 < x2){
// 				x1 = pos2->first;
// 			} else {
// 				x2 = pos2->first;
// 			}
// 			if(last_material_s1 == last_material_s2){
// 				if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 				} else {
// 					g2_pen(dev,colB);
// 				}
// 			} else {
// 				g2_pen(dev,colH);
// 			}
// // 			cerr << x1 << ' ' << x2 << endl;
// 			g2_line(dev,((double)x1 / LOCI)*width,
// 							(height-(i*10))-5,
// 							((double)x2 / LOCI)*width,
// 							(height-(i*10))-5);
// 			last_material_s2 = pos2->second;
// 			pos2++;
// 		}
// 		
// 		while(pos1->first != genome[0][i].end()->first){
// 			if(x1 < x2){
// 				x1 = pos1->first;
// 			} else {
// 				x2 = pos1->first;
// 			}
// 			if(last_material_s1 == last_material_s2){
// 				if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 				} else {
// 					g2_pen(dev,colB);
// 				}
// 			} else {
// 				g2_pen(dev,colH);
// 			}
// // 			cerr << x1 << ' ' << x2 << endl;
// 			g2_line(dev,((double)x1 / LOCI)*width,
// 							(height-(i*10))-5,
// 							((double)x2 / LOCI)*width,
// 							(height-(i*10))-5);
// 			last_material_s1 = pos1->second;
// 			pos1++;
// 		}
// // 		cerr << pos1->first << " " << genome[0][i].end()->first << endl;
// // 		cerr << pos2->first << " " << genome[1][i].end()->first << endl;
// 		
// 		if(x1 < x2){
// 				x1 = LOCI;
// 			} else {
// 				x2 = LOCI;
// 		}
// 		if(last_material_s1 == last_material_s2){
// 			if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 			} else {
// 					g2_pen(dev,colB);
// 			}
// 		} else {
// 			g2_pen(dev,colH);
// 		}
// 		g2_line(dev,((double)x1 / LOCI)*width,
// 						(height-(i*10))-5,
// 						((double)x2 / LOCI)*width,
// 						(height-(i*10))-5);
// 	}
// 	return;
// }
// 
// void Individual::plotGenotype(int dev,int line,int height, int width, int demesize){
// 	char last_material_s1, last_material_s2;
// 	int x_buf_1 = 1, x_buf_2 = 1;
// 	int x1 = 1, x2 = 1, y = 1;
// 	int colA = 3, colB = 25, colH = 7;
// 	g2_set_line_width(dev,height);
// // // 	A = modra populace; B = zluta
// 	for(int i=0; i < NUMBERofCHROMOSOMES; i++){
// 		map<int, char>::const_iterator pos1=genome[0][i].begin();
// 		map<int, char>::const_iterator pos2=genome[1][i].begin();
// 		last_material_s1 = pos1->second;
// 		last_material_s2 = pos2->second;
// 		pos1++;
// 		pos2++;
// 		x_buf_1 = 1;
// 		x_buf_2 = 1;
// 		while(pos1->first != genome[0][i].end()->first){
// 			if(pos1->first <= pos2->first){
// 				if(x_buf_1 < x_buf_2){
// 					x_buf_1 = pos1->first;
// 				} else {
// 					x_buf_2 = pos1->first;
// 				}
// 				if(last_material_s1 == last_material_s2){
// 					if(last_material_s1 == 'A'){
// 						g2_pen(dev,colA);
// 					} else {
// 						g2_pen(dev,colB);
// 					}
// 				} else {
// 					g2_pen(dev,colH);
// 				}
// 				x1 = (((double)x_buf_1 / LOCI)*width) + (i * width);
// 				y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
// 				x2 = (((double)x_buf_2 / LOCI)*width) + (i * width);
// 				g2_line(dev,x1,y,x2,y);
// 				
// 				last_material_s1 = pos1->second;
// 				pos1++;
// 				continue;
// 			}
// 			if(pos2->first == genome[1][i].end()->first){
// 				break;
// 			}
// 			
// 			while((pos2->first <= pos1->first) and pos2->first != genome[1][i].end()->first){
// 				if(x_buf_1 < x_buf_2){
// 					x_buf_1 = pos2->first;
// 				} else {
// 					x_buf_2 = pos2->first;
// 				}
// 				if(last_material_s1 == last_material_s2){
// 					if(last_material_s1 == 'A'){
// 						g2_pen(dev,colA);
// 					} else {
// 						g2_pen(dev,colB);
// 					}
// 				} else {
// 					g2_pen(dev,colH);
// 				}
// 				x1 = (((double)x_buf_1 / LOCI)*width) + (i * width);
// 				y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
// 				x2 = (((double)x_buf_2 / LOCI)*width) + (i * width);
// 				g2_line(dev,x1,y,x2,y);
// 				
// 				last_material_s2 = pos2->second;
// 				pos2++;
// 			}
// 		}
// 		
// 		while(pos2->first != genome[1][i].end()->first){
// 			if(x_buf_1 < x_buf_2){
// 				x_buf_1 = pos2->first;
// 			} else {
// 				x_buf_2 = pos2->first;
// 			}
// 			if(last_material_s1 == last_material_s2){
// 				if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 				} else {
// 					g2_pen(dev,colB);
// 				}
// 			} else {
// 				g2_pen(dev,colH);
// 			}
// 			x1 = (((double)x_buf_1 / LOCI)*width) + (i * width);
// 			y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
// 			x2 = (((double)x_buf_2 / LOCI)*width) + (i * width);
// 			g2_line(dev,x1,y,x2,y);
// 				
// 			last_material_s2 = pos2->second;
// 			pos2++;
// 		}
// 		
// 		while(pos1->first != genome[0][i].end()->first){
// 			if(x_buf_1 < x_buf_2){
// 				x_buf_1 = pos1->first;
// 			} else {
// 				x_buf_2 = pos1->first;
// 			}
// 			if(last_material_s1 == last_material_s2){
// 				if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 				} else {
// 					g2_pen(dev,colB);
// 				}
// 			} else {
// 				g2_pen(dev,colH);
// 			}
// 			x1 = (((double)x_buf_1 / LOCI)*width) + (i * width);
// 			y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
// 			x2 = (((double)x_buf_2 / LOCI)*width) + (i * width);
// 			g2_line(dev,x1,y,x2,y);
// 			
// 			last_material_s1 = pos1->second;
// 			pos1++;
// 		}
// 		
// 		if(x_buf_1 < x_buf_2){
// 				x_buf_1 = LOCI;
// 			} else {
// 				x_buf_2 = LOCI;
// 		}
// 		
// 		if(last_material_s1 == last_material_s2){
// 			if(last_material_s1 == 'A'){
// 					g2_pen(dev,colA);
// 			} else {
// 					g2_pen(dev,colB);
// 			}
// 		} else {
// 			g2_pen(dev,colH);
// 		}
// 		x1 = (((double)x_buf_1 / LOCI)*width) + (i * width);
// 		y = ((height*demesize) - (line * height)) - ((height - 1) / 2);
// 		x2 = (((double)x_buf_2 / LOCI)*width) + (i * width);
// 		g2_line(dev,x1,y,x2,y);
// 	}
// 	return;
// }

// DEME
// 		void plotDeme();
// 		void plotDeme(const char* filename);
// 		void quickBreed();

// void Deme::quickBreed(){
// 	vector<Individual> metademe;
// 	metademe.reserve(DEMEsize);
// 	vector<Chromosome> gamete1, gamete2;
// 	for(int i=0;i<DEMEsize;i++){
// 		deme[pickAnIndividual()].makeGamete(gamete1);
// 		deme[pickAnIndividual()].makeGamete(gamete2);
// 		metademe.push_back(Individual(gamete1,gamete2));
// 	}
// 	for(int i=0;i<DEMEsize;i++){
// 		deme[i] = metademe[i];
// 	}
// }

// void Deme::plotDeme(){
// 	int line = 0, dev = 0;
// 	int height = 1;
// 	int width = 50;
// 	dev = g2_open_X11(width*NUMBERofCHROMOSOMES,height*DEMEsize);
// 	g2_set_auto_flush(dev, 0);
// 	for(int i=0;i<DEMEsize;i++){
// 		line = i + 1;
// // 		cout << "Individual: " << i+1 << endl;
// 		deme[i].plotGenotype(dev, line, height, width, DEMEsize);
// 	}
// 	g2_flush(dev);
// }
// 
// void Deme::plotDeme(const char* filename){
// // 	cout << filename << endl;
// 	int line = 0, dev = 0;
// 	int height = 1;
// 	int width = 50;
// 	dev = g2_open_gd(filename,width*NUMBERofCHROMOSOMES,height*DEMEsize, g2_gd_png);
// 	for(int i=0;i<DEMEsize;i++){
// 		line = i + 1;
// // 		cout << "Individual: " << i+1 << endl;
// 		deme[i].plotGenotype(dev, line, height, width, DEMEsize);
// 	}
// 	g2_close(dev);
// }

// from LD computing function
// 		cout << endl << "updated states: ";
// 		for(int k = 0;k < TotalCHnum;k++){
// 			cout << states[k] << " ";
// 		}
// 		cout << endl;


// UNIVERSE
// void globalNaiveBreeding();
// 		void plotOneDeme(int index);
// 		void plotOneDeme(int index, const char*  fileName);
// void plotDemesOneByOne();
// void plotDemesOneByOne(char fileNamepattern[]);

// 		function used for testing
// 		int getNumOfDemesInColumn(){return number_of_demes_u_d;};
// 		int getSpaceSize(){return space.size();};
// 		int getIndex(int i);

// void Universe::globalNaiveBreeding(){
// 	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
// 		i->second->quickBreed();
// 	}
// }

// void Universe::infSimulator(vector<Imigrant>& GogolBordello, int NUMofGEN){
// 	for(int generation = 1; generation < NUMofGEN; generation ++){
// // 		migration
// 		for(int i = 0; i < DEMEsize;i++){
// 			GogolBordello.push_back(Imigrant('B'));
// 		}
// // 		selection & reproduction
// 		
// 		
// 	}
// // 	create zero dimensional space
// // 	one specialized deme with redefined migration and breeding?
// }

// void Universe::plotDemesOneByOne(){
// 	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
// 		cout << "***** DEME " << i->first << " *****" << endl;
// 		i->second->plotDeme();
// 		cin.get();
// 	}
// }

// void Universe::plotDemesOneByOne(char fileNamepattern[]){
// 	int len = strlen(fileNamepattern);
//   static char* newName = fileNamepattern;
// 	char decimal = '0', unit = '0';
// /*potebuju nejak z paterny udelat specificky nazev ktery budu moct poslat jako argument do plotovaci fce*/
// /* Mám pocit, že to bude fungovat jen pro 20 nik */ 
// 	for (map<int, Deme*>::const_iterator i=space.begin(); i!=space.end(); ++i){
// 		if(i->first > 10){
// 			decimal = '1';
// 			if(i->first > 20){
// 				decimal = '2';
// 				if(i->first == 30){
// 					cerr << "WARNING: Too much pictures. Only first 30 are plotted.";
// 					break;
// 				}
// 			}
// 		}
// 		cout << "Unit: " << i->first % 10 << ' '; 
// 		unit = char('0' + i->first % 10);
// 
// 		fileNamepattern[len - 6] = decimal;
// 		fileNamepattern[len - 5] = unit;
// 		cout << "***** DEME " << i->first << " *****" << endl;
// 		i->second->plotDeme(newName);
// 		cin.get();
// 	}
// }

// void Universe::plotOneDeme(int index){
// 	space[index]->plotDeme();
// }
// 
// void Universe::plotOneDeme(int index, const char* fileName){
// 	space[index]->plotDeme(fileName);
// }


// MAIN
// #include<g2.h>
// #include<g2_X11.h>
// #include<g2_PS.h>
// #include<g2_gd.h>

// int testParameters(Universe* World){
// 	World->set(0,"halfAhalfhetero");
// 	for(int i=0;i < NUMBERofGENERATIONS;i++){
// 		cerr << "Genertion " << i << endl; 
// 		cerr << "Homozygotes A: " << World->getProportionOfHomozygotes(0,'A') << endl;
// 		cerr << "Heterozygotes: "<< World->getProportionOfHeterozygotes(0) << endl;
// 		cerr << "Homozygotes B: "<< World->getProportionOfHomozygotes(0,'B') << endl;
// 		World->Breed(0);
// 	}
// 	
// 	return 0;
// }