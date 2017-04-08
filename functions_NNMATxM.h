void printNNMATRIXxM(NNMATRIXxM *nmxm){
  int i;
  for(i=0;i<NUM_M;i++){
    printf("No[%d]\n",i);
    printNNMATRIX(&(nmxm->No[i]));
  }
  return;
}

void printNNMATRIXxM_T(NNMATRIXxM *nmxm){
  int i;
  for(i=0;i<NUM_M;i++){
    printf("Transpose No[%d]\n",i);
    printNNMATRIX_T(&(nmxm->No[i]));
  }
  return;
}

void randomNNMATRIXxM(NNMATRIXxM *nmxm,gsl_rng *r){
  int i;
  for(i=0;i<NUM_M;i++){
    randomNNMATRIX(&(nmxm->No[i]),r);
  }
  return;
}

void MMMATRIXoNNMATRIXxM(NNMATRIXxM *ans,MMMATRIX *mm,NNMATRIXxM *nmxm){
  NNMATRIXxM tmp=*nmxm;
  NNMATRIX *nntmp=(NNMATRIX*)malloc(sizeof(NNMATRIX));
  MLENBITS *mbtmp=(MLENBITS*)malloc(sizeof(MLENBITS));

  int i,j;
  for(i=0;i<NUM_M;i++){
    *mbtmp=mm->Transpose[i];
    resetNNMATRIX(nntmp);
    for(j=0;j<NUM_M;j++){
      if((mbtmp->_4byte[0])&(0x80000000)){	
	NNMATRIXpNNMATRIX(nntmp,nntmp,&(tmp.No[j]));
      }
      leftshiftMLENBITS(mbtmp);
    }
    ans->No[i]=*nntmp;
  }
    
  free(mbtmp);
  free(nntmp);
  return;
}

unsigned int chkeqNNMATRIXxM(NNMATRIXxM *nmxma,NNMATRIXxM *nmxmb){
  int i;
  for(i=0;i<NUM_M;i++){
    if(!chkeqNNMATRIX(&(nmxma->No[i]),&(nmxmb->No[i]))) return 0;
  }
  return 1;
}

void quadraticNNMATRIXxMcompression(NNMATRIXxM *nmxm){
  int i;
  for(i=0;i<NUM_M;i++){
    quadraticNNMATRIXcompression(&(nmxm->No[i]));
  }
  return;
}

void NNMATRIXxMoNNMATRIX(NNMATRIXxM *ans,NNMATRIXxM *nmxm,NNMATRIX *nm){
  int i;
  for(i=0;i<NUM_M;i++){
    NNMATRIXxNNMATRIX(&(ans->No[i]),nm,&(nmxm->No[i]));
    NNMATRIXxNNMATRIX_T(&(ans->No[i]),&(ans->No[i]),nm);
  }
  return;
}

void MMMATRIXoNNMATRIXxMoNNMATRIX(NNMATRIXxM *ans,MMMATRIX *mm,NNMATRIXxM *nmxm,NNMATRIX *nm){
  NNMATRIXxMoNNMATRIX(ans,nmxm,nm);
  MMMATRIXoNNMATRIXxM(ans,mm,ans);
  quadraticNNMATRIXxMcompression(ans);
  return;
}

void randomF(NNMATRIXxM *F,gsl_rng *r){
  int i,j,k;
  NLENBITS *tmp=(NLENBITS*)malloc(sizeof(NLENBITS));
  for(i=0;i<NUM_M;i++){
    randomNNMATRIX(&(F->No[i]),r);
  
    //tmp 最上位ビット0
    for(j=0;j<INTS_N;j++){
      tmp->_4byte[j]=(~0);
    }
    tmp->_4byte[INTS_N-1]=tmp->_4byte[INTS_N-1]<<ZEROBITS_N;
    tmp->_1byte[3].bit8=0;

    //Matrix上三角切り出し(対角成分0)
    for(k=0;k<NUM_N;k++){
      for(j=0;j<INTS_N;j++){
	F->No[i].Matrix[k]._4byte[j]=F->No[i].Matrix[k]._4byte[j]&tmp->_4byte[j];
      }
      rightshiftNLENBITS(tmp);
    }
    NNMATRIXtoTRANSPOSE(&(F->No[i]));
  }

  free(tmp);
  return;
}

void NNMATRIXxMtoTRANSPOSE(NNMATRIXxM *nmxm){
  int i;
  for(i=0;i<NUM_M;i++){
    NNMATRIXtoTRANSPOSE(&(nmxm->No[i]));
  }
  return;
}

//testing
void HASHtoNNMATRIXxM(NNMATRIXxM *nmxm,unsigned char *seed,unsigned char *sha1,unsigned char *num_n,unsigned char *num_m,SHA_CTX *c){
  int *i=(int*)malloc(sizeof(int));
  unsigned int *tmp=(unsigned int*)malloc(sizeof(unsigned int));

  for((*i)=0;(*i)<SEED_LEN;(*i)++){
    sha1[*i]=sha1[*i]^seed[*i];
  }

  if((*num_m)<NUM_M){
    if((*num_n)<NUM_N){
      SHA1_Init(c);
      SHA1_Update(c,sha1,20);
      SHA1_Final(sha1,c);
      for((*i)=0;(*i)<4*INTS_N;(*i)++){
	nmxm->No[*num_m].Matrix[*num_n]._1byte[*i]._8bit=sha1[*i];
      }
      *tmp=~0;
      *tmp=(*tmp)<<ZEROBITS_N;
      nmxm->No[*num_m].Matrix[*num_n]._4byte[INTS_N-1]=(nmxm->No[*num_m].Matrix[*num_n]._4byte[INTS_N-1])&(*tmp);
      (*num_n)++;
      free(tmp);
      free(i);
      HASHtoNNMATRIXxM(nmxm,seed,sha1,num_n,num_m,c);
    }else{
      (*num_m)++;
      (*num_n)=0;
      HASHtoNNMATRIXxM(nmxm,seed,sha1,num_n,num_m,c);
    }
  }
  return;
}

void GenNNMATRIXxM(NNMATRIXxM *nmxm,unsigned char *seed){
  int *i=(int*)malloc(sizeof(int));

  unsigned char *sha1=(unsigned char*)malloc(sizeof(unsigned char)*20);
  for((*i)=0;(*i)<20;(*i)++){
    sha1[*i]=0;
  }
  free(i);
  
  unsigned char *num_n=(unsigned char*)malloc(sizeof(unsigned char));
  *num_n=0;
  unsigned char *num_m=(unsigned char*)malloc(sizeof(unsigned char));
  *num_m=0;

  SHA_CTX *c=(SHA_CTX*)malloc(sizeof(SHA_CTX));

  HASHtoNNMATRIXxM(nmxm,seed,sha1,num_n,num_m,c);

  i=(int*)malloc(sizeof(int));
  int j,k;
  //cutting
  
  NLENBITS *tmp=(NLENBITS*)malloc(sizeof(NLENBITS));
  for((*i)=0;(*i)<NUM_M;(*i)++){
    //tmp 最上位ビット0
    for(j=0;j<INTS_N;j++){
      tmp->_4byte[j]=(~0);
    }
    tmp->_4byte[INTS_N-1]=tmp->_4byte[INTS_N-1]<<ZEROBITS_N;
    tmp->_1byte[3].bit8=0;

    //Matrix上三角切り出し(対角成分0)
    for(k=0;k<NUM_N;k++){
      for(j=0;j<INTS_N;j++){
	nmxm->No[*i].Matrix[k]._4byte[j]=nmxm->No[*i].Matrix[k]._4byte[j]&tmp->_4byte[j];
      }
      rightshiftNLENBITS(tmp);
    }
    NNMATRIXtoTRANSPOSE(&(nmxm->No[*i]));
  }
  free(i);
  free(tmp);
  

  NNMATRIXxMtoTRANSPOSE(nmxm);
  free(sha1);
  free(num_n);
  free(num_m);
  free(c);
  return;
}
