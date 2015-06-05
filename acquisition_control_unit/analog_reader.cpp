/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#include "analog_reader.h"


AnalogPins::AnalogPins(SetupMessageType* inputMessage, u_int_8 desiredPinCount)
{
  this->setupMessage = inputMessage;
  this->pinCount = desiredPinCount;

  int i;
  for (i = 0; i < pinCount; i++)
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
  for (i = 0; i < pinCount; i++)
  {
    if (setupMessage->analog_pin_operations[i] == digital_in)
    {
      values[i] = digitalRead(setupMessage->analog_pin_assignments[i]);
    }
    else if (setupMessage->analog_pin_operations[i] == analog_in)
    {
      values[i] = analogRead(setupMessage->analog_pin_assignments[i]);
    }
  }
}

void AnalogPins::write()
{
  int i;
  for (i = 0; i < pinCount; i++)
  {
    if (setupMessage->analog_pin_operations[i] == digital_out)
    {
      digitalWrite(setupMessage->analog_pin_assignments[i],outputs[i]);
    }
  }
}

void AnalogPins::readPins(DataMessageType* outputBuffer)
{
  memcpy(outputBuffer->analog_values, values, sizeof(values));
}

void AnalogPins::writePins(DataMessageType* inputBuffer)
{
  memcpy(outputs, inputBuffer->analog_values, sizeof(outputs));
}

