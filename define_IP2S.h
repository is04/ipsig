#define NUM_N 96
#define NUM_M 96
#define NUM_L 80
#define SEED_BITLEN 80

#define SEED_LEN (int)SEED_BITLEN/8

#define ZEROBITS_N (32-NUM_N%32)%32
#define ZEROBITS_M (32-NUM_M%32)%32
#define ZEROBITS_L (32-NUM_L%32)%32

#if ZEROBITS_N!=0
  #define INTS_N (NUM_N+ZEROBITS_N)/32
#else
  #define INTS_N NUM_N/32
#endif

#if ZEROBITS_M!=0
  #define INTS_M (NUM_M+ZEROBITS_M)/32
#else
  #define INTS_M NUM_M/32
#endif

#if ZEROBITS_L!=0
  #define INTS_L (NUM_L+ZEROBITS_L)/32
#else
  #define INTS_L NUM_L/32
#endif

//About CHARBITS
//unsigned char 1byte -- 8bits
//   |<----_8bit---->| ._8bit ------------ 0 ~ 255
//   MSB<--------->LSB
//bit 8 7 6 5 4 3 2 1  .bitn(n=1..8) ----- 0,1
//
//ex.)
//  CHARBITS cb;
//  cb._8bit=254;
//  printf("%d%d%d%d%d%d%d%d\n",cb.bit8
//                             ,cb.bit7
//                             ,cb.bit6
//                             ,cb.bit5
//                             ,cb.bit4
//                             ,cb.bit3
//                             ,cb.bit2
//                             ,cb.bit1);
// then
//  11111110
typedef union{
  unsigned char _8bit;
  struct{
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
    unsigned char bit8:1;
  };
}CHARBITS;

//About NLENBITS
//         _4byte[0] _4byte[1] ..               _4byte[INTS_N]
//_1byte[]  3 2 1 0   7 6 5 4  .. INTS_N*2-1 INTS_N*2-2 INTS_N*2-3 INTS_N*2-4
//ex.)
//  #define INTS_N 1
//
//  int i;
//  NLENBITS nb;
//  nb._4byte[0]=0x01f203f4;
//  printf("%08x\n",nb._4byte[0]);
//  for(i=0;i<4;i++){
//    printf("%02x ",nb._1byte[i]._8bit);
//  }
//  printf("\n");
// then
//  01f203f4
//  f4 03 f2 01
typedef union{
  unsigned int _4byte[INTS_N];
  CHARBITS _1byte[4*INTS_N];
}NLENBITS;

typedef union{
  unsigned int _4byte[INTS_M];
  CHARBITS _1byte[4*INTS_M];
}MLENBITS;

typedef union{
  unsigned int _4byte[INTS_L];
  CHARBITS _1byte[4*INTS_L];
}LLENBITS;

typedef struct{
  NLENBITS Matrix[NUM_N];
  NLENBITS Transpose[NUM_N];
}NNMATRIX;

typedef struct{
  MLENBITS Matrix[NUM_M];
  MLENBITS Transpose[NUM_M];
}MMMATRIX;

typedef struct{
  NNMATRIX NN;
  MMMATRIX MM;
}MATRIXPAIR;

typedef struct{
  NNMATRIX No[NUM_M];
}NNMATRIXxM;
