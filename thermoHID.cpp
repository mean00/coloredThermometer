#include <USBComposite.h>
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "HidSensorSpec.h"

#define ALIGNMENT(x) __attribute__ ((aligned (x)))


 ALIGNMENT(4) const uint8_t reportDescription[] = {

        HID_USAGE_PAGE_SENSOR,  HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
        
        HID_COLLECTION(Physical),
            //feature reports (xmit/receive)
            HID_USAGE_PAGE_SENSOR,      HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
                HID_LOGICAL_MIN_8(0),
                HID_LOGICAL_MAX_8(255),
                HID_REPORT_SIZE(32), // 1*32 bits
                HID_REPORT_COUNT(1),
                HID_UNIT_EXPONENT(0),
                HID_FEATURE(Data_Var_Abs),

            HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MAX),
                HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
                HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
                HID_REPORT_SIZE(16), // 1* 16 bits
                HID_REPORT_COUNT(1),
                HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
                HID_FEATURE(Data_Var_Abs),

            HID_USAGE_SENSOR_DATA(HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,HID_USAGE_SENSOR_DATA_MOD_MIN),
                HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
                HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
                HID_REPORT_SIZE(16),
                HID_REPORT_COUNT(1), // 1*16 bits
                HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
                HID_FEATURE(Data_Var_Abs),


            //input reports (transmit)
            HID_USAGE_PAGE_SENSOR,  HID_USAGE_SENSOR_STATE,
                HID_LOGICAL_MIN_8(0),
                HID_LOGICAL_MAX_8(6),
                HID_REPORT_SIZE(8), // 1*8bits
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
                    HID_LOGICAL_MAX_8(16), // 1 * 8 bits
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
                    HID_REPORT_SIZE(16),  // 1* 16 bits
                    HID_REPORT_COUNT(1),
                    HID_UNIT_EXPONENT(0x0E), // scale default unit Celsius to provide 2 digits past the decimal point
                    HID_INPUT(Data_Var_Abs),
        HID_END_COLLECTION

} ;

 /**
  */
 class TemperatureReport : public HIDReporter
 {
 public:
        TemperatureReport(USBHID &parent,uint8_t reportID) : HIDReporter(HID, (uint8_t*)&temperature, sizeof(temperature), reportID) 
        {
        }
public:
        uint32_t temperature;
 };
 
/**
 */
class TemperatureSensorHID : public USBHID
{
public:
            TemperatureSensorHID()
            {
            }
            void begin()
            {
                report=new TemperatureReport(*this, 0);
                USBHID::begin(reportDescription, sizeof(reportDescription));  
                
            }
            void setTemperature(float t)
            {
                _temp=t;
                report->temperature=(uint32_t)t;
    //            doReport();
            }
            void doReport()
            {
                report->sendReport();
            }
protected:
            TemperatureReport *report;
            float _temp;
    
};

// Derived from https://github.com/majbthrd/HIDsensor.git

static void HidTask(void *a);

void startUSBHID()
{
     xTaskCreate( HidTask, "HidTask", 250, NULL, 13, NULL );   
}

void HidTask(void *a)
{
    USBComposite.setProductString("Temp Sensor");
    USBComposite.setManufacturerString("Bluepill");
    TemperatureSensorHID *sensorHid=new TemperatureSensorHID;
    sensorHid->begin();
    sensorHid->setTemperature(22);
    while(1)
    {
        sensorHid->setTemperature(21);    
        xDelay(100);
    }
    
}
#if 0

0x05, 0x20,        // Usage Page (0x20)
0x09, 0x33,        // Usage (0x33)
0xA1, 0x00,        // Collection (Physical)
0x05, 0x20,        //   Usage Page (0x20)
0x0A, 0x0E, 0x03,  //   Usage (0x030E)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0xFF,        //   Logical Maximum (-1)
0x75, 0x20,        //   Report Size (32)
0x95, 0x01,        //   Report Count (1)
0x55, 0x00,        //   Unit Exponent (0)
0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x0A, 0x34, 0x24,  //   Usage (0x2434)
0x16, 0x01, 0x80,  //   Logical Minimum (-32767)
0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
0x75, 0x10,        //   Report Size (16)
0x95, 0x01,        //   Report Count (1)
0x55, 0x0E,        //   Unit Exponent (-2)
0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x0A, 0x34, 0x34,  //   Usage (0x3434)
0x16, 0x01, 0x80,  //   Logical Minimum (-32767)
0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
0x75, 0x10,        //   Report Size (16)
0x95, 0x01,        //   Report Count (1)
0x55, 0x0E,        //   Unit Exponent (-2)
0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
0x05, 0x20,        //   Usage Page (0x20)
0x0A, 0x01, 0x02,  //   Usage (0x0201)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x06,        //   Logical Maximum (6)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0xA1, 0x02,        //   Collection (Logical)
0x0A, 0x00, 0x08,  //     Usage (0x0800)
0x0A, 0x01, 0x08,  //     Usage (0x0801)
0x0A, 0x02, 0x08,  //     Usage (0x0802)
0x0A, 0x03, 0x08,  //     Usage (0x0803)
0x0A, 0x04, 0x08,  //     Usage (0x0804)
0x0A, 0x05, 0x08,  //     Usage (0x0805)
0x0A, 0x06, 0x08,  //     Usage (0x0806)
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0x0A, 0x02, 0x02,  //   Usage (0x0202)
0x15, 0x00,        //   Logical Minimum (0)
0x25, 0x10,        //   Logical Maximum (16)
0x75, 0x08,        //   Report Size (8)
0x95, 0x01,        //   Report Count (1)
0xA1, 0x02,        //   Collection (Logical)
0x0A, 0x10, 0x08,  //     Usage (0x0810)
0x0A, 0x11, 0x08,  //     Usage (0x0811)
0x0A, 0x12, 0x08,  //     Usage (0x0812)
0x0A, 0x13, 0x08,  //     Usage (0x0813)
0x0A, 0x14, 0x08,  //     Usage (0x0814)
0x0A, 0x15, 0x08,  //     Usage (0x0815)
0x0A, 0x16, 0x08,  //     Usage (0x0816)
0x0A, 0x17, 0x08,  //     Usage (0x0817)
0x0A, 0x18, 0x08,  //     Usage (0x0818)
0x0A, 0x19, 0x08,  //     Usage (0x0819)
0x0A, 0x1A, 0x08,  //     Usage (0x081A)
0x0A, 0x1B, 0x08,  //     Usage (0x081B)
0x0A, 0x1C, 0x08,  //     Usage (0x081C)
0x0A, 0x1D, 0x08,  //     Usage (0x081D)
0x0A, 0x1E, 0x08,  //     Usage (0x081E)
0x0A, 0x1F, 0x08,  //     Usage (0x081F)
0x0A, 0x20, 0x08,  //     Usage (0x0820)
0x81, 0x00,        //     Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              //   End Collection
0x0A, 0x34, 0x04,  //   Usage (0x0434)
0x16, 0x01, 0x80,  //   Logical Minimum (-32767)
0x26, 0xFF, 0x7F,  //   Logical Maximum (32767)
0x75, 0x10,        //   Report Size (16)
0x95, 0x01,        //   Report Count (1)
0x55, 0x0E,        //   Unit Exponent (-2)
0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
0xC0,              // End Collection


HID_USAGE_PAGE_SENSOR,
    HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
    HID_COLLECTION(Physical),
 
    //feature reports (xmit/receive)
    HID_USAGE_PAGE_SENSOR,
    // Sensor Connection Type - RO
    HID_USAGE_SENSOR_PROPERTY_SENSOR_CONNECTION_TYPE,  
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(2),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_INTEGRATED_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_ATTACHED_SEL_SEL,
        HID_USAGE_SENSOR_PROPERTY_CONNECTION_TYPE_PC_EXTERNAL_SEL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
   // Reporting State - RW
    HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_SEL_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS_SEL_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS_SEL_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_THRESHOLD_EVENTS_WAKE_SEL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    // Power State - RW
    HID_USAGE_SENSOR_PROPERTY_POWER_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(5),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_UNDEFINED_SEL,          
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER_SEL,      
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D1_LOW_POWER_SEL,       
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D2_STANDBY_WITH_WAKE_SEL,
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D3_SLEEP_WITH_WAKE_SEL, 
        HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF_SEL,       
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    // Sensor State - RW
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL_SEL,
        HID_FEATURE(Data_Arr_Abs),
        HID_END_COLLECTION,
    // Report Interval - RW
    HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL,
    HID_LOGICAL_MIN_32(0x00,0x00,0x00,0x00),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0),
    HID_FEATURE(Data_Var_Abs),
    // Minimum Report Interval  -  RO
    HID_USAGE_SENSOR_PROPERTY_MINIMUM_REPORT_INTERVAL,  
    HID_LOGICAL_MIN_32(0x00,0x00,0x00,0x00),
    HID_LOGICAL_MAX_32(0xFF,0xFF,0xFF,0xFF),
    HID_REPORT_SIZE(32),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0),
    HID_FEATURE(Data_Var_Abs),
    // Sensor Accuracy - RO
    HID_USAGE_SENSOR_PROPERTY_ACCURACY,
    HID_LOGICAL_MIN_8(0x00),  //0   
    HID_LOGICAL_MAX_8(0xFF),  //255   
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0E), // x0.01
    HID_FEATURE(Data_Var_Abs),
    // Sensor Resolution - RO
    HID_USAGE_SENSOR_PROPERTY_RESOLUTION,
    HID_LOGICAL_MIN_8(0x00),      // 0
    HID_LOGICAL_MAX_16(0xFF,0xFF),// 65535
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0C), // x0.0001
    HID_FEATURE(Data_Var_Abs),
    // Range Maximum - RO
    HID_USAGE_SENSOR_PROPERTY_RANGE_MAXIMUM,
    HID_LOGICAL_MIN_8(0x00),  //0
    HID_LOGICAL_MAX_8(0xFF),  //255
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x00), // x1
    HID_FEATURE(Data_Var_Abs),
    // Range Minimum - RO
    HID_USAGE_SENSOR_PROPERTY_RANGE_MINIMUM,
    HID_LOGICAL_MIN_8(0x81), //-127
    HID_LOGICAL_MAX_8(0xFF), // 127
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x00), // x1
    HID_FEATURE(Data_Var_Abs),
    // Change Sensitivity Absolute - RW
    HID_USAGE_SENSOR_PROPERTY_CHANGE_SENSITIVITY_ABS,
    HID_LOGICAL_MIN_8(0x00),  //0
    HID_LOGICAL_MAX_16(0xFF,0xFF),// 65535
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0D), // x0.001
    HID_FEATURE(Data_Var_Abs),
 
    //input reports (transmit)
    HID_USAGE_PAGE_SENSOR,
    // Sensor state
    HID_USAGE_SENSOR_STATE,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(6),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_STATE_UNKNOWN_SEL_SEL,
        HID_USAGE_SENSOR_STATE_READY_SEL_SEL,
        HID_USAGE_SENSOR_STATE_NOT_AVAILABLE_SEL_SEL,
        HID_USAGE_SENSOR_STATE_NO_DATA_SEL_SEL,
        HID_USAGE_SENSOR_STATE_INITIALIZING_SEL_SEL,
        HID_USAGE_SENSOR_STATE_ACCESS_DENIED_SEL_SEL,
        HID_USAGE_SENSOR_STATE_ERROR_SEL_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    // Sensor Events 
    HID_USAGE_SENSOR_EVENT,
    HID_LOGICAL_MIN_8(0),
    HID_LOGICAL_MAX_8(16),
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_COLLECTION(Logical),
        HID_USAGE_SENSOR_EVENT_UNKNOWN_SEL_SEL,
        HID_USAGE_SENSOR_EVENT_STATE_CHANGED_SEL_SEL,
        HID_USAGE_SENSOR_EVENT_PROPERTY_CHANGED_SEL_SEL,
        HID_USAGE_SENSOR_EVENT_DATA_UPDATED_SEL_SEL,
        HID_USAGE_SENSOR_EVENT_POLL_RESPONSE_SEL_SEL,
        HID_USAGE_SENSOR_EVENT_CHANGE_SENSITIVITY_SEL_SEL,
        HID_INPUT(Data_Arr_Abs),
        HID_END_COLLECTION,
    // Temperature data.
    HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,
    HID_LOGICAL_MIN_16(0x01,0x80), //    LOGICAL_MINIMUM (-32767)
    HID_LOGICAL_MAX_16(0xFF,0x7F), //    LOGICAL_MAXIMUM (32767)
    HID_REPORT_SIZE(16),
    HID_REPORT_COUNT(1),
    HID_UNIT_EXPONENT(0x0E), // x0.01
    HID_INPUT(Data_Var_Abs),
 
    HID_END_COLLECTION
 
};
#endif
