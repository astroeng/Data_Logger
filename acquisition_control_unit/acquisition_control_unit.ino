/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#include <software_serial_buffer.h>
#include <primitive_scheduler.h>

#include "message_defs.h"
#include "digital_reader.h"
#include "analog_reader.h"
#include "message_classes.h"

/* Uncomment this for debugging code to be enabled. With debugging
 * enabled pin 13 will have undesired output.
 */
//#define DEBUG

#ifndef DEBUG
  #define DEBUG_LINE(x)
#else
  #define DEBUG_LINE(x) x
#endif


/******************************************************
 */

/* Setup some serial parameters
 */

#define RECEIVE_BUFFER_SIZE 256
#define SERIAL_RATE 9600
#define SERIAL_WORD 10
#define SERIAL_BYTE_USECONDS (1000000/SERIAL_RATE)*SERIAL_WORD

/* Define the states for the device
 */

enum {Initialize,
      Setup,
      Active,
      Waiting,
      Reset
} typedef DeviceStateType;

PrimitiveScheduler activeSchedule(4);

/* Global space elements are used here to save stack and provide 
 * faster execution of the program.
 */

/* Global Buffers
 */
SoftwareSerialBuffer receiveBuffer(RECEIVE_BUFFER_SIZE);
SetupMessageType pinSetupBuffer;
DataMessageType pinStateBuffer;
DataMessageType outputBuffer;
StateMessageType stateBuffer;

/* Global Variables
 */
DeviceStateType deviceState;
MessageKindType lastMessageType;

DigitalPins* digitalPins;
AnalogPins* analogPins;

u_int_8 messageCountSend;

/* This function is only called once at startup by the Arduino
 * framework. Any single case initialization should be done here.
 */
void setup()
{
  /* Start the serial port 
   */
  Serial.begin(SERIAL_RATE);
  
  /* Initialize all of the global buffers. 
   */
  memset(receiveBuffer.buffer,0,RECEIVE_BUFFER_SIZE);
  memset(&pinSetupBuffer,0,sizeof(SetupMessageType));
  memset(&pinStateBuffer,0,sizeof(DataMessageType));
  memset(&outputBuffer,0,sizeof(DataMessageType));
  memset(&stateBuffer,0,sizeof(StateMessageType));
  
  deviceState = Initialize;
  lastMessageType = no_message;
  messageCountSend = 0;
  
  /* On the arduino PIN 13 is tied to a LED that can
   * be used to help debug execution.
   */ 
  DEBUG_LINE(pinMode(13,OUTPUT));
  DEBUG_LINE(activeSchedule.addTask(send_stats,10000));

  /* Add the tasks that will be used during the active state.
   * TODO: Add the time information from the setup message.
   *       Instead of it being hard coded.
   */
  activeSchedule.addTask(sample_pins,100);
  activeSchedule.addTask(send_message,1000);

  /* This is here to prevent a runaway start if the serial port
   * is being written to immediatly. Sometimes if the serial port
   * is written to immediatly the device will not program correctly.
   */
  delay(2000);
  
  DEBUG_LINE(Serial.println((int)SERIAL_BYTE_USECONDS));
  DEBUG_LINE(Serial.println(sizeof(receiveBuffer.buffer)));
  DEBUG_LINE(Serial.println(sizeof(pinSetupBuffer)));
  DEBUG_LINE(Serial.println(sizeof(pinStateBuffer)));
  DEBUG_LINE(Serial.println(sizeof(outputBuffer)));
  DEBUG_LINE(Serial.println(sizeof(stateBuffer)));
  
}


void loop()
{

  switch(deviceState)
  {
  case Initialize:
    /* In the initialize state the device is waiting for a setup message
     * to be received. Upon receipt of a setup message the device will be
     * configured and sent to the Setup state.
     */
    if (lastMessageType == setup_message)
    {
      DEBUG_LINE(Serial.println("Initialize -> Setup"));
      
      /* This is done on the transition to the setup so that it is
       * only done once.
       */
      digitalPins = new DigitalPins(&pinSetupBuffer, MAX_DIGITAL);
      analogPins = new AnalogPins(&pinSetupBuffer, MAX_ANALOG);

      deviceState = Setup;
    }
    
    break;

  case Setup:
    /* In the setup state the device is waiting for either a reset message
     * or a start message. 
     * - If a reset message is received then the state goes back to Initialize. 
     * - If a start message is received then the state goes to Active.
     */
    if (lastMessageType == start_message)
    {
      DEBUG_LINE(Serial.println("Setup -> Active"));
      
      deviceState = Active;
    }
    else if (lastMessageType == reset_message)
    {
      DEBUG_LINE(Serial.println("Setup -> Reset"));
      
      deviceState = Reset;
    }
    break;

  case Active:
    /* In the active state the device runs the sample and send tasks.
     * - If a stop message is received then the device moves to the waiting state.
     * - If a data message is received then the new data is written to the device outputs.
     * - If a reset message is received then the device moved to the initialize state.
     */
    activeSchedule.run();
    
    DEBUG_LINE(delay(10));
    DEBUG_LINE(Serial.println("Active-Schedule"));
    
    if (lastMessageType == stop_message)
    {
      DEBUG_LINE(Serial.println("Active -> Waiting"));
      
      deviceState = Waiting;
    }
    else if (lastMessageType == data_message)
    {
      DEBUG_LINE(Serial.println("Update Pins"));
      
      write_pins();
    }
    else if (lastMessageType == reset_message)
    {
      DEBUG_LINE(Serial.println("Active -> Reset"));
      
      deviceState = Reset;
    }
    
    lastMessageType = no_message;
    
    break;

  case Waiting:
    /* In the waiting state the device can either be reset or told to 
     * resume the active state.
     */
    if (lastMessageType == start_message)
    {
      DEBUG_LINE(Serial.println("Waiting -> Active"));
      
      deviceState = Active;
    }
    else if (lastMessageType == reset_message)
    {
      DEBUG_LINE(Serial.println("Waiting -> Reset"));
      
      deviceState = Reset;
    }
    
    break;
    
  case Reset:
  
    DEBUG_LINE(Serial.println("Reset -> Init"));
    
    delete digitalPins;
    delete analogPins;
  
    deviceState = Initialize;
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
    
    DEBUG_LINE(digitalWrite(13, HIGH));

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

  DEBUG_LINE(digitalWrite(13, LOW));

  /* Check to see what kind of message was received. 
   */
  if (Setup_Message_Class(&receiveBuffer).isValid())
  {
    Setup_Message_Class(&receiveBuffer).parseMessage(&pinSetupBuffer);
    lastMessageType = setup_message;
    
    DEBUG_LINE(Serial.println("setupMessage"));
    
    receiveBuffer.purge(sizeof(SetupMessageType));
  }
  
  else if (Data_Message_Class(&receiveBuffer).isValid())
  {
    Data_Message_Class(&receiveBuffer).parseMessage(&pinStateBuffer);
    
    digitalPins->writePins(&pinStateBuffer);
    analogPins->writePins(&pinStateBuffer);
    
    lastMessageType = data_message;
    
    DEBUG_LINE(Serial.println("dataMessage"));
    
    receiveBuffer.purge(sizeof(DataMessageType));
  }

  else if (Start_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = start_message;
  
    DEBUG_LINE(Serial.println("startMessage"));
    
    receiveBuffer.purge(sizeof(StateMessageType));
  }  

  else if (Stop_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = stop_message;
  
    DEBUG_LINE(Serial.println("stopMessage"));
    
    receiveBuffer.purge(sizeof(StateMessageType));
  }

  else if (Reset_Message_Class(&receiveBuffer).isValid())
  {
    lastMessageType = reset_message;
    
    DEBUG_LINE(Serial.println("resetMessage"));
    
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

/* This function will read from the actual Arduino pins. Is stores the data
 * that can be retreived with the readPins function.
 */
static void sample_pins( void )
{
  DEBUG_LINE(Serial.print("S"));
  
  digitalPins->sample();
  analogPins->sample();
}

/* This function will write out to the actual Arduino pins. It uses the data
 * that was set with the writePins function.
 */
static void write_pins( void )
{
  DEBUG_LINE(Serial.print("W"));
  
  digitalPins->write();
  analogPins->write();
}

/* This whole task is for debugging only... So don't include it when not debugging.
 */
DEBUG_LINE(
static void send_stats ( void )
{
  int i;
  
  Serial.print("T");
  
  for (i = 0; i < activeSchedule.getTaskCount(); i++)
  {
    Serial.print(activeSchedule.getTaskExecutionTime(i));
    Serial.print(":");
  }
  Serial.println();
}
)

/* This function will send a message back to the host with the pinState information.
 * The rate at which this function is called is controlled from the setup message
 * sent to the Arduino by the host.
 */
static void send_message( void )
{
  DEBUG_LINE(Serial.print("M"));
  
  outputBuffer.header.message_kind  = data_message;
  outputBuffer.header.message_count = messageCountSend++;
  outputBuffer.header.data_pad_aa   = 0;
  outputBuffer.header.message_time  = millis();
  
  digitalPins->readPins(&outputBuffer);
  analogPins->readPins(&outputBuffer);
  
  Serial.write((const u_int_8*)&outputBuffer, sizeof(DataMessageType));
}
