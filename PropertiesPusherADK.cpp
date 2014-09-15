#include "PropertiesPusherADK.h"

ANDROID_ACCESSORY_INFORMATION myDeviceInfo = {
  manufacturer, sizeof(manufacturer),
  model, sizeof(model),
  description, sizeof(description),
  version, sizeof(version),
  uri, sizeof(uri),
  serial, sizeof(serial)
  };
  
StringStream ADKStream;
TokenParser ADKToken(&ADKStream);

  // Replyes for Android device
#define REPLY_OK "ok"
#define REPLY_GETOUT "getout"
#define REPLY_UNKNOWN_CMD "dontunderstand"
  // Local variables
  BOOL deviceAttached = FALSE;
static void* deviceHandle = NULL;
static char read_buffer[128];
static char write_buffer[128];
int write_size;
BOOL writeInProgress = FALSE;

BOOL USBEventHandlerApplication( uint8_t address, USB_EVENT event, void *data, DWORD size ) {
  BOOL fRet = FALSE;

  // Call event handler from base host controller
  // (this is important to be done, because it also turns on and off power on microcontroller
  // pins on events EVENT_VBUS_REQUEST_POWER Ð¸ EVENT_VBUS_RELEASE_POWER)
  fRet = USBHost.DefaultEventHandler(address, event, data, size);

  switch( event ) {
  case EVENT_ANDROID_DETACH:
    deviceAttached = FALSE;
    return TRUE;
    break;

  case EVENT_ANDROID_ATTACH:
    deviceAttached = TRUE;
    deviceHandle = data;
    return TRUE;

  default :
    break;
  }
  return fRet;
}


/**
 * Process input - parse string, execute command.
 * @return size of reply in bytes (0 for no reply).
 */
int processInput(Quick *q, Properties *properties, char* buffer, int size, char* reply_buffer) {
  int replySize = 0;
  reply_buffer[0] = 0;
  buffer[size++] = '\r'; //make string end with a carriage return for token parser
  buffer[size] = 0;

  strcpy(ADKStream.inbuffer,buffer);
  ADKStream.inbufferlength = size;
  ADKStream.inbufferposition = 0;
  ADKStream.outbufferposition = 0;
  
  while(ADKStream.inbufferlength>ADKStream.inbufferposition) {
    if(ADKToken.scan()) {
      q->command(ADKToken);
      properties->evaluate(ADKToken);
      //additional commands from android
      //if(ADKToken.compare("somecommand")) {
      //}
    }
  }
  strcpy(reply_buffer, ADKStream.outbuffer);// Prepare reply
  replySize = strlen(reply_buffer) + 1;
  return replySize;
}

void ProcessHost(Quick *q, Properties *properties) {
  DWORD readSize;
  BOOL readyToRead = TRUE;
  DWORD writeSize;
  uint8_t errorCode;

  // Run periodic tasks to keep USB stack alive and healthy.
  // Run at least once per cycle or when you need to update inner state of USB host controller.
  USBTasks();

  if(deviceAttached) {
    // Read data from Android device - wait for a command
    if(readyToRead) {
      // Call is not blocked - will check if read is complete with AndroidAppIsReadComplete
      errorCode = USBAndroidHost.AppRead(deviceHandle, (uint8_t*)&read_buffer, (DWORD)sizeof(read_buffer));
      if(errorCode == USB_SUCCESS) {
        // Received command - will not read the next one until all data is received,
        // will check for that in next loop iterations.
        readyToRead = FALSE;
      }
    }

    // Let's check if read is complete
    if(USBAndroidHost.AppIsReadComplete(deviceHandle, &errorCode, &readSize)) {
      if(errorCode == USB_SUCCESS) {
        // Data portion is read, let's add finishing zero to make it zero-terminated string.
        read_buffer[readSize] = 0;

        // and we can execute the command now, reply will go to write_buffer
        writeSize = processInput(q, properties, read_buffer, readSize, write_buffer);

        // Allow to read next command
        readyToRead = TRUE;
        readSize = 0;

        // If writeSize is not 0, send back reply - init write procedure
        // for the next loop iteration (data is already inside write_buffer)
        write_size = writeSize;
      } 
      else {
        //                Serial1.print("Error trying to complete read: errorCode=");
        //                Serial1.println(errorCode, HEX);
      }
    }

    // Send data to Android device
    if(write_size > 0 && !writeInProgress) {
      //            Serial1.print("Write: ");
      //            Serial1.print(write_buffer);
      //            Serial1.println();

      writeSize = write_size;
      // Require command is already in the buffer to be sent.
      // Call is not blocked - will check if write is complete with AndroidAppIsWriteComplete
      errorCode = USBAndroidHost.AppWrite(deviceHandle, (uint8_t*)&write_buffer, (DWORD) writeSize);

      if(errorCode == USB_SUCCESS) {
        writeInProgress = TRUE;
      } 
      else {
        Serial1.print("Error trying to complete read: errorCode=");
        Serial1.println(errorCode, HEX);

        write_size = 0;
      }
    }

    if(writeInProgress) {
      // Let's check if write is complete
      if(USBAndroidHost.AppIsWriteComplete(deviceHandle, &errorCode, &writeSize)) {
        writeInProgress = FALSE;
        write_size = 0;

        if(errorCode != USB_SUCCESS) {
          //                    Serial1.print("Error trying to complete read: errorCode=");
          //                    Serial1.println(errorCode, HEX);
        }
      }
    }
  }
}


