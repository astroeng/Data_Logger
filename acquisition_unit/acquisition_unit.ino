#include <software_serial_buffer.h>
#include <primitive_scheduler.h>

#include "message_defs.h"
#include "digital_reader.h"
//#include "pulse_reader.h"
#include "analog_reader.h"

/******************************************************
*/

class Reset_Message_Class
{
public: 
  Reset_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
private:
  boolean valid;
  
};

Reset_Message_Class::Reset_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == reset_message) && (buffer->usedSpace() > sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Reset_Message_Class::isValid()
{
  return valid;
}

/******************************************************
*/

class Stop_Message_Class
{
public: 
  Stop_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
private:
  boolean valid;
  
};

Stop_Message_Class::Stop_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == stop_message) && (buffer->usedSpace() > sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Stop_Message_Class::isValid()
{
  return valid;
}

/******************************************************
*/

class Start_Message_Class
{
public: 
  Start_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
private:
  boolean valid;
  
};

Start_Message_Class::Start_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == start_message) && (buffer->usedSpace() > sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Start_Message_Class::isValid()
{
  return valid;
}

/******************************************************
*/

SoftwareSerialBuffer receiveBuffer(256);
PrimitiveScheduler schedule;

void setup()
{
  Serial.begin(9600); 
}


void loop()
{

}

void serialEvent()
{
  while (Serial.available() > 0 && receiveBuffer.hasSpace() > 0)
  {
    receiveBuffer.save(Serial.read());
  }
  
  if (Reset_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("resetMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }
  
  if (Start_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("startMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }
  
  if (Stop_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("stopMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }
  
}


