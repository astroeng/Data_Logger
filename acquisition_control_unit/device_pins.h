/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#ifndef DEVICE_PINS_H
#define DEVICE_PINS_H

#include "message_defs.h"

class DevicePins
{
public:
  virtual void readPins(DataMessageType* outputBuffer);
  virtual void writePins(DataMessageType* inputBuffer);
  virtual void sample();
  virtual void write();

protected:
  u_int_16 sampleInterval;
  u_int_16 sampleFilterLength;
  
  u_int_8 pinCount;

  u_int_16* values;
  u_int_16* outputs;

  SetupMessageType* setupMessage;
};



#endif
