// demo: CAN-BUS Shield, receive data with check mode
// send data coming to fast, such as less than 10ms, you can use this way
// loovee, 2014-6-13


//#include <SPI.h>
//#include <Time.h>
#include "mcp_can.h"

#define ID_Mot_L 0x30
#define ID_Conf_Mot_L 0x31
#define ID_Mot_R 0x35
#define ID_Conf_Mot_D 0x36

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

    buf[0] = 0x00;
    //CAN.sendMsgBuf(ID_Conf_Mot_L, 0, 1, buf);
    //CAN.sendMsgBuf(ID_Conf_Mot_D, 0, 1, buf);
    buf[1] = 0x50;
    buf[2] = 0x00;
    
    while(1) {
        if (Var_delay == 0 | Var_delay == 50 | Var_delay == 100 | Var_delay == 150 | Var_delay == 200 | Var_delay == 250 | Var_delay == 300 | Var_delay == 350 | Var_delay == 400 | Var_delay == 450) 
        { 
          CAN.sendMsgBuf(ID_Mot_L, 0, 3, buf);
          CAN.sendMsgBuf(ID_Mot_R, 0, 3, buf);
        }
        if (Var_delay == 410) { 
          buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00;
          CAN.sendMsgBuf(ID_Mot_L, 0, 3, buf);
          CAN.sendMsgBuf(ID_Mot_R, 0, 3, buf);
          buf[0] = 0x01; buf[1] = 0x50; buf[2] = 0x00;
        }
        if (Var_delay == 500 | Var_delay == 550 | Var_delay == 600 | Var_delay == 650 | Var_delay == 700 | Var_delay == 750 | Var_delay == 800 | Var_delay == 850 | Var_delay == 900 | Var_delay == 950) 
        { 
          CAN.sendMsgBuf(ID_Mot_L, 0, 3, buf);
          CAN.sendMsgBuf(ID_Mot_R, 0, 3, buf);
        }
        if (Var_delay == 910) { 
          buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00;
          CAN.sendMsgBuf(ID_Mot_L, 0, 3, buf);
          CAN.sendMsgBuf(ID_Mot_R, 0, 3, buf);
          buf[0] = 0x00; buf[1] = 0x50; buf[2] = 0x00;
        }
        
//      // check if data coming
//        if(CAN_MSGAVAIL == CAN.checkReceive())            
//        {
//            CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
//            unsigned char canId = CAN.getCanId();
//            if(CAN.isRemoteRequest()) { Serial.println("Remote");}
//            Serial.println(canId);
//        }
        
        delay(10);
        Var_delay++;
        if (Var_delay == 1000) { Var_delay=0;}
    }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
