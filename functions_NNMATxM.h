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

//ここアウト?
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
