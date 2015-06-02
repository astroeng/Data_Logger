#include <iostream>
//#include <fstream>



#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include "../message_defs.h"

using namespace std;

#define BUFFER 64

/*

class SerialPortClass
{
public:
  SerialPortClass(int serial_buffer_size);
  int openPort(const char* device);
  int readPort(char* string);
  int writePort(const char* string, int length);
  int closePort();
  fstream serial_port;
private:
  int buffer_size;

};


SerialPortClass::SerialPortClass(int serial_buffer_size)
{
  buffer_size = serial_buffer_size; 

}

int SerialPortClass::openPort(const char* device)
{
  serial_port.open(device, ios::in | ios::out | ios::binary);

  
  return 0;
}

int SerialPortClass::readPort(char* string)
{
  if (serial_port.is_open())
  {
    serial_port >> string;
    cout << "sync" << serial_port.sync();
    cout << " good" << serial_port.good() << 
            " bad"<< serial_port.bad() <<
            " fail" << serial_port.fail() << 
            " eof" << serial_port.eof() << endl;
  }
  else
  {
    return 0;
  }

}


int SerialPortClass::writePort(const char* string, int length)
{
  if (serial_port.is_open())
  {
    cout << "actual" << string << endl;


    serial_port.write(string, length);
    cout << "flush" << serial_port.flush();
    cout << " good" << serial_port.good() << 
            " bad"<< serial_port.bad() <<
            " fail" << serial_port.fail() << 
            " eof" << serial_port.eof() << endl;
    return 0;
  }
  else
  {
    return 1;
  }
}

int SerialPortClass::closePort()
{
  serial_port.close();

  return 0;
}

*/

class SerialPortClass
{
public:
  SerialPortClass(int serial_buffer_size);
  int openPort(const char* device);
  int readPort(char* string);
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

int SerialPortClass::readPort(char* string)
{
  read(serial_port, string, BUFFER);

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




char Message[][8] = {{2,0,0,0,0,0,0,0},
                     {2,0,0,0,0,0,0,0},
                     {3,0,0,0,0,0,0,0},
                     {2,0,0,0,0,0,0,0},
                     {3,0,0,0,0,0,0,0},
                     {2,0,0,0,0,0,0,0}};

//char Message[][8] = {"aaaaaaa",
//                     "baaaaaa",
//                     "caaaaaa",
//                     "daaaaaa",
//                     "eaaaaaa",
//                     "faaaaaa"};



char inputMessage[BUFFER];

void clearInputMessage()
{
  for (int i = 0; i < BUFFER; i++)
  {
    inputMessage[i] = 0;
  }
}

int main()
{
  clearInputMessage();

  cout << sizeof(long) << ":" << sizeof(StateMessageType) << endl;
  
  cout << "Declare Port" << endl;
  
  SerialPortClass serialPort(BUFFER);
  

  cout << "Open Port" << endl;
  
  serialPort.openPort("/dev/ttyACM0");
  
  for (int i = 0; i < 6; i++)
  {

    sleep(1);
  
    cout << endl <<  "Send Message" << endl;
  
    serialPort.writePort((char*)Message[i],8);

    sleep(1);
  
    cout << "Read Message" << endl;
  
    serialPort.readPort(inputMessage);

    cout << inputMessage << endl;


  }
  cout << "All Done" << endl;
  
  serialPort.closePort();

}
