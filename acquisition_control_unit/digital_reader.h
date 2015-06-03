#ifndef DIGITAL_PINS_H
#define DIGITAL_PINS_H

#include "rep_types.h"
#include "message_defs.h"
#include "device_pins.h"
#include <Arduino.h>

class DigitalPins : public DevicePins
{
public:
  DigitalPins(SetupMessageType* setupMessage);
  void sample();
  void write();
  void readPins(DataMessageType* outputBuffer);
  void writePins(DataMessageType* inputBuffer);
  
};

#endif