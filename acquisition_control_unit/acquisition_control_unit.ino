#include <software_serial_buffer.h>
#include <primitive_scheduler.h>

#include "message_defs.h"
#include "digital_reader.h"
#include "analog_reader.h"
#include "message_classes.h"


/******************************************************
*/

#define RECEIVE_BUFFER_SIZE 256
#define SERIAL_RATE 9600
#define SERIAL_WORD 10
#define SERIAL_BYTE_USECONDS (1000000/SERIAL_RATE)*SERIAL_WORD

enum {Initialize,
      Setup,
      Active,
      Waiting,
      Reset
} typedef DeviceStateType;

SoftwareSerialBuffer receiveBuffer(RECEIVE_BUFFER_SIZE);

PrimitiveScheduler schedule(4);

SetupMessageType pinSetupBuffer;
DataMessageType pinStateBuffer;
DataMessageType outputBuffer;
StateMessageType stateBuffer;

DeviceStateType deviceState;
MessageKindType lastMessageType;

DigitalPins* digitalPins;
AnalogPins* analogPins;

void setup()
{
  int i;
  
  Serial.begin(SERIAL_RATE);
  
  memset(receiveBuffer.buffer,0,RECEIVE_BUFFER_SIZE);
  memset(&pinSetupBuffer,0,sizeof(SetupMessageType));
  memset(&pinStateBuffer,0,sizeof(DataMessageType));
  memset(&outputBuffer,0,sizeof(DataMessageType));
  memset(&stateBuffer,0,sizeof(StateMessageType));
  
  deviceState = Initialize;
  lastMessageType = no_message;
  
  pinMode(13,OUTPUT);
  
  schedule.addTask(send_stats,1000);
  schedule.addTask(sample_pins,1000);
  schedule.addTask(send_message,1000);

  
  delay(2000);
  
  //Serial.println((int)SERIAL_BYTE_USECONDS);
  //Serial.println(sizeof(receiveBuffer.buffer));
  //Serial.println(sizeof(pinSetupBuffer));
  //Serial.println(sizeof(pinStateBuffer));
  //Serial.println(sizeof(outputBuffer));
  //Serial.println(sizeof(stateBuffer));

}


void loop()
{

  switch(deviceState)
  {
  case Initialize:
    if (lastMessageType == setup_message)
    {
      Serial.println("Initialize -> Setup");
      digitalPins = new DigitalPins(&pinSetupBuffer);
      analogPins = new AnalogPins(&pinSetupBuffer);
      deviceState = Setup;
    }
    
    break;

  case Setup:
    if (lastMessageType == start_message)
    {
      Serial.println("Setup -> Active");
      deviceState = Active;
    }
    break;

  case Active:
    
    schedule.run();
    delay(1);
    //Serial.println("Active-Schedule");
    
    if (lastMessageType == stop_message)
    {
      Serial.println("Start -> Waiting");
      deviceState = Waiting;
    }
    else if (lastMessageType == data_message)
    {
      Serial.println("Update Pins");
      write_pins();
    }
    break;

  case Waiting:
    if (lastMessageType == start_message)
    {
      Serial.println("Waiting -> Start");
      deviceState = Active;
    }
    
    break;

  default:
    break;
  }

}


/******************************************************
*/

void serialEvent()
{
  /* loop while there are bytes in the serial buffer and read them into
   * the receive buffer as long as it has space.
   */
  while (Serial.available() > 0 && receiveBuffer.hasSpace() > 0)
  {
    
    digitalWrite(13, HIGH);
    receiveBuffer.save(Serial.read());
    
    /* Delay the approximate amount of time that a serial word should take
     * to travel the wire. This will allow contiguous messages to be received
     * as one entity. But the program should only delay if there are a few
     * bytes in the serial buffer.
     */
    if (Serial.available() < 4)
    {
      delayMicroseconds(SERIAL_BYTE_USECONDS + 100);
    }
  }
  digitalWrite(13, LOW);
  /* Check to see what kind of message was received. 
   */
  if (Setup_Message_Class(&receiveBuffer).isValid())
  {
    Setup_Message_Class(&receiveBuffer).parseMessage(&pinSetupBuffer);
    lastMessageType = setup_message;
    
    Serial.println ("setupMessage");
    receiveBuffer.purge(sizeof(SetupMessageType));
  }
  
  else if (Data_Message_Class(&receiveBuffer).isValid())
  {
    //Data_Message_Class(&receiveBuffer).parseMessage(&pinStateBuffer);
    lastMessageType = data_message;
    
    Serial.println("dataMessage");
    receiveBuffer.purge(sizeof(DataMessageType));
  }

  else if (Start_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = start_message;
  
    Serial.println("startMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }  

  else if (Stop_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = stop_message;
  
    Serial.println("stopMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }

  else if (Reset_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = reset_message;
    
    Serial.println("resetMessage");
    receiveBuffer.purge(sizeof(StateMessageType));
  }
  
  else
  {
    int looper;
    lastMessageType = no_message;
    
    /* Since the delay is used during the receipt of a message getting to
     * this part of the code should mean that an invalid message was
     * received and should be discarded.
     */
    Serial.print("ERROR:BadMessage-Header(bytes):");
    for (looper = 0; looper < 8; looper++)
    {
      Serial.print((int)receiveBuffer.buffer[looper]);
      Serial.print(":");
    }
    Serial.println();
    receiveBuffer.purge(sizeof(StateMessageType));
  }
}

/******************************************************************************
 */

static void sample_pins( void )
{
  Serial.print("S");
  digitalPins->sample();
  analogPins->sample();
}

static void write_pins( void )
{
  digitalPins->write();
  analogPins->write();
}

static void send_stats ( void )
{
  int i;
  Serial.print("T");
  for (i = 0; i < schedule.getTaskCount(); i++)
  {
    Serial.print(schedule.getTaskExecutionTime(i));
    Serial.print(":");
  }
  Serial.println();
}

static void send_message( void )
{
  Serial.print("M");
  
  outputBuffer.header.message_kind  = data_message;
  outputBuffer.header.message_count = 123;
  outputBuffer.header.data_pad_aa   = 0;
  outputBuffer.header.message_time  = millis();
  
  digitalPins->readPins(&outputBuffer);
  analogPins->readPins(&outputBuffer);
  
  Serial.write((const u_int_8*)&outputBuffer, sizeof(DataMessageType));
}
