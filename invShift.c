#include<stdio.h>

void InvShiftRows(unsigned int *state){
  //shift each row of the state 
  unsigned int temp[4];
  int i=0;
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff0000;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xff00ffff) | temp[(i+3)%4];
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff00;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xffff00ff) | temp[(i+2)%4];
  for(i=0;i<4;i++)
    temp[i] = state[i] & 0xff;
  for(i=0;i<4;i++)
    state[i] = (state[i] & 0xffffff00) | temp[(i+1)%4];
}

int main(){
  unsigned int state[4]={0xd4bf5d30,0xe0b452ae,0xb84111f1,0x1e2798e5};
  InvShiftRows(state);
  int i=0;
  for(i=0;i<4;i++){
    printf("%08x\n",state[i]);
  }
  return 0;
}
