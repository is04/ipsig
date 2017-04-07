void resetNLENBITS(NLENBITS *nb){
  int i;
  for(i=0;i<INTS_N;i++){
    nb->_4byte[i]=0;
  }
  return;
}

void printNLENBITS(NLENBITS *nb){
  int i,j;
  for(j=0;j<INTS_N;j++){
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
    for(i=0;i<32*INTS_N/*N*/;i++){
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
    /*
    for(i=N;i<NUM_N+ZEROBITS_N;i++){
      for(j=0;j<INTS_N;j++){
	if(32*j+k<NUM_N){
	  nm->Transpose[32*j+k]._4byte[i/32]=(nm->Transpose[32*j+k]._4byte[i/32]<<1);
	}
      }
    }*/
  }
  return;
}

void NNTRANSPOSEtoMATRIX(NNMATRIX *nm){
  unsigned int tmp;
  int i,j,k;
  for(k=0;k<32;k++){
    tmp=1<<(31-k);
    for(i=0;i<32*INTS_N/*N*/;i++){
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

void leftshiftNLENBITS(NLENBITS *nb){
  int i;
  unsigned int tmp=0,tmp2=0;
  for(i=0;i<INTS_N;i++){
    tmp=(nb->_4byte[INTS_N-1-i])&(0x80000000);
    nb->_4byte[INTS_N-1-i]=((nb->_4byte[INTS_N-1-i])<<1)+(tmp2>>31);
    tmp2=tmp;
  }
  return;
}

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

/*
unsigned int XORvectorbits(NLENBITS nb){
  int i,j;
  unsigned int res=0;
  for(i=0;i<INTS_N;i++){
    for(j=0;j<32;j++){
      res=res^((nb._4byte[i]>>j)&1);
    }
  }
  return res;
}
*/

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

void HASHtoNNMATRIX(NNMATRIX *nm,unsigned char *seed,unsigned char *sha1,unsigned char *num,SHA_CTX *c){
  int *i=(int*)malloc(sizeof(int));
  unsigned int *tmp=(unsigned int*)malloc(sizeof(unsigned int));

  for((*i)=0;(*i)<SEED_LEN;(*i)++){
    sha1[*i]=sha1[*i]^seed[*i];
  }

  if((*num)<NUM_N){
    SHA1_Init(c);
    SHA1_Update(c,sha1,20);
    SHA1_Final(sha1,c);
    for((*i)=0;(*i)<4*INTS_N;(*i)++){
      nm->Matrix[*num]._1byte[*i]._8bit=sha1[*i];
    }
    *tmp=~0;
    *tmp=(*tmp)<<ZEROBITS_N;
    nm->Matrix[*num]._4byte[INTS_N-1]=(nm->Matrix[*num]._4byte[INTS_N-1])&(*tmp);
    (*num)++;
    free(tmp);
    free(i);
    HASHtoNNMATRIX(nm,seed,sha1,num,c);
  }
  return;
}

unsigned char detNNMATRIX(NNMATRIX *nm){
  unsigned char **MATRIX=(unsigned char**)malloc(sizeof(unsigned char*)*NUM_N);
  int i,j,k,flag;
  unsigned char res;
  for(i=0;i<NUM_N;i++){
    MATRIX[i]=(unsigned char*)malloc(sizeof(unsigned char)*NUM_N);
  }

  for(i=0;i<NUM_N;i++){
    flag=0;
    for(j=0;j<INTS_N;j++){
      for(k=0;k<4;k++){
	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit8;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit7;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit6;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit5;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit4;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit3;
	  flag++;
	}else{
	  break;
	}
	
	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit2;
	  flag++;
	}else{
	  break;
	}
	
	if(flag<NUM_N){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit1;
	  flag++;
	}else{
	  break;
	}
      }
    }
  }

  res=pivot_gauss_jordan_GF2(MATRIX,NUM_N);
  /*
  printf("det=%d\n",res);
  for(i=0;i<NUM_N;i++){
    for(j=0;j<NUM_N;j++){
      printf("%d",MATRIX[i][j]);
      if(j%8==7) printf(" ");
    }
    if(i%8==7) printf("\n");
    printf("\n");
  }
  */
  for(i=0;i<NUM_N;i++){
    free(MATRIX[i]);
  }
  free(MATRIX);
  return res;
}

unsigned char GenNNMATRIX(NNMATRIX *nm,unsigned char *seed){
  int *i=(int*)malloc(sizeof(int));

  unsigned char *sha1=(unsigned char*)malloc(sizeof(unsigned char)*20);
  for((*i)=0;(*i)<20;(*i)++){
    sha1[*i]=0;
  }
  free(i);
  
  unsigned char *num=(unsigned char*)malloc(sizeof(unsigned char));
  *num=0;

  SHA_CTX *c=(SHA_CTX*)malloc(sizeof(SHA_CTX));

  HASHtoNNMATRIX(nm,seed,sha1,num,c);
  NNMATRIXtoTRANSPOSE(nm);
  free(sha1);
  free(num);
  free(c);
  //return 1;//ここにdet!=0の判定を入れる(関数呼び出し);
  return detNNMATRIX(nm);
}
