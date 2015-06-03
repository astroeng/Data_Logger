

#include "analog_reader.h"


AnalogPins::AnalogPins(SetupMessageType* setupMessage) //:
//DevicePins(setupMessage)
{
  this->setupMessage = setupMessage;
  int i;
  for (i = 0; i < MAX_ANALOG; i++)
  {
    if (setupMessage->analog_pin_operations[i] == digital_in)
    {
      pinMode(setupMessage->analog_pin_assignments[i], INPUT);
    }
    else if (setupMessage->analog_pin_operations[i] == analog_in)
    {
      pinMode(setupMessage->analog_pin_assignments[i], INPUT);
    }
    else if (setupMessage->analog_pin_operations[i] == digital_out)
    {
      pinMode(setupMessage->analog_pin_assignments[i], OUTPUT);
    }
    
  }
}

void AnalogPins::sample()
{
  int i;
  for (i = 0; i < MAX_ANALOG; i++)
  {
    if (setupMessage->analog_pin_operations[i] == digital_in)
    {
      analogValues[i] = digitalRead(setupMessage->analog_pin_assignments[i]);
    }
    else if (setupMessage->analog_pin_operations[i] == analog_in)
    {
      analogValues[i] = analogRead(setupMessage->analog_pin_assignments[i]);
    }
  }
}

void AnalogPins::write()
{
  int i;
  for (i = 0; i < MAX_ANALOG; i++)
  {
    if (setupMessage->analog_pin_operations[i] == digital_out)
    {
      digitalWrite(setupMessage->analog_pin_assignments[i],analogOutputs[i]);
    }
  }
}

void AnalogPins::readPins(DataMessageType* outputBuffer)
{
  memcpy(outputBuffer->analog_values,analogValues,sizeof(analogValues));
}

void AnalogPins::writePins(DataMessageType* inputBuffer)
{
  memcpy(analogOutputs,inputBuffer->analog_values,sizeof(analogOutputs));
}

