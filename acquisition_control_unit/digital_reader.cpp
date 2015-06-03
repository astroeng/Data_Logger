

#include "digital_reader.h"


DigitalPins::DigitalPins(SetupMessageType* setupMessage) // :
//DevicePins(setupMessage)
{
  this->setupMessage = setupMessage;
  int i;
  for (i = 0; i < MAX_DIGITAL; i++)
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
  for (i = 0; i < MAX_DIGITAL; i++)
  {
    if (setupMessage->digital_pin_operations[i] == digital_in)
    {
      digitalValues[i] = digitalRead(setupMessage->digital_pin_assignments[i]);
    }
    else if (setupMessage->digital_pin_operations[i] == pulse_in)
    {
      digitalValues[i] = pulseIn(setupMessage->digital_pin_assignments[i], HIGH);
    }
  }
}

void DigitalPins::write()
{
  int i;
  for (i = 0; i < MAX_DIGITAL; i++)
  {
    if (setupMessage->digital_pin_operations[i] == digital_out)
    {
      digitalWrite(setupMessage->digital_pin_assignments[i],digitalOutputs[i]);
    }
    else if (setupMessage->digital_pin_operations[i] == pwm_out)
    {
      analogWrite(setupMessage->digital_pin_assignments[i],digitalOutputs[i]);
    }
  }
}

void DigitalPins::readPins(DataMessageType* outputBuffer)
{
  memcpy(outputBuffer->digital_values,digitalValues,sizeof(digitalValues));
}

void DigitalPins::writePins(DataMessageType* inputBuffer)
{
  memcpy(digitalOutputs,inputBuffer->digital_values,sizeof(digitalOutputs));
}

