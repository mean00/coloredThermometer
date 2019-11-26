#include <USBComposite.h>
#include "MapleFreeRTOS1000.h"
#include "MapleFreeRTOS1000_pp.h"

#define TXSIZE 256
#define RXSIZE 300

USBHID HID;
HIDRaw<TXSIZE,RXSIZE> raw(HID);
uint8 buf[RXSIZE];

const uint8_t reportDescription[] = {
   HID_RAW_REPORT_DESCRIPTOR(TXSIZE,RXSIZE)
};

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

