#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90614 Arduino library

const byte IRAddress1 = 0x5B;
const byte IRAddress2 = 0x5A;

//////////////////////////////////
// Setup light sensor variables //
//////////////////////////////////
const bool LIGHTSENSOR = true;
int lightPin = 0;
int lightReading;
const int numReadings = 10;
int total = 0;
int average = 0;
float lux;

///////////////////////////////
// Setup IR sensor variables //
///////////////////////////////
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

  if(packetSize > 0)
  {
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    String datReq(packetBuffer);

    if (datReq == "All")
    {
      therm1.read();
      therm2.read();

      float t1 = therm1.object();
      char buf[30];
      dtostrf(t1, 6, 2, buf);

      float t2 = therm2.object();
      char buf2[10];
      dtostrf(t2, 6, 2, buf2);

      if (LIGHTSENSOR == true) {
        lux = getLux();
        char buf3[10];
        dtostrf(lux, 6, 2, buf3);

        strcat(buf, buf2, buf3);
      }
      else {
        strcat(buf, buf2);
      }

      Serial.println(buf);

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(buf);
      Udp.endPacket();
    }
    else if (datReq == "Ttwo")
    {
      therm2.read(); //therm2.read();
      float t2 = therm2.object();

      Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
      Udp.print(t2);
      Udp.endPacket();
    }
  }
  delay(2000);
}

////////////////////////////////////////////////////
// Function to get the ambient light level in lux //
////////////////////////////////////////////////////
float getLux() {
    // Average light sensor readings over 'numReadings'
  for (int i = 0; i < numReadings; i++){
    lightReading = analogRead(lightPin);
    total = total + lightReading;
    //Serial.println(total);
    delay(10);
  }
  average = total / numReadings;
  total = 0;

  // Convert analog light reading to lux
  lux = pow(10,6.6162) * pow(average,-1.9191);

  // Print readings to serial monitor
  Serial.print("Reading: "); Serial.print(average);
  Serial.print(" ==> "); Serial.print(lux); Serial.println(" lux");
  return lux;
}
