#include <dht.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

RH_RF95 rf95; // Create radio object
float frequency = 917.20;
dht DHT;
#define DHT11_PIN 4
#define BUZZER_PIN 5
#define RED_LED 6
#define GREEN_LED 7

uint8_t buzzer;
uint8_t redLED;
uint8_t greenLED;

void setup() 
{
  pinMode(DHT11_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
  {
    Serial.println("init failed");
  }
  rf95.setFrequency(frequency);     // Setup Power,dBm
  rf95.setTxPower(3);               // Do not increase the transmit power as you do not need to operate over a long range
  rf95.setSpreadingFactor(7);       // Setup Spreading Factor (6 ~ 12)
  rf95.setSignalBandwidth(125000);  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  rf95.setCodingRate4(5);           // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  char buf_send [RH_RF95_MAX_MESSAGE_LEN] = {};  //initialize every loop
  Serial.println("Sending message to LoRa Server");
  StaticJsonDocument<200> doc;
  doc["id"] = 100;
  doc["t"] = String(DHT.temperature);
  doc["ph"] = String(DHT.humidity);
  //doc["mb"] = sizeof doc message
  serializeJson(doc, buf_send, RH_RF95_MAX_MESSAGE_LEN);          //ASCII Encoded JSON
  //serializeMsgPack(doc, data_send, RH_RF95_MAX_MESSAGE_LEN);    //MessagePack Encoded JSON
  doc.clear();

  rf95.send(buf_send, sizeof(buf_send));
  rf95.waitPacketSent();      // Now wait for a reply
  uint8_t buf_receive[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf_receive);

  if (rf95.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf_receive, &len))
   {
      Serial.print("got a reply: ");
      Serial.println();
      
      Serial.println((char*)buf_receive);
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
  }

  DeserializationError err = deserializeJson(doc,buf_receive);
  
  if (err)
  {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(err.c_str());
  }

  if(!err)
  {
    buzzer = doc["b"];
    redLED = doc["r"]; 
    greenLED = doc["g"];
  }

  serializeJson(doc, Serial);
  Serial.println();
  doc.clear();

  if (buzzer == true)
  {
    Serial.println("BUZZER IS ON!!");
    digitalWrite(BUZZER_PIN,1);
  }
  else if (buzzer == false)
  {
    Serial.println("BUZZER IS OFF!!");
    digitalWrite(BUZZER_PIN,0);
  }
  
    if (redLED == true)
  {
    Serial.println("Red LED IS ON!!");
    digitalWrite(RED_LED,1);
  }
  else if (redLED == false)
  {
    Serial.println("Red LED IS OFF!!");
    digitalWrite(RED_LED,0);
  }
  
    if (greenLED == true)
  {
    Serial.println("Green LED IS ON!!");
    digitalWrite(GREEN_LED,1);
  }
  else if (greenLED == false)
  {
    Serial.println("Green LED IS OFF!!");
    digitalWrite(GREEN_LED,0);
  }
  
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(2000);
}
