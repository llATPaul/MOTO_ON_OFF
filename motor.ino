#include <CurieBLE.h> //non la usa esclusivamente, alterna questa con ArduinoBLE!!!

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); //creo un'istanza Servizio
BLEUnsignedCharCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite); //creo caratteristica come UCHAR

// ****   bleCharateristic::value() restituisce uint8_t* non stringa ---->                                                          
// ****  (https://github.com/arduino-libraries/ArduinoBLE/blob/master/src/BLECharacteristic.h), riga 58


const int ledPin = 13;                                //non commentare, lasciare attivo come indicatore di accensione/spegnimento su scheda          

void setup()                                          //verificare funzionamento con alimentazione esterna!!!
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);                            //modificare ed aggiungere pin dei relè                    
  BLE.begin();                                    

  BLE.setLocalName("LED");             
  BLE.setAdvertisedService(ledService);               //imposto il servizio da collegare all'oggetto Servizio

  ledService.addCharacteristic(switchCharacteristic); //imposto relativa caratteristica, setto a 0 
  BLE.addService(ledService);
  switchCharacteristic.setValue(0);

  BLE.advertise();                                    //diffondo

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
        if (readValue == 97)                     //coerente con macro su nFC Connect, ricrodarsi di valutarlo come UINT
        {   
          Serial.println("LED on");
          digitalWrite(ledPin, HIGH);            //qui codice per avvio 
        } 
        else if (readValue == 98)                //coerente con macro su nFC Connect, , ricrodarsi di valutarlo come UINT
        {                             
          Serial.println(F("LED off"));
          digitalWrite(ledPin, LOW);             //qui codice per stop
        }
      }
    }

    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
