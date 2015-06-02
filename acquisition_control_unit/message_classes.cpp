
#include "message_classes.h"
#include "message_defs.h"


Setup_Message_Class::Setup_Message_Class(SoftwareSerialBuffer* buffer)
{
  valid = false;
  if ((buffer->buffer[0] == start_message) && (buffer->usedSpace() >= sizeof(SetupMessageType)))
  {
    valid = true;
  }
  
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





