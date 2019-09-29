/*
 Examples to show how to publish data to Baidu IoT hub using the MQTT protocol. https://cloud.baidu.com/doc/IOT/Quickstart.html 

Device Requirements:
 * LG01
 * Arduino 1.5.8 IDE or above

Modified : 2016-Dec-14 Support@dragino.com

 */


#include <Process.h>
#include <Console.h>
#include <SPI.h>
#include "RH_RF95.h"
unsigned long lastMillis = 0;
uint8_t mqttReady = 0;
// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 917.20;
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
//If you use Dragino IoT Mesh Firmware, uncomment below lines.
//For product: LG01. 
#define BAUDRATE 115200

//If you use Dragino Yun Firmware, uncomment below lines.
//For product: Yun Shield 
//#define BAUDRATE 250000

void setup() 
{
    // Initialize Bridge
    Bridge.begin(BAUDRATE);
   // Initialize Serial
    Console.begin();
 //----------------------------------
  //pinMode(led, OUTPUT);     
  Bridge.begin(BAUDRATE);
  Console.begin();
  while (!Console) ; // Wait for console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    Console.println("init failed");

  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(3);
  
  // Setup Spreading Factor (6 ~ 12)
  rf95.setSpreadingFactor(7);
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);
  
  Console.print("Listening on frequency: ");
  Console.println(frequency);
}

void loop()
{
    if(millis() - lastMillis > 5000) // Upload Per 20 seconds
    {
  //--------------------------------------------------------
        if (rf95.available())
  {
    // Should be a message for us now   
    
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      //digitalWrite(led, HIGH);
      RH_RF95::printBuffer("request: ", buf, len);
      Console.print("got request: ");
      Console.println((char*)buf);
      Console.print("RSSI: ");
      Console.println(rf95.lastRssi(), DEC);
      mqttReady = 1;
      // Send a reply
      /*uint8_t data[] = "And hello back to you Group 1";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Console.println("Sent a reply");
      digitalWrite(led, LOW);*/
    }
    else
    {
      Console.println("recv failed");
    }
  }
  //--------------------------------------------------------
        lastMillis = millis();
        if (mqttReady == 1)
        {
          Console.println("Publish to local mqtt");
          mqtt_publish();
          mqttReady = 0;
        }
    }
}

String generate_data() // Generate a random data
{
    String count = buf ;
    //count = random(100, 500);
    return count;
}

void mqtt_publish()
{
    Process p;    // Create a process and call it "p"
    p.begin("mosquitto_pub"); // Process that launch the "mosquitto_pub" command
    //p.addParameter("-d");
    p.addParameter("-h");
    p.addParameter("10.130.1.202");
    //p.addParameter("-i");
    //p.addParameter("deviceId-wY8HTBUTtu");  // Add Baidu Device ID
    p.addParameter("-p");
    p.addParameter("1883");
    //p.addParameter("-q");
    //p.addParameter("0");
    p.addParameter("-m");
    p.addParameter(generate_data());// Add data
    //p.addParameter("-u");
    //p.addParameter("lishunan/mything");      // User Name for Baidu IoT hub
    //p.addParameter("-P");
   // p.addParameter("a6bv9iszcxxxxxxxxxc9mq1vnkPwugjiWRB+Q=");   // Password for Biadu IoT Hub
    p.addParameter("-t");
    p.addParameter("measurement");    // Publish to this topic
    p.run();    // Run the process and wait for its termination
  
    // Print arduino logo over the Serial
    // A process output can be read with the stream methods
    while (p.available() > 0) {
      char c = p.read();
      Console.print(c);
    }
  //   Ensure the last bit of data is sent.
    Console.flush();
}
