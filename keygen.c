//確実にfreeされているかを確かめていない
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
#include "functions_NNMATxM.h"

int main(){
  struct stat stat_buf;
  if(0 == stat("./KEYS", &stat_buf)){
    if(system("rm -rf ./KEYS") == -1){
      return 1;
    }
  }
  
  mkdir("./KEYS",
	S_IRUSR | S_IWUSR | S_IXUSR |
	S_IRGRP | S_IWGRP | S_IXGRP |
	S_IROTH | S_IWOTH | S_IXOTH);

  struct timeval tv;
  gettimeofday(&tv,NULL);
  gsl_rng *r;
  gsl_rng_env_setup();
  r=gsl_rng_alloc(gsl_rng_default);
  gsl_rng_set(r,tv.tv_sec+tv.tv_usec);

  char BUFFER[256];
    
  int i;

  FILE *fp;

  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  randomNLENBITS(Message,r);

  if((fp=fopen("./KEYS/Msg.bin","wb"))==NULL){
    printf("File Msg.bin can't open as writable.\n");

    free(r);
    free(Message);
    return 1;
  }

  for(i=0;i<4*INTS_N;i++){
    fputc(Message->_1byte[i]._8bit,fp);
  }
  fclose(fp);
  free(Message);
  
  NNMATRIX *S=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  unsigned char *seed=(unsigned char*)malloc(sizeof(unsigned char)*SEED_LEN);
  while(1){
    for(i=0;i<SEED_LEN;i++){
      seed[i]=gsl_rng_uniform_int(r,256);
    }
    if(GenNNMATRIX(S,seed)) break;
  }
  
  if((fp=fopen("./KEYS/skS.bin","wb"))==NULL){
    printf("File skS.bin can't open as writable.\n");

    free(r);
    free(S);
    free(seed);
    return 1;
  }
  
  for(i=0;i<SEED_LEN;i++){
    fputc(seed[i],fp);
  }
  fclose(fp);
  
  MMMATRIX *T=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  while(1){
    for(i=0;i<SEED_LEN;i++){
      seed[i]=gsl_rng_uniform_int(r,256);
    }
    if(GenMMMATRIX(T,seed)) break;
  }
  
  if((fp=fopen("./KEYS/skT.bin","wb"))==NULL){
    printf("File skT.bin can't open as writable.\n");

    free(r);
    free(S);
    free(T);
    free(seed);
    return 1;
  }
  
  for(i=0;i<SEED_LEN;i++){
    fputc(seed[i],fp);
  }
  fclose(fp);

  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  for(i=0;i<SEED_LEN;i++){
    seed[i]=gsl_rng_uniform_int(r,256);
  }
  
  GenNNMATRIXxM(F,seed);
  
  if((fp=fopen("./KEYS/pkF.bin","wb"))==NULL){
    printf("File pkF.bin can't open as writable.\n");
 
    free(r);
    free(S);
    free(T);
    free(F);
    free(seed);
    return 1;
  }

  for(i=0;i<SEED_LEN;i++){
    fputc(seed[i],fp);
  }
  fclose(fp);
  
  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  MMMATRIXoNNMATRIXxMoNNMATRIX(G,T,F,S);
  
  if((fp=fopen("./KEYS/pkG.bin","wb"))==NULL){
    printf("File pkG.bin can't open as writable.\n");
    
    free(r);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }
  
  int j,k,l;
  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	fputc(G->No[i].Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  fclose(fp);
  free(S);
  free(T);
  free(F);
  
  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  for(l=0;l<NUM_L;l++){
    while(1){
      for(i=0;i<SEED_LEN;i++){
	seed[i]=gsl_rng_uniform_int(r,256);
      }
      if(GenNNMATRIX(R,seed)) break;
    }
    sprintf(BUFFER,"./KEYS/alR%d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File alR%d.bin can't open as writable.\n",l);

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
    sprintf(BUFFER,"./KEYS/alL%d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File alL%d.bin can't open as writable.\n",l);

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
    sprintf(BUFFER,"./KEYS/alY%d.bin",l);
    if((fp=fopen(BUFFER,"wb"))==NULL){
      printf("File alY%d.bin can't open as writable.\n",l);

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
  free(G);
  free(R);
  free(L);
  free(Y);
  return 0;
}
