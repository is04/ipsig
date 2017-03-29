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
#include "functions_LLEN.h"
#include "functions_NNMATxM.h"

int LoadKeys(NNMATRIXxM *G,NNMATRIXxM *F,NLENBITS *Message,LLENBITS *b,MATRIXPAIR *Z){
  FILE *fp;
  int i,j,k;
  if((fp=fopen("Msg.bin","rb"))==NULL){
    printf("File Msg.bin can't open as readable.\n");
    return 1;
  }
  
  for(k=0;k<4*INTS_N;k++){
    Message->_1byte[k]._8bit=fgetc(fp);
  }
  fclose(fp);

  //seed->matrix
  if((fp=fopen("pkF.bin","rb"))==NULL){
    printf("File pkF.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	F->No[i].Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
  }
  NNMATRIXxMtoTRANSPOSE(F);
  fclose(fp);

  if((fp=fopen("pkG.bin","rb"))==NULL){
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
  
  if((fp=fopen("sgb.bin","rb"))==NULL){
    printf("File sgb.bin can't open as readable.\n");
    return 1;
  }
  
  for(k=0;k<4*INTS_L;k++){
    b->_1byte[k]._8bit=fgetc(fp);
  }
  fclose(fp);

  //seed&matrix->matrix
  if((fp=fopen("sgZ.bin","rb"))==NULL){
    printf("File sgZ.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	Z[i].NN.Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
    NNMATRIXtoTRANSPOSE(&(Z[i].NN));
    for(j=0;j<NUM_M;j++){
      for(k=0;k<4*INTS_M;k++){
	Z[i].MM.Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
    MMMATRIXtoTRANSPOSE(&(Z[i].MM));  
  }
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

  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  
  LLENBITS *b=(LLENBITS*)malloc(sizeof(LLENBITS));
  MATRIXPAIR *Z=(MATRIXPAIR*)malloc(sizeof(MATRIXPAIR)*NUM_L);

  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM)*NUM_L);
  
  unsigned int tmp;
  
  int i;

  
  if(LoadKeys(G,F,Message,b,Z)==1){
    free(r);
    free(Message);
    free(F);
    free(G);
    free(b);
    free(Z);
    return 1;
  }

  LLENBITS LBTMP=*b;

  for(i=0;i<NUM_L;i++){
    if(LBTMP._4byte[0]&(0x80000000)){
      MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[i]),&(Z[i].MM),F,&(Z[i].NN));
    }else{
      MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[i]),&(Z[i].MM),G,&(Z[i].NN));
    }
    leftshiftLLENBITS(&LBTMP);
  }    

  SHA_CTX c;
  unsigned char sha1[20];
  SHA1_Init(&c);
  SHA1_Update(&c,Message,4*INTS_N);
  SHA1_Update(&c,Y,4*INTS_N*NUM_N*2*NUM_M*NUM_L);
  SHA1_Final(sha1,&c);
  
  for(i=0;i<4*INTS_L;i++){
    LBTMP._1byte[i]._8bit=sha1[i];
  }
  tmp=~0;
  tmp=tmp<<ZEROBITS_L;
  LBTMP._4byte[INTS_L-1]=(LBTMP._4byte[INTS_L-1])&tmp;

  //printf("result\n");
  //printf(" sgb  : ");
  //printLLENBITS(b);
  //printf("hashb : ");
  //printLLENBITS(&LBTMP);
  if(chkeqLLENBITS(b,&(LBTMP))){
    printf("Verify OK!\n");
  }else{
    printf("Verify NG!\n");
  }

  free(r);
  free(Message);
  free(F);
  free(G);
  free(b);
  free(Y);
  free(Z);
  return 0;
}
