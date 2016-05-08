#ifndef ARRANGE_H
#define ARRANGE_H

/*struct arrangeClass {
  bool operator() (int i,int j);
} arrangeObject;*/

struct arrangeClass {
  bool operator() (int i,int j) { return (i<j);}
} arrangeObject;

#endif
