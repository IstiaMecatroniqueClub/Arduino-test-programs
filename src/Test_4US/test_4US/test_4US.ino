// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13


//#include <SPI.h>
//#include <Time.h>
#include "mcp_can.h"

#define ID_US1 0xC0
#define ID_US2 0xC1
#define ID_US3 0xC2
#define ID_US4 0xC3

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

unsigned int Var_delay = 0;


void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
}


void loop()
{
    unsigned char len = 0;
    unsigned char buf[8];

    // send data :
    if (Var_delay == 0) { CAN.sendMsgBuf(ID_US1, 0, 1,0, buf);}
    if (Var_delay == 250) { CAN.sendMsgBuf(ID_US2, 0, 1,0, buf);}
    if (Var_delay == 500) { CAN.sendMsgBuf(ID_US3, 0, 1,0, buf);}
    if (Var_delay == 750) { CAN.sendMsgBuf(ID_US4, 0, 1,0, buf);}
    
  // check if data coming
    if(CAN_MSGAVAIL == CAN.checkReceive())            
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
        unsigned char canId = CAN.getCanId();
        if(canId == ID_US1){
           Serial.print("US1 : ");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           Serial.println(dst);
        }
        if(canId == ID_US2){
           Serial.print("US2 : ");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           Serial.println(dst); 
        }
        if(canId == ID_US3){
           Serial.print("US3 : ");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           Serial.println(dst); 
        }
        if(canId == ID_US4){
           Serial.print("US4 : ");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           Serial.println(dst); 
        }
    }
    
    delay(1);
    Var_delay++;
    if (Var_delay == 1000) { Var_delay=0;}
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
