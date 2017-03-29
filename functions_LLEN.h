/*
void resetNLENBITS(NLENBITS *nb){
  int i;
  for(i=0;i<INTS_N;i++){
    nb->_4byte[i]=0;
  }
  return;
}
*/
void printLLENBITS(LLENBITS *lb){
  int i,j;
  for(j=0;j<INTS_L;j++){
    for(i=3+4*j;i>4*j-1;i--){
      printf("%d%d%d%d%d%d%d%d ",
	     lb->_1byte[i].bit8,
	     lb->_1byte[i].bit7,
	     lb->_1byte[i].bit6,
	     lb->_1byte[i].bit5,
	     lb->_1byte[i].bit4,
	     lb->_1byte[i].bit3,
	     lb->_1byte[i].bit2,
	     lb->_1byte[i].bit1
	     );
    }
  }
  printf("\n");
  return;
}

unsigned char chkeqLLENBITS(LLENBITS *lba,LLENBITS *lbb){
  int i;
  for(i=0;i<INTS_L;i++){
    if((lba->_4byte[i])!=(lbb->_4byte[i])) return 0;
  }
  return 1;
}
/*
void randomNLENBITS(NLENBITS *nb,gsl_rng *r){
  int i;
  for(i=0;i<4*INTS_N;i++){
    nb->_1byte[i]._8bit=gsl_rng_uniform_int(r,256);
  }
  nb->_4byte[INTS_N-1]=nb->_4byte[INTS_N-1]<<ZEROBITS_N;
  
  return;
}

void resetNNMATRIX(NNMATRIX *nm){
  int i,j;
  for(j=0;j<NUM_N;j++){
    for(i=0;i<INTS_N;i++){
      nm->Matrix[j]._4byte[i]=0;
      nm->Transpose[j]._4byte[i]=0;
    }
  }
  return;
}

void printNNMATRIX(NNMATRIX *nm){
  int i,j,k;
  for(k=0;k<NUM_N;k++){
    printf("%03d ",k);
    for(j=0;j<INTS_N;j++){
      for(i=3+4*j;i>4*j-1;i--){
	printf("%d%d%d%d%d%d%d%d ",
	       nm->Matrix[k]._1byte[i].bit8,
	       nm->Matrix[k]._1byte[i].bit7,
	       nm->Matrix[k]._1byte[i].bit6,
	       nm->Matrix[k]._1byte[i].bit5,
	       nm->Matrix[k]._1byte[i].bit4,
	       nm->Matrix[k]._1byte[i].bit3,
	       nm->Matrix[k]._1byte[i].bit2,
	       nm->Matrix[k]._1byte[i].bit1
	       );
      }
    }
    printf("\n");
    if(k%8==7) printf("\n");
  }
  return;
}

void printNNMATRIX_T(NNMATRIX *nm){
  int i,j,k;
  for(k=0;k<NUM_N;k++){
    printf("%03d ",k);
    for(j=0;j<INTS_N;j++){
      for(i=3+4*j;i>4*j-1;i--){
	printf("%d%d%d%d%d%d%d%d ",
	       nm->Transpose[k]._1byte[i].bit8,
	       nm->Transpose[k]._1byte[i].bit7,
	       nm->Transpose[k]._1byte[i].bit6,
	       nm->Transpose[k]._1byte[i].bit5,
	       nm->Transpose[k]._1byte[i].bit4,
	       nm->Transpose[k]._1byte[i].bit3,
	       nm->Transpose[k]._1byte[i].bit2,
	       nm->Transpose[k]._1byte[i].bit1
	       );
      }
    }
    printf("\n");
    if(k%8==7) printf("\n");
  }
  return;
}

unsigned int chkeqNNMATRIX(NNMATRIX *nma,NNMATRIX *nmb){
  int i,j;
  for(j=0;j<NUM_N;j++){
    for(i=0;i<INTS_N;i++){
      if(nma->Matrix[j]._4byte[i]!=nmb->Matrix[j]._4byte[i]) return 0;
    }
  }

  for(j=0;j<NUM_N;j++){
    for(i=0;i<INTS_N;i++){
      if(nma->Transpose[j]._4byte[i]!=nmb->Transpose[j]._4byte[i]) return 0;
    }
  }
  return 1;
}

void NNMATRIXtoTRANSPOSE(NNMATRIX *nm){
  unsigned int tmp;
  int i,j,k;
  for(k=0;k<32;k++){
    tmp=1<<(31-k);
    for(i=0;i<32*INTS_N;i++){
      for(j=0;j<INTS_N;j++){
	if(32*j+k<NUM_N){
	  if(i<NUM_N){
	    nm->Transpose[32*j+k]._4byte[i/32]=(nm->Transpose[32*j+k]._4byte[i/32]<<1)+((nm->Matrix[i]._4byte[j]&tmp)>>(31-k));
	  }else{
	    nm->Transpose[32*j+k]._4byte[i/32]=(nm->Transpose[32*j+k]._4byte[i/32]<<1);
	  }
	}
      }
    }
  }
  return;
}

void NNTRANSPOSEtoMATRIX(NNMATRIX *nm){
  unsigned int tmp;
  int i,j,k;
  for(k=0;k<32;k++){
    tmp=1<<(31-k);
    for(i=0;i<32*INTS_N;i++){
      for(j=0;j<INTS_N;j++){
	if(32*j+k<NUM_N){
	  if(i<NUM_N){
	    nm->Matrix[32*j+k]._4byte[i/32]=(nm->Matrix[32*j+k]._4byte[i/32]<<1)+((nm->Transpose[i]._4byte[j]&tmp)>>(31-k));
	  }else{
	    nm->Matrix[32*j+k]._4byte[i/32]=(nm->Matrix[32*j+k]._4byte[i/32]<<1);
	  }
	}
      }
    }
  }
  return;
}

void randomNNMATRIX(NNMATRIX *nm,gsl_rng *r){
  int i,j;
  for(j=0;j<NUM_N;j++){
    for(i=0;i<4*INTS_N;i++){
      nm->Matrix[j]._1byte[i]._8bit=gsl_rng_uniform_int(r,256);
    }
    nm->Matrix[j]._4byte[INTS_N-1]=nm->Matrix[j]._4byte[INTS_N-1]<<ZEROBITS_N;
  }
  NNMATRIXtoTRANSPOSE(nm);
  return;
}

void rightshiftNLENBITS(NLENBITS *nb){
  int i;
  unsigned int tmp=0,tmp2=0;
  for(i=0;i<INTS_N;i++){
    tmp=(nb->_4byte[i])&1;
    nb->_4byte[i]=(nb->_4byte[i])>>1;
    nb->_4byte[i]=(nb->_4byte[i])+(tmp2<<31);
    tmp2=tmp;
  }
  tmp=(~0);
  nb->_4byte[INTS_N-1]=nb->_4byte[INTS_N-1]&(tmp<<ZEROBITS_N);
  return;
}
*/
void leftshiftLLENBITS(LLENBITS *nb){
  int i;
  unsigned int tmp=0,tmp2=0;
  for(i=0;i<INTS_L;i++){
    tmp=(nb->_4byte[INTS_L-1-i])&(0x80000000);
    nb->_4byte[INTS_L-1-i]=((nb->_4byte[INTS_L-1-i])<<1)+(tmp2>>31);
    tmp2=tmp;
  }
  return;
}

/*
//上三角行列化
void quadraticNNMATRIXcompression(NNMATRIX *nm){
  NLENBITS *tmp=(NLENBITS*)malloc(sizeof(NLENBITS));
  int i,j;
  
  //tmp 最上位ビット0
  for(i=0;i<INTS_N;i++){
    tmp->_4byte[i]=(~0);
  }
  tmp->_4byte[INTS_N-1]=tmp->_4byte[INTS_N-1]<<ZEROBITS_N;
  tmp->_1byte[3].bit8=0;

  //nm->Matrix上三角切り出し(対角成分0)
  for(i=0;i<NUM_N;i++){
    for(j=0;j<INTS_N;j++){
      nm->Matrix[i]._4byte[j]=nm->Matrix[i]._4byte[j]&tmp->_4byte[j];
    }
    rightshiftNLENBITS(tmp);
  }

  //tmp 全ビット1
  for(i=0;i<INTS_N;i++){
    tmp->_4byte[i]=(~0);
  }
  tmp->_4byte[INTS_N-1]=tmp->_4byte[INTS_N-1]<<ZEROBITS_N;
  
  //nm->Matrix XOR nm->Transpose & 上三角切り出し
  for(i=0;i<NUM_N;i++){
    for(j=0;j<INTS_N;j++){
      nm->Matrix[i]._4byte[j]=(nm->Matrix[i]._4byte[j]^nm->Transpose[i]._4byte[j])&tmp->_4byte[j];
    }
    rightshiftNLENBITS(tmp);
  }

  NNMATRIXtoTRANSPOSE(nm);
  free(tmp);
  return;
}

unsigned int XORNLENBITS(NLENBITS nb){
  int i;
  CHARBITS res;
  res._8bit=0;
  for(i=0;i<4*INTS_N;i++){
    res._8bit=res._8bit^(nb._1byte[i]._8bit);
  }
  return res.bit1^res.bit2^res.bit3^res.bit4^res.bit5^res.bit6^res.bit7^res.bit8;
}

unsigned int NLENBITSxNLENBITS(NLENBITS nba,NLENBITS nbb){
  NLENBITS nbc;
  int i;
  for(i=0;i<INTS_N;i++){
    nbc._4byte[i]=nba._4byte[i]&nbb._4byte[i];
  }
  return XORNLENBITS(nbc);
}

//nb x nm
void NLENBITSxNNMATRIX(NLENBITS *ans,NLENBITS *nb,NNMATRIX *nm){
  NLENBITS tmp=*nb;
  int i;
  for(i=0;i<NUM_N;i++){
    ans->_4byte[i/32]=(ans->_4byte[i/32]<<1)+NLENBITSxNLENBITS(tmp,nm->Transpose[i]);
  }
  ans->_4byte[INTS_N-1]=ans->_4byte[INTS_N-1]<<ZEROBITS_N;
  return;
}

//nb x nm^T
void NLENBITSxNNMATRIX_T(NLENBITS *ans,NLENBITS *nb,NNMATRIX *nm){
  NLENBITS tmp=*nb;
  int i;
  for(i=0;i<NUM_N;i++){
    ans->_4byte[i/32]=(ans->_4byte[i/32]<<1)+NLENBITSxNLENBITS(tmp,nm->Matrix[i]);
  }
  ans->_4byte[INTS_N-1]=ans->_4byte[INTS_N-1]<<ZEROBITS_N;
  return;
}

//nm matrix x nb
void NNMATRIXxNLENBITS(NLENBITS *ans,NNMATRIX *nm,NLENBITS *nb){
  NLENBITS tmp=*nb;
  int i;
  for(i=0;i<NUM_N;i++){
    ans->_4byte[i/32]=(ans->_4byte[i/32]<<1)+NLENBITSxNLENBITS(tmp,nm->Matrix[i]);
  }
  ans->_4byte[INTS_N-1]=ans->_4byte[INTS_N-1]<<ZEROBITS_N;
  return;
}

//nma x nmb
void NNMATRIXxNNMATRIX(NNMATRIX *ans,NNMATRIX *nma,NNMATRIX *nmb){
  NNMATRIX tmpa=*nma,tmpb=*nmb;
  NLENBITS *tmp=(NLENBITS*)malloc(sizeof(NLENBITS));
  int i;
  for(i=0;i<NUM_N;i++){
    NLENBITSxNNMATRIX(tmp,&(tmpa.Matrix[i]),&tmpb);
    ans->Matrix[i]=*tmp;
  }
  NNMATRIXtoTRANSPOSE(ans);
  free(tmp);
  return;
}

//nma x nmb^T
void NNMATRIXxNNMATRIX_T(NNMATRIX *ans,NNMATRIX *nma,NNMATRIX *nmb){
  NNMATRIX tmpa=*nma,tmpb=*nmb;
  NLENBITS *tmp=(NLENBITS*)malloc(sizeof(NLENBITS));
  int i;
  for(i=0;i<NUM_N;i++){
    NLENBITSxNNMATRIX_T(tmp,&(tmpa.Matrix[i]),&tmpb);
    ans->Matrix[i]=*tmp;
  }
  NNMATRIXtoTRANSPOSE(ans);
  free(tmp);
  return;
}

void NNMATRIXpNNMATRIX(NNMATRIX *ans,NNMATRIX *nma,NNMATRIX *nmb){
  NNMATRIX tmpa=*nma,tmpb=*nmb;
  int i,j;
  for(i=0;i<NUM_N;i++){
    for(j=0;j<INTS_N;j++){
      ans->Matrix[i]._4byte[j]=tmpa.Matrix[i]._4byte[j]^tmpb.Matrix[i]._4byte[j];
    }
  }
  NNMATRIXtoTRANSPOSE(ans);
  return;
}
*/
