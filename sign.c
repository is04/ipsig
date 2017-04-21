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

char LoadKeys(MMMATRIX *T,NNMATRIX *S,unsigned char *seed){
  FILE *fp;
  int i;
  
  if((fp=fopen("./KEYS/skS.bin","rb"))==NULL){
    printf("File skS.bin can't open as readable.\n");
    return 1;
  }
  for(i=0;i<SEED_LEN;i++){
      seed[i]=fgetc(fp);
  }
  GenNNMATRIX(S,seed);
  fclose(fp);

  if((fp=fopen("./KEYS/skT.bin","rb"))==NULL){
    printf("File skT.bin can't open as readable.\n");
    return 1;
  }  
  for(i=0;i<SEED_LEN;i++){
      seed[i]=fgetc(fp);
  }
  GenMMMATRIX(T,seed);
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

char LoadSHA1Y(unsigned char *sha1_Y){
  FILE *fp;
  int i;
  if((fp=fopen("./KEYS/alY.bin","rb"))==NULL){
    printf("File %s can't open as readable.\n","./KEYS/alY.bin");
    return 1;
  }
  
  for(i=0;i<20;i++){
    sha1_Y[i]=fgetc(fp);
  }

  fclose(fp);
  return 0;
}

char LoadalR(NNMATRIX *R,int *num,unsigned char *seed,char *BUFFER){
  FILE *fp;
  sprintf(BUFFER,"./KEYS/alR%05d.bin",*num);
  int i;
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  
  for(i=0;i<SEED_LEN;i++){
    seed[i]=fgetc(fp);
  }
  GenNNMATRIX(R,seed);
  
  fclose(fp);
  return 0;
}

char LoadalL(MMMATRIX *L,int *num,unsigned char *seed,char *BUFFER){
  FILE *fp;
  sprintf(BUFFER,"./KEYS/alL%05d.bin",*num);
  int i;
  if((fp=fopen(BUFFER,"rb"))==NULL){
    printf("File %s can't open as readable.\n",BUFFER);
    return 1;
  }
  
  for(i=0;i<SEED_LEN;i++){
    seed[i]=fgetc(fp);
  }
  GenMMMATRIX(L,seed);

  fclose(fp);
  return 0;
}
int main(){
  //LOAD MSG
  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  if(LoadMsg(Message)){
    free(Message);
    return 1;
  }

  //LOAD MSG,Y AND CALC b
  unsigned char *sha1_Y=(unsigned char*)malloc(20*sizeof(unsigned char));
  if(LoadSHA1Y(sha1_Y)){
    free(Message);
    free(sha1_Y);
    return 1;
  }
  
  SHA_CTX c1;
  unsigned char sha1[20];
  char *BUFFER=(char*)malloc(sizeof(char)*256);

  SHA1_Init(&c1);
  SHA1_Update(&c1,Message,4*INTS_N);
  SHA1_Update(&c1,sha1_Y,20);
  SHA1_Final(sha1,&c1);
  free(sha1_Y);
  free(Message);
  
  int *i=(int*)malloc(sizeof(int));
  LLENBITS *b=(LLENBITS*)malloc(sizeof(LLENBITS));
  for((*i)=0;(*i)<4*INTS_L;(*i)++){
    b->_1byte[*i]._8bit=sha1[*i];
  }
  b->_4byte[INTS_L-1]=(b->_4byte[INTS_L-1])&(0xFFFFFFFF<<ZEROBITS_L);
  
  //ここでbを出力
  FILE *fp;
  if((fp=fopen("./KEYS/sgb.bin","wb"))==NULL){
    printf("File sgb.bin can't open as writable.\n");    
    free(b);
    free(i);
    free(BUFFER);
    return 1;
  }
  for((*i)=0;(*i)<4*INTS_L;(*i)++){
    fputc(b->_1byte[*i]._8bit,fp);
  }
  fclose(fp);
  
  //LOAD S,T
  unsigned char *seed=(unsigned char*)malloc(sizeof(unsigned char)*SEED_LEN);
  NNMATRIX *S=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *T=(MMMATRIX*)malloc(sizeof(MMMATRIX));
  if(LoadKeys(T,S,seed)){
    free(i);
    free(BUFFER);
    free(b);
    free(S);
    free(T);
    free(seed);
    return 1;
  }

  int j,k;
  NNMATRIX *R=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MMMATRIX *L=(MMMATRIX*)malloc(sizeof(MMMATRIX));

  NNMATRIX NNTMP;
  resetNNMATRIX(&(NNTMP));
  MMMATRIX MMTMP;
  resetMMMATRIX(&(MMTMP));
  
  for((*i)=0;(*i)<NUM_L;(*i)++){
    if(b->_4byte[0]&(0x80000000)){
      if(LoadalR(R,i,seed,BUFFER)){
	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      NNMATRIXxNNMATRIX(&(NNTMP),R,S);
      sprintf(BUFFER,"./KEYS/sgZ%05d-0.bin",*i);
      if((fp=fopen(BUFFER,"wb"))==NULL){
	printf("File %s can't open as writable.\n",BUFFER);

	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<NUM_N;j++){
	for(k=0;k<4*INTS_N;k++){
	  fputc(NNTMP.Matrix[j]._1byte[k]._8bit,fp);
	}
      }
      fclose(fp);
      
      if(LoadalL(L,i,seed,BUFFER)){
      	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      MMMATRIXxMMMATRIX(&(MMTMP),T,L);
      sprintf(BUFFER,"./KEYS/sgZ%05d-1.bin",*i);
      if((fp=fopen(BUFFER,"wb"))==NULL){
	printf("File %s can't open as writable.\n",BUFFER);

	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<NUM_M;j++){
	for(k=0;k<4*INTS_M;k++){
	  fputc(MMTMP.Matrix[j]._1byte[k]._8bit,fp);
	}
      }
      fclose(fp);
    }else{
      sprintf(BUFFER,"./KEYS/alR%05d.bin",*i);
      if((fp=fopen(BUFFER,"rb"))==NULL){
	printf("File %s can't open as readable.\n",BUFFER);
	
	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<SEED_LEN;j++){
	seed[j]=fgetc(fp);
      }      
      fclose(fp);

      sprintf(BUFFER,"./KEYS/sgZ%05d-0.bin",*i);
      if((fp=fopen(BUFFER,"wb"))==NULL){
	printf("File %s can't open as writable.\n",BUFFER);

	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<SEED_LEN;j++){
	fputc(seed[j],fp);
      }      
      fclose(fp);

      sprintf(BUFFER,"./KEYS/alL%05d.bin",*i);
      if((fp=fopen(BUFFER,"rb"))==NULL){
	printf("File %s can't open as readable.\n",BUFFER);
	
	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<SEED_LEN;j++){
	seed[j]=fgetc(fp);
      }      
      fclose(fp);

      sprintf(BUFFER,"./KEYS/sgZ%05d-1.bin",*i);
      if((fp=fopen(BUFFER,"wb"))==NULL){
	printf("File %s can't open as writable.\n",BUFFER);

	free(i);
	free(BUFFER);
	free(S);
	free(T);	
	free(b);
	free(R);
	free(L);
	free(seed);
	return 1;
      }
      for(j=0;j<SEED_LEN;j++){
	fputc(seed[j],fp);
      }      
      fclose(fp);
    }
    leftshiftLLENBITS(b);
  }

  free(i);
  free(BUFFER);
  free(seed);
  free(S);
  free(T);  
  free(b);
  free(R);
  free(L);
  return 0;
}
