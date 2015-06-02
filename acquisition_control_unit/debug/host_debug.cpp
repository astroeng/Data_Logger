#include <iostream>
#include <fstream>

using namespace std;

#include <unistd.h>
//#include <fcntl.h>
#include "../message_defs.h"


class SerialPortClass
{
public:
  SerialPortClass(int serial_buffer_size);
  int openPort(const char* device);
  int readPort(char* string);
  int writePort(const char* string, int length);
  int closePort();

private:
  bool port_open;
  int buffer_size;
  fstream serial_port;
  //int serial_port_fd;
};


SerialPortClass::SerialPortClass(int serial_buffer_size)
{
  buffer_size = serial_buffer_size; 

}

int SerialPortClass::openPort(const char* device)
{
  serial_port.open(device, ios::in | ios::out);
  
  //if (serial_port_fd > 0)
  if (serial_port.is_open())
  {
    port_open = true;

  }
  
  return 0;
}

int SerialPortClass::readPort(char* string)
{
  if (port_open)
  {
    return serial_port.getline(string, buffer_size);
    //getline(serial_port_fd, string, buffer_size);
    //return 0;
  }
  else
  {
    return 0;
  }

}


int SerialPortClass::writePort(const char* string, int length)
{
  int written = 0;
  if (port_open)
  {
    written = serial_port.write(string, length);
    //write(serial_port_fd, string, length);
    //serial_port.flush();
    
    return written;
  }
  else
  {
    return 0;
  }
}

int SerialPortClass::closePort()
{
  serial_port.close();
  //close(serial_port_fd);
  return 0;
}

char dummyMessage[] = {0,2,0,0,0,0,0,0};

int main()
{
  cout << sizeof(long) << ":" << sizeof(StateMessageType) << endl;
  
  cout << "Declare Port" << endl;
  
  SerialPortClass serialPort(128);
  
  cout << "Open Port" << endl;
  
  serialPort.openPort("/dev/tty.usbmodem1d11");
  //serialPort.openPort("file.txt");
  
  sleep(2);
  
  cout << "Send Message" << endl;
  
  serialPort.writePort((char*)dummyMessage,8);
  
  sleep(1);
  
  cout << "Read Message" << endl;
  
  serialPort.readPort((char*)dummyMessage);
  
  cout << dummyMessage << endl;
  
  cout << "All Done" << endl;
  
  serialPort.closePort();
  
  
  //fstream file;
  //file.open("/dev/tty.usbmodem1d11");
  //cout << "OPENED" << endl;
  //file << dummyMessage <<endl <<flush;
  //file.close();
}