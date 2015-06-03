#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

using namespace std;

#define MAX_ANALOG   8
#define MAX_DIGITAL 16
#define BUFFER 256


class SerialPortClass
{
public:
  SerialPortClass(int serial_buffer_size);
  int openPort(const char* device);
  int readPort(unsigned char* string);
  int writePort(const char* string, int length);
  int closePort();
  int serial_port;
private:
  int buffer_size;

};


SerialPortClass::SerialPortClass(int serial_buffer_size)
{
  buffer_size = serial_buffer_size; 

}

int SerialPortClass::openPort(const char* device)
{
  serial_port = open(device, O_RDWR);// | O_NOCTTY | O_NDELAY);

/*  if (serial_port != -1)
  {
    fcntl(serial_port, F_SETFL, 0);

    struct termios port_settings;
  
    cfsetispeed(&port_settings, B9600);
    cfsetospeed(&port_settings, B9600);

    port_settings.c_cflag &= ~PARENB;
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
    port_settings.c_cflag |= CREAD;

    port_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tcsetattr(serial_port, TCSANOW, &port_settings);
  }
*/
  
  return 0;
}

int SerialPortClass::readPort(unsigned char* string)
{
  return read(serial_port, string, BUFFER);

}


int SerialPortClass::writePort(const char* string, int length)
{
  write(serial_port, string, length);
}

int SerialPortClass::closePort()
{
  close(serial_port);

  return 0;
}

typedef unsigned char u_int_8;
typedef unsigned short u_int_16;
typedef unsigned int u_int_32;

typedef struct {
short message_kind;
unsigned char count;
unsigned char pad;
unsigned int time;
} StateMessageType;

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
  pin_A0  = 14,
  pin_A1  = 15,
  pin_A2  = 16,
  pin_A3  = 17,
  pin_A4  = 18,
  pin_A5  = 19,
  pin_A6  = 20,
  pin_A7  = 21,
  unused  = 99
} DevicePinType;


typedef struct
{
  u_int_16 message_kind;   /* message type as defined above. */
  u_int_8         message_count;  /* sequence count for this message type. */
  u_int_8         data_pad_aa;    /* needed to play nice with architectures larger than 8 bit */
  u_int_32        message_time;   /* milliSeconds since the last message. */

} MessageHeaderType; /* This should be 8 bytes */


typedef struct
{
  
  MessageHeaderType header;
  u_int_16          analog_pin_assignments[MAX_ANALOG];
  u_int_16          digital_pin_assignments[MAX_DIGITAL];
  u_int_16          analog_pin_operations[MAX_ANALOG];
  u_int_16          digital_pin_operations[MAX_DIGITAL];
  u_int_16          message_interval;
  u_int_16          sample_interval;
  u_int_16          sample_filter_length;
  u_int_16          padder;
  
} SetupMessageType; /* This should be 110 (112 for 32 bit alignment) bytes */

char messageKinds[] = {0,1,2,3,4,5};


unsigned char inputMessage[BUFFER];



void clearInputMessage()
{
  for (int i = 0; i < BUFFER; i++)
  {
    inputMessage[i] = 0;
  }
}

int main()
{
  StateMessageType stateMessage;

  SetupMessageType setupMessage = {setup_message,0,0,0,
                                   14,15,16,17,18,19,20,21,
                                   0,1,2,3,4,5,6,7,8,9,10,11,12,13,0,0,
                                   analog_in,analog_in,analog_in,analog_in,
                                   analog_in,analog_in,analog_in,analog_in,
                                   digital_in,digital_in,digital_in,digital_in,
                                   digital_in,digital_in,digital_in,digital_in,
                                   digital_in,digital_in,digital_in,digital_in,
                                   digital_in,digital_in,digital_in,digital_in,
                                   1000,125,4,0};

  cout << setupMessage.sample_filter_length << endl;

  cout << sizeof (short) << ":" << sizeof (unsigned int)  << ":" << sizeof(StateMessageType) << ":" << sizeof(SetupMessageType) << endl;

  clearInputMessage();
  
  cout << "Declare Port" << endl;
  
  SerialPortClass serialPort(BUFFER);
  

  cout << "Open Port" << endl;
  
  serialPort.openPort("/dev/ttyACM0");
  
  sleep(4);

  cout << "Setup Message" << endl;

  serialPort.writePort((char*)&setupMessage,110);

  sleep(1);

  cout << "Start Message" << endl;

  stateMessage.message_kind = start_message;
  stateMessage.count = 0;
  stateMessage.pad = 0;
  stateMessage.time = 123;

  serialPort.writePort((char*)&stateMessage,8);

  for (int i = 0; i < 6; i++)
  {
    int looper = 0;
    cout << "Read Message" << endl;
  
    int read = serialPort.readPort(inputMessage);

    cout << inputMessage << endl;

    for (looper = 0; looper < read; looper++)
    {
      cout << (unsigned int)inputMessage[looper] << ":";
    }
    cout << endl;
    clearInputMessage();
    sleep(1);

  }
  cout << "All Done" << endl;
  
  serialPort.closePort();

}
