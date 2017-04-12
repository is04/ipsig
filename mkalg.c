#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <sys/time.h>
#include <openssl/sha.h>
#include <time.h>
#include <sys/stat.h>

#include "define_IP2S.h"
#include "func_pivot_gauss_jordan.h"
#include "functions_NLEN_NNMAT.h"
#include "functions_MLEN_MMMAT.h"
#include "functions_LLEN.h"
#include "functions_NNMATxM.h"

unsigned char LoadKeys(NNMATRIXxM *G){
  FILE *fp;
  int i,j,k;
  if((fp=fopen("./KEYS/pkG.bin","rb"))==NULL){
    printf("File pkG.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	G->No[i].Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
  }
  NNMATRIXxMtoTRANSPOSE(G);
  fclose(fp);

  return 0;
}

int main(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  gsl_rng *r;
  gsl_rng_env_setup();
  r=gsl_rng_alloc(gsl_rng_default);
  gsl_rng_set(r,tv.tv_sec+tv.tv_usec);

  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  if(LoadKeys(G)){
    free(G);
    free(r);
    return 1;
  }

  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  unsigned char *seed=(unsigned char*)malloc(sizeof(unsigned char)*SEED_LEN);

  FILE *fp;
  int i,j,k,l;
  char BUFFER[256];
  
  for(l=0;l<NUM_L;l++){
    while(1){
      for(i=0;i<SEED_LEN;i++){
	seed[i]=gsl_rng_uniform_int(r,256);
      }
      if(GenNNMATRIX(R,seed)) break;
    }
    sprintf(BUFFER,"./KEYS/alR%05d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File %s can't open as writable.\n",BUFFER);

      free(r);
      free(G);
      free(R);
      free(L);
      free(Y);
      free(seed);
      return 1;
    }
    for(i=0;i<SEED_LEN;i++){
      fputc(seed[i],fp);
    }
    fclose(fp);
    
    while(1){
      for(i=0;i<SEED_LEN;i++){
	seed[i]=gsl_rng_uniform_int(r,256);
      }
      if(GenMMMATRIX(L,seed)) break;
    }
    sprintf(BUFFER,"./KEYS/alL%05d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File %s can't open as writable.\n",BUFFER);

      free(r);
      free(G);
      free(R);
      free(L);
      free(Y);
      free(seed);
      return 1;
    }
    for(i=0;i<SEED_LEN;i++){
      fputc(seed[i],fp);
    }
    fclose(fp);
    
    MMMATRIXoNNMATRIXxMoNNMATRIX(Y,L,G,R);
    sprintf(BUFFER,"./KEYS/alY%05d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File %s can't open as writable.\n",BUFFER);

      free(r);
      free(G);
      free(R);
      free(L);
      free(Y);
      free(seed);
      return 1;
    }
    
    for(i=0;i<NUM_M;i++){
      for(j=0;j<NUM_N;j++){
	for(k=0;k<4*INTS_N;k++){
	  fputc(Y->No[i].Matrix[j]._1byte[k]._8bit,fp);
	}
      }
    }
    fclose(fp);
  }

  free(seed);
  free(r);
  free(Y);
  free(R);
  free(L);
  free(G);
  return 0;
}
