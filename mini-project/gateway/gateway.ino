#include <Process.h>
#include <Console.h>
#include <SPI.h>
#include "RH_RF95.h"

unsigned long lastMillis = 0;
uint8_t mqttReady = 0;
String thisString = "";

RH_RF95 rf95;
float frequency = 917.20;
uint8_t buf_receive[RH_RF95_MAX_MESSAGE_LEN];

#define BAUDRATE 115200

void setup() 
{
  // Initialize Bridge
  Bridge.begin(BAUDRATE);
  // Initialize Serial
  Console.begin(); 
  Bridge.begin(BAUDRATE);
  Console.begin();
  while (!Console) ; // Wait for console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    {
      Console.println("init failed");
    }

  rf95.setFrequency(frequency);    //Setup operating frequncy
  rf95.setTxPower(3);              // Setup Power,dBm
  rf95.setSpreadingFactor(7);      // Setup Spreading Factor (6 ~ 12)
  rf95.setSignalBandwidth(125000); // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000 
  rf95.setCodingRate4(5);          // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8)
  
  Console.print("Listening on frequency: ");
  Console.println(frequency);
}

void loop()
{
  char buf_send [RH_RF95_MAX_MESSAGE_LEN] = {};


  if(millis() - lastMillis > 5000) // Upload Per 5 seconds
    {
      Console.println("Subscribe to MQTT broker");
      mqtt_subscribe();
      thisString.toCharArray(buf_send, sizeof(buf_send));
      if (rf95.available())
        {
          uint8_t len = sizeof(buf_receive);
          if (rf95.recv(buf_receive, &len))
            {
              RH_RF95::printBuffer("request: ", buf_receive, len);
              Console.print("got request: ");
              Console.println((char*)buf_receive);
              Console.print("RSSI: ");
              Console.println(rf95.lastRssi(), DEC);
              mqttReady = 1;
              
              rf95.send(buf_send, sizeof(buf_send));
              rf95.waitPacketSent();
              Console.println("Sent a reply");
              
            }
          else
            {
              Console.println("recv failed");
            }
          }   
      lastMillis = millis();
      if (mqttReady == 1)
      {
        Console.println("Publish to local MQTT broker");
        mqtt_publish();
        mqttReady = 0;
      }
    }
  thisString = ""; 
}

String string_data() // char array to string
{
    String sData = buf_receive;
    return sData;
}

void mqtt_publish()
{
  Process p;    // Create a process and call it "p"
  p.begin("mosquitto_pub"); // Process that launch the "mosquitto_pub" command
  p.addParameter("-h");
  p.addParameter("10.130.1.202");
  p.addParameter("-p");
  p.addParameter("1883");
  p.addParameter("-m");
  p.addParameter(string_data());  // Add data to publish
  p.addParameter("-t");
  p.addParameter("measurement");    // Publish to this topic
  p.run();    // Run the process and wait for its termination
  
    // Print arduino logo over the Serial
    // A process output can be read with the stream methods
  while (p.available() > 0)
    {
      char c = p.read();
      Console.print(c);
    }
  //   Ensure the last bit of data is sent.
  Console.flush();
}

void mqtt_subscribe()
{
  Process p;    // Create a process and call it "p"
  p.begin("mosquitto_sub"); // Process that launch the "mosquitto_pub" command
  p.addParameter("-h");
  p.addParameter("10.130.1.202");
  p.addParameter("-p");
  p.addParameter("1883");
  p.addParameter("-t");
  p.addParameter("actuate");
  p.addParameter("-C");
  p.addParameter("1");
  p.run();    // Run the process and wait for its termination
  
  // Print arduino logo over the Serial
  // A process output can be read with the stream methods
  while (p.available() > 0) 
  {
    char c; 
    c = p.read();
    thisString = thisString + c;      
  }
  Console.flush();
}
