#include<stdio.h>
#include<string.h>

unsigned char MC_Matrix[] = { 0x02, 0x03, 0x01, 0x01, 0x01, 0x02, 0x03, 0x01,0x01, 0x01, 0x02, 0x03, 0x03, 0x01, 0x01, 0x02 };

unsigned int Rcon[11] = {0x0,0x01000000, 0x02000000, 0x04000000, 0x08000000, 
 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000}; 

unsigned char sBox[] =

  {/*  0    1     2     3    4     5    6     7    8    9     a     b    c    d    e    f */

    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,/*0*/ 

    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,/*1*/

    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,/*2*/

    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,/*3*/

    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,/*4*/

    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,/*5*/

    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,/*6*/ 

    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,/*7*/

    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,/*8*/

    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,/*9*/

    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,/*a*/

    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,/*b*/

    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,/*c*/

    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,/*d*/

    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,/*e*/

    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16 /*f*/

  };

unsigned int key[44];

void Encrypt(unsigned int *target, unsigned int *w);

void AddRoundKey(unsigned int *state, unsigned int *key, int round){
  //the state XOR with the roundkey
  int i=0;
  for(i=0;i<4;i++)
    state[i] = state[i] ^ key[round*4+i];
}

unsigned int SubWord(unsigned int temp){
  unsigned int byte[4];
  byte[0]=temp&0xff;
  byte[1]=(temp&0xff00)>>8;
  byte[2]=(temp&0xff0000)>>16;
  byte[3]=(temp&0xff000000)>>24;
  int i=0;
  for(i=0;i<4;i++)
    byte[i]=sBox[byte[i]];
  return (byte[0]+(byte[1]<<8)+(byte[2]<<16)+(byte[3]<<24));
}
// a S_box substitution 

void SubBytes(unsigned int *state){
  //use the S_box to substitute the state                                      
  int i=0;
  for(i=0;i<4;i++)
    state[i]=SubWord(state[i]);
}

void ShiftRows(unsigned int *state){
  //shift each row of the state                                                \
                                                                                
  unsigned int temp[4];
  int i=0;
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff0000;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xff00ffff) | temp[(i+1)%4];
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff00;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xffff00ff) | temp[(i+2)%4];
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xffffff00) | temp[(i+3)%4];
}

unsigned int FFmul(unsigned int x, unsigned int y);

unsigned int RotWord(unsigned int temp){
  // left shift the words
  return (temp<<8)|(temp>>24);
}

void KeyExpansion(unsigned int* key, unsigned int* w, int round){
  int i=4;
  int temp=0;
  memcpy(key,w,sizeof(unsigned int)*4);
  while(i<4*(round+1)){
    temp=key[i-1];
    if(i%4==0){
      temp=SubWord(RotWord(temp));
      temp=temp ^ Rcon[i/4];
    }
    key[i]=temp ^ key[i-4];
    i++;
  }
}

void MixColumns(unsigned int *state){
  //mix the columns of the state with a matrix                                  
  unsigned int b[16]={0};
  int i=0;
  for(i=0;i<4;i++){
    b[i*4]=(state[i]&0xff000000)>>24;
    b[i*4+1]=(state[i]&0xff0000)>>16;
    b[i*4+2]=(state[i]&0xff00)>>8;
    b[i*4+3]=state[i]&0xff;
  }
  for(i=0;i<4;i++){
    state[i]=0;
    int j=0;
    for(j=0;j<4;j++){
      state[i]+=FFmul(b[i*4],MC_Matrix[j*4]) ^ FFmul(b[i*4+1],MC_Matrix[j*4+1])
        ^ FFmul(b[i*4+2],MC_Matrix[j*4+2]) ^ FFmul(b[i*4+3],MC_Matrix[j*4+3]);
      if(j<3)
        state[i] <<= 8;
    }
  }
}

unsigned int FFmul(unsigned int x, unsigned int y){
  // return the product of two bytes
  // it is needed in MixColumn
  unsigned int ret=0;
  int i=0;
  for(i=0;i<8;i++){
    if(y & 0x1)
      ret ^= x;
    if(x & 0x80){
      x <<= 1;
      x ^= 0x1b;
    }else
      x <<= 1;
    y >>= 1;
  }
  return ret&0xff;
}

int main() {
  //CTR Decryption
  FILE* keyPtr, *cryptPtr;
  if ((keyPtr = fopen("CTRkey", "r")) == NULL) {
    printf("Key file could not be opened!\n");
    return 0;
  }
  if ((cryptPtr = fopen("CTRciphertext", "r")) == NULL) {
    printf("Ciphertext file could not be opened!\n");
    return 0;
  }
  unsigned int w[4];
  unsigned int IV[4];
  int i=0;
  //key read
  for(i=0;i<4;i++)
    fscanf(keyPtr,"%8x",&w[i]);
  
  /* for(i=0;i<4;i++)
    printf("%08x\n",w[i]);
    printf("\n");*/
  
  //IV read
  for(i=0;i<4;i++)
    fscanf(cryptPtr,"%8x",&IV[i]);

  /*  for(i=0;i<4;i++)
    printf("%08x\n",IV[i]);
    printf("\n");*/
  
  //ciphertext read
  int ct=0;
  unsigned int buff=0;
  unsigned int state[4]={0};
  int n=0;
  unsigned int tmp[4]={0};
  for(n=0;n<4;n++)
    tmp[n]=IV[n];
  while(!feof(cryptPtr)){
    fscanf(cryptPtr,"%2x",&buff);
    state[ct/4] <<= 8;
    state[ct/4] += buff;
    ct++;
    if(ct==16){
      ct=0;
      //Decrypt
      Encrypt(IV,w);
      tmp[3]++;
      //Xor
      for(n=0;n<4;n++){
	state[n]=state[n]^IV[n];
	//print
	int j=0;
	for(j=0;j<4;j++)
	  printf("%c",(state[n]>>(3-j)*8)&0xff);
	state[n]=0;
	IV[n]=tmp[n];
      }
    }
  }
  printf("\n");
  return 0;
}

void Encrypt(unsigned int *target, unsigned int *w){
  static int first=1;
  if(first){
    KeyExpansion(key,w,10);
    first=0;
  }
  /*int j=0;                                                                    
  for(j=0;j<44;j++)                                                             
  printf("%08x\n",key[j]);*/
  AddRoundKey(target,key,0);
  int i=1;
  for(i=1;i<10;i++){
    SubBytes(target);
    ShiftRows(target);
    MixColumns(target);
    AddRoundKey(target,key,i);
  }
  SubBytes(target);
  ShiftRows(target);
  AddRoundKey(target,key,i);
}
