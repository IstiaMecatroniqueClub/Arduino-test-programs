// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13


//#include <SPI.h>
//#include <Time.h>
#include "mcp_can.h"

#define ID_US1 0xC0
#define ID_US2 0xC2

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


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
    
    
    //CAN.sendMsgBuf(0x1C, 1, 1, buf);
    
    // send data only when you receive data:
      if (Serial.available() > 0) {
              // read the incoming byte:
              int incomingByte = Serial.read();
              if(incomingByte == '1'){
                  //INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U rtr, INT8U len, INT8U *buf);   /* send buf                     */
                  CAN.sendMsgBuf(ID_US1, 0, 1,0, buf);
              }
              else if(incomingByte == '2'){
                  //INT8U sendMsgBuf(INT32U id, INT8U ext, INT8U rtr, INT8U len, INT8U *buf);   /* send buf                     */
                  CAN.sendMsgBuf(ID_US2, 0, 1,0, buf);
              }
              while(Serial.available() > 0){
                  incomingByte = Serial.read();
              }
              Serial.println("sent");
      }
        

    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned char canId = CAN.getCanId();
        
        if(canId == ID_US1){

            Serial.println("-------------US1----------------");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           
           Serial.println(dst); 
        }
        if(canId == ID_US2){

            Serial.println("-------------US2----------------");
           long unsigned int dst = (buf[0]*256 + buf[1]);
           
           Serial.println(dst); 
        }
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
