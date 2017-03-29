void resetMLENBITS(MLENBITS *nb){
  int i;
  for(i=0;i<INTS_M;i++){
    nb->_4byte[i]=0;
  }
  return;
}

void printMLENBITS(MLENBITS *nb){
  int i,j;
  for(j=0;j<INTS_M;j++){
    for(i=3+4*j;i>4*j-1;i--){
      printf("%d%d%d%d%d%d%d%d ",
	     nb->_1byte[i].bit8,
	     nb->_1byte[i].bit7,
	     nb->_1byte[i].bit6,
	     nb->_1byte[i].bit5,
	     nb->_1byte[i].bit4,
	     nb->_1byte[i].bit3,
	     nb->_1byte[i].bit2,
	     nb->_1byte[i].bit1
	     );
    }
  }
  printf("\n");
  return;
}

void randomMLENBITS(MLENBITS *nb,gsl_rng *r){
  int i;
  for(i=0;i<4*INTS_M;i++){
    nb->_1byte[i]._8bit=gsl_rng_uniform_int(r,256);
  }
  nb->_4byte[INTS_M-1]=nb->_4byte[INTS_M-1]<<ZEROBITS_M;
  return;
}

void resetMMMATRIX(MMMATRIX *nm){
  int i,j;
  for(j=0;j<NUM_M;j++){
    for(i=0;i<INTS_M;i++){
      nm->Matrix[j]._4byte[i]=0;
      nm->Transpose[j]._4byte[i]=0;
    }
  }
  return;
}

void printMMMATRIX(MMMATRIX *nm){
  int i,j,k;
  for(k=0;k<NUM_M;k++){
    printf("%03d ",k);
    for(j=0;j<INTS_M;j++){
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

void printMMMATRIX_T(MMMATRIX *nm){
  int i,j,k;
  for(k=0;k<NUM_M;k++){
    printf("%03d ",k);
    for(j=0;j<INTS_M;j++){
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

unsigned int chkeqMMMATRIX(MMMATRIX *nma,MMMATRIX *nmb){
  int i,j;
  for(j=0;j<NUM_M;j++){
    for(i=0;i<INTS_M;i++){
      if(nma->Matrix[j]._4byte[i]!=nmb->Matrix[j]._4byte[i]) return 0;
    }
  }

  for(j=0;j<NUM_M;j++){
    for(i=0;i<INTS_M;i++){
      if(nma->Transpose[j]._4byte[i]!=nmb->Transpose[j]._4byte[i]) return 0;
    }
  }
  return 1;
}

void MMMATRIXtoTRANSPOSE(MMMATRIX *nm){
  unsigned int tmp;
  int i,j,k;
  for(k=0;k<32;k++){
    tmp=1<<(31-k);
    for(i=0;i<32*INTS_M/*M*/;i++){
      for(j=0;j<INTS_M;j++){
	if(32*j+k<NUM_M){
	  if(i<NUM_M){
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

void MMTRANSPOSEtoMATRIX(MMMATRIX *nm){
  unsigned int tmp;
  int i,j,k;
  for(k=0;k<32;k++){
    tmp=1<<(31-k);
    for(i=0;i<32*INTS_M/*M*/;i++){
      for(j=0;j<INTS_M;j++){
	if(32*j+k<NUM_M){
	  if(i<NUM_M){
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

void randomMMMATRIX(MMMATRIX *nm,gsl_rng *r){
  int i,j;
  for(j=0;j<NUM_M;j++){
    for(i=0;i<4*INTS_M;i++){
	nm->Matrix[j]._1byte[i]._8bit=gsl_rng_uniform_int(r,256);
    }
    nm->Matrix[j]._4byte[INTS_M-1]=nm->Matrix[j]._4byte[INTS_M-1]<<ZEROBITS_M;
  }
  MMMATRIXtoTRANSPOSE(nm);
  return;
}

void rightshiftMLENBITS(MLENBITS *nb){
  int i;
  unsigned int tmp=0,tmp2=0;
  for(i=0;i<INTS_M;i++){
    tmp=(nb->_4byte[i])&1;
    nb->_4byte[i]=(nb->_4byte[i])>>1;
    nb->_4byte[i]=(nb->_4byte[i])+(tmp2<<31);
    tmp2=tmp;
  }
  tmp=(~0);
  nb->_4byte[INTS_M-1]=nb->_4byte[INTS_M-1]&(tmp<<ZEROBITS_M);
  return;
}

void leftshiftMLENBITS(MLENBITS *nb){
  int i;
  unsigned int tmp=0,tmp2=0;
  for(i=0;i<INTS_M;i++){
    tmp=(nb->_4byte[INTS_M-1-i])&(0x80000000);
    nb->_4byte[INTS_M-1-i]=((nb->_4byte[INTS_M-1-i])<<1)+(tmp2>>31);
    tmp2=tmp;
  }
  return;
}

//上三角行列化
void quadraticMMMATRIXcompression(MMMATRIX *nm){
  MLENBITS *tmp=(MLENBITS*)malloc(sizeof(MLENBITS));
  int i,j;
  
  //tmp 最上位ビット0
  for(i=0;i<INTS_M;i++){
    tmp->_4byte[i]=(~0);
  }
  tmp->_4byte[INTS_M-1]=tmp->_4byte[INTS_M-1]<<ZEROBITS_M;
  tmp->_1byte[3].bit8=0;

  //nm->Matrix上三角切り出し(対角成分0)
  for(i=0;i<NUM_M;i++){
    for(j=0;j<INTS_M;j++){
      nm->Matrix[i]._4byte[j]=nm->Matrix[i]._4byte[j]&tmp->_4byte[j];
    }
    rightshiftMLENBITS(tmp);
  }

  //tmp 全ビット1
  for(i=0;i<INTS_M;i++){
    tmp->_4byte[i]=(~0);
  }
  tmp->_4byte[INTS_M-1]=tmp->_4byte[INTS_M-1]<<ZEROBITS_M;
  
  //nm->Matrix XOR nm->Transpose & 上三角切り出し
  for(i=0;i<NUM_M;i++){
    for(j=0;j<INTS_M;j++){
      nm->Matrix[i]._4byte[j]=(nm->Matrix[i]._4byte[j]^nm->Transpose[i]._4byte[j])&tmp->_4byte[j];
    }
    rightshiftMLENBITS(tmp);
  }

  MMMATRIXtoTRANSPOSE(nm);
  free(tmp);
  return;
}

/*
unsigned int XORvectorbits(MLENBITS nb){
  int i,j;
  unsigned int res=0;
  for(i=0;i<INTS_M;i++){
    for(j=0;j<32;j++){
      res=res^((nb._4byte[i]>>j)&1);
    }
  }
  return res;
}
*/

unsigned int XORMLENBITS(MLENBITS nb){
  int i;
  CHARBITS res;
  res._8bit=0;
  for(i=0;i<4*INTS_M;i++){
    res._8bit=res._8bit^(nb._1byte[i]._8bit);
  }
  return res.bit1^res.bit2^res.bit3^res.bit4^res.bit5^res.bit6^res.bit7^res.bit8;
}

unsigned int MLENBITSxMLENBITS(MLENBITS nba,MLENBITS nbb){
  MLENBITS nbc;
  int i;
  for(i=0;i<INTS_M;i++){
    nbc._4byte[i]=nba._4byte[i]&nbb._4byte[i];
  }
  return XORMLENBITS(nbc);
}

//nb x nm transpose
//vec x matrixとmatrix x vecを比較し、其の結果如何で
//randommatrix関数においてtranspose関数を呼び出し、Transposeを設定する。
void MLENBITSxMMMATRIX(MLENBITS *ans,MLENBITS *nb,MMMATRIX *nm){
  MLENBITS tmp=*nb;
  int i;
  for(i=0;i<NUM_M;i++){
    ans->_4byte[i/32]=(ans->_4byte[i/32]<<1)+MLENBITSxMLENBITS(tmp,nm->Transpose[i]);
  }
  ans->_4byte[INTS_M-1]=ans->_4byte[INTS_M-1]<<ZEROBITS_M;
  return;
}

//nm matrix x nb
void MMMATRIXxMLENBITS(MLENBITS *ans,MMMATRIX *nm,MLENBITS *nb){
  MLENBITS tmp=*nb;
  int i;
  for(i=0;i<NUM_M;i++){
    ans->_4byte[i/32]=(ans->_4byte[i/32]<<1)+MLENBITSxMLENBITS(tmp,nm->Matrix[i]);
  }
  ans->_4byte[INTS_M-1]=ans->_4byte[INTS_M-1]<<ZEROBITS_M;
  return;
}

//nma x nmb
void MMMATRIXxMMMATRIX(MMMATRIX *ans,MMMATRIX *nma,MMMATRIX *nmb){
  MMMATRIX tmpa=*nma,tmpb=*nmb;
  MLENBITS *tmp=(MLENBITS*)malloc(sizeof(MLENBITS));
  int i;
  for(i=0;i<NUM_M;i++){
    MLENBITSxMMMATRIX(tmp,&(tmpa.Matrix[i]),&tmpb);
    ans->Matrix[i]=*tmp;
  }
  MMMATRIXtoTRANSPOSE(ans);
  free(tmp);
  return;
}

void MMMATRIXpMMMATRIX(MMMATRIX *ans,MMMATRIX *nma,MMMATRIX *nmb){
  MMMATRIX tmpa=*nma,tmpb=*nmb;
  int i,j;
  for(i=0;i<NUM_M;i++){
    for(j=0;j<INTS_M;j++){
      ans->Matrix[i]._4byte[j]=tmpa.Matrix[i]._4byte[j]^tmpb.Matrix[i]._4byte[j];
    }
  }
  MMMATRIXtoTRANSPOSE(ans);
  return;
}
