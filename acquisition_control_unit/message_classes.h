/*************************************************************
 * Derek Schacht
 * 2015/06/04
 *
 * *** Use Statement and License ***
 * Free for non commercial use! Anything else is not authorized without consent.
 *  Contact [dschacht ( - at - ) gmail ( - dot - ) com] for use questions.
 *************************************************************
 */

#ifndef MESSAGE_CLASSES_H
#define MESSAGE_CLASSES_H


#include <Arduino.h>
#include <software_serial_buffer.h>
#include "message_defs.h"



class Message_Class
{
public:
  virtual boolean isValid();
protected:
  SoftwareSerialBuffer* buffer;
  boolean valid;
};

/******************************************************
 */

class Setup_Message_Class : public Message_Class
{
public:
  Setup_Message_Class(SoftwareSerialBuffer* input);
  void parseMessage(SetupMessageType* messageBuffer);
  boolean isValid();
};


/******************************************************
 */

class Data_Message_Class : public Message_Class
{
public:
  Data_Message_Class(SoftwareSerialBuffer* input);
  void parseMessage(DataMessageType* messageBuffer);
  boolean isValid();
};


/******************************************************
 */

class Reset_Message_Class : public Message_Class
{
public: 
  Reset_Message_Class(SoftwareSerialBuffer* input);
  boolean isValid();
};


/******************************************************
 */

class Stop_Message_Class : public Message_Class
{
public: 
  Stop_Message_Class(SoftwareSerialBuffer* input);
  boolean isValid();
};


/******************************************************
 */

class Start_Message_Class : public Message_Class
{
public: 
  Start_Message_Class(SoftwareSerialBuffer* input);
  boolean isValid();
};

#endif
