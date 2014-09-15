#include "StringStream.h"

#include <chipKITUSBHost.h>
#include <chipKITUSBAndroidHost.h>

#include "pic32lib/Quick.h"
#include "pic32lib/TokenParser.h"
#include "pic32lib/Properties.h"

static char manufacturer[] = "PONTECH";
static char model[] = "Demo";
static char description[] = "Properties Pusher";
static char version[] = "1.0";
static char uri[] = "http://www.pontech.com/";
static char serial[] = "N/A";

extern StringStream ADKStream;
extern TokenParser ADKToken;
extern ANDROID_ACCESSORY_INFORMATION myDeviceInfo;
  
BOOL USBEventHandlerApplication( uint8_t address, USB_EVENT event, void *data, DWORD size );
int processInput(Quick *q, Properties *properties, char* buffer, int size, char* reply_buffer);
void ProcessHost(Quick *q, Properties *properties);



