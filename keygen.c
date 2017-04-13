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

  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  randomNLENBITS(Message,r);

  NNMATRIX *S=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  randomNNMATRIX(S,r);

  MMMATRIX *T=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  randomMMMATRIX(T,r);

  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  randomF(F,r);

  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  MMMATRIXoNNMATRIXxMoNNMATRIX(G,T,F,S);

  FILE *fp;
  int i,j,k;
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
  free(Message);
  
  if((fp=fopen("./KEYS/pkF.bin","wb"))==NULL){
    printf("File pkF.bin can't open as writable.\n");
    free(r);
    
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
  free(F);
  
  if((fp=fopen("./KEYS/pkG.bin","wb"))==NULL){
    printf("File pkG.bin can't open as writable.\n");
    free(r);
    
    free(S);
    free(T);
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

  if((fp=fopen("./KEYS/skS.bin","wb"))==NULL){
    printf("File skS.bin can't open as writable.\n");
    free(r);
    
    free(S);
    free(T);
    free(G);
    return 1;
  }
  
  for(j=0;j<NUM_N;j++){
    for(k=0;k<4*INTS_N;k++){
      fputc(S->Matrix[j]._1byte[k]._8bit,fp);
    }
  }
  fclose(fp);
  free(S);
  
  if((fp=fopen("./KEYS/skT.bin","wb"))==NULL){
    printf("File skT.bin can't open as writable.\n");
    free(r);
    
    free(T);
    free(G);
    return 1;
  }
  
  for(j=0;j<NUM_M;j++){
    for(k=0;k<4*INTS_M;k++){
      fputc(T->Matrix[j]._1byte[k]._8bit,fp);
    }
  }
  fclose(fp);
  free(T);

  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX)*NUM_L);
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX)*NUM_L);
  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM)*NUM_L);

  for(i=0;i<NUM_L;i++){
    randomNNMATRIX(&(R[i]),r);
    randomMMMATRIX(&(L[i]),r);
    MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[i]),&(L[i]),G,&(R[i]));
  }
  free(G);
  free(r);
  
  if((fp=fopen("./KEYS/alR.bin","wb"))==NULL){
    printf("File alR.bin can't open as writable.\n");
    free(R);
    free(L);
    free(Y);
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
  free(R);
  
  if((fp=fopen("./KEYS/alL.bin","wb"))==NULL){
    printf("File alL.bin can't open as writable.\n");
    free(L);
    free(Y);
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
  free(L);
  
  //Hash Y
  unsigned char sha1_Y[20];  
  SHA_CTX c;
  SHA1_Init(&c);
  SHA1_Update(&c,Y,4*INTS_N*NUM_N*2*NUM_M*NUM_L);
  SHA1_Final(sha1_Y,&c);
  free(Y);
  
  if((fp=fopen("./KEYS/alY.bin","wb"))==NULL){
    printf("File alY.bin can't open as writable.\n");
    return 1;
  }
  
  for(i=0;i<20;i++){
    fputc(sha1_Y[i],fp);
  }
  fclose(fp);
  return 0;
}
