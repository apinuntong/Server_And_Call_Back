/*
ESP8266 library

When you use with UNO board, uncomment the follow line in uartWIFI.h.
*/
#define UNO
/*
When you use with MEGA board, uncomment the follow line in uartWIFI.h.
#define MEGA

Connection:
When you use it with UNO board, the connection should be like these:
ESP8266_TX->D0
ESP8266_RX->D1
ESP8266_CH_PD->3.3V
ESP8266_VCC->3.3V
ESP8266_GND->GND

FTDI_RX->D3			//The baud rate of software serial can't be higher that 19200, so we use software serial as a debug port
FTDI_TX->D2

When you use it with MEGA board, the connection should be like these:
ESP8266_TX->RX1(D19)
ESP8266_RX->TX1(D18)
ESP8266_CH_PD->3.3V
ESP8266_VCC->3.3V
ESP8266_GND->GND

When you want to output the debug information, please use DebugSerial. For example,

DebugSerial.println("hello");


Note:	The size of message from ESP8266 is too big for arduino sometimes, so the library can't receive the whole buffer because  
the size of the hardware serial buffer which is defined in HardwareSerial.h is too small.

Open the file from \arduino\hardware\arduino\avr\cores\arduino\HardwareSerial.h.
See the follow line in the HardwareSerial.h file.

#define SERIAL_BUFFER_SIZE 64

The default size of the buffer is 64. Change it into a bigger number, like 256 or more.





*/
#include <SoftwareSerial.h>//RX TX


#define SSID       "OpenWrt_NAT_71_DEVICES_2"
#define PASSWORD   "devicenetwork"


#include "uartWIFI.h"
#include <SoftwareSerial.h>

WIFI wifi;

int in1=10,in2=11;

extern int chlID;	//client id(0-4)


void setup()
{
  
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(50) ;
  
// DebugSerial.println("BEGIN"); 
  wifi.begin();
  DebugSerial.println("BEGIN");
  bool b = wifi.Initialize(STA, SSID, PASSWORD);
  if(!b)
  {
    DebugSerial.println("Init error");
  }
  
  delay(8000);  //make sure the module can have enough time to get an IP address 
  String ipstring  = wifi.showIP();
  DebugSerial.println(ipstring);		//show the ip address of module
  
  delay(4000);
  wifi.confMux(1);
  delay(100);
  if(wifi.confServer(1,2000))
	DebugSerial.println("Server is set up");
	

}
void loop()
{
  
  char buf[100];
  int iLen = wifi.ReceiveMessage(buf);
  if(iLen > 0)
  {
    DebugSerial.println(iLen);
    DebugSerial.println(strlen(buf));
    DebugSerial.println(strcmp(buf,"HELLO"));
    
    //if receive a "HELLO", send a "HELLO BACK" back to the client
    if (strcmp(buf, "HELLO") == 0)
    {
      DebugSerial.print("Get a message from id ");
      DebugSerial.print(chlID);
      DebugSerial.println(":");
      DebugSerial.println(buf);
      
      DebugSerial.print("Send a message back to id ");
      DebugSerial.print(chlID);
      DebugSerial.println(":");
      DebugSerial.println("HELLO BACK");      
      wifi.Send(chlID,"HELLO BACK");
    }
    
    if(strcmp(buf, "S10")==0){    //when you need switch 1 off
        digitalWrite(in1,LOW);
        wifi.Send(chlID,"Switch 1 off");
    }else if(strcmp(buf, "S11")==0){ //when you need Switch 1 on
        digitalWrite(in1,HIGH);
         wifi.Send(chlID,"Switch 1 on");
    }else if(strcmp(buf, "S20")==0){  //when you need switch 2 off
        digitalWrite(in2,LOW);   
        wifi.Send(chlID,"Switch 2 off"); 
    }else if(strcmp(buf, "S21")==0){  //when you need switch 2 on
        digitalWrite(in2,HIGH);
         wifi.Send(chlID,"Switch 2 on");
    }
    
    

  }
}
