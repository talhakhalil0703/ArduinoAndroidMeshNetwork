/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>


//define the pins used by the transceiver module
#define ss 6
#define rst 5
#define dio0 3


void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  Serial.println("Enter a new message:");

}

char rx_byte = 0;
String message = "";

void loop() {

  readDataLoRa(LoRa.parsePacket());
  
  writeDataLoRa();

}

void readDataLoRa(int packetSize){
  if(packetSize == 0){ return; }

  else{
        // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      Serial.println("");
    }
  }
}

void writeDataLoRa(){

    if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if (rx_byte != '\n') {
      // a character of the string was received
      message += rx_byte;
    }
    else {
     Serial.print("Sending packet: ");
      Serial.println(message);

      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();
      
      message = "";                // clear the string for reuse
      Serial.println("");
      Serial.println("Enter a new message");    }
  }
 
}
