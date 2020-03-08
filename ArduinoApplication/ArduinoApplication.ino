#include <TimedAction.h>
#include <SPI.h>
#include <LoRa.h>

#define messageSize 60
#define systemDelay 500
#define ss 6
#define rst 5
#define dio0 3


char readDataBluetooth[messageSize];
char writeDataBluetooth[messageSize];
char readDataLoRa[messageSize];
char writeDataLoRa[messageSize];

void setup() {
  Serial.begin(9600);
  //SPI.begin(/*sck*/ 16, /*miso*/ 15, /*mosi*/ 14, /*ss*/ 13);
  delay(1000);
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(915E6));
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa intialized OK!");

}

void loop() {
  readBluetoothDataMethod();
  delay(systemDelay);
  writeDataLoRaMethod();
  delay(systemDelay);
  readDataLoRaMethod();
  delay(systemDelay);
  writeBluetoothDataMethod();
  delay(systemDelay);
}

void readDataLoRaMethod() {
 String LoRaData;
   if(LoRa.parsePacket() == 0){ return; }

  else{
        // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      Serial.println("");
    }
  }
  

    for (int i = 0; i < messageSize; i++) {
      readDataLoRa[i] = LoRaData[i];
    }

    Serial.print("ReadLoRa: " );
    Serial.print(readDataLoRa);
    Serial.println();

    for (int i = 0; i < messageSize; i++) {
      writeDataBluetooth[i] = readDataLoRa[i];
    }

  //}
  return;
}


void writeDataLoRaMethod() {
  String writeDataString = "1111111111111111111111111111111111111111111111111111111111\n";
  boolean terminate = false;
  for (int i = 0; i < messageSize; i++) {
    if (writeDataLoRa[i] == '\0'){
      terminate = true;
    }
    
    if (terminate){
      writeDataString[i] = '\0';
    } else{
       writeDataString[i] = writeDataLoRa[i];
    }
  }
  writeDataString[59] = '\n';

  LoRa.beginPacket();
  LoRa.print(writeDataString);
  LoRa.endPacket();
 
  Serial.print("writeLoRa: ");
  Serial.print(writeDataString);
  Serial.println();
  return;
}

void readBluetoothDataMethod() {
  int i = 0;

  while (Serial.available()) {
    char readch = Serial.read();
    if (i < 60) {
      readDataBluetooth[i] = readch;
      i = i + 1;
    }
  }

  Serial.print("ReadBluetooth: ");
  Serial.println(readDataBluetooth);
  
  boolean terminate = false;
  
  for (int i = 0; i < messageSize; i++) {
    if (readDataBluetooth[i] == '\0'){
      terminate = true;
    }
    
    if (terminate){
          writeDataLoRa[i] = '\0';
    } else{
          writeDataLoRa[i] = readDataBluetooth[i];
    }
  }

}

void writeBluetoothDataMethod() {
  if (Serial.available()) {
    if (writeDataBluetooth[0] != 0) {
      Serial.println(writeDataBluetooth);
    }
  }
}
