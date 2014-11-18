/*
 * Wiegand.h
 *
 *  Created on: Jul 7, 2013
 *      Author: Sterling Lewis Peet <sterling.peet@gatech.edu>
 *
 *  Justification:
 *    - Need to check card parity bits to reduce sending mistakes
 *    - Need to know why reads fail and when, so the server that interacts
 *        with the unit running this library can log it (and raise alarms
 *        in case the hardware appears to have died)
 *    - Need to properly handle HID corporate 1000 format, only examples that try do
 *        so in the main sketch (but I need it abstracted away, since this
 *        is only a piece of what the unit does)
 */

#ifndef WIEGAND_H_
#define WIEGAND_H_

#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "uint64.h"

const bool WIEGAND_DEBUG = false;       // enable extra debugging output from library
const bool WIEGAND_DEBUG_READER_BITS = false; // enable bit display as bits are read in
const int WIEGAND_WAIT_TIME = 250;    // timeout after we assume reader is done sending bits (milliseconds)
const int WIEGAND_MAX_BITS = 36;      // max number of bits we should read before reset

class Wiegand
{
  public:
    void begin();
    void setOneIntPin(int oneIntPin);
    void setZeroIntPin(int zeroIntPin);
    void attach(int zeroPin, int onePin);
    void attach();
    boolean available();
    void reset();
    unsigned long getCardCode() const;
    unsigned long getFacilityCode() const;
    uint8_t version() { return 0; }   // Library version number

  private:
    int _zeroIntPin;                      // interrupt for zero bits to arrive upon
    int _oneIntPin;                       // interrupt for one bits to arrive upon
    volatile unsigned static int _bitCount; // number of bits captured so far
    volatile static bool _flagDone;       // false indicates data is being read
    volatile unsigned static long _readTimeout; // timeout tells us the data stream is complete
    volatile unsigned static long _dataBits[2]; // raw data received
    unsigned long _facilityCode;          // decoded facility code
    unsigned long _cardCode;              // decoded card code
    bool decode();
    bool process35bits();
    bool process26bits();
    static void ZeroISR();
    static void OneISR();
};

#endif /* WIEGAND_H_ */
