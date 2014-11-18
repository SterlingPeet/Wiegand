/*
 uint64.h
 *
  Created on: Jul 6, 2013
      Author: Sterling Lewis Peet <sterling.peet@gatech.edu>
 *
  Based on retrieved from: http://forum.arduino.cc/index.php/topic,7144.0.html
  Original code by: Dave Brink
 */

#ifndef UINT64_H_
#define UINT64_H_

#include <Arduino.h>

class uint64 {
//private:
//  static unsigned long zero64[];
public:
  static void init64(unsigned long an[], unsigned long bigPart, unsigned long littlePart );
  static void shl64(unsigned long an[]);
  static void shr64(unsigned long an[]);
  static void add64(unsigned long an[], unsigned long ann[]);
  static void sub64(unsigned long an[], unsigned long ann[]);
  static boolean eq64(unsigned long an[], unsigned long ann[]);
  static boolean lt64(unsigned long an[], unsigned long ann[]);
//  static void div64(unsigned long num[], unsigned long den[]);
//  static void mul64(unsigned long an[], unsigned long ann[]);
};

#endif /*UINT64_H_ */
