#ifndef _STRINGSTREAM_H_
#define _STRINGSTREAM_H_

#include <wprogram.h>
#include <HardwareSerial.h>

class StringStream : public Stream {
  protected:
  public:
  char inbuffer[128];
  unsigned char inbufferlength;
  unsigned char inbufferposition;
  char outbuffer[511];
  unsigned short outbufferposition; //position equals length in this case
  StringStream()  {
  };
  void begin() {
    inbuffer[0] = 0;
    inbufferlength = 0;
    inbufferposition = 0;
    outbuffer[0] = 0;
    outbufferposition = 0;
  };
  void write(uint8_t in) {
    outbuffer[outbufferposition++] = in;
    outbuffer[outbufferposition] = 0;
  };
  void write(const char *str) {
    while(*str!=0) {
      outbuffer[outbufferposition++] = *str;
      str++;
    }
    outbuffer[outbufferposition] = 0;
  };
  void write(const uint8_t *buffer, size_t size) {
    size_t counter;
    for(counter = 0; counter <= size; counter++) {
      outbuffer[outbufferposition++] = buffer[counter];
    }
    outbuffer[outbufferposition] = 0;
  };
  
  int available() {
    return inbufferlength - inbufferposition;
  }
  
  int read() {
    return inbuffer[inbufferposition++];
  }

  int peek() {
    return inbuffer[inbufferposition];
  }

  void flush() {
    //does nothing in this context
  }
  
  void print(const String &in) {
    unsigned char pos = 0;
    while(in[pos]!=0) {
      outbuffer[outbufferposition++] = in[pos];
      pos++;
    }
    outbuffer[outbufferposition] = 0;
  };
  void print(const char* in) {
    while(*in!=0) {
      outbuffer[outbufferposition++] = (*in<10 ? *in + '0' : *in);
      in++;
    }
    outbuffer[outbufferposition] = 0;
  };
  void print(char in, int type = PRINT_BYTE) {
    outbuffer[outbufferposition++] = (in<10 ? in + '0' : in);
    outbuffer[outbufferposition] = 0;
  };
  void print(unsigned char in, int type = PRINT_BYTE) {
    outbuffer[outbufferposition++] = (in<10 ? in + '0' : in);
    outbuffer[outbufferposition] = 0;
  };
  void print(int in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d",in);
  };
  void print(unsigned int in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d",in);
  };
  void print(long in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d",in);
  };
  void print(unsigned long in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d",in);
  };
  void print(double in, int type = 2) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%f",in);
  };
  
  void println(const String &s) {
    unsigned char pos = 0;
    while(s[pos]!=0) {
      outbuffer[outbufferposition++] = s[pos];
      pos++;
    }
    outbuffer[outbufferposition++] = '\r';
    outbuffer[outbufferposition++] = '\n';
    outbuffer[outbufferposition] = 0;
  };
  void println(const char* in) {
    while(*in!=0) {
      outbuffer[outbufferposition++] = (*in<10 ? *in + '0' : *in);
      in++;
    }
    outbuffer[outbufferposition++] = '\r';
    outbuffer[outbufferposition++] = '\n';
    outbuffer[outbufferposition] = 0;
  };
  void println(char in, int type = PRINT_BYTE) {
    outbuffer[outbufferposition++] = (in<10 ? in + '0' : in);
    outbufferposition += sprintf(outbuffer+outbufferposition,"\r\n");
  };
  void println(unsigned char in, int type = PRINT_BYTE) {
    outbuffer[outbufferposition++] = (in<10 ? in + '0' : in);
    outbufferposition += sprintf(outbuffer+outbufferposition,"\r\n");
  };
  void println(int in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d\r\n",in);
  };
  void println(unsigned int in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d\r\n",in);
  };
  void println(long in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d\r\n",in);
  };
  void println(unsigned long in, int type = PRINT_DEC) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%d\r\n",in);
  };
  void println(double in, int type = 2) {
    outbufferposition += sprintf(outbuffer+outbufferposition,"%f\r\n",in);
  };
};

#endif
