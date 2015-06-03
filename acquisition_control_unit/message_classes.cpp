
#include "message_classes.h"

Setup_Message_Class::Setup_Message_Class(SoftwareSerialBuffer* input)
{

  valid = false;
  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((input->usedSpace() > sizeof(MessageHeaderType)) && (input->usedSpace() >= sizeof(SetupMessageType)))
  {
    MessageHeaderType header;
    memcpy(&header,input->buffer,sizeof(header));
    
    if (header.message_kind == setup_message)
    {
      valid = true;
      this->buffer = input;
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

Data_Message_Class::Data_Message_Class(SoftwareSerialBuffer* input)
{
  valid = false;

  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((input->usedSpace() > sizeof(MessageHeaderType)) && (input->usedSpace() >= sizeof(DataMessageType)))
  {
    MessageHeaderType header;
    memcpy(&header,input->buffer,sizeof(header));
    
    if (header.message_kind == data_message)
    {
      valid = true;
      this->buffer = input;
    }
  }
}

void Data_Message_Class::parseMessage(DataMessageType* messageBuffer)
{
  memcpy(messageBuffer,buffer->buffer,sizeof(DataMessageType));
}

boolean Data_Message_Class::isValid()
{
  return valid;
}

/******************************************************************************/

Start_Message_Class::Start_Message_Class(SoftwareSerialBuffer* input)
{
  valid = false;

  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((input->usedSpace() >= sizeof(MessageHeaderType)) && (input->usedSpace() >= sizeof(StateMessageType)))
  {
    
    MessageHeaderType header;
    memcpy(&header,input->buffer,sizeof(MessageHeaderType));
    
    if (header.message_kind == start_message)
    {
      valid = true;
      this->buffer = input;
    }
  }
  
}

boolean Start_Message_Class::isValid()
{
  return valid;
}


/******************************************************************************/

Stop_Message_Class::Stop_Message_Class(SoftwareSerialBuffer* input)
{
  valid = false;

  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((input->usedSpace() >= sizeof(MessageHeaderType)) && (input->usedSpace() >= sizeof(StateMessageType)))
  {
    MessageHeaderType header;
    memcpy(&header,input->buffer,sizeof(header));

    if (header.message_kind == stop_message)
    {
      valid = true;
      this->buffer = input;
    }
  }
}

boolean Stop_Message_Class::isValid()
{
  return valid;
}


/******************************************************************************/

Reset_Message_Class::Reset_Message_Class(SoftwareSerialBuffer* input)
{
  valid = false;

  /* Construct the header from the header bytes. Also, check to make sure 
   * the buffer contains enough information to actually be a setup message.
   * Then check to see if the header has the correct message kind in it.
   */
  if ((input->usedSpace() >= sizeof(MessageHeaderType)) && (input->usedSpace() >= sizeof(StateMessageType)))
  {
    MessageHeaderType header;
    memcpy(&header,input->buffer,sizeof(header));
    
    if (header.message_kind == reset_message)
    {
      valid = true;
      this->buffer = input;
    }
  }
  
}

boolean Reset_Message_Class::isValid()
{
  return valid;
}

