#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100001


/* Francesco's random number generator with external storage of the seed */
double randomGen(int* seedIn, int* seedOut) 
{
  int i;
  double randOut;
  double temp;

  seedOut[0] = abs( (171 * seedIn[0]) % 30269 );
  seedOut[1] = abs( (172 * seedIn[1]) % 30307 );
  seedOut[2] = abs( (170 * seedIn[2]) % 30323 );

  // zero is a poor Seed, therfore substitute 1; 
  for (i=0; i<3; i++) {
    if (seedOut[i]==0) seedOut[i] = 1;
  }
  
  temp = seedOut[0]/30269.0 + seedOut[1]/30307.0 + seedOut[2]/30323.0;
  
  // rem() in modelica returns the integer remainder of the division of two reals
  randOut = fmod( temp,1.0 );

  return randOut;
}

int **seedOrig;
//int **seedOrig[N][3];
double rand_casella(int i) 
{
  double ret;
  int seedOut[3],k;

  ret = randomGen(seedOrig[i], seedOut);
    for (k=0; k<3; k++)
      seedOrig[i][k] = seedOut[k];

  return ret;
}
void init_seeds() {
  int k,j;
  srand(1);
  seedOrig=(int**)malloc(sizeof(int*)*N);
  for (k=0;k<N;k++) 
    seedOrig[k]=(int*)malloc(sizeof(int)*3);
  for (k=0;k<N;k++) 
    for (j=0;j<3;j++) 
      seedOrig[k][j]=rand();
}
int np[N];
int init=0;
int get_npeople_qss(int i) {
  int k,j,r;
  if (!init) {
    init_seeds();
    init=1;
  }
  if (np[i]==0) return (np[i]+=1);
  r=(np[i]+= (rand_casella(i)>0.5 ? 1:-1));
  return r;
}

int get_npeople(int i, double dummy_t) { return get_npeople_qss(i); }

double get_nextpeople_qss(int i) {
  double d=rand_casella(i);
  return 1000.0+1000*d;
}
double get_nextpeople(int i, double dummy_t) {
  return get_nextpeople_qss(i);
}


