#include<stdio.h>

unsigned char InvMC_Matrix[] = {0xe,0xb,0xd,0x9,0x9,0xe,0xb,0xd,0xd,0x9,0xe,0xb,0xb,0xd,0x9,0xe};


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

void InvMixColumns(unsigned int *state){
  //inverse mix the columns of the state with a matrix
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
      state[i] += FFmul(b[i*4],InvMC_Matrix[j*4]) ^ 
	FFmul(b[i*4+1],InvMC_Matrix[j*4+1]) ^ 
	FFmul(b[i*4+2],InvMC_Matrix[j*4+2]) ^ 
	FFmul(b[i*4+3],InvMC_Matrix[j*4+3]);
      if(j<3)
	state[i] <<= 8;
    }
  }
}

int main(){
  unsigned int s[]={0x046681e5,0xe0cb199a,0x48f8d37a,0x2806264c};
  InvMixColumns(s);
  int i=0;
  for(i=0;i<4;i++)
    printf("%08x\n",s[i]);
  return 0;
}
