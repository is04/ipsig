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

char LoadKeys(NNMATRIXxM *G,NNMATRIXxM *F,LLENBITS *b,unsigned char *seed){
  FILE *fp;
  int i;
  int *j=(int*)malloc(sizeof(int));
  int *k=(int*)malloc(sizeof(int));
  
  if((fp=fopen("./KEYS/pkG.bin","rb"))==NULL){
    printf("File pkG.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<NUM_M;i++){
    for((*j)=0;(*j)<NUM_N;(*j)++){
      for((*k)=0;(*k)<4*INTS_N;(*k)++){
	G->No[i].Matrix[*j]._1byte[*k]._8bit=fgetc(fp);
      }
    }
  }
  free(j);
  free(k);
  NNMATRIXxMtoTRANSPOSE(G);
  fclose(fp);

  if((fp=fopen("./KEYS/pkF.bin","rb"))==NULL){
    printf("File pkF.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<SEED_LEN;i++){
    seed[i]=fgetc(fp);
  }
  GenNNMATRIXxM(F,seed);
  fclose(fp);

  if((fp=fopen("./KEYS/sgb.bin","rb"))==NULL){
    printf("File sgb.bin can't open as readable.\n");
    return 1;
  }
  
  for(i=0;i<4*INTS_L;i++){
    b->_1byte[i]._8bit=fgetc(fp);
  }
  fclose(fp);
  return 0;
}

char LoadMsg(NLENBITS *Message){
  FILE *fp;
  int i;
  if((fp=fopen("./KEYS/Msg.bin","rb"))==NULL){
    printf("File Msg.bin can't open as readable.\n");
    return 1;
  }  
  for(i=0;i<4*INTS_N;i++){
    Message->_1byte[i]._8bit=fgetc(fp);
  }
  fclose(fp);
  return 0;
}

char LoadZMatrix(NNMATRIX *Z0,MMMATRIX *Z1,int *num,char *BUFFER){
  FILE *fp;
  int i,j;
  sprintf(BUFFER,"./KEYS/sgZ%05d-0.bin",*num);
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  for(i=0;i<NUM_N;i++){
    for(j=0;j<4*INTS_N;j++){
      Z0->Matrix[i]._1byte[j]._8bit=fgetc(fp);
    }
  }
  NNMATRIXtoTRANSPOSE(Z0);
  fclose(fp);
  
  sprintf(BUFFER,"./KEYS/sgZ%05d-1.bin",*num);
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  for(i=0;i<NUM_M;i++){
    for(j=0;j<4*INTS_M;j++){
      Z1->Matrix[i]._1byte[j]._8bit=fgetc(fp);
    }
  }
  MMMATRIXtoTRANSPOSE(Z1);
  fclose(fp);

  return 0;
}

char LoadZSeed(NNMATRIX *Z0,MMMATRIX *Z1,unsigned char *seed,int *num,char *BUFFER){
  FILE *fp;
  int i;
  sprintf(BUFFER,"./KEYS/sgZ%05d-0.bin",*num);
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  for(i=0;i<SEED_LEN;i++){
    seed[i]=fgetc(fp);
  }
  GenNNMATRIX(Z0,seed);
  fclose(fp);
  
  sprintf(BUFFER,"./KEYS/sgZ%05d-1.bin",*num);
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  for(i=0;i<SEED_LEN;i++){
    seed[i]=fgetc(fp);
  }
  GenMMMATRIX(Z1,seed);
  fclose(fp);

  return 0;
}

int main(){
  unsigned char *seed=(unsigned char*)malloc(SEED_LEN*sizeof(unsigned char));
  NNMATRIXxM *F=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  NNMATRIXxM *G=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM));
  LLENBITS *b=(LLENBITS*)malloc(sizeof(LLENBITS));

  if(LoadKeys(G,F,b,seed)){
    free(seed);
    free(F);
    free(G);
    free(b);
    return 1;
  }

  int *i=(int*)malloc(sizeof(int));
  NNMATRIXxM *Y=(NNMATRIXxM*)malloc(sizeof(NNMATRIXxM)*NUM_L);
  NNMATRIX *Z0=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *Z1=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  char *BUFFER=(char*)malloc(sizeof(char)*256);
  
  LLENBITS LBTMP=*b;
  for((*i)=0;(*i)<NUM_L;(*i)++){
    if(LBTMP._4byte[0]&(0x80000000)){
      if(LoadZMatrix(Z0,Z1,i,BUFFER)){
	free(seed);
	free(F);
	free(G);
	free(b);
	free(i);
	free(Y);
	free(Z0);
	free(Z1);
	free(BUFFER);
	return 1;
      }
      MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[*i]),Z1,F,Z0);
    }else{
      if(LoadZSeed(Z0,Z1,seed,i,BUFFER)){
	free(seed);
	free(F);
	free(G);
	free(b);
	free(i);
	free(Y);
	free(Z0);
	free(Z1);
	free(BUFFER);
	return 1;
      }
      MMMATRIXoNNMATRIXxMoNNMATRIX(&(Y[*i]),Z1,G,Z0);
    }
    leftshiftLLENBITS(&(LBTMP));
  }

  free(BUFFER);
  free(Z0);
  free(Z1); 
  free(F);
  free(G);
  free(seed);
  
  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  if(LoadMsg(Message)){
    free(Message);
    free(Y);
    free(i);
    free(b);
  }
  
  SHA_CTX c;
  unsigned char sha1[20];
  SHA1_Init(&c);
  SHA1_Update(&c,Message,4*INTS_N);
  SHA1_Update(&c,Y,4*INTS_N*NUM_N*2*NUM_M*NUM_L);
  SHA1_Final(sha1,&c);
  
  for((*i)=0;(*i)<4*INTS_L;(*i)++){
    LBTMP._1byte[*i]._8bit=sha1[*i];
  }
  LBTMP._4byte[INTS_L-1]=(LBTMP._4byte[INTS_L-1])&(0xFFFFFFFF<<ZEROBITS_L);
  free(i);
  
  if(chkeqLLENBITS(b,&(LBTMP))){
    printf("Verify OK!\n");
  }else{
    printf("Verify NG!\n");
  }

  free(Message);
  free(b);
  free(Y);
  return 0;
}
