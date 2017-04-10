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

int main(){
  struct timeval tv;
  gettimeofday(&tv,NULL);
  gsl_rng *r;
  gsl_rng_env_setup();
  r=gsl_rng_alloc(gsl_rng_default);
  gsl_rng_set(r,tv.tv_sec+tv.tv_usec);

  NLENBITS *Message=(NLENBITS*)malloc(sizeof(NLENBITS));
  randomNLENBITS(Message,r);

  FILE *fp;
  int i;
  
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

  free(r);  
  free(Message);
  return 0;
}
