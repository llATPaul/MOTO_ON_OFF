#include <CurieBLE.h> //derivato da CurieBLE->Peripheral->LED Arduino 101 demo Sketch

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); 


// const int ledPin = 13;                               
void setup()                                          
{
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  BLE.begin();                                    

  BLE.setLocalName("MODULO CONTROLLO MOTORE");             
  BLE.setAdvertisedService(ledService);               

  ledService.addCharacteristic(switchCharacteristic); 
  BLE.addService(ledService);
  switchCharacteristic.setValue(0);

  BLE.advertise();                                   

  Serial.println("Setup OK!");
}

void loop() 
{
  BLEDevice central = BLE.central();

  if (central)
  {
    Serial.print ("Connected to central: ");
    Serial.println (central.address());
    while (central.connected()) 
    {
      if (switchCharacteristic.written()) 
      {
        int readValue = switchCharacteristic.value();
        if (readValue == 97)                     
        {   
          Serial.println("LED on");
          digitalWrite(8, HIGH);
          delay (500);
          digitalWrite(8, LOW);
        } 
        else if (readValue == 98)                
        {                             
          Serial.println(F("LED off"));
          digitalWrite(9, HIGH);
          delay (500);
          digitalWrite(9, LOW);
        }
      }
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
