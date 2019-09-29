#include <dht.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

RH_RF95 rf95; // Create radio object
//StaticJsonDocument<200> doc;
float frequency = 917.20; // This frequency is for Group 7, you will need to change your frequency according to the provided table
dht DHT;
#define DHT11_PIN 5



void setup() 
{
  pinMode(4, INPUT);
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  
  rf95.setFrequency(frequency); 
  
  // Setup Power,dBm
  rf95.setTxPower(3); // Do not increase the transmit power as you do not need to operate over a long range
  
  rf95.setSpreadingFactor(7); // Setup Spreading Factor (6 ~ 12)
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.println("Sending message to LoRa Server");
  
  // Send a message to LoRa Server
  //String thisString = String(DHT.temperature);
  //thisString = "{ \"t\": " + thisString + ", \"ph\": " + String(DHT.humidity) + "}" ; //Read a sensor data from DHT-11 and PIN 5
  char data [50] = {};  //initialize every loop
  /*char temp[10];
  char dh[10];
  String(DHT.temperature).toCharArray(temp, sizeof(temp));
  String(DHT.humidity).toCharArray(dh, sizeof(dh));
  doc["t"] = temp;
  doc["ph"] = dh;*/
  StaticJsonDocument<200> doc;
  doc["t"] = String(DHT.temperature);
  doc["ph"] = String(DHT.temperature);
  serializeJson(doc, data, 50);
  //serializeMsgPack(doc, data, 50);
  doc.clear();

  //thisString.toCharArray(data, sizeof(data));
  rf95.send(data, sizeof(data));
  
  rf95.waitPacketSent();
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  /*if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got a reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is LoRa server running?");
  }*/

  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(5000);
}
