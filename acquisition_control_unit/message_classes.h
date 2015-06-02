
#ifndef MESSAGE_CLASSES_H
#define MESSAGE_CLASSES_H

#include <Arduino.h>
#include <Software_Serial_Buffer.h>



class Message_Class
{
public:
  virtual boolean isValid();
protected:
  boolean valid;
};

/******************************************************
 */

class Setup_Message_Class : public Message_Class
{
public:
  Setup_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
};


/******************************************************
 */

class Data_Message_Class : public Message_Class
{
public:
  Data_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
};


/******************************************************
 */

class Reset_Message_Class : public Message_Class
{
public: 
  Reset_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
};


/******************************************************
 */

class Stop_Message_Class : public Message_Class
{
public: 
  Stop_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
};


/******************************************************
 */

class Start_Message_Class : public Message_Class
{
public: 
  Start_Message_Class(SoftwareSerialBuffer* buffer);
  boolean isValid();
};

#endif
