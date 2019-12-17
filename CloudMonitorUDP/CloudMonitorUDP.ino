#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

const byte IRAddress1 = 0x5B;
const byte IRAddress2 = 0x5A;

float temp1 = 1;
float temp2 = 2;

IRTherm therm1;
IRTherm therm2;

// Enter a MAC address and IP address for your controller below.
byte mac[] = { 0xBA, 0xD1, 0xDE, 0xA5, 0x00, 0x00 };
IPAddress ip(10, 0, 20, 10);
unsigned int localPort = 8888;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq;
int packetSize;

EthernetUDP Udp;
// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
//EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // Initialize the IR sensors and set units to Celsius
  therm1.begin(IRAddress1);
  therm2.begin(IRAddress2);
  therm1.setUnit(TEMP_C);
  therm2.setUnit(TEMP_C);

  // Send a reset to the W5500 to make sure it works
  pinMode(9, OUTPUT); 
  digitalWrite(9, LOW);
  delayMicroseconds(500);
  digitalWrite(9, HIGH);
  delayMicroseconds(1000);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  // server.begin();
//  Serial.print("server is at ");
//  Serial.println(Ethernet.localIP());
  delay(1500);
}

void loop() {

  packetSize = Udp.parsePacket();
  //Serial.println('yabba');
  
  if(packetSize > 0)
  {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    String datReq(packetBuffer);
    //Serial.println(datReq);

    if (datReq == "All")
    {
      therm1.read();
      therm2.read();
      
      float t1 = therm1.object();
      char buf[20];
      dtostrf(t1, 6, 2, buf);
      
      float t2 = therm2.object();
      char buf2[10];
      dtostrf(t2, 6, 2, buf2);
      
      strcat(buf, buf2);
      Serial.println(buf);
      
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(buf);
      Udp.endPacket();
      //Serial.println(therm1.object());
    }
    else if (datReq == "Ttwo")
    {
      therm2.read(); //therm2.read();
      //float t1 = therm1.object();
      float t2 = therm2.object();
      //char replyBuffer[20];
      //sprintf(replyBuffer, "%f", t1);
      //Serial.println(replyBuffer);
      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(t2);
      //Udp.print(replyBuffer);
      Udp.endPacket();
      Serial.println(therm2.object());
    }
  }
  delay(2000);
// listen for incoming clients
//  EthernetClient client = server.available();
//  if (client) {
//    Serial.println("new client");
//    // an http request ends with a blank line
//    boolean currentLineIsBlank = true;
//    while (client.connected()) {
//      if (client.available()) {
//        temp1 = therm1.object();
//        temp2 = therm2.object();
//        char c = client.read();
//        Serial.write(c);
//        // if you've gotten to the end of the line (received a newline
//        // character) and the line is blank, the http request has ended,
//        // so you can send a reply
//        if (c == '\n' && currentLineIsBlank) {
//          // send a standard http response header
//          client.println("HTTP/1.1 200 OK");
//          client.println("Content-Type: text/html");
//          client.println("Connection: close");  // the connection will be closed after completion of the response
//          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
//          client.println();
//          client.println("<!DOCTYPE HTML>");
//          client.println("<html>");
//
//          client.print("Temp 1: ");
//          if (therm1.read()) { client.print(therm1.object());}
//          client.println("<br />");
//          client.print("Temp 2: ");
//          if (therm2.read()) { client.print(therm2.object());}
//          client.println("<br />");
//          client.println("------- -----");
//          client.println("<br />");
//          client.print("T1-T2:  ");
//          client.println(therm1.object()-therm2.object());
//          client.println("</html>");
//          break;
//        }
//        if (c == '\n') {
//          // you're starting a new line
//          currentLineIsBlank = true;
//        }
//        else if (c != '\r') {
//          // you've gotten a character on the current line
//          currentLineIsBlank = false;
//        }
//      }
//    }
//    // give the web browser time to receive the data
//    delay(1);
//    // close the connection:
//    client.stop();
//    Serial.println("client disconnected");
//  }
}
