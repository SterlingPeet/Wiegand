/**
 *  Wiegand.ino
 * 
 *  Created on: Jul 5, 2013
 *      Author: Sterling Lewis Peet <sterling.peet@gatech.edu>
 *
 * The MIT License (MIT)
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Wiegand.h"

const int greenLed = 10;
const int redLed = 11; 

Wiegand prox;

void setup()
{
  prox.begin();
  // Attach the 0-bit Wiegand signal to Arduino's Interrupt 0 (Pin 2 for UNO)
  // Attach the 1-bit Wiegand signal to Arduino's Interrupt 1 (Pin 3 for UNO)
  prox.attach(0,1);

  pinMode(redLed, OUTPUT);  // LED
  pinMode(greenLed, OUTPUT); // Green LED

  digitalWrite(greenLed, HIGH);

  Serial.begin(9600);
  Serial.println("Wiegand Reader Test");
}

void loop()
{
  // Check if there is a new Wiegand message, if so print it out
  //prox.feed(bitCount, dataBits, readTimeout, flagDone);
  if(prox.available()==true)
  {
    Serial.print(" Facility Code = ");
    Serial.print(prox.getFacilityCode(),DEC);
    Serial.print(", Card Code = ");
    Serial.println(prox.getCardCode(),DEC);
    prox.reset();
  }

  delay(900);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  delay(100);
  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed,LOW);
}
