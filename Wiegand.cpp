/*
 * Wiegand.cpp
 *
 *  Created on: Jul 7, 2013
 *      Author: Sterling Lewis Peet <sterling.peet@gatech.edu>
 */

//#include <avr/interrupt.h>
#include "Wiegand.h"

volatile unsigned int Wiegand::_bitCount; // number of bits captured so far
volatile bool Wiegand::_flagDone;       // false indicates data is being read
volatile unsigned long Wiegand::_readTimeout; // timeout tells us the data stream is complet
volatile unsigned long Wiegand::_dataBits[2]; // raw data received

// interrupt that happens when INTO goes low (0 bit)
void Wiegand::ZeroISR() {
  if (WIEGAND_DEBUG_READER_BITS)
    Serial.print("0"); // TODO: use a string buffer
  _bitCount++;
  // This section is uint64::shl64(_dataBits);
  _dataBits[0] <<= 1;
  if(_dataBits[1] & 0x80000000)
    _dataBits[0]++;
  _dataBits[1] <<= 1;
  // TODO: make uint64 a static library so we can use it here
  _flagDone = false;
  _readTimeout = millis() + WIEGAND_WAIT_TIME;
}

// interrupt that happens when INT1 goes low (1 bit)
void Wiegand::OneISR() {
  if (WIEGAND_DEBUG_READER_BITS)
    Serial.print("1"); // TODO: use a string buffer
  _bitCount++;
  // This section is uint64::shl64(_dataBits);
  _dataBits[0] <<= 1;
  if(_dataBits[1] & 0x80000000)
    _dataBits[0]++;
  _dataBits[1] <<= 1;
  // TODO: make uint64 a static library so we can use it here
  _dataBits[1]++;
  _flagDone = false;
  _readTimeout = millis() + WIEGAND_WAIT_TIME;
}

void Wiegand::begin() {
  _zeroIntPin = 0;
  _oneIntPin = 1;
  reset();
}

void Wiegand::setOneIntPin(int oneIntPin) {
  this->_oneIntPin = oneIntPin;
}

void Wiegand::setZeroIntPin(int zeroIntPin) {
  this->_zeroIntPin = zeroIntPin;
}

void Wiegand::attach(int zeroPin, int onePin){
  _zeroIntPin = zeroPin;
  _oneIntPin = onePin;
  attach();
}

void Wiegand::attach() {
  pinMode(2, INPUT);     // (INT0)
  pinMode(3, INPUT);     // (INT1)
  // binds the ISR functions to the falling edge of INTO and INT1
  attachInterrupt(_zeroIntPin, ZeroISR, FALLING);
  attachInterrupt(_oneIntPin, OneISR, FALLING);
  reset();
}

// This method services _flagDone, so we should try to run it at least
// every quarter second or so, preferably as fast as the micro can loop.
boolean Wiegand::available()
{
  if(WIEGAND_DEBUG)
  {
    Serial.println();
    Serial.print("flagDone: ");
    Serial.print(_flagDone);
    Serial.print("  readTimeout remaining: ");
    Serial.println(_readTimeout - millis());
    delay(100); // We do need to keep from crashing the computer serial port here
  }
  // This waits to make sure that there have been no more data pulses before processing data
  if (!_flagDone) {
    if ((_readTimeout < millis()) && decode()) //--wiegand_counter == 0 && decode())
    {
      if(WIEGAND_DEBUG) Serial.println("  Returning available()=true");
      _flagDone = 1;
      return true;
    }
    else{
    } // TODO: This means the read didn't timeout, or the decode didn't work
  }
  else
  {
    // TODO: This actually means the last decoded card hasn't been reset, but probably another card has not been swiped yet
    return false;
  }
  return false;
}

bool Wiegand::decode()
{
  // if we have bits
  if (_bitCount > 0 )
  {
    if(WIEGAND_DEBUG){
      Serial.print(" Read ");
      Serial.print(_bitCount);
      Serial.println(" bits. ");
    }
    // TODO: at least re-comment
    // we will decode the bits differently depending on how many bits we have
    // see www.pagemac.com/azure/data_formats.php for more info
    if (_bitCount == 35)
    {
      if(WIEGAND_DEBUG) Serial.println("Processing 35 bits");
      return process35bits();
    }
    else if (_bitCount == 26)
    {
      return process26bits();
    }
    else {
      // TODO: This should emit an error code corresponding to "bad decode"
      // add other formats here-ish
      if(WIEGAND_DEBUG) 
      { //TODO: need to be able to export this information
        Serial.print(" Unable to decode, unknown bit count: ");
        Serial.println(_bitCount);
      }
      if(_bitCount >= WIEGAND_MAX_BITS){
        reset();
      }
      return false;
    }
  }
  return false;
}

bool Wiegand::process35bits()
{
  // 35 bit HID Corporate 1000 format
  // First check parity bits
  // TODO: Still need to run the actual parity check!
  long unsigned int tmp[2] = { 
    0, 0   };
  //uint64::init64(temp, 0, 0);

  // Card Code: bits 15 to 34 inclusive
  tmp[0] = _dataBits[0];
  tmp[1] = _dataBits[1];
  if(WIEGAND_DEBUG)
  {
    Serial.print("process35 bits data: ");
    Serial.print(_dataBits[0], HEX);
    Serial.print(" ");
    Serial.println(_dataBits[1], HEX);
    Serial.print("process35 bits tmp: ");
    Serial.print(tmp[0], HEX);
    Serial.print(" ");
    Serial.println(tmp[1], HEX);
  }
  uint64::shr64(tmp);       // lose last parity bit
  _cardCode = tmp[1] & 0x000FFFFF;
  // Facility Code: bits 2 to 14 inclusive
  uint64::shr64(tmp);       // shift top facility bit into one uint32
  tmp[1] >>= 19;
  _facilityCode = tmp[1] & 0x00000FFF; // Mask out the facility code TODO: May be unnecessary?
  return true;
}

bool Wiegand::process26bits()
{
  unsigned long tmp = _dataBits[1];
  // standard 26 bit format

  // Card Code: bits 10 to 23 inclusive
  tmp >>= 1;       // lose last parity bit
  _cardCode = tmp & 0x00000FFF;

  // Facility Code: bits 2 to 14 inclusive
  tmp >>= 12;
  _facilityCode = tmp & 0x000000FF;
  return true;
}

void Wiegand::reset()
{
  // cleanup and get ready for the next transmission
  _bitCount = 0;
  _facilityCode = 0;
  _cardCode = 0;
  _dataBits[0] = 0;
  _dataBits[1] = 0;
  _flagDone = false;
}

unsigned long Wiegand::getCardCode() const {
  return _cardCode;
}

unsigned long Wiegand::getFacilityCode() const {
  return _facilityCode;
}

