
#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

int incomingByte = 0;   // for incoming serial data

void setup() {
    Serial.begin(115200);     // opens serial port, sets data rate to 9600 bps    
    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(50);
    }
    Serial.println("\n\nCAN BUS Shield init ok!\n\n");
    Serial.println("------");
    Serial.println("format of the expected CAN messages:");
    Serial.println("[*] 1A2#120B01 for a data message with the ID '0x1A2' and the data '12 0B 01'");
    Serial.println("[*] 012#R for a remote request without data field");
    Serial.println("[*] 023#01 for a data message with the ID '0x023' and the data '0x01'");
}

uint8_t qtoi(unsigned char quartet){
    if(quartet >='A' && quartet <='F'){
       return quartet-'A'+10;
    }else{
       return quartet-'0';
    }
}

uint8_t htoi(unsigned char msq, unsigned char lsq){
    return ( (qtoi(msq) << 4) + (qtoi(lsq)&0x0F));
}

uint8_t isFrameOk(const unsigned char* str, uint8_t strsize, uint16_t* id, uint8_t* dlc, uint8_t * buf, uint8_t* rtr){ 
  // format of the expected frame:
  // 123#120022
  // 012#R
  // 023#01
  if(str[3]!='#'){
     Serial.println("Missing the # separator");
     return 0; 
  }
  for(int i=0; i<3; i++){ //check if the id is only hexa number {0,..,9,A,...F}
     if(  (str[i] > '9' || str[i] <'0') && (str[i] < 'A' || str[i] > 'F') ){
        Serial.println("The id is not correct");
        return 0; 
     }
  }
  
  if(str[4]=='R'){ // check if it is a remote request
     if( ((strsize-5)%2!=0) || (strsize-5)>8 ){ //check if the number of bytes is OK
        Serial.println("The remote request data size is not correct");
        return 0; 
     }
     for(int i=5; i< strsize; i++){ //check if the id is only hexa number {0,..,9,A,...F}
         if( (str[i] > '9' || str[i] <'0') && (str[i] < 'A' || str[i] > 'F') ){
            Serial.println("The remote request data value is not correct");
            return 0; 
         }
     }
     // else everything is OK, lets get the ID
     *id = htoi(str[1], str[2]);
     *id += (qtoi(str[0]) << 8);
     // lets get the DLC
     *dlc = (strsize-5)/2;
     // lets get the data
     for(int i=5, j=0; i< strsize; i+=2, j++){ //check if the id is only hexa number {0,..,9,A,...F}
       buf[j] = htoi(str[i], str[i+1]);
     }
     *rtr = 1;
  }else{ // it is a data frame
     if( ((strsize-4)%2!=0) || (strsize-4)>8 ){ //check if the number of bytes is OK
        Serial.println("The data size is not correct");
        return 0; 
     }
     for(int i=4; i< strsize; i++){ //check if the id is only hexa number {0,..,9,A,...F}
         if( (str[i] > '9' || str[i] <'0') && (str[i] < 'A' || str[i] > 'F') ){
            Serial.println("The data value is not correct");
            return 0; 
         }
     }
     // else everything is OK, lets get the ID
     *id = htoi(str[1], str[2]);
     *id += (qtoi(str[0]) << 8);
     // lets get the DLC
     *dlc = (strsize-4)/2;
     // lets get the data
     for(int i=4, j=0; i< strsize; i+=2, j++){ //check if the id is only hexa number {0,..,9,A,...F}
       buf[j] = htoi(str[i], str[i+1]);
     }
     *rtr = 0;
    
  }
  return 1;

}

uint16_t id;
uint8_t dlc;
uint8_t rtr;
uint8_t data[8];

void loop() {
    unsigned char str[256];
    // send data only when you receive data:
    if (Serial.available() > 0) {
        uint8_t i=0;
        while(Serial.available()>0 && i <=255){
           str[i] =  Serial.read();
           i++;
           delay(10);
        }
        if(isFrameOk(str, i, &id, &dlc, data, &rtr)){
            CAN.sendMsgBuf(id, rtr, dlc, data);
            Serial.print("Message sent:");
            Serial.print(id, HEX);Serial.print("|");
            Serial.print(rtr, HEX);Serial.print("|");
            for(int j=0; j<dlc; j++){
                Serial.print(data[j], HEX); Serial.print(" - ");
            }Serial.println();
        }
        
    }
    
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&dlc, data);    // read data,  len: data length, buf: data buf

        unsigned char canId = CAN.getCanId();
        
        if(canId != 144 && canId!= 128){

          Serial.println("-----------------------------");
          Serial.println("get data from ID: ");
          Serial.println(canId, HEX);
  
          for(int i = 0; i<dlc; i++)    // print the data
          {
              Serial.print(data[i], HEX);
              Serial.print("\t");
          }
          Serial.println();
        }
    }
    delay(100);
    
}
