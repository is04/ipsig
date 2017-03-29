//ピボット選択を用いたガウス・ジョルダンの消去法を用いて連立一次方程式を解く関数
//  
unsigned char pivot_gauss_jordan_GF2(unsigned char **Matrix,int n){
  int i,j,k,l;
  int tmp,pivot;

  //対角成分が1で正規化された階段行列を作る(前進消去法)
  for(i=0;i<n;i++){
    //対角成分Matrix[i][i]が逆元をもたないとき
    if(Matrix[i][i]==0){
      //逆元が存在するMatrix[][i]を探索
      for(j=i;j<n;j++){
	//逆元を持つMatrix[j][i]があれば
	if(Matrix[j][i]!=0){
	  //Matrix[i][]とMatrix[j]を入れ替え
 	  for(k=0;k<n;k++){
	    tmp=Matrix[i][k];
	    Matrix[i][k]=Matrix[j][k];
	    Matrix[j][k]=tmp;
	  }
	  break;
	}
      }
    }

    //対角成分の選択、この値で行成分を正規化
    pivot=Matrix[i][i];
    //pivotが逆元を持たないとき返り値0を返し終了
    if(pivot==0) return 0;
    
    //右側に逆行列を作る為、現在の行より上の行についてi列目の成分が0になるように基本変形
    k=0;
    while(k<i){
      //現在より上の行の一つであるk行目のi列目の値を記録
      if(Matrix[k][i]){
	for(l=i;l<n;l++){
	  //k行目から(現在の行 x k行目のi列目の値)を引き、有限体に収める
	  //Matrix[k][l]=(Matrix[k][l]-Matrix[i][l]);
	  if(Matrix[k][l]!=Matrix[i][l]) Matrix[k][l]=1;
	  else Matrix[k][l]=0;
	  //有限体に収まり、かつ正であるような値にする
	  //if(Matrix[k][l]<0) Matrix[k][l]+=2;
	}
      }
      k++;
    }
    //右側に逆行列を作る為、現在の行より下の行についてi列目の成分が0になるように基本変形
    for(k=i+1;k<n;k++){
      //現在より下の行の一つであるk行目のi列目の値を記録
      if(Matrix[k][i]){
	for(l=i;l<n;l++){
	  //k行目から(現在の行 x k行目のi列目の値)を引き、有限体に収める
	  //Matrix[k][l]=(Matrix[k][l]-Matrix[i][l]);
	  if(Matrix[k][l]!=Matrix[i][l]) Matrix[k][l]=1;
	  else Matrix[k][l]=0;
	  //有限体に収まり、かつ正であるような値にする
	  //if(Matrix[k][l]<0) Matrix[k][l]+=2;
	}
      }
    }
  }

  /*
  printf("inverse\n");
  for(i=0;i<n;i++){
    for(j=0;j<2*n;j++){
      printf("%d ",Matrix[i][j]);
    }
    printf("\n");
  }
  */
  return 1;
}
