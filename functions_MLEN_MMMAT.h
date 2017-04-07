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

void HASHtoMMMATRIX(MMMATRIX *nm,unsigned char *seed,unsigned char *sha1,unsigned char *num,SHA_CTX *c){
  int *i=(int*)malloc(sizeof(int));
  unsigned int *tmp=(unsigned int*)malloc(sizeof(unsigned int));

  for((*i)=0;(*i)<SEED_LEN;(*i)++){
    sha1[*i]=sha1[*i]^seed[*i];
  }

  if((*num)<NUM_M){
    SHA1_Init(c);
    SHA1_Update(c,sha1,20);
    SHA1_Final(sha1,c);
    for((*i)=0;(*i)<4*INTS_M;(*i)++){
      nm->Matrix[*num]._1byte[*i]._8bit=sha1[*i];
    }
    *tmp=~0;
    *tmp=(*tmp)<<ZEROBITS_M;
    nm->Matrix[*num]._4byte[INTS_M-1]=(nm->Matrix[*num]._4byte[INTS_M-1])&(*tmp);
    (*num)++;
    free(tmp);
    free(i);
    HASHtoMMMATRIX(nm,seed,sha1,num,c);
  }
  return;
}

unsigned char detMMMATRIX(MMMATRIX *nm){
  unsigned char **MATRIX=(unsigned char**)malloc(sizeof(unsigned char*)*NUM_M);
  int i,j,k,flag;
  unsigned char res;
  for(i=0;i<NUM_M;i++){
    MATRIX[i]=(unsigned char*)malloc(sizeof(unsigned char)*NUM_M);
  }

  for(i=0;i<NUM_M;i++){
    flag=0;
    for(j=0;j<INTS_M;j++){
      for(k=0;k<4;k++){
	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit8;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit7;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit6;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit5;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit4;
	  flag++;
	}else{
	  break;
	}

	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit3;
	  flag++;
	}else{
	  break;
	}
	
	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit2;
	  flag++;
	}else{
	  break;
	}
	
	if(flag<NUM_M){
	  MATRIX[i][flag]=nm->Matrix[i]._1byte[4*j+3-k].bit1;
	  flag++;
	}else{
	  break;
	}
      }
    }
  }

  res=pivot_gauss_jordan_GF2(MATRIX,NUM_M);
  /*
  printf("det=%d\n",res);
  for(i=0;i<NUM_M;i++){
    for(j=0;j<NUM_M;j++){
      printf("%d",MATRIX[i][j]);
      if(j%8==7) printf(" ");
    }
    if(i%8==7) printf("\n");
    printf("\n");
  }
  */
  for(i=0;i<NUM_M;i++){
    free(MATRIX[i]);
  }
  free(MATRIX);
  return res;
}

unsigned char GenMMMATRIX(MMMATRIX *nm,unsigned char *seed){
  int *i=(int*)malloc(sizeof(int));

  unsigned char *sha1=(unsigned char*)malloc(sizeof(unsigned char)*20);
  for((*i)=0;(*i)<20;(*i)++){
    sha1[*i]=0;
  }
  free(i);
  
  unsigned char *num=(unsigned char*)malloc(sizeof(unsigned char));
  *num=0;

  SHA_CTX *c=(SHA_CTX*)malloc(sizeof(SHA_CTX));

  HASHtoMMMATRIX(nm,seed,sha1,num,c);
  MMMATRIXtoTRANSPOSE(nm);
  free(sha1);
  free(num);
  free(c);
  return detMMMATRIX(nm);
}
