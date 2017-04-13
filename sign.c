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

int LoadKeys(NNMATRIXxM *G,MMMATRIX *T,NNMATRIXxM *F,NNMATRIX *S,NLENBITS *Message,NNMATRIX *R,MMMATRIX *L,unsigned char *sha1_Y){
  FILE *fp;
  int i,j,k;
  if((fp=fopen("./KEYS/Msg.bin","rb"))==NULL){
    printf("File Msg.bin can't open as readable.\n");
    return 1;
  }
  
  for(k=0;k<4*INTS_N;k++){
    Message->_1byte[k]._8bit=fgetc(fp);
  }
  fclose(fp);

  //80bit->matrix
  if((fp=fopen("./KEYS/pkF.bin","rb"))==NULL){
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

  //80bit->matrix
  if((fp=fopen("./KEYS/skS.bin","rb"))==NULL){
    printf("File skS.bin can't open as readable.\n");
    return 1;
  }
  
  for(j=0;j<NUM_N;j++){
    for(k=0;k<4*INTS_N;k++){
      S->Matrix[j]._1byte[k]._8bit=fgetc(fp);
    }
  }
  NNMATRIXtoTRANSPOSE(S);
  fclose(fp);

  //80bit->matrix
  if((fp=fopen("./KEYS/skT.bin","rb"))==NULL){
    printf("File skT.bin can't open as readable.\n");
    return 1;
  }
  
  for(j=0;j<NUM_M;j++){
    for(k=0;k<4*INTS_M;k++){
      T->Matrix[j]._1byte[k]._8bit=fgetc(fp);
    }
  }
  MMMATRIXtoTRANSPOSE(T);  
  fclose(fp);

  //80bit->matrix
  if((fp=fopen("./KEYS/alR.bin","rb"))==NULL){
    printf("File alR.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	R[i].Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
    NNMATRIXtoTRANSPOSE(&(R[i]));
  }
  fclose(fp);

  //80bit->matrix
  if((fp=fopen("./KEYS/alL.bin","rb"))==NULL){
    printf("File alL.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_M;j++){
      for(k=0;k<4*INTS_M;k++){
	L[i].Matrix[j]._1byte[k]._8bit=fgetc(fp);
      }
    }
    MMMATRIXtoTRANSPOSE(&(L[i]));
  }
  fclose(fp);

  if((fp=fopen("./KEYS/alY.bin","rb"))==NULL){
    printf("File alY.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<20;i++){
    sha1_Y[i]=fgetc(fp);
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

  unsigned int tmp;
  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  NNMATRIX *S=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *T=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));

  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX)*NUM_L);
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX)*NUM_L);
  unsigned char *sha1_Y=(unsigned char*)malloc(20*sizeof(unsigned char));

  if(LoadKeys(G,T,F,S,Message,R,L,sha1_Y)==1){
    free(r);
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    free(R);
    free(L);
    free(sha1_Y);
    return 1;
  }

  LLENBITS *b=(LLENBITS*)malloc(sizeof(LLENBITS));
  MATRIXPAIR *Z=(MATRIXPAIR*)malloc(sizeof(MATRIXPAIR)*NUM_L);

  NNMATRIX NNTMP;
  resetNNMATRIX(&(NNTMP));
  MMMATRIX MMTMP;
  resetMMMATRIX(&(MMTMP));
      
  int i,j,k;
  
  SHA_CTX c1;
  unsigned char sha1[20];
  SHA1_Init(&c1);
  SHA1_Update(&c1,Message,4*INTS_N);
  SHA1_Update(&c1,sha1_Y,20);
  SHA1_Final(sha1,&c1);

  for(i=0;i<4*INTS_L;i++){
    b->_1byte[i]._8bit=sha1[i];
  }
  tmp=~0;
  tmp=tmp<<ZEROBITS_L;
  b->_4byte[INTS_L-1]=(b->_4byte[INTS_L-1])&tmp;
  
  //ここでbを出力
  FILE *fp;
  if((fp=fopen("./KEYS/sgb.bin","wb"))==NULL){
    printf("File sgb.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    
    free(b);
    free(R);
    free(L);
    free(sha1_Y);
    free(Z);
    return 1;
  }
  for(i=0;i<4*INTS_L;i++){
    fputc(b->_1byte[i]._8bit,fp);
  }
  fclose(fp);
  
  for(i=0;i<NUM_L;i++){
    if(b->_4byte[0]&(0x80000000)){
      NNMATRIXxNNMATRIX(&(NNTMP),&(R[i]),S);
      MMMATRIXxMMMATRIX(&(MMTMP),T,&(L[i]));
      Z[i].NN=NNTMP;
      Z[i].MM=MMTMP;
    }else{
      Z[i].NN=R[i];//seed
      Z[i].MM=L[i];//seed
    }
    leftshiftLLENBITS(b);
  }
  
  //Zを出力
  //seedとmatrixが混合
  if((fp=fopen("./KEYS/sgZ.bin","wb"))==NULL){
    printf("File sgZ.bin can't open as writable.\n");
    free(r);
    
    free(Message);
    free(S);
    free(T);
    free(F);
    free(G);
    
    free(b);
    free(R);
    free(L);
    free(sha1_Y);
    free(Z);
    return 1;
  }
  for(i=0;i<NUM_L;i++){
    for(j=0;j<NUM_N;j++){
      for(k=0;k<4*INTS_N;k++){
	fputc(Z[i].NN.Matrix[j]._1byte[k]._8bit,fp);
      }
    }
    for(j=0;j<NUM_M;j++){
      for(k=0;k<4*INTS_M;k++){
	fputc(Z[i].MM.Matrix[j]._1byte[k]._8bit,fp);
      }
    }
  }
  
  fclose(fp);
  
  free(r);
  
  free(Message);
  free(S);
  free(T);
  free(F);
  free(G);
  
  free(b);
  free(R);
  free(L);
  free(sha1_Y);
  free(Z);
  return 0;
}
