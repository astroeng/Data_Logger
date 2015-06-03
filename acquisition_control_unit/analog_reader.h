#ifndef ANALOG_PINS_H
#define ANALOG_PINS_H

#include "rep_types.h"
#include "message_defs.h"
#include "device_pins.h"
#include <Arduino.h>
class AnalogPins : public DevicePins
{
public:
  AnalogPins(SetupMessageType* setupMessage);
  void sample();
  void write();
  void readPins(DataMessageType* outputBuffer);
  void writePins(DataMessageType* inputBuffer);
};

#endif