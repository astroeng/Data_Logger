#ifndef MESSAGE_DEFS_H
#define MESSAGE_DEFS_H

#include <Arduino.h>
#include "rep_types.h"

#define MAX_ANALOG   8
#define MAX_DIGITAL 16

/* Message Kind Type
 */
typedef enum
{
  setup_message = 0,
  data_message  = 1, 
  start_message = 2, /* A type of state message */
  stop_message  = 3, /* A type of state message */
  reset_message = 4, /* A type of state message */
  error_message = 5,
  no_message    = 6
  
} MessageKindType;


/* Error Type
 */
typedef enum
{
  dummy_error
} ErrorType;


/* Pin Operation Type
 */
typedef enum
{
  digital_in,       /* Available on all pins */
  digital_out,      /* Available on all pins */
  analog_in,        /* Only available on pin_Ax pins */
  pulse_in,
  pwm_out           /* Only available on certain pins */
} PinOperationType;


/* Device Pin Type
 */
typedef enum
{
  pin_D0   = 0,
  pin_D1   = 1,
  pin_D2   = 2,
  pin_D3   = 3,
  pin_D4   = 4,
  pin_D5   = 5,
  pin_D6   = 6,
  pin_D7   = 7,
  pin_D8   = 8,
  pin_D9   = 9,
  pin_D10 = 10,
  pin_D11 = 11,
  pin_D12 = 12,
  pin_D13 = 13,
  pin_D14 = 14,
  pin_D15 = 15,
  pin_A0  = A0, //14
  pin_A1  = A1, //15
  pin_A2  = A2, //16
  pin_A3  = A3, //17
  pin_A4  = A4, //18
  pin_A5  = A5, //19
  pin_A6  = A6, //20
  pin_A7  = A7, //21
  unused  = 99
} DevicePinType;


/* Message Header Type
 * This record will be sent with all messages and contains information useful for verifying message sequence and sample rate.
 */
typedef struct
{
  MessageKindType message_kind;   /* message type as defined above. */
  u_int_8         message_count;  /* sequence count for this message type. */
  u_int_8         data_pad_aa;    /* needed to play nice with architectures larger than 8 bit */
  u_int_32        message_time;   /* milliSeconds since the last message. */

} MessageHeaderType; /* This should be 8 bytes */

#define MESSAGEHEADERTYPE_SIZE 8

/* State Message Type
 * This message will just be a header and is intened to update the state of the "aquisition unit"
 */
typedef struct
{
  MessageHeaderType header;
} StateMessageType; /* This should be 8 bytes */

#define STATEMESSAGETYPE_SIZE 8

/* Data Message Type
 * This message can be sent between the "aquisition unit" and the "data server".
 *
 * - The [header] will contain the data as defined by the {MessageHeaderType}.
 *
 * - The [content] of the message will use the {message_content_type} fields as described below.
 *
 * - [analog_values] + Will be used to send back the value of the analog channels defined by the {SetupMessageType}.
 *   The value returned will be a 16 bit value 0 = 0 volts and 0xFFFF = 5 volts.
 *
 * - [digital_values] + Will be used to send back the value of the digital channels defined by the {SetupMessageType}.
 *   This will either be populated with the state of the pin, the length of the pulse input, or the current PWM value.
 *
 *
 * This message can also be sent between the "data server" and the "aquisition unit".
 *
 * - The [header] will contain the data as defined by the {MessageHeaderType} except message time will always be 0 (zero).
 *
 * - [analog_values] + Will be used to send desired state values for these pins. This is only valid when these are used 
 *      as digital outputs.
 *
 * - [digital_values] + Will be used to send back the value of the digital channels defined by the {SetupMessageType}.
 *   This will either be populated with the state of the pin, the length of the pulse input, or the current PWM value.
 */
typedef struct
{

  MessageHeaderType header;
  u_int_16          analog_values[MAX_ANALOG];
  u_int_16          digital_values[MAX_DIGITAL];

} DataMessageType; /* This should be 12 bytes */

#define DATAMESSAGETYPE_SIZE 12

/* Setup Message Type
 * This message will be set between the "data server" and the "aquisition unit". Generally speaking the [message_interval]
 * should be greater than [sample_interval] * [sample_filter_length]. If [message_interval] is less than [sample_interval] *
 * [sample_filter_length] then [message_interval] will be set to [sample_interval] * [sample_filter_length].
 *
 * - The [header] will contain the data as defined by the {MessageHeaderType}.
 *
 * - [pin_assignments] will assign pins to the array positions in the [content] record of the {DataMessageType}.
 * 
 * - [pin_operations] will assign jobs to the pins passed in the [pin_assignments] record.
 *
 * - [message_interval] how often in milliseconds to send the data message.
 *
 * - [sample interval] how often in milliseconds to sample the pins.
 *
 * - [sample_filter_length] the length of the sample filter, the filter is always an average.
 */
typedef struct
{
  
  MessageHeaderType header;
  DevicePinType     analog_pin_assignments[MAX_ANALOG];
  DevicePinType     digital_pin_assignments[MAX_DIGITAL];
  PinOperationType  analog_pin_operations[MAX_ANALOG];
  PinOperationType  digital_pin_operations[MAX_DIGITAL];
  //message_content_type pin_assignments;
  //message_content_type pin_operations;
  u_int_16          message_interval;
  u_int_16          sample_interval;
  u_int_16          sample_filter_length;
  
} SetupMessageType; /* This should be 110 bytes */

#define SETUPMESSAGETYPE_SIZE 110

#endif
