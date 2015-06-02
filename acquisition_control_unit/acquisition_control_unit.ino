#include <software_serial_buffer.h>
#include <primitive_scheduler.h>

#include "message_defs.h"
#include "digital_reader.h"
#include "pulse_reader.h"
#include "analog_reader.h"
#include "message_classes.h"


/******************************************************
*/

SoftwareSerialBuffer receiveBuffer(256);

void setup()
{
  int i;
  Serial.begin(9600);
  for (i = 0; i < 256; i++)
  {
    receiveBuffer.buffer[i] = 0;
  }
  //Serial.println(sizeof(MessageHeaderType));
  //Serial.println(sizeof(SetupMessageType));
  //Serial.println(sizeof(DataMessageType));
  //Serial.println(sizeof(StateMessageType));

}


void loop()
{

}


/******************************************************
*/

void serialEvent()
{
  while (Serial.available() > 0 && receiveBuffer.hasSpace() > 0)
  {
    receiveBuffer.save(Serial.read());
    delayMicroseconds(2000); 
    
  }
  
  if (Setup_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println ("setupMessage : ");
    receiveBuffer.purge(sizeof(SetupMessageType));
  }
  
  else if (Data_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("dataMessage");
    receiveBuffer.purge(sizeof(DataMessageType));
  }

  else if (Start_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("startMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }  

  else if (Stop_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("stopMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }

  else if (Reset_Message_Class(&receiveBuffer).isValid())
  {
    Serial.println("resetMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }
  
  else
  {
    Serial.print("something");
    Serial.println((char*)receiveBuffer.buffer);
    receiveBuffer.purge(sizeof(StateMessageType));
  }
}

