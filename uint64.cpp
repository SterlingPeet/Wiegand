/*
 * uint64.cpp
 *
 *  Created on: Jul 6, 2013
 *      Author: Sterling Lewis Peet <sterling.peet@gatech.edu>
 *
 *  retrieved from: http://forum.arduino.cc/index.php/topic,7144.0.html
 *  Original code by: Dave Brink
 */
 
#include "Arduino.h"
#include "uint64.h"

//uint64::zero64[]= {0,0}; //just for comparisons sake
//realized by Dave Brink

void uint64::init64(unsigned long an[], unsigned long bigPart, unsigned long littlePart ){
  an[0] = bigPart;
  an[1] = littlePart;
}

//left shift 64 bit "number"
void uint64::shl64(unsigned long an[]){
 an[0] <<= 1;
 if(an[1] & 0x80000000)
   an[0]++;
 an[1] <<= 1;
}

//right shift 64 bit "number"
void uint64::shr64(unsigned long an[]){
 an[1] >>= 1;
 if(an[0] & 0x1)
   an[1]+=0x80000000;
 an[0] >>= 1;
}

//add ann to an
void uint64::add64(unsigned long an[], unsigned long ann[]){
  an[0]+=ann[0];
  if(an[1] + ann[1] < ann[1])
    an[0]++;
  an[1]+=ann[1];
}

//subtract ann from an
void uint64::sub64(unsigned long an[], unsigned long ann[]){
  an[0]-=ann[0];
  if(an[1] < ann[1]){
    an[0]--;
  }
  an[1]-= ann[1];
}

//true if an == ann
boolean uint64::eq64(unsigned long an[], unsigned long ann[]){
  return (an[0]==ann[0]) && (an[1]==ann[1]);
}

//true if an < ann
boolean uint64::lt64(unsigned long an[], unsigned long ann[]){
  if(an[0]>ann[0]) return false;
  return (an[0]<ann[0]) || (an[1]<ann[1]);
}

////divide num by den
//void uint64::div64(unsigned long num[], unsigned long den[]){
//  unsigned long quot[2];
//  unsigned long qbit[2];
//  unsigned long tmp[2];
//  init64(quot,0,0);
//  init64(qbit,0,1);
//  uint64::zero64 = {0,0};
//
//  if (eq64(num, zero64)) {  //numerator 0, call it 0
//    init64(num,0,0);
//    return;
//  }
//
//  if (eq64(den, zero64)) { //numerator not zero, denominator 0, infinity in my book.
//    init64(num,0xffffffff,0xffffffff);
//    return;
//  }
//
//  init64(tmp,0x80000000,0);
//  while(lt64(den,tmp)){
//    shl64(den);
//    shl64(qbit);
//  }
//
//  while(!eq64(qbit,zero64)){
//    if(lt64(den,num) || eq64(den,num)){
//      sub64(num,den);
//      add64(quot,qbit);
//    }
//    shr64(den);
//    shr64(qbit);
//  }
//
//  //remainder now in num, but using it to return quotient for now
//  init64(num,quot[0],quot[1]);
//}
//
//
////multiply num by den
//void uint64::mul64(unsigned long an[], unsigned long ann[]){
//  unsigned long p[2] = {0,0};
//  unsigned long y[2] = {ann[0], ann[1]};
//  zero64 = {0,0};
//  while(!eq64(y,zero64)) {
//    if(y[1] & 1)
//      add64(p,an);
//    shl64(an);
//    shr64(y);
//  }
//  init64(an,p[0],p[1]);
//}

//void setup(){
//
//  Serial.begin(9600);
//  unsigned long a1[]={0x2,0xFFFFFFFF};
//  unsigned long a2[]={0x0,0x0FFFFFFF};
//  prt(a1);
//  mul64(a1,a2);
//  prt(a1);
//  div64(a1,a2);
//  prt(a1);
//  init64(a2,a1[0],a1[1]);
//  div64(a1,a2);
//  prt(a1);
//
//}
//void loop(){}
//
//void prt(unsigned long  an[]){
//  Serial.print(an[0],HEX);
//  Serial.print(" ");
// Serial.println(an[1],HEX);
//}


