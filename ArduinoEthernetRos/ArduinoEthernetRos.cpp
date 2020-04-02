#define ENC28J60
//this definition is for using tcp rosserial with this specific chip, comment if you use another.

#define ROSSERIAL_ARDUINO_TCP
//this defines that we are to use tcp/ip instead of usb serial.

#include <Arduino.h>
#include <ethernet_comp.h>
#include <HardwareSerial.h>
#include <IPAddress.h>
#include <stdint.h>
//#include <SPI.h>

#include <UIPEthernet.h>
//this library is for ENC28j60 Ethernet module, change it with appropriate.

#include <WString.h>
#include <stdint.h>
#include <ros.h>
#include <stdint.h>
#include <ArduinoTcpHardware.h>
#include <ros/node_handle.h>
#include <ros/publisher.h>
#include <stdint.h>

#include <stdint.h>

#include <std_msgs/String.h>

// Set the shield settings
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Set the rosserial socket server IP address
IPAddress server(192, 168, 0, 11);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

ros::NodeHandle nh;
// Make a chatter publisher
std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);

// Be polite and say hello
char hello[13] = "hello world!";
uint16_t period = 1000;
uint32_t last_time = 0;

void setup()
{
  // Use serial to monitor the process
  Serial.begin(115200);

  // Connect the Ethernet
  Ethernet.begin(mac);

  // Let some time for the Ethernet Shield to be initialized
  delay(1000);

  Serial.println("");
  Serial.println("Ethernet connected");
  Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());

  // Set the connection to rosserial socket server
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();

  // Another way to get IP
  Serial.print("IP = ");
  Serial.println(nh.getHardware()->getLocalIP());

  // Start to be polite
  nh.advertise(chatter);
}

void loop()
{
  if (millis() - last_time >= period)
  {
    last_time = millis();
    if (nh.connected())
    {
      Serial.println("Connected");
      // Say hello
      str_msg.data = hello;
      chatter.publish(&str_msg);
    }
    else
    {
      Serial.println("Not Connected");
    }
  }

  nh.spinOnce();
  //this function processes all callbacks and stuff. make sre to call it as frequent as possible
  //too big delay between calls will drop the node
  delay(1);
}
