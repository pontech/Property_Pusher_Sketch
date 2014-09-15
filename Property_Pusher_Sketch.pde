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

int led1_id;
int led2_id;
int led1_var = PIN_LED1;
int led2_var = PIN_LED2;
bool led1enable = true;
bool led2enable = true;
int led1rate = 400;
int led2rate = 415;

void blink_task(int id, void * tptr) {
  int led_pin;
  led_pin = *(int *)getTaskVar(id);
  digitalWrite(led_pin, !digitalRead(led_pin));   // Toggle pin state
}

void led1enablecall(String var) {
  led1enable = (var == "true");
  if(led1enable) {
    setTaskState(led1_id, TASK_ENABLE);
  } else {
    setTaskState(led1_id, TASK_DISABLE);
  }
}
void led2enablecall(String var) {
  led2enable = (var == "true");
  if(led2enable) {
    setTaskState(led2_id, TASK_ENABLE);
  } else {
    setTaskState(led2_id, TASK_DISABLE);
  }
}
void led1ratecall(String var) {
  led1rate = Variant(&var[0]).toInt();
  setTaskPeriod(led1_id, led1rate);
}
void led2ratecall(String var) {
  led2rate = Variant(&var[0]).toInt();
  setTaskPeriod(led2_id, led2rate);
}

void setup() {
  Wire.begin();

  properties.addBool("LED_1_Enabled", true, led1enablecall);
  properties.addBool("LED_2_Enabled", true, led2enablecall);
  properties.addNumber("LED_1_Rate_ms", led1rate, led1ratecall);
  properties.addNumber("LED_2_Rate_ms", led2rate, led2ratecall);

  ADKStream.begin();
  pinMode(led1_var, OUTPUT);
  pinMode(led2_var, OUTPUT);
  led1_id = createTask(blink_task, led1rate, TASK_ENABLE, &led1_var);
  led2_id = createTask(blink_task, led2rate, TASK_ENABLE, &led2_var);

  // Init USB Host controller:
  // Pass the instance for event handler
  USBHost.Begin(USBEventHandlerApplication);
  // Send info about device to Android driver
  USBAndroidHost.AppStart(&myDeviceInfo);
}

void loop() {
  ProcessHost(&q, &properties);
}
