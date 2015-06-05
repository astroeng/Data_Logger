/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#ifndef DIGITAL_PINS_H
#define DIGITAL_PINS_H

#include "rep_types.h"
#include "message_defs.h"
#include "device_pins.h"

class DigitalPins : public DevicePins
{
public:
  DigitalPins(SetupMessageType* inputMessage, u_int_8 desiredPinCount);
  void sample();
  void write();
  void readPins(DataMessageType* outputBuffer);
  void writePins(DataMessageType* inputBuffer);

};

#endif
