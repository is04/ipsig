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
    
  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  NNMATRIX *S=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *T=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));

  
  randomNLENBITS(Message,r);

  randomNNMATRIX(S,r);
  //
  randomMMMATRIX(T,r);
  //
  randomF(F,r);
  //
  
  MMMATRIXoNNMATRIXxMoNNMATRIX(G,T,F,S);
  
  FILE *fp;
  int i,j,k,l;
  
  if((fp=fopen("./KEYS/Msg.bin","wb"))==NULL){
    printf("File Msg.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }

  for(k=0;k<4*INTS_N;k++){
    fputc(Message->_1byte[k]._8bit,fp);
  }
  fclose(fp);

  if((fp=fopen("./KEYS/pkF.bin","wb"))==NULL){
    printf("File pkF.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }

  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	fputc(F->No[i].Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  fclose(fp);
      
  if((fp=fopen("./KEYS/pkG.bin","wb"))==NULL){
    printf("File pkG.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }

  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	fputc(G->No[i].Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  fclose(fp);

  //
  if((fp=fopen("./KEYS/skS.bin","wb"))==NULL){
    printf("File skS.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }
  
  for(j=0;j<NUM_N;j++){
    for(k=0;k<4*INTS_N;k++){
      fputc(S->Matrix[j]._1byte[k]._8bit,fp);
    }
  }
  fclose(fp);

  //
  if((fp=fopen("./KEYS/skT.bin","wb"))==NULL){
    printf("File skT.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }

  for(j=0;j<NUM_M;j++){
    for(k=0;k<4*INTS_M;k++){
      fputc(T->Matrix[j]._1byte[k]._8bit,fp);
    }
  }
  fclose(fp);
  
  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX)*NUM_L);
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX)*NUM_L);
  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM)*NUM_L);

  for(i=0;i<NUM_L;i++){
    randomNNMATRIX(&(R[i]),r);
    //
    randomMMMATRIX(&(L[i]),r);
    //
    MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[i]),&(L[i]),G,&(R[i]));
  }
  
  //
  if((fp=fopen("./KEYS/alR.bin","wb"))==NULL){
    printf("File alR.bin can't open as writable.\n");
    free(R);
    free(L);
    free(Y);
    
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	fputc(R[i].Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  fclose(fp);

  //
  if((fp=fopen("./KEYS/alL.bin","wb"))==NULL){
    printf("File alL.bin can't open as writable.\n");
    free(R);
    free(L);
    free(Y);
    
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_M;j++){
      for(k=0;k<4*INTS_M;k++){
	fputc(L[i].Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  fclose(fp);

  //Hash Y
  if((fp=fopen("./KEYS/alY.bin","wb"))==NULL){
    printf("File alY.bin can't open as writable.\n");
    free(R);
    free(L);
    free(Y);
    
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_M;j++){
      for(k=0;k<NUM_N;k++){
	for(l=0;l<4*INTS_N;l++){
	  fputc(Y[i].No[j].Matrix[k]._1byte[l]._8bit,fp);
	}
      }
    }
  }
  fclose(fp);
  
  free(R);
  free(L);
  free(Y);

  free(r);

  free(Message);
  free(S);
  free(T);
  free(F);
  free(G);
  return 0;
}
