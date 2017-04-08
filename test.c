#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <sys/time.h>
#include <openssl/sha.h>
#include <time.h>

#include "define_IP2S.h"
#include "func_pivot_gauss_jordan.h"
#include "functions_NLEN_NNMAT.h"
#include "functions_MLEN_MMMAT.h"
#include "functions_NNMATxM.h"

int main(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  gsl_rng *r;
  gsl_rng_env_setup();
  r=gsl_rng_alloc(gsl_rng_default);
  gsl_rng_set(r,tv.tv_sec+tv.tv_usec);
  
  MMMATRIX *nm=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  NNMATRIXxM *nmxm=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  unsigned char *seed=(unsigned char*)malloc(sizeof(unsigned char)*SEED_LEN);
  int i,cnt=0;
  
  while(1){
    for(i=0;i<SEED_LEN;i++){
      seed[i]=gsl_rng_uniform_int(r,256);
    }
    cnt++;
    if(GenMMMATRIX(nm,seed)) break;
  }
  
  printf("Trial counter=%d\n",cnt);
  
  GenNNMATRIXxM(nmxm,seed);
  printMMMATRIX(nm);
  printNNMATRIXxM(nmxm);
  free(r);
  free(nm);
  free(nmxm);
  free(seed);
  return 0;
}
