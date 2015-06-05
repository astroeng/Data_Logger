/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#include "digital_reader.h"


DigitalPins::DigitalPins(SetupMessageType* inputMessage, u_int_8 desiredPinCount)
{
  this->setupMessage = inputMessage;
  this->pinCount = desiredPinCount;
  
  outputs = new u_int_16[pinCount];
  values = new u_int_16[pinCount];
  
  memset(outputs,0,sizeof(outputs));
  memset(values,0,sizeof(values));
  
  int i;
  
  for (i = 0; i < pinCount; i++)
  {
    if (setupMessage->digital_pin_operations[i] == digital_in)
    {
      pinMode(setupMessage->digital_pin_assignments[i], INPUT);
    }
    else if (setupMessage->digital_pin_operations[i] == digital_out)
    {
      pinMode(setupMessage->digital_pin_assignments[i], OUTPUT);
    }
    else if (setupMessage->digital_pin_operations[i] == pwm_out)
    {
      pinMode(setupMessage->digital_pin_assignments[i], OUTPUT);
    }
    else if (setupMessage->digital_pin_operations[i] == pulse_in)
    {
      pinMode(setupMessage->digital_pin_assignments[i], INPUT);
    }
    
  }
}

void DigitalPins::sample()
{
  int i;
  for (i = 0; i < pinCount; i++)
  {
    if (setupMessage->digital_pin_operations[i] == digital_in)
    {
      values[i] = digitalRead(setupMessage->digital_pin_assignments[i]);
    }
    else if (setupMessage->digital_pin_operations[i] == pulse_in)
    {
      values[i] = pulseIn(setupMessage->digital_pin_assignments[i], HIGH);
    }
  }
}

void DigitalPins::write()
{
  int i;
  for (i = 0; i < pinCount; i++)
  {
    if (setupMessage->digital_pin_operations[i] == digital_out)
    {
      digitalWrite(setupMessage->digital_pin_assignments[i],outputs[i]);
    }
    else if (setupMessage->digital_pin_operations[i] == pwm_out)
    {
      analogWrite(setupMessage->digital_pin_assignments[i],outputs[i]);
    }
  }
}

void DigitalPins::readPins(DataMessageType* outputBuffer)
{
  memcpy(outputBuffer->digital_values, values, sizeof(values));
}

void DigitalPins::writePins(DataMessageType* inputBuffer)
{
  memcpy(outputs, inputBuffer->digital_values, sizeof(outputs));
}

