
#include "message_classes.h"



Setup_Message_Class::Setup_Message_Class(SoftwareSerialBuffer* buffer)
{
  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((buffer->usedSpace() > sizeof(MessageHeaderType)) && (buffer->usedSpace() >= sizeof(SetupMessageType)))
  {
    MessageHeaderType header;
    memcpy(&header,buffer,sizeof(header));
    
    valid = false;
    
    if (header.message_kind == start_message)
    {
      valid = true;
      this->buffer = buffer;
    }
  }
}

void Setup_Message_Class::parseMessage(SetupMessageType* messageBuffer)
{
  memcpy(messageBuffer,buffer->buffer,sizeof(SetupMessageType));
}

boolean Setup_Message_Class::isValid()
{
  return valid;
}

/******************************************************************************/

Data_Message_Class::Data_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == start_message) && (buffer->usedSpace() >= sizeof(DataMessageType)))
  {
    valid = true;
  }
  
}

boolean Data_Message_Class::isValid()
{
  return valid;
}

/******************************************************************************/

Start_Message_Class::Start_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == start_message) && (buffer->usedSpace() >= sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Start_Message_Class::isValid()
{
  return valid;
}


/******************************************************************************/

Stop_Message_Class::Stop_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == stop_message) && (buffer->usedSpace() >= sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Stop_Message_Class::isValid()
{
  return valid;
}


/******************************************************************************/

Reset_Message_Class::Reset_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  
  if ((buffer->buffer[0] == reset_message) && (buffer->usedSpace() >= sizeof(StateMessageType)))
  {
    valid = true;
  }
  
}

boolean Reset_Message_Class::isValid()
{
  return valid;
}





