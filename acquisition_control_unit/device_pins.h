#ifndef DEVICE_PINS_H
#define DEVICE_PINS_H

#include "message_defs.h"

class DevicePins
{
public:
  //DevicePins(SetupMessageType* setupMessage);
  virtual void readPins(DataMessageType* outputBuffer);
  virtual void writePins(DataMessageType* inputBuffer);
  virtual void sample();
  virtual void write();

protected:
  u_int_16 sample_interval;
  u_int_16 sample_filter_length;
  
  u_int_16 analogValues[MAX_ANALOG];
  u_int_16 digitalValues[MAX_DIGITAL];
  
  u_int_16 analogOutputs[MAX_ANALOG];
  u_int_16 digitalOutputs[MAX_DIGITAL];
  
  SetupMessageType* setupMessage;
};



#endif