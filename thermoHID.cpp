#include <USBComposite.h>
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"
#include "HidSensorSpec.h"

#define ALIGNMENT(x) __attribute__ ((aligned (x)))

xBinarySemaphore *usbLock=NULL;

 ALIGNMENT(4) const uint8_t reportDescription[] = {
        HID_USAGE_PAGE_SENSOR,  HID_USAGE_SENSOR_TYPE_ENVIRONMENTAL_TEMPERATURE,
        
        HID_COLLECTION(Physical),
            HID_USAGE_SENSOR_DATA_ENVIRONMENTAL_TEMPERATURE,
                    HID_LOGICAL_MIN_16(0x01,0x80), // LOGICAL_MINIMUM (-32767)
                    HID_LOGICAL_MAX_16(0xFF,0x7F), // LOGICAL_MAXIMUM (32767)
                    HID_REPORT_ID(1),
                    HID_REPORT_SIZE(32),  // 1* 16 bits
                    HID_REPORT_COUNT(1),                   
                    HID_INPUT(Data_Var_Abs),
        HID_END_COLLECTION
    
} ;

 /**
  */
 class TemperatureReport : public HIDReporter
 {
 public:
        TemperatureReport(USBHID &parent,uint8_t reportID) : HIDReporter(parent, _report, sizeof(_report), reportID) 
        {
            _temp=20.12;
        }
        uint8_t xdigit(uint32_t v)
        {
            uint8_t r=v%10;
            return r;            
        }
        
        void doReport()
        {
            static uint32_t t;
            t=(uint32_t)(_temp*100.);
            _report[0]=1;            // Report ID
            
            _report[4]= xdigit(t);
            _report[3]= xdigit(t/10);
            _report[2]= xdigit(t/100);
            _report[1]= xdigit(t/1000);
            
            uint32_t r=(uint32_t)_resistor*10;
            _report[8]= xdigit(r);
            _report[7]= xdigit(r/10);
            _report[6]= xdigit(r/100);
            _report[5]= xdigit(r/1000);
            
            sendReport();
        }
public:
        uint8_t _report[8+1];
        float   _temp;
        float   _resistor;
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
                USBComposite.setProductString("Temp Sensor");
                USBComposite.setManufacturerString("Bluepill");
                USBComposite.setSerialString("1234");
                USBHID::begin(reportDescription, sizeof(reportDescription));  
                
            }
            void setTempAndResistor(float t, float resistor)  
            {
                report->_resistor=resistor;
                report->_temp=t;                
            }
            void doReport()
            {
                report->doReport();
            }
protected:
            TemperatureReport *report;            
    
};

// Derived from https://github.com/majbthrd/HIDsensor.git

static void HidTask(void *a);
TemperatureSensorHID *sensorHid=NULL;
void startUSBHID()
{
    xTaskCreate( HidTask, "HidTask", 250, NULL, 5, NULL );   
}
void hidSetTempAndResistor(float temp,float resistor)
{
    if(sensorHid)
        sensorHid->setTempAndResistor(temp,resistor);
}
void HidTask(void *a)
{
    sensorHid=new TemperatureSensorHID;
    usbLock=new xBinarySemaphore();
    //usbLock->give();
    xDelay(1000);
    sensorHid->begin();
    while(1)
    {        
        //usbLock->take();
        sensorHid->doReport();
        //usbLock->give();
        xDelay(1000);
    }
    
}
