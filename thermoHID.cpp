#include <USBComposite.h>
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "HidSensorSpec.h"
#define TXSIZE 256
#define RXSIZE 300

USBHID HID;
HIDRaw<TXSIZE,RXSIZE> raw(HID);
uint8 buf[RXSIZE];
// Derived from https://github.com/majbthrd/HIDsensor.git


#define ALIGNMENT(x) __attribute__ ((aligned (x)))

ALIGNMENT(4) const uint8_t reportDescription[] = {

        HID_USAGE_PAGE_SENSOR,
        HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
        HID_COLLECTION(Physical),

        //feature reports (xmit/receive)
        HID_USAGE_PAGE_SENSOR,

        HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
        HID_LOGICAL_MIN_8(0),
        HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
        HID_REPORT_SIZE(32),
        HID_REPORT_COUNT(1),
        HID_UNIT_EXPONENT(0),
        HID_FEATURE(Data_Var_Abs),

        HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MAX),
        HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
        HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
        HID_REPORT_SIZE(16),
        HID_REPORT_COUNT(1),
        HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
        HID_FEATURE(Data_Var_Abs),

        HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MIN),
        HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
        HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
        HID_REPORT_SIZE(16),
        HID_REPORT_COUNT(1),
        HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
        HID_FEATURE(Data_Var_Abs),

        //input reports (transmit)
        HID_USAGE_PAGE_SENSOR,

        HID_USAGE_SENSOR_STATE,
        HID_LOGICAL_MIN_8(0),
        HID_LOGICAL_MAX_8(6),
        HID_REPORT_SIZE(8),
        HID_REPORT_COUNT(1),
        HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN,
        HID_USAGE_SENSOR_STATE_READY,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE,
        HID_USAGE_SENSOR_STATE_NO_DATA,
        HID_USAGE_SENSOR_STATE_INITIALIZING,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED,
        HID_USAGE_SENSOR_STATE_ERROR,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,

        HID_USAGE_SENSOR_EVENT,
        HID_LOGICAL_MIN_8(0),
        HID_LOGICAL_MAX_8(16),
        HID_REPORT_SIZE(8),
        HID_REPORT_COUNT(1),
        HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY,
        HID_USAGE_SENSOR_EVENT_MAX_REACHED,
        HID_USAGE_SENSOR_EVENT_MIN_REACHED,
        HID_USAGE_SENSOR_EVENT_HIGH_THRESHOLD_CROSS_UPWARD,
        HID_USAGE_SENSOR_EVENT_HIGH_THRESHOLD_CROSS_DOWNWARD,
        HID_USAGE_SENSOR_EVENT_LOW_THRESHOLD_CROSS_UPWARD,
        HID_USAGE_SENSOR_EVENT_LOW_THRESHOLD_CROSS_DOWNWARD,
        HID_USAGE_SENSOR_EVENT_ZERO_THRESHOLD_CROSS_UPWARD,
        HID_USAGE_SENSOR_EVENT_ZERO_THRESHOLD_CROSS_DOWNWARD,
        HID_USAGE_SENSOR_EVENT_PERIOD_EXCEEDED,
        HID_USAGE_SENSOR_EVENT_FREQUENCY_EXCEEDED,
        HID_USAGE_SENSOR_EVENT_COMPLEX_TRIGGER,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,

        HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,
        HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
        HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
        HID_REPORT_SIZE(16),
        HID_REPORT_COUNT(1),
        HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
        HID_INPUT(Data_Var_Abs),
        HID_END_COLLECTION

} ;

static void HidTask(void *a);

void startUSBHID()
{
     xTaskCreate( HidTask, "HidTask", 250, NULL, 13, NULL );   
}

void HidTask(void *a)
{
  HID.begin(reportDescription, sizeof(reportDescription));  
  raw.begin();
  while(1)
  {
    if (raw.getOutput(buf)) 
    {
        for (int i=0;i<RXSIZE;i++) buf[i]++;
     raw.send(buf+RXSIZE-min(RXSIZE,TXSIZE),min(RXSIZE,TXSIZE));
    }else
     xDelay(100);
  }
}

