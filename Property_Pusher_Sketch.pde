//20140316
#include "StringStream.h"
#include <Wire.h>
#include <chipKITUSBHost.h>
#include <chipKITUSBAndroidHost.h>
#include "PropertiesPusherADK.h"

#include "pic32lib/KardIO.h"
#include "pic32lib/Quick.h"
#include "pic32lib/DetectEdge.h"
#include "pic32lib/TokenParser.h"
#include "pic32lib/Variant.h"
#include "pic32lib/Properties.h"
#include "pic32lib/Cron.h"
#include "pic32lib/StepAndDirection.h"

Quick q;

#line 20 "Property_Pusher_Sketch.pde"

Properties properties(10);

void dummy(String var) {
}

void setup() {
  Wire.begin();

//  properties.addString("somestring", "a_String", 0, Properties::RO);
  properties.addNumber("anumber", 42, dummy);
//  properties.addBool("readonlybool", false, 0, Properties::RO);
  properties.addBool("editablebool", false);
//  properties.addString("anotherstring", "blank");

  ADKStream.begin();

  // Init USB Host controller:
  // Pass the instance for event handler
  USBHost.Begin(USBEventHandlerApplication);
  // Send info about device to Android driver
  USBAndroidHost.AppStart(&myDeviceInfo);
}

void loop() {
  ProcessHost(&q, &properties);
}
